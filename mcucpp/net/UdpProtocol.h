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

namespace Mcucpp
{
namespace Net
{
	class UdpProtocol :public IIpIncapsulatingSubProtocol
	{
		INetIncapsulatingProtocol &_netProtocol;
		enum{MaxListners = 8};
		IPortListner *_listners[MaxListners];
		uint16_t _nextTempPort;
	public:
		UdpProtocol(INetIncapsulatingProtocol &netProtocol);
		
		void ProcessMessage(const Net::IpAddr &srcAddr, const Net::IpAddr &destAddr, DataBuffer &buffer);
		bool SendMessage(const Net::IpAddr &destAddr, uint16_t srcPort, uint16_t destPort, DataBuffer &buffer);
		void AddListener(IPortListner *listner);
		uint16_t GetTempPort();
	};
	
}}