

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
#include <net/INetDispatch.h>
#include <net/INetProtocol.h>
#include <Dispatcher.h>
#include <array.h>

namespace Mcucpp
{
namespace Net
{

	class NetDispatch: public INetDispatch
	{
		enum{MaxInterfaces = 4};
		enum{MaxProtocols = 4};
		Dispatcher &_dispatcher;
		
		struct ProtocolIdPair
		{
			ProtocolIdPair(INetProtocol *p, uint16_t i) :protocol(p), id(i) {}
			INetProtocol *protocol;
			uint16_t id;
		};
		
		Containers::FixedArray<MaxInterfaces, NetInterface *> _interfaces;
		Containers::FixedArray<MaxProtocols, ProtocolIdPair> _protocols;
		
	public: // INetDispatch
		virtual void TxComplete(TransferId txId, bool success);
		virtual void RxComplete(const Net::MacAddr &srcAddr, const Net::MacAddr &destAddr, uint16_t protocoId, DataBuffer &buffer);
		virtual bool SendMesage(const Net::MacAddr &destAddr, uint16_t protocoId, DataBuffer &buffer);
	public:
		NetDispatch(Dispatcher &dispatcher);
		void AddInterface(NetInterface *interface);
		void AddProtocol(uint16_t protocoId, INetProtocol *protocol);
		void Poll();
	};
	
}}