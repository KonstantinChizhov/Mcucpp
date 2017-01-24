//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2016
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or 
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************

#pragma once
#include <net/NetInterface.h>
#include <net/IpProtocolId.h>
#include <net/INetProtocol.h>
#include <net/Ip4Protocol.h>
#include <net/IAddressResolve.h>
#include <Dispatcher.h>
#include <array.h>

namespace Mcucpp
{
namespace Net
{
	enum ArpHardware
	{
		ArpEthernet                   = 0x0001,
		ArpExperimentalEthernet       = 0x0002,
		ArpAmateurRadioAX             = 0x0003,
		ArpProteonProNETTokenRing     = 0x0004,
		ArpChaos                      = 0x0005,
		ArpIEEE802                    = 0x0006,
		ArpARCNET_RFC_1201            = 0x0007,
		ArpHyperchannel               = 0x0008,
		ArpLanstar                    = 0x0009,
		ArpAutonetShortAddress        = 0x0010,
		ArpLocalTalk                  = 0x0011,
		ArpLocalNet                   = 0x0012,
		ArpUltraLink                  = 0x0013,
		ArpSMDS                       = 0x0014,
		ArpFrameRelay                 = 0x0015,
		ArpATM1                       = 0x0016,
		ArpHDLC                       = 0x0017,
		ArpFibreChannelRFC4338        = 0x0018,
		ArpATM2                       = 0x0019,
		ArpSerialLine                 = 0x0020,
		ArpATM3                       = 0x0021,
		ArpMIL_STD_188_220            = 0x0022,
		ArpMetricom                   = 0x0023,
		ArpIEEE_1394_1995             = 0x0024,
		ArpMAPOS                      = 0x0025,
		ArpTwinaxial                  = 0x0026,
		ArpEUI_64                     = 0x0027,
		ArpHIPARP                     = 0x0028,
		ArpIP_and_ARP_over_ISO_7816_3 = 0x0029,
		ArpARPSec                     = 0x0030,
		ArpIPsecTunnel_RFC_3456       = 0x0031,
		ArpInfiniband_RFC_4391        = 0x0032,
		ArpCAI_TIA                    = 0x0033,
		ArpWiegand_Interface          = 0x0034,
		ArpPureIP                     = 0x0035
	};
	
	struct ArpEntry
	{
		ArpEntry()
			:lastUpdated(0)
		{}
		
		Net::MacAddr macAddr;
		Net::IpAddr ipAddr;
		uint32_t lastUpdated;
	};
	
	struct ArpPendingRequest
	{
		ArpPendingRequest()
			:requestedTime(0), attempts(0)
		{}
		
		Net::IpAddr ipAddr;
		uint32_t requestedTime;
		unsigned attempts;
	};
	
	class ArpTable
	{
		enum{ArpTableSize = 6};
		ArpEntry _table[ArpTableSize];
		uint32_t _cacheTimeoutMsec;
	public:
		ArpTable();
		void SetCacheTimeout(uint32_t value);
		uint32_t GetCacheTimeout();
		void Add(const Net::IpAddr &ipAddr, const Net::MacAddr &macAddr, uint32_t timeStamp);
		bool Resolve(const Net::IpAddr &ipAddr, /*[Out]*/ Net::MacAddr &macAddr);
		bool Resolve(const Net::MacAddr &macAddr, /*[Out]*/ Net::IpAddr &ipAddr);
		void Invalidate(uint32_t timeStamp);
	};
	
	class ArpProtocol: public INetProtocol, public IAddressResolve
	{
		Dispatcher &_dispatcher;
		INetDispatch &_netDispatch;
		NetInterface &_iface;
		IP4Protocol *_ipProtocol;
		ArpTable _arpTable;
		enum{PendingRequestsCount = 6, ArpAttempts = 3, ArpResendTimeout = 100};
		ArpPendingRequest _pendingRequests[PendingRequestsCount];
		void ProcessPendingRequests();
	public: // INetProtocol
		virtual void ProcessMessage(const Net::MacAddr &srcAddr, const Net::MacAddr &destAddr, Net::NetBuffer &buffer);
	public: // IAddressResolve
		virtual bool AddressResolve(const Net::IpAddr &ip, Net::MacAddr &result);
	public:
		ArpProtocol(Dispatcher &dispatcher, INetDispatch &netDispatch, NetInterface &iface);
		void SetIp4Protocol(IP4Protocol *ipProtocol);
		void SetCacheTimeout(uint32_t value);
		uint32_t GetCacheTimeout();
		void InvalidateChache();
		void AddAddress(const Net::IpAddr &ip, const Net::MacAddr &mac);
	public:
		void SendMessage(const Net::IpAddr &destIp, const MacAddr &destAddr = MacAddr(), bool request = true);
	};
	
}}