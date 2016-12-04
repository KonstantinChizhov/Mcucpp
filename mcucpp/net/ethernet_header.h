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

#include <stdint.h>
#include <net/net_addr.h>
#include <net/ether_type.h>

namespace Mcucpp
{
namespace Net
{
	class EthernetHeader
	{
		enum{AddrLen = 6};
		uint8_t _bytes[AddrLen + AddrLen + 2 + 4];
	public:
		size_t Size()
		{
			return AddrLen + AddrLen + 2; // TODO: add VLAN frame support
		}
		
		void SetSrcAddr(const MacAddr &addr)
		{
			for(unsigned i = 0; i < AddrLen; i++)
				_bytes[AddrLen + i] = addr[i];
		}
		
		void SetDestAddr(const MacAddr &addr)
		{
			for(unsigned i = 0; i < AddrLen; i++)
				_bytes[i] = addr[i];
		}
		
		MacAddr GetSrcAddr()
		{
			return MacAddr(&_bytes[AddrLen]);
		}
		
		MacAddr GetDestAddr()
		{
			return MacAddr(&_bytes[0]);
		}
		
		void SetEtherType(uint16_t type)
		{
			_bytes[AddrLen + AddrLen + 0] = (type >> 8) & 0xff;
			_bytes[AddrLen + AddrLen + 1] = type & 0xff;
		}
		
		unsigned GetEtherType()
		{
			unsigned etherType = _bytes[AddrLen + AddrLen + 0] << 8 |
			       _bytes[AddrLen + AddrLen + 1];
			if(etherType == VLAN_Tagged)
			{
				etherType = _bytes[AddrLen + AddrLen + 4] << 8 |
			       _bytes[AddrLen + AddrLen + 5];
			}
			return etherType;
		}
	};
}
}