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

#ifndef USART_HPP
#define USART_HPP

#include <stdint.h>
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ioreg.h"
#include <static_assert.h>
#include <enum.h>
#include <debug.h>

namespace Mcucpp
{

#ifdef URSEL
enum{ursel = 1 << URSEL};
#else
enum{ursel = 0};
#endif

#define DECLARE_HW_USART(ClassName, UDR_Reg, UCSRA_Reg, UCSRB_Reg, UCSRC_Reg, UBRRL_Reg, UBRRH_Reg)\
struct ClassName\
{\
	IO_REG_WRAPPER(UDR_Reg, Udr, uint8_t);\
	IO_REG_WRAPPER(UCSRA_Reg, Ucsra, uint8_t);\
	IO_REG_WRAPPER(UCSRB_Reg, Ucsrb, uint8_t);\
	IO_REG_WRAPPER(UCSRC_Reg, Ucsrc, uint8_t);\
	IO_REG_WRAPPER(UBRRL_Reg, Ubrrl, uint8_t);\
	IO_REG_WRAPPER(UBRRH_Reg, Ubrrh, uint8_t);\
};

#ifdef UDR //the one usart
DECLARE_HW_USART(Usart0Regs, UDR, UCSRA, UCSRB, UCSRC, UBRRL, UBRRH)
#endif

#ifdef UDR0 //first usart
DECLARE_HW_USART(Usart0Regs, UDR0, UCSR0A, UCSR0B, UCSR0C, UBRR0L, UBRR0H)
#endif

#ifdef UDR1 //second usart
DECLARE_HW_USART(Usart1Regs, UDR1, UCSR1A, UCSR1B, UCSR1C, UBRR1L, UBRR1H)
#endif

class UsartBase
{
public:
	
	enum InterruptFlags
	{
		NoInterrupt = 0,
		RxNotEmptyInt = 1 << RXCIE,
		TxCompleteInt = 1 << TXCIE,
		TxEmptyInt = 1 << UDRIE,
		AllInterrupts = RxNotEmptyInt | TxCompleteInt | TxEmptyInt
	};
	
	enum UsartMode
	{
		DataBits5 = (0 << UCSZ2) | (((0 << UCSZ1) | (0 << UCSZ0)) << 8),
		DataBits6 = (0 << UCSZ2) | (((0 << UCSZ1) | (1 << UCSZ0)) << 8),
		DataBits7 = (0 << UCSZ2) | (((1 << UCSZ1) | (0 << UCSZ0)) << 8),
		DataBits8 = (0 << UCSZ2) | (((1 << UCSZ1) | (1 << UCSZ0)) << 8),
		DataBits9 = (1 << UCSZ2) | (((1 << UCSZ1) | (1 << UCSZ0)) << 8),
		
		NoneParity = ((0 << UPM0) | (0 << UPM1)) << 8,
		EvenParity = ((0 << UPM0) | (1 << UPM1)) << 8,
		OddParity  = ((1 << UPM0) | (1 << UPM1)) << 8,

		NoClock = 0,

		Disabled = 0,
		RxEnable = 1 << RXEN,
		TxEnable = 1 << TXEN,
		RxTxEnable  = RxEnable | TxEnable,

		OneStopBit         = (0 << USBS) << 8,
		HalfStopBit        = (0 << USBS) << 8,
		TwoStopBits        = (1 << USBS) << 8,
		OneAndHalfStopBits = (1 << USBS) << 8,
		
		Default = RxTxEnable | DataBits8 | NoneParity | OneStopBit
	};
	
	enum Error
	{
		NoError = 0,
		OverrunError = 1 << DOR,
		NoiseError = 0,
		FramingError = 1 << FE,
#if defined(__ATmega128__) || defined(__AVR_ATmega128__)
		ParityError = 1 << UPE,
#else
		ParityError = 1 << PE,
#endif
		ErrorMask = OverrunError | FramingError | ParityError
	 };
};

DECLARE_ENUM_OPERATIONS(UsartBase::UsartMode)

template<class Regs=Usart0Regs>
class Usart :public UsartBase
{
public:
	template<unsigned long baund>
	static inline void SetBaudRate()
	{
		const unsigned int ubrr = (F_CPU/16/baund-1);
		const unsigned int ubrr2x =	(F_CPU/8/baund-1);
		const unsigned long rbaund = (F_CPU/16/(ubrr+1));	
 		const unsigned long rbaund2x = (F_CPU/8/(ubrr2x+1));

		const unsigned long err1 = baund > rbaund ? 
					(baund - rbaund)*1000/baund :
					(rbaund - baund)*1000/rbaund;

		const unsigned long err2 = baund > rbaund2x ?
					(baund - rbaund2x)*1000/baund :
					(rbaund2x - baund)*1000/rbaund2x;

		// 2.5 % baud rate error tolerance
		STATIC_ASSERT(err2 < 25 || err1 < 25);
		unsigned int ubrrToUse;
		if(err1 > err2)
		{
			Regs::Ucsra::Set(1 << U2X);
			ubrrToUse = ubrr2x;
		}
		else
		{
			Regs::Ucsra::Set(0x00);
			ubrrToUse = ubrr;
		}
		Regs::Ubrrl::Set(ubrrToUse);
		Regs::Ubrrh::Set(ubrrToUse>>8);
	}

	static inline void SetBaudRate(unsigned long baund)
	{
		unsigned int ubrr = (F_CPU/16/baund-1);
		unsigned int ubrr2x =	(F_CPU/8/baund-1);
		unsigned long rbaund = (F_CPU/16/(ubrr+1));	
 		unsigned long rbaund2x = (F_CPU/8/(ubrr2x+1));

		unsigned long err1;
		if(baund > rbaund)
			err1 = (baund - rbaund)*1000/baund;
		else
			err1 = (rbaund - baund)*1000/rbaund;

		unsigned long err2;
		if(baund > rbaund2x)
			err2 = (baund - rbaund2x)*1000/baund;
		else
			err2 = (rbaund2x - baund)*1000/rbaund2x;

		unsigned int ubrrToUse;
		if(err1 > err2)
		{
			Regs::Ucsra::Set(1 << U2X);
			ubrrToUse = ubrr2x;
		}
		else
		{
			Regs::Ucsra::Set(0x00);
			ubrrToUse = ubrr;
		}
		Regs::Ubrrl::Set(ubrrToUse);
		Regs::Ubrrh::Set(ubrrToUse>>8);
	}

	static void DisableTx()
	{
		Regs::Ucsrb::And(~(1 << TXEN));
	}

	static void EnableTx()
	{
		Regs::Ucsrb::Or(1 << TXEN);
	}

	static void Disable()
	{
		Regs::Ucsra::Set(0);
		Regs::Ucsrb::Set(0);
		Regs::Ucsrc::Set(ursel | 0);
		Regs::Ubrrl::Set(0);
		Regs::Ubrrh::Set(0);
	}

	template<unsigned long baud>
	static inline void Init(UsartMode usartMode = Default)
	{
		SetBaudRate<baud>();
		Regs::Ucsrb::Set(0x00); 
		Regs::Ucsrc::Set(ursel | (uint8_t)(usartMode >> 8));
		Regs::Ucsrb::Set((uint8_t)(usartMode));
	}

	static inline void Init(unsigned long baud, UsartMode usartMode = Default)
	{
		SetBaudRate(baud);
		Regs::Ucsrb::Set(0x00); 
		Regs::Ucsrc::Set(ursel | (uint8_t)(usartMode >> 8));
		Regs::Ucsrb::Set((uint8_t)(usartMode));
	}

	void EnableInterrupt(InterruptFlags interruptSources = AllInterrupts)
	{
		Regs::Ucsrb::Or((uint8_t)interruptSources);
	}

	void DisableInterrupt(InterruptFlags interruptSources = AllInterrupts)
	{
		Regs::Ucsrb::And((uint8_t)~interruptSources);
	}

	static void Write(uint8_t c)
	{
		while(!TxReady())
			;
		Regs::Udr::Set(c);
	}

	static uint8_t Read()
	{
		while(! RxReady())
			;
		return Regs::Udr::Get();
	}

	static bool TxReady()
	{
		return Regs::Ucsra::template BitIsSet<(UDRE)>() && 
			usartData.txBuffer == 0;
	}

	static bool RxReady()
	{
		return Regs::Ucsra::template BitIsSet<(RXC)>();
	}
		
	static InterruptFlags InterruptSource()
	{
		InterruptFlags result = NoInterrupt;
		if(Regs::Ucsra::template BitIsSet<(RXC)>())
			result |= RxNotEmptyInt;
		if(Regs::Ucsra::template BitIsSet<(UDRE)>())
			result |= TxEmptyInt;
		if(Regs::Ucsra::template BitIsSet<(TXC)>())
			result |= TxCompleteInt;
		return NoInterrupt;
	}
	
	static void ClearInterruptFlag(InterruptFlags interruptFlags)
	{
	
	}
	
	static Error GetError()
	{
		return Regs::Ucsra::Get() & ErrorMask;
	}
	
	template<uint8_t TxPinNumber, uint8_t RxPinNumber>
	static void SelectTxRxPins()
	{
		STATIC_ASSERT(TxPinNumber == 0 && RxPinNumber == 0);
	}
	
	static void SelectTxRxPins(uint8_t TxPinNumber, uint8_t RxPinNumber)
	{
		MCUCPP_ASSERT(TxPinNumber == 0 && RxPinNumber == 0);
	}
	
	static void Write(uint8_t *buffer, size_t size, bool async=true)
	{
		if(!async)
		{
			while(size--) 
				Write(*buffer++);
		}
		else
		{
			Write(*buffer++);
			usartData.txBuffer = buffer;
			usartData.txSize = size - 1;
			EnableInterrupt(TxEmptyInt);
		}
	}
	
	static void TxIntHandler()
	{
		if(*usartData.txBuffer)
		{
			Regs::Udr::Set(*usartData.txBuffer++);
			if(--usartData.txSize)
			{
				usartData.txBuffer = 0;
			}
		}
	}
private:
	struct UsartData
	{
		uint8_t *txBuffer;
		uint8_t *rxBuffer;
		size_t txSize;
		size_t rxSize;
	};
	static UsartData usartData;
};

template<class Regs>
typename Usart<Regs>::UsartData Usart<Regs>::usartData;


}

#endif
