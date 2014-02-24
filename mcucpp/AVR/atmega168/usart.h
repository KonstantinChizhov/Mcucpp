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
		IO_REG_WRAPPER(UDR0, Udr, uint8_t);
		IO_REG_WRAPPER(UCSR0A, Ucsra, uint8_t);
		IO_REG_WRAPPER(UCSR0B, Ucsrb, uint8_t);
		IO_REG_WRAPPER(UCSR0C, Ucsrc, uint8_t);
		IO_REG_WRAPPER(UBRR0L, Ubrrl, uint8_t);
		IO_REG_WRAPPER(UBRR0H, Ubrrh, uint8_t);
		
		enum
		{
			Rxc  = 1 << RXC0 ,
			Txc  = 1 << TXC0 ,
			Udre = 1 << UDRE0,
			Fe   = 1 << FE0  ,
			Dor  = 1 << DOR0 ,
			Upe  = 1 << UPE0 ,
			U2x  = 1 << U2X0 ,
			Mpcm = 1 << MPCM0,

			Rxcie = 1 << RXCIE0,
			Txcie = 1 << TXCIE0,
			Udrie = 1 << UDRIE0,
			Rxen  = 1 << RXEN0 ,
			Txen  = 1 << TXEN0 ,
			Ucsz2 = 1 << UCSZ02,
			Rxb8  = 1 << RXB80 ,
			Txb8  = 1 << TXB80 ,

			Umsel1 = 1 << UMSEL01,
			Umsel0 = 1 << UMSEL00,
			Upm1   = 1 << UPM01  ,
			Upm0   = 1 << UPM00  ,
			Usbs   = 1 << USBS0  ,
			Ucsz1  = 1 << UCSZ01 ,
			Ucsz0  = 1 << UCSZ00 ,
			Ucpol  = 1 << UCPOL0
		};
	};

	typedef BasicUsart<Usart0Regs> Usart1;
	#define MCUCPP_HAS_USART1 1
}
