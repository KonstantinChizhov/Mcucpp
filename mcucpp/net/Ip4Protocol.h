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
#include <net/IIpSubProtocol.h>
#include <net/IAddressResolve.h>
#include <Dispatcher.h>
#include <array.h>
#include <ring_buffer.h>

namespace Mcucpp
{
namespace Net
{

	class Ip4Checksum
	{
		uint32_t result;
	public:
		Ip4Checksum()
			:result(0)
		{
			
		}
		
		uint16_t Result()
		{
			while(result & 0xffff0000)
				result = (result & 0xffff) + (result >> 16);
			return ~(uint16_t)result;
		}
		
		void Feed(uint16_t value)
		{
			result += value;
		}
		
		void Feed(uint8_t b1, uint8_t b2)
		{
			result += b1 | (b2 << 8);
		}
		
		void Feed(const Net::IpAddr &addr)
		{
			uint32_t v = addr.ToInt32();
			result += (uint16_t)v;
			result += (uint16_t)(v >> 16);
		}
	};
	
	class PendingPacket
	{
	public:
		DataBuffer *buffer;
		Net::IpAddr destAddr;
		uint32_t timestamp;
		
		PendingPacket(DataBuffer *b = 0, Net::IpAddr a = Net::IpAddr())
			:buffer(b), destAddr(a){}
		
		void Reset()
		{
			buffer = 0;
			timestamp = 0;
			destAddr = Net::IpAddr();
		}
		
		bool IsFree(){return !buffer;}
	};
	
	class IP4Protocol: public INetIncapsulatingProtocol
	{
	public:
		enum
		{
			MaxProtocols = 4,
			MaxPendingPackets = 8
		};
		enum
		{
			Ip4HeaderSize = 20, 
			DefaultTLL = 64,
			AddressResoleTimeoutMsek = 200
		};
		
	protected:
		friend class PendingPacket;
		
		struct ProtocolId
		{
			ProtocolId(IIpSubProtocol *p, uint16_t i):protocol(p), id(i){}
			IIpSubProtocol *protocol;
			uint16_t id;
		};
		
		Dispatcher &_dispatcher;
		INetDispatch &_netDispatch;
		NetInterface &_iface;
		Containers::FixedArray<MaxProtocols, ProtocolId> _protocols;
		PendingPacket _pendingPackets[MaxPendingPackets];
		
		IAddressResolve *_adresssResolve;
		uint32_t _sequence;
		bool EnqueuePacket(const PendingPacket &packet);
		void ProcessPendingPackets();
	public:
		virtual void ProcessMessage(const Net::MacAddr &srcAddr, const Net::MacAddr &destAddr, Net::NetBuffer &buffer);
		virtual bool SendMessage(const Net::IpAddr &destAddr, IpProtocolId protocolId, Net::NetBuffer &buffer);
	public:
		IP4Protocol(Dispatcher &dispatcher, INetDispatch &netDispatch, NetInterface &iface);
		void AddProtocol(uint16_t protocolId, IIpSubProtocol *protocol);
		void SetAddressResolver(IAddressResolve *adresssResolve);
		IAddressResolve * GetAddressResolver();
		void AddressResolved(const Net::IpAddr &ipAddr, const Net::MacAddr &macAddr);
		uint32_t PacketId(const Net::IpAddr &destAddr, IpProtocolId protocolId);
	public: // static
		static uint16_t HeaderChecksum(uint8_t *header, size_t len);
		
	};
	
}}