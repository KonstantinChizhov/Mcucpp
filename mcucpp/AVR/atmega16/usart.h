//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
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

#include <basic_usart.h>

namespace Mcucpp
{
	struct Usart0Regs
	{
		IO_REG_WRAPPER(UDR, Udr, uint8_t);
		IO_REG_WRAPPER(UCSRA, Ucsra, uint8_t);
		IO_REG_WRAPPER(UCSRB, Ucsrb, uint8_t);
		IO_REG_WRAPPER(UCSRC, Ucsrc, uint8_t);
		IO_REG_WRAPPER(UBRRL, Ubrrl, uint8_t);
		IO_REG_WRAPPER(UBRRH, Ubrrh, uint8_t);
		
		enum
		{
			Rxc  = 1 << RXC ,
			Txc  = 1 << TXC ,
			Udre = 1 << UDRE,
			Fe   = 1 << FE  ,
			Dor  = 1 << DOR ,
			Upe  = 1 << PE ,
			U2x  = 1 << U2X ,
			Mpcm = 1 << MPCM,

			Rxcie = 1 << RXCIE,
			Txcie = 1 << TXCIE,
			Udrie = 1 << UDRIE,
			Rxen  = 1 << RXEN ,
			Txen  = 1 << TXEN ,
			Ucsz2 = 1 << UCSZ2,
			Rxb8  = 1 << RXB8 ,
			Txb8  = 1 << TXB8 ,

			Umsel1 = (1 << URSEL),
			Umsel0 = (1 << UMSEL)| (1 << URSEL),
			Upm1   = (1 << UPM1)  | (1 << URSEL),
			Upm0   = (1 << UPM0)  | (1 << URSEL),
			Usbs   = (1 << USBS)  | (1 << URSEL),
			Ucsz1  = (1 << UCSZ1) | (1 << URSEL),
			Ucsz0  = (1 << UCSZ0) | (1 << URSEL),
			Ucpol  = (1 << UCPOL) | (1 << URSEL)
		};
	};

	typedef BasicUsart<Usart0Regs> Usart1;
	#define MCUCPP_HAS_USART1 1
}
