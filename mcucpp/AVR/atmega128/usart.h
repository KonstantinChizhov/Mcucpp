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

			Umsel1 = 0,
			Umsel0 = 1 << UMSEL0,
			Upm1   = 1 << UPM01  ,
			Upm0   = 1 << UPM00  ,
			Usbs   = 1 << USBS0  ,
			Ucsz1  = 1 << UCSZ01 ,
			Ucsz0  = 1 << UCSZ00 ,
			Ucpol  = 1 << UCPOL0
		};
	};
	
	struct Usart1Regs
	{
		IO_REG_WRAPPER(UDR1, Udr, uint8_t);
		IO_REG_WRAPPER(UCSR1A, Ucsra, uint8_t);
		IO_REG_WRAPPER(UCSR1B, Ucsrb, uint8_t);
		IO_REG_WRAPPER(UCSR1C, Ucsrc, uint8_t);
		IO_REG_WRAPPER(UBRR1L, Ubrrl, uint8_t);
		IO_REG_WRAPPER(UBRR1H, Ubrrh, uint8_t);
		
		enum
		{
			Rxc  = 1 << RXC1 ,
			Txc  = 1 << TXC1 ,
			Udre = 1 << UDRE1,
			Fe   = 1 << FE1  ,
			Dor  = 1 << DOR1 ,
			Upe  = 1 << UPE1 ,
			U2x  = 1 << U2X1 ,
			Mpcm = 1 << MPCM1,

			Rxcie = 1 << RXCIE1,
			Txcie = 1 << TXCIE1,
			Udrie = 1 << UDRIE1,
			Rxen  = 1 << RXEN1 ,
			Txen  = 1 << TXEN1 ,
			Ucsz2 = 1 << UCSZ12,
			Rxb8  = 1 << RXB81 ,
			Txb8  = 1 << TXB81 ,

			Umsel1 = 0,
			Umsel0 = 1 << UMSEL1,
			Upm1   = 1 << UPM11  ,
			Upm0   = 1 << UPM10  ,
			Usbs   = 1 << USBS1  ,
			Ucsz1  = 1 << UCSZ11 ,
			Ucsz0  = 1 << UCSZ10 ,
			Ucpol  = 1 << UCPOL1
		};
	};

	typedef BasicUsart<Usart0Regs> Usart1;
	typedef BasicUsart<Usart1Regs> Usart2;
	
#define MCUCPP_HAS_USART1 1
#define MCUCPP_HAS_USART2 1
}
