//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2017
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
#include <net/IIpSubProtocol.h>
#include <net/INetProtocol.h>
#include <net/IPortListner.h>
void Print(const char*str, uint32_t t);

namespace Mcucpp
{
namespace Net
{
	enum IcmpMessageType
	{
		IcmpEchoReply = 0,
		IcmpDestinationUnreachable = 3,
		IcmpRedirectMessage = 5,
		IcmpEchoRequest = 8,
		IcmpRouterAdvertisement = 9,
		IcmpRouterSolicitation = 10,
		IcmpTimeExceeded = 11,
		IcmpParameterProblem = 12,
		IcmpTimestamp = 13,
		IcmpTimestampReply = 14
	};

	

	class IcmpProtocol :public IIpSubProtocol
	{
		INetIncapsulatingProtocol &_netProtocol;
	public:
		IcmpProtocol(INetIncapsulatingProtocol &netProtocol)
		:_netProtocol(netProtocol)
		{
			
		}
		
		virtual void ProcessMessage(const Net::IpAddr &srcAddr, const Net::IpAddr &destAddr, Net::NetBuffer &buffer)
		{
			(void)destAddr;
			Ip4Checksum checksumCalc;
			uint8_t type = buffer.Read();
			uint8_t code = buffer.Read();
			uint16_t checksum = buffer.ReadU16Be();
			checksumCalc.Feed(type, code);
			checksumCalc.Feed(checksum);
			
			switch(type)
			{
			case IcmpEchoRequest:
			{
				uint16_t id = buffer.ReadU16Be();
				uint16_t seq = buffer.ReadU16Be();
				EchoReply(srcAddr, id, seq, buffer);
				break;
			}
			default:
				break;
			}
		}
		
		void EchoReply(const Net::IpAddr &destAddr, uint16_t id, uint16_t seq, Net::NetBuffer &buffer)
		{
			Ip4Checksum checksumCalc;
			size_t dataSize = buffer.Size() - buffer.Tell() - 4;
			Net::NetBuffer reply;
			reply.InsertFront(8 + dataSize);
			reply.Seek(0);
			reply.WriteU8(IcmpEchoReply); // type
			reply.WriteU8(0); // code
			reply.WriteU16Be(0); //checksum
			reply.WriteU16Be(id);
			reply.WriteU16Be(seq);

			checksumCalc.Feed(0);
			checksumCalc.Feed(id);
			checksumCalc.Feed(seq);

			size_t dataWords = dataSize >> 1;
			for(unsigned i = 0; i < dataWords; i++)
			{
				uint16_t v = buffer.ReadU16Be();
				checksumCalc.Feed(v);
				reply.WriteU16Be(v);
			}

			if(dataSize & 1)
			{
				uint8_t b = buffer.Read();
				checksumCalc.Feed((uint16_t)b);
				reply.Write(b);
			}
			buffer.Clear();
			
			reply.Seek(2);
			reply.WriteU16Be(checksumCalc.Result());
			_netProtocol.SendMessage(destAddr, ICMP, reply);
			Print("echo send ", reply.Size());
		}
	};
	
}}