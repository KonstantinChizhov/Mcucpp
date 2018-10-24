
#include <net/UdpProtocol.h>
#include <net/Ip4Checksum.h>

using namespace Mcucpp;
using namespace Mcucpp::Net;

static const uint16_t UdpTempPortStart = 49152;

UdpProtocol::UdpProtocol(INetIncapsulatingProtocol &netProtocol)
:_netProtocol(netProtocol),
_nextTempPort(UdpTempPortStart)
{
	for(unsigned i = 0; i < MaxListners; i++)
	{
		_listners[i] = 0;
	}
}

void UdpProtocol::ProcessMessage(const Net::IpAddr &srcAddr, const Net::IpAddr &destAddr, DataBuffer &buffer)
{
	Ip4Checksum checksumCalc;
		
	uint16_t srcPort = buffer.ReadU16Be();
	uint16_t destPort = buffer.ReadU16Be();
	uint16_t messageLen = buffer.ReadU16Be();
	uint16_t checksum = buffer.ReadU16Be();
	
	if(messageLen < 8)
	{
		return;
	}
	
	size_t dataSize = messageLen - 8;
	
	if(checksum != 0)
	{
		// pseudo header
		checksumCalc.Feed(srcAddr);
		checksumCalc.Feed(destAddr);
		checksumCalc.Feed(0, UDP);
		// UDP header
		checksumCalc.Feed(srcPort);
		checksumCalc.Feed(destPort);
		checksumCalc.Feed(messageLen);
		checksumCalc.Feed(checksum);
	
		size_t dataOffset = buffer.Tell();
		for(unsigned i = 0; i < dataSize; i+=2)
		{
			checksumCalc.Feed( buffer.ReadU16Be());
		}
		if(dataSize & 1)
		{
			checksumCalc.Feed( buffer.ReadU8());
		}
		
		uint16_t expectedChecksum =  checksumCalc.Result();
		if(checksum != expectedChecksum)
		{
			return;
		}
		buffer.Seek(dataOffset);
	}
	
	for(unsigned i = 0; i < MaxListners; i++)
	{
		if(!_listners[i])
		{
			if(_listners[i]->DestPort() == destPort)
			{
				_listners[i]->ProcessMessage(srcAddr, srcPort, dataSize, buffer);
			}
		}
	}
}


bool UdpProtocol::SendMessage(const Net::IpAddr &destAddr, uint16_t srcPort, uint16_t destPort, DataBuffer &buffer)
{
	Ip4Checksum checksumCalc;
	
	if(!buffer.InsertFront(8))
	{
		return false;
	}
	
	buffer.Seek(0);
	uint16_t messageLen = buffer.Size();
	
	buffer.WriteU16Be(srcPort);
	buffer.WriteU16Be(destPort);
	buffer.WriteU16Be(messageLen);
	buffer.WriteU16Be(0);
	
	// pseudo header
	checksumCalc.Feed(_netProtocol.GetIpAddr());
	checksumCalc.Feed(destAddr);
	checksumCalc.Feed(0, UDP);
	// UDP header
	checksumCalc.Feed(srcPort);
	checksumCalc.Feed(destPort);
	checksumCalc.Feed(messageLen);
	checksumCalc.Feed(0);
	
	size_t dataSize = messageLen - 8;
	bool calcChecksum = true;
	
	if(calcChecksum)
	{
		for(unsigned i = 0; i < dataSize; i+=2)
		{
			checksumCalc.Feed( buffer.ReadU16Be());
		}
		if(dataSize & 1)
		{
			checksumCalc.Feed( buffer.ReadU8());
		}
		buffer.Seek(6);
		buffer.WriteU16Be(checksumCalc.Result());
	}
	
	return _netProtocol.SendMessage(destAddr, UDP, buffer);
}

void UdpProtocol::AddListener(IPortListner *listner)
{
	for(unsigned i = 0; i < MaxListners; i++)
	{
		if(!_listners[i])
		{
			_listners[i] = listner;
			break;
		}
	}
}

uint16_t UdpProtocol::GetTempPort()
{
	uint16_t port = _nextTempPort;
	bool found;
	do
	{
		port++;
		if(_nextTempPort == 0)
		{
			port = UdpTempPortStart;
		}
		found = true;
		for(unsigned i = 0; i < MaxListners; i++)
		{
			if(_listners[i]->DestPort() == _nextTempPort)
			{
				found = false;
			}
		}
	}while(!found);
	_nextTempPort = port;
	return _nextTempPort;
}

