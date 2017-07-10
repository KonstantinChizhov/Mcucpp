
#include <net/ArpProtocol.h>
#include <net/Ip4Protocol.h>
#include <net/ether_type.h>


using namespace Mcucpp;
using namespace Mcucpp::Net;

ArpTable::ArpTable()
	:_cacheTimeoutMsec(10000)
{
	
}

void ArpTable::SetCacheTimeout(uint32_t value)
{
	_cacheTimeoutMsec = value;
}

uint32_t ArpTable::GetCacheTimeout()
{
	return _cacheTimeoutMsec;
}

void ArpTable::Add( const Net::IpAddr &ipAddr, const Net::MacAddr &macAddr, uint32_t timeStamp)
{
	for(unsigned i = 0; i < ArpTableSize; i++)
	{
		ArpEntry &entry = _table[i];
		if(entry.ipAddr == ipAddr)
		{
			entry.macAddr = macAddr;
			entry. lastUpdated = timeStamp;
			return;
		}
	}
	
	for(unsigned i = 0; i < ArpTableSize; i++)
	{
		ArpEntry &entry = _table[i];
		if(entry.ipAddr.IsBroadcast() || entry.ipAddr.IsEmpty())
		{
			entry.macAddr = macAddr;
			entry.ipAddr = ipAddr;
			entry.lastUpdated = timeStamp;
			return;
		}
	}
}

bool ArpTable::Resolve(const Net::IpAddr &ipAddr, /*[Out]*/ Net::MacAddr &macAddr)
{
	for(unsigned i = 0; i < ArpTableSize; i++)
	{
		ArpEntry &entry = _table[i];
		if(entry.ipAddr == ipAddr)
		{
			macAddr = entry.macAddr;
			return true;
		}
	}
	return false;
}

bool ArpTable::Resolve(const Net::MacAddr &macAddr, /*[Out]*/ Net::IpAddr &ipAddr)
{
	for(unsigned i = 0; i < ArpTableSize; i++)
	{
		ArpEntry &entry = _table[i];
		if(entry.macAddr == macAddr)
		{
			ipAddr = entry.ipAddr;
			return true;
		}
	}
	return false;
}

void ArpTable::Invalidate(uint32_t timeStamp)
{
	for(unsigned i = 0; i < ArpTableSize; i++)
	{
		ArpEntry &entry = _table[i];
		if(timeStamp - entry.lastUpdated > _cacheTimeoutMsec)
		{
			entry.ipAddr = Net::IpAddr::Broadcast();
			entry.macAddr = Net::MacAddr::Broadcast();
			entry.lastUpdated = 0;
		}
	}
}

void  ArpProtocol::SetIp4Protocol(class IP4Protocol *ipProtocol)
{
	_ipProtocol = ipProtocol;
}

void ArpProtocol::ProcessMessage(const Net::MacAddr &srcAddr, const Net::MacAddr &destAddr, Net::NetBuffer &buffer)
{
	(void)srcAddr;
	(void)destAddr;

		
	uint16_t hardwareType = buffer.ReadU16Be();
	(void)hardwareType;
	
	uint16_t protocolType = buffer.ReadU16Be();
	if(protocolType != EtherIPv4)
	{
		return;
	}
	
	uint8_t hardwareSize = buffer.ReadU8();
	uint8_t protocolSize = buffer.ReadU8();
	
	if(hardwareSize != 6 || protocolSize != 4)
	{
		return;
	}
	
	uint16_t opCode = buffer.ReadU16Be();
	MacAddr srcMac = buffer.ReadMac();
	IpAddr srcIp = buffer.ReadIp();
	
	MacAddr destMac = buffer.ReadMac(); 
	(void)destMac;
	
	IpAddr destIp = buffer.ReadIp();
	buffer.Clear();
	
	if(opCode == 0x0002)
	{
		if(_ipProtocol)
		{
			if(destIp != _ipProtocol->GetIpAddr())
			{
				return;
			}
		}
		
		for(unsigned i = 0; i < PendingRequestsCount; i++)
		{
			if(_pendingRequests[i].ipAddr == srcIp)
			{
				_pendingRequests[i].ipAddr = IpAddr();
				_pendingRequests[i].requestedTime = 0;
				_pendingRequests[i].attempts = 0;
				break;
			}
		}
	
		_arpTable.Add(srcIp, srcMac, _dispatcher.GetTicks());
		if(_ipProtocol)
		{
			_ipProtocol->AddressResolved(srcIp, srcMac);
		}
		return;
	}

	if(opCode == 0x0001)
	{
		if(_ipProtocol)
		{
			if(destIp == _ipProtocol->GetIpAddr())
			{
				SendMessage(srcIp, srcMac, false);
				return;
			}
		}
	}
}

ArpProtocol::ArpProtocol(Dispatcher &dispatcher, INetDispatch &netDispatch, NetInterface &iface)
:_dispatcher(dispatcher), _netDispatch(netDispatch), _iface(iface)
{
	_dispatcher.SetTask<ArpProtocol, &ArpProtocol::InvalidateChache>(this);
}

bool ArpProtocol::AddressResolve(const Net::IpAddr &ipAddr, Net::MacAddr &result)
{
	if(_arpTable.Resolve(ipAddr, result))
	{
		return true;
	}
	SendMessage(ipAddr, MacAddr(), true);
	uint32_t ticks = _dispatcher.GetTicks();
	for(unsigned i = 0; i < PendingRequestsCount; i++)
	{
		if(_pendingRequests[i].ipAddr.IsEmpty() || _pendingRequests[i].ipAddr == ipAddr)
		{
			_pendingRequests[i].ipAddr = ipAddr;
			_pendingRequests[i].requestedTime = ticks;
			_pendingRequests[i].attempts = ArpAttempts - 1;
			break;
		}
	}
	_dispatcher.SetTimer<ArpProtocol, &ArpProtocol::ProcessPendingRequests>(ArpResendTimeout, this);
	
	return false;
}

void ArpProtocol::InvalidateChache()
{
	_arpTable.Invalidate(_dispatcher.GetTicks());
	_dispatcher.SetTimer<ArpProtocol, &ArpProtocol::InvalidateChache>(_arpTable.GetCacheTimeout(), this);
}

void ArpProtocol::SetCacheTimeout(uint32_t value)
{
	_arpTable.SetCacheTimeout(value);
}

uint32_t ArpProtocol::GetCacheTimeout()
{
	return _arpTable.GetCacheTimeout();
}

void ArpProtocol::ProcessPendingRequests()
{
	uint32_t ticks = _dispatcher.GetTicks();
	bool invokeAgain = false;
	for(unsigned i = 0; i < PendingRequestsCount; i++)
	{
		if(!_pendingRequests[i].ipAddr.IsEmpty())
		{
			if(_pendingRequests[i].attempts > 0)
			{
				if(_pendingRequests[i].requestedTime + ArpResendTimeout < ticks)
				{
					_pendingRequests[i].attempts --;
					_pendingRequests[i].requestedTime = ticks;
					SendMessage(_pendingRequests[i].ipAddr, MacAddr(), true);
					invokeAgain = true;
				}
			}
			else
			{
				_pendingRequests[i].requestedTime= 0;
				_pendingRequests[i].ipAddr = Net::IpAddr();
				// TODO: Handle failed ARP request
			}
		}
	}
	if(invokeAgain)
	{
		_dispatcher.SetTimer<ArpProtocol, &ArpProtocol::ProcessPendingRequests>(ArpResendTimeout, this);
	}
}

void ArpProtocol::SendMessage(const Net::IpAddr &destIp, const MacAddr &destAddr, bool request)
{
	const size_t ArpRequestSize = 28;
	NetBuffer buffer;
	if(!buffer.InsertFront(ArpRequestSize))
	{
		// TODO: Handle out of memory
		return;
	}
	buffer.Seek(0);
	buffer.WriteU16Be(ArpEthernet);
	buffer.WriteU16Be(EtherIPv4);
	buffer.WriteU8(6);
	buffer.WriteU8(4);
	buffer.WriteU16Be(request ? 0x0001 : 0x0002); // operation - request
	buffer.WriteMac(_iface.GetMacAddress(0));
	IpAddr srcAddr;
	if(_ipProtocol)
		srcAddr = _ipProtocol->GetIpAddr();
	buffer.WriteIp(srcAddr);
	
	buffer.WriteMac(destAddr); 
	buffer.WriteIp(destIp);

	TransferId txId = _iface.Transmit(request ? MacAddr::Broadcast() : destAddr, EtherARP, buffer);
	if(!txId)
	{
		// TODO: handle
	}
}

void ArpProtocol::AddAddress(const Net::IpAddr &ip, const Net::MacAddr &mac)
{
	_arpTable.Add(ip, mac, _dispatcher.GetTicks());
}
