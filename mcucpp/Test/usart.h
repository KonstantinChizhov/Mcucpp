//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2012
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
// usartData, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************

#ifndef USART_HPP
#define USART_HPP
#include <stdint.h>
#include <stddef.h>
#include <delegate.h>

namespace Mcucpp
{

	class UsartBase
	{
	public:
		
		enum InterruptFlags
		{
			NoInterrupt = 0,
			RxNotEmptyInt = 1,
			TxCompleteInt = 2,
			TxEmptyInt = 4,
			AllInterrupts = RxNotEmptyInt | TxCompleteInt | TxEmptyInt
		};
		
		enum UsartMode
		{
			DataBits5 = (0 << 0) | (0 << 1) | (0 << 2),
			DataBits6 = (0 << 0) | (0 << 1) | (1 << 2),
			DataBits7 = (0 << 0) | (1 << 1) | (0 << 2),
			DataBits8 = (0 << 0) | (1 << 1) | (1 << 2),
			DataBits9 = (1 << 0) | (1 << 1) | (1 << 2),
			
			NoneParity = (0 << 3) | (0 << 4),
			EvenParity = (0 << 3) | (1 << 4),
			OddParity  = (1 << 3) | (1 << 4),

			NoClock = 0,

			Disabled = 0,
			RxEnable = 1 << 5,
			TxEnable = 1 << 6,
			RxTxEnable  = RxEnable | TxEnable,

			OneStopBit         = (0 << 7),
			HalfStopBit        = (0 << 7),
			TwoStopBits        = (1 << 7),
			OneAndHalfStopBits = (1 << 7),
			
			Default = RxTxEnable | DataBits8 | NoneParity | OneStopBit
		};
		
		enum Error
		{
			NoError = 0,
			OverrunError = 1 << 0,
			NoiseError = 0,
			FramingError = 1 << 1,
			ParityError = 1 << 2,
			ErrorMask = OverrunError | FramingError | ParityError
		 };
	};

	inline UsartBase::UsartMode operator|(UsartBase::UsartMode left, UsartBase::UsartMode right)
	{	return static_cast<UsartBase::UsartMode>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}
	
	inline UsartBase::InterruptFlags operator|(UsartBase::InterruptFlags left, UsartBase::InterruptFlags right)
	{	return static_cast<UsartBase::InterruptFlags>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}

	inline UsartBase::Error operator|(UsartBase::Error left, UsartBase::Error right)
	{	return static_cast<UsartBase::Error>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}

	template<int number>
	class TestUsart :public UsartBase
	{
	public:
		struct UsartData
		{
			bool WriteReady;
			bool ReadReady;
			InterruptFlags interruptFlags;
			Error error;
			uint8_t data;
			Delegate1<void, uint8_t> OutFunc;
			Delegate<uint8_t> InFunc;
		};
		
		static UsartData usartData;
		
		template<unsigned long baud>
		static inline void SetBaudRate()
		{
			
		}

		static inline void SetBaudRate(unsigned long baud)
		{
			
		}

		static void Disable()
		{
			
		}

		template<unsigned long baund>
		static inline void Init(UsartMode usartMode = Default)
		{
			Init(baund, usartMode);
		}

		static inline void Init(unsigned long baud, UsartMode usartMode = Default)
		{
			usartData.error = NoError;
			usartData.interruptFlags = NoInterrupt;
			usartData.WriteReady = true;
			usartData.ReadReady = true;
		}

		static void EnableInterrupt(InterruptFlags interruptSources = AllInterrupts)
		{
			
		}

		static void DisableInterrupt(InterruptFlags interruptSources = AllInterrupts)
		{
			
		}

		static void Write(uint8_t c)
		{
			usartData.OutFunc(c);
			usartData.data = c;
		}

		static uint8_t Read()
		{
			return usartData.InFunc();
		}

		static bool WriteReady()
		{
			return usartData.WriteReady;
		}

		static bool ReadReady()
		{
			return usartData.ReadReady;
		}
			
		static InterruptFlags InterruptSource()
		{
			return usartData.interruptFlags;
		}
		
		static Error GetError()
		{
			return usartData.error;
		}

		static void EnableTx()
		{
			
		}

		static void DisableTx()
		{
			
		}

		static void ClearInterruptFlag(InterruptFlags interruptFlags)
		{
			usartData.interruptFlags = (InterruptFlags)(usartData.interruptFlags & ~interruptFlags);
		}
		
		template<uint8_t TxPinNumber, uint8_t RxPinNumber>
		static void SelectTxRxPins()
		{
			SelectTxRxPins(TxPinNumber, RxPinNumber);
		}

		static void SelectTxRxPins(uint8_t, uint8_t)
		{
		}
		
		static void Write(const void *data, size_t size, bool async = false)
		{
			(void)async;
			uint8_t *ptr = (uint8_t*)data;
			while(size--)
			{
				Write(*ptr);
				ptr++;
			}
		}
	};

	template<int number>
	typename TestUsart<number>::UsartData TestUsart<number>::usartData;

	typedef TestUsart<1> Usart1;
	typedef TestUsart<2> Usart2;
	typedef TestUsart<3> Usart3;

}

#endif
