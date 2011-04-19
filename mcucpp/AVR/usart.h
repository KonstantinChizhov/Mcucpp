#ifndef USART_HPP
#define USART_HPP

//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2010
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



#include <avr/io.h>
#include "containers.h"
#include <avr/interrupt.h>

#ifdef URSEL
enum{ursel = 1 << URSEL};
#else
enum{ursel = 0};
#endif

#define DECLARE_HW_USART(CLASS_NAME, _UDR_, _UCSRA_, _UCSRB_, _UCSRC_, _UBRRL_, _UBRRH_)\
struct CLASS_NAME\
{\
	static inline uint8_t Read()\
	{\
		return _UDR_;\
	}\
	static inline void Write(uint8_t value)\
	{\
		_UDR_ = value;\
	}\
	static inline void SetBaundRate(unsigned long baund)\
	{\
		unsigned int ubrr = (F_CPU/16/baund-1);\
		unsigned int ubrr2x 	(F_CPU/8/baund-1);\
		unsigned long rbaund = (F_CPU/16/(ubrr+1));	\
 		unsigned long rbaund2x (F_CPU/8/(ubrr2x+1));\
	\
		unsigned long err1;\
		if(baund > rbaund)\
			err1 = (baund - rbaund)*1000/baund;\
		else\
			err1 = (rbaund - baund)*1000/rbaund;\
	\
		unsigned long err2;\
		if(baund > rbaund2x)\
			err2 = (baund - rbaund2x)*1000/baund;\
		else\
			err2 = (rbaund2x - baund)*1000/rbaund2x;\
	\
		unsigned int ubrrToUse;\
		if(err1 > err2)\
		{\
			_UCSRA_ = (1 << U2X);\
			ubrrToUse = ubrr2x;\
		}\
		else\
		{\
			_UCSRA_ = 0x00;\
			ubrrToUse = ubrr;\
		}\
		_UBRRL_=(ubrrToUse);\
		_UBRRH_=(ubrrToUse)>>8;\
	}\
	static inline void EnableTxRx()\
	{\
		_UCSRB_ = 0x00; \
		_UCSRC_ = ursel | (1 << UCSZ1) | (1 << UCSZ0);\
		_UCSRB_ = (1 << RXCIE) | (0 << TXCIE) | (1 << UDRIE) | (1 << RXEN) | (1 << TXEN);\
	}\
	\
	static inline bool CanWriteData()\
	{\
		return _UCSRA_ & (1<<UDRE);\
	}\
	static inline void TXIntDisable()\
	{\
		_UCSRB_ &= ~(1 << UDRIE);\
	}\
	static inline void TXIntEnable()\
	{\
		_UCSRB_ |= (1 << UDRIE);\
	}\
	static inline void Disable()\
	{\
		_UCSRA_ = 0;\
		_UCSRB_ = 0;\
		_UCSRC_ = ursel | 0;\
		_UBRRL_ = 0;\
		_UBRRH_ = 0;\
	}\
};\

#ifdef UDR //the one usart
DECLARE_HW_USART(Usart0Traits, UDR, UCSRA, UCSRB, UCSRC, UBRRL, UBRRH)
#endif

#ifdef UDR0 //first usart
DECLARE_HW_USART(Usart0Traits, UDR0, UCSR0A, UCSR0B, UCSR0C, UBRR0L, UBRR0H)
#endif

#ifdef UDR1 //second usart
DECLARE_HW_USART(Usart1Traits, UDR1, UCSR1A, UCSR1B, UCSR1C, UBRR1L, UBRR1H)
#endif


template<int TxSize, int RxSize, class Traits=Usart0Traits>
class Usart
{
public:
	static inline void Init(unsigned long baund)
	{
		Traits::SetBaundRate(baund);
		Traits::EnableTxRx();
	}

	static uint8_t Putch(uint8_t c)//__attribute__ ((noinline))
	{
		if(_tx.IsEmpty())
		{
			while(!Traits::CanWriteData());
			Traits::Write(c);
			Traits::TXIntEnable();
			return 1;
		}else
		return _tx.Write(c);
	}

	static uint8_t Getch(uint8_t &c)//__attribute__ ((noinline))
	{
		return _rx.Read(c);
	}

	static inline void TxHandler()
	{
		uint8_t c;
		if(_tx.Read(c))
			Traits::Write(c);
		else
			Traits::TXIntDisable();
	}

	static	inline void RxHandler()
	{
		if(!_rx.Write(Traits::Read()))//buffer overlow
		{
			//TODO: error handling
			_rx.Clear();
		}
	}

	static void DropBuffers()
	{
		_rx.Clear();
	}

	static void Disable()
	{
		Traits::Disable();
		_rx.Clear();
		_tx.Clear();
	}

	static uint8_t BytesRecived()
	{
		return _rx.Count();
	}

	static void BeginTxFrame()
	{}

	static void EndTxFrame()
	{}

	static void BeginRx()
	{}

	static void EndRx()
	{}


private:
	static Queue<RxSize> _rx;
	static Queue<TxSize> _tx;
};

template<int TxSize, int RxSize, class Traits>
	Queue<RxSize> Usart<TxSize, RxSize, Traits>::_rx;
template<int TxSize, int RxSize, class Traits>
	Queue<TxSize> Usart<TxSize, RxSize, Traits>::_tx;



#endif
