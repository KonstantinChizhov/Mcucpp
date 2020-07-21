//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2014
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
#include <stddef.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <ioreg.h>
#include <static_assert.h>
#include <enum.h>
#include <clock.h>
#include <data_transfer.h>
#include <atomic.h>

namespace Mcucpp
{
	template<class Regs>
	class BasicUsart
	{
	public:
		enum InterruptFlags
		{
			NoInterrupt = 0,
			RxNotEmptyInt = Regs::Rxcie,
			TxCompleteInt = Regs::Txcie,
			TxEmptyInt    = Regs::Udrie,
			AllInterrupts = RxNotEmptyInt | TxCompleteInt | TxEmptyInt
		};
		
		enum UsartMode
		{
			DataBits5 = 0,
			DataBits6 = Regs::Ucsz0 << 8,
			DataBits7 = Regs::Ucsz1 << 8,
			DataBits8 = (Regs::Ucsz1 << 8) | (Regs::Ucsz0 << 8),
			DataBits9 = Regs::Ucsz2 | (Regs::Ucsz1 << 8) | ( Regs::Ucsz0 << 8),
			
			NoneParity = 0,
			EvenParity = Regs::Upm1 << 8,
			OddParity  = Regs::Upm0 | (Regs::Upm1 << 8),

			Disabled = 0,
			RxEnable = Regs::Rxen,
			TxEnable = Regs::Txen,
			RxTxEnable = RxEnable | TxEnable,

			OneStopBit         = 0,
			HalfStopBit        = 0,
			TwoStopBits        = Regs::Usbs << 8,
			OneAndHalfStopBits = Regs::Usbs << 8,
			
			Default = RxTxEnable | DataBits8 | NoneParity | OneStopBit
		};
		
		enum Error
		{
			NoError = 0,
			OverrunError = Regs::Dor,
			NoiseError = 0,
			FramingError = Regs::Fe,
			ParityError = Regs::Upe,
			ErrorMask = OverrunError | FramingError | ParityError
		};
		

		static inline void SetBaudRate(unsigned long baund)
		{
			uint32_t clock = Clock::SysClock::ClockFreq();
			unsigned int ubrr = (clock/16/baund-1);
			unsigned int ubrr2x =	(clock/8/baund-1);
			unsigned long rbaund = (clock/16/(ubrr+1));	
			unsigned long rbaund2x = (clock/8/(ubrr2x+1));

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
				Regs::Ucsra::Set(Regs::U2x);
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
			Regs::Ucsrb::And(~Regs::Txen);
		}

		static void EnableTx()
		{
			Regs::Ucsrb::Or(Regs::Txen);
		}

		static void Disable()
		{
			Regs::Ucsra::Set(0);
			Regs::Ucsrb::Set(0);
			Regs::Ucsrc::Set(0);
			Regs::Ubrrl::Set(0);
			Regs::Ubrrh::Set(0);
		}

		static inline void Init(unsigned long baud, UsartMode usartMode = Default)
		{
			usartData.txBuffer = 0;
			usartData.rxBuffer = 0;
			usartData.txSize = 0;
			usartData.rxSize = 0;
			usartData.txCount = 0;
			usartData.rxCount = 0;
			usartData.writeCallback = 0;
			usartData.readCallback = 0;
		
			Regs::Ucsrb::Set(0x00); 
			Regs::Ucsrc::Set((uint8_t)(usartMode >> 8));
			Regs::Ucsrb::Set((uint8_t)(usartMode));
			SetBaudRate(baud);

		}

		static void EnableInterrupt(InterruptFlags interruptSources = AllInterrupts)
		{
			Regs::Ucsrb::Or((uint8_t)interruptSources);
		}

		static void DisableInterrupt(InterruptFlags interruptSources = AllInterrupts)
		{
			Regs::Ucsrb::And((uint8_t)~interruptSources);
		}

		static void Write(uint8_t c)
		{
			while(!WriteReady())
				;
			Regs::Udr::Set(c);
		}

		static uint8_t Read()
		{
			while(! ReadReady())
				;
			return Regs::Udr::Get();
		}

		static bool WriteReady()
		{
			return (Regs::Ucsra::Get() & Regs::Udre) && 
				Atomic::Fetch(&usartData.txSize) == 0;
		}

		static bool ReadReady()
		{
			return (Regs::Ucsra::Get() & Regs::Rxc) &&
				Atomic::Fetch(&usartData.rxSize) == 0;
		}
			
		static InterruptFlags InterruptSource()
		{
			InterruptFlags result = NoInterrupt;
			if(Regs::Ucsra::Get() & Regs::Rxc)
				result |= RxNotEmptyInt;
			if(Regs::Ucsra::Get() & Regs::Udre)
				result |= TxEmptyInt;
			if(Regs::Ucsra::Get() & Regs::Txc)
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
			(void)TxPinNumber;
			(void)RxPinNumber;
		}
		
		static void Write(void *data, size_t size, bool async=true)
		{
			uint8_t *ptr = (uint8_t*)data;
			if(!async)
			{
				while(size--) 
					Write(*ptr++);
			}
			else
			{
				Write(*ptr++);
				usartData.txBuffer = ptr;
				usartData.txSize = size - 1;
				usartData.rxCount = 0;
				EnableInterrupt(TxEmptyInt);
			}
		}
		
		static void Read(void *data, size_t size, bool async = false)
		{
			uint8_t *ptr = (uint8_t*)data;
			if(async && size > 1)
			{
				if(ReadReady())
				{
					*ptr = Read();
					ptr++;
					size--;
				}
				usartData.rxBuffer = ptr;
				usartData.rxSize = size;
				usartData.rxCount = 0;
				EnableInterrupt(RxNotEmptyInt);
			}
			else
			{
				while(size--)
				{
					*ptr = Read();
					ptr++;
				}
			}
		}
		
		MCUCPP_FORCEINLINE static void RxIntHandler()
		{
			if(usartData.rxBuffer)
			{
				*usartData.txBuffer = Regs::Udr::Get();
				if(usartData.rxCount >= usartData.rxSize)
				{
					usartData.readCallback(usartData.rxBuffer, usartData.rxSize, true);
					usartData.rxBuffer = 0;
					usartData.rxSize = 0;
					DisableInterrupt(RxNotEmptyInt);
				}
			}
		}
		
		MCUCPP_FORCEINLINE static void TxIntHandler()
		{
			if(usartData.txBuffer)
			{
				Regs::Udr::Set(usartData.txBuffer[usartData.txCount++]);
				if(usartData.txCount >= usartData.txSize)
				{
					usartData.writeCallback(usartData.txBuffer, usartData.txSize, true);
					usartData.txBuffer = 0;
					usartData.txSize = 0;
					DisableInterrupt(TxEmptyInt);
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
			size_t txCount;
			size_t rxCount;
			TransferCallback writeCallback;
			TransferCallback readCallback;
		};
		static UsartData usartData;
	};
	
	DECLARE_ENUM_OPERATIONS_TAMPLATE(typename BasicUsart<Regs>::UsartMode, template <class Regs>)

	template<class Regs>
	typename BasicUsart<Regs>::UsartData BasicUsart<Regs>::usartData;
}

