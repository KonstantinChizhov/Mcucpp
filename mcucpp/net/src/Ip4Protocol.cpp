
#include <net/Ip4Protocol.h>
#include <net/ether_type.h>

using namespace Mcucpp;
using namespace Mcucpp::Net;

uint32_t IP4Protocol::PacketId(const Net::IpAddr &destAddr, IpProtocolId protocolId)
{
	return (++_sequence) + (( _ipAddr.ToInt32() * (protocolId  + 17) * 7) ^ (destAddr.ToInt32() * (protocolId+13)));
}

uint16_t IP4Protocol::HeaderChecksum(uint8_t *header, size_t len)
{
	uint32_t sum = 0;
	while(len > 1)
	{
		sum += *((uint16_t*) header);
		header += 2;
		len -= 2;
	}

	if(len)
		sum += (uint16_t) *header;

	while(sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);

	return ~sum;
}

bool IP4Protocol::EnqueuePacket(const PendingPacket &packet)
{
	for(unsigned i = 0; i < MaxPendingPackets; i++)
	{
		if(_pendingPackets[i].IsFree())
		{
			_pendingPackets[i] = packet;
			_pendingPackets[i].timestamp = _dispatcher.GetTicks();
			return true;
		}
	}
	return false;
}


void IP4Protocol::ProcessMessage(const Net::MacAddr &srcAddr, const Net::MacAddr &destAddr, DataBuffer &buffer)
{
	Ip4Checksum checksumCalc;
	unsigned ipVerHLen = buffer.Read();
	if((ipVerHLen >> 4) != 4)
	{
		return; // wrong version
	}
	unsigned headerLengthWords = (ipVerHLen & 0x0f);
	if(headerLengthWords < 5)
	{
		return; 
	}

	unsigned dscpEcn = buffer.Read();
	checksumCalc.Feed((uint8_t)ipVerHLen, (uint8_t)dscpEcn);
	
	unsigned packetSize = buffer.ReadU16Be();
	checksumCalc.Feed((uint16_t)packetSize);
	unsigned fragmentId = buffer.ReadU16Be();
	checksumCalc.Feed((uint16_t)fragmentId);
	unsigned fragmentOffsetAndFalgs = buffer.ReadU16Be();
	checksumCalc.Feed((uint16_t)fragmentOffsetAndFalgs);
	
	unsigned ttl = buffer.Read();
	IpProtocolId protocolId = (IpProtocolId)buffer.Read();
	checksumCalc.Feed((uint8_t)ttl, (uint8_t)protocolId);
	
	
	uint16_t checkSum = buffer.ReadU16Be();
	
	checksumCalc.Feed(checkSum);
	
	Net::IpAddr srcIpAddr = buffer.ReadIp();
	Net::IpAddr destIpAddr = buffer.ReadIp();
	
	checksumCalc.Feed(srcIpAddr);
	checksumCalc.Feed(destIpAddr);
	
	if(destIpAddr != _ipAddr)
	{
		// TODO: routing
		return;
	}
	
	// read options if any
	for(unsigned i = 0; i < headerLengthWords - 5; i++)
	{
		uint16_t o1 = buffer.ReadU16Be();
		uint16_t o2 = buffer.ReadU16Be();
		checksumCalc.Feed(o1);
		checksumCalc.Feed(o2);
	}
	
	if(checkSum != 0 && checksumCalc.Result() !=0)
	{
		// TODO: checksum error handling
		//return;
	}
	// TODO: packet reassembly
	
	for(unsigned i = 0; i < _protocols.size(); i++)
	{
		if(_protocols[i].id == protocolId)
		{
			if(_protocols[i].protocol)
			{
				
				_protocols[i].protocol->ProcessMessage(srcIpAddr, destIpAddr, buffer);
				break;
			}
		}
	}
}

bool IP4Protocol::SendMessage(const Net::IpAddr &destAddr, IpProtocolId protocolId, DataBuffer &buffer)
{
	if(!buffer.InsertFront(Ip4HeaderSize))
	{
		return false;
	}
	buffer.Seek(0);
	
	buffer.Write(0x45); // IP version 4, 5 dwords header
	buffer.Write(0x00);
	buffer.WriteU16Be(buffer.Size());
	
	buffer.WriteU16Be((uint16_t)PacketId(destAddr, protocolId)); //fragment ID
	buffer.WriteU16Be(0); //flags and fragment offset
	
	buffer.Write(DefaultTLL);
	buffer.Write(protocolId);
	buffer.WriteU16Be(0); // checkSum
	
	buffer.WriteIp(_ipAddr);
	buffer.WriteIp(destAddr);
	
	uint16_t checkSum = HeaderChecksum(buffer.BufferList()->Data(), Ip4HeaderSize);
	buffer.Seek(10);
	buffer.WriteU16Be(checkSum);
	
	Net::IpAddr localDestAddr = destAddr;

	if((destAddr & _netMask) != (_ipAddr & _netMask))
	{
		localDestAddr = _ipDefaultGateway;
	}
	
	Net::MacAddr destMacAddr;
	
	if(_adresssResolve && _adresssResolve->AddressResolve(destAddr, destMacAddr))
	{
		TransferId txId = _iface.Transmit(destMacAddr, EtherIPv4, buffer);
		if(!txId)
			return false;
	}else
	{
		DataBuffer *bufferList = buffer.MoveToBufferList();
		if(!EnqueuePacket(PendingPacket(bufferList, destAddr)))
		{
			DataBuffer::ReleaseRecursive(bufferList);
			return false;
		}
	}
	return true;
}


void IP4Protocol::AddressResolved(const Net::IpAddr &ipAddr, const Net::MacAddr &macAddr)
{
	for(unsigned i = 0; i < MaxPendingPackets; i++)
	{
		if(!_pendingPackets[i].IsFree() && _pendingPackets[i].destAddr == ipAddr)
		{
			DataBuffer buffer(_pendingPackets[i].buffer);
			_pendingPackets[i].Reset();
			TransferId txId = _iface.Transmit(macAddr, EtherIPv4, buffer);
			if(!txId)
			{
				// TODO: handle
			}
		}
	}
}

void IP4Protocol::ProcessPendingPackets()
{
	uint32_t time = _dispatcher.GetTicks();
	
	for(unsigned i = 0; i < MaxPendingPackets; i++)
	{
		if(!_pendingPackets[i].IsFree())
		{
			Net::MacAddr destMacAddr;
			if(_adresssResolve->AddressResolve(_pendingPackets[i].destAddr, destMacAddr))
			{
				DataBuffer buffer(_pendingPackets[i].buffer);
				_pendingPackets[i].Reset();
				TransferId txId = _iface.Transmit(destMacAddr, EtherIPv4, buffer);
				if(!txId)
				{
					// TODO: handle
				}
			}else if(time > _pendingPackets[i].timestamp + AddressResoleTimeoutMsek)
			{
				DataBuffer::ReleaseRecursive(_pendingPackets[i].buffer);
				_pendingPackets[i].Reset();
			}
		}
	}
}

IP4Protocol::IP4Protocol(Dispatcher &dispatcher, INetDispatch &netDispatch, NetInterface &iface)
	:_dispatcher(dispatcher), _netDispatch(netDispatch), _iface(iface),
	_adresssResolve(0),
	_sequence(123)
{
	
}

void IP4Protocol::AddProtocol(uint16_t protocolId, IIpSubProtocol *protocol)
{
	_protocols.push_back(ProtocolId(protocol, protocolId));
}

void IP4Protocol::SetAddressResolver(IAddressResolve *adresssResolve)
{
	_adresssResolve = adresssResolve;
}
IAddressResolve * IP4Protocol::GetAddressResolver()
{
	return _adresssResolve;
}
