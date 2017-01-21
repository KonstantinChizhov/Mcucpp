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

#include <net/net_addr.h>
#include <net/INetDispatch.h>

namespace Mcucpp
{
namespace Net
{
	enum NetInterfaceParameter
	{
		NetIfFramesSend,
		NetIfFramesRecived,
		NetIfFramesAppMissed,
		NetIfFramesMacMissed,
		NetIfSendErrors,
		NetIfReciveErrors,
		NetIfLinked,
		NetIfLinkSpeedKbps,
		NetIfHwState,
		NetIfSupportHwCheckSum
	};
	

	class NetInterface
	{
	protected:
		INetDispatch *_dispatch;
	public:
		NetInterface()
		:_dispatch(0)
		{}
	public:
		virtual bool SetMacAddress(unsigned addrNumber, const Net::MacAddr &macaddr)=0;
		virtual unsigned MaxAddresses()=0;
		virtual const Net::MacAddr& GetMacAddress(unsigned addrNumber)=0;
		virtual bool IsLinked()=0;
		virtual void PauseCommand(uint16_t time)=0;
		virtual void Poll()=0;
		virtual uint32_t GetParameter(NetInterfaceParameter parameterId)=0;

		virtual TransferId Transmit(const Net::MacAddr &destAddr, uint16_t protocoId, Net::NetBuffer &buffer)=0;
		virtual bool TxCompleteFor(TransferId txId)=0;
		
	public:
		void SetDispatch(INetDispatch *dispatch)
		{
			_dispatch = dispatch;
		}
	};
}
}
