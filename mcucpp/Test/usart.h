
#ifndef USART_HPP
#define USART_HPP
#include <stdint.h>

namespace Mcucpp
{

class UsartBase
{
public:
	
	enum InterruptFlags
	{
		NoInterrupt = 0,
		RxNotEmpty = 1,
		TxCompleteInt = 2,
		TxEmpty = 4,
		AllInterrupts = RxNotEmpty | TxCompleteInt | TxEmpty
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

class TestUsart :public UsartBase
{
public:
	static bool txReady;
	static bool rxReady;
	static InterruptFlags interruptFlags;
	static Error error;
	static uint8_t data;

	template<unsigned long baund>
	static inline void SetBaundRate()
	{
		
	}

	static inline void SetBaundRate(unsigned long baund)
	{
		
	}

	static void Disable()
	{
		
	}

	template<unsigned long baund>
	static inline void Init(UsartMode usartMode = Default)
	{
		
	}

	static inline void Init(unsigned long baund, UsartMode usartMode = Default)
	{
	
	}

	static void EnableInterrupts(InterruptFlags interruptSources = AllInterrupts)
	{
		
	}

	static void DisableInterrupts(InterruptFlags interruptSources = AllInterrupts)
	{
		
	}

	static void Write(uint8_t c)
	{
		data = c;
	}

	static uint8_t Read()
	{
		return data;
	}

	static bool TxReady()
	{
		return txReady;
	}

	static bool RxReady()
	{
		return rxReady;
	}
		
	static InterruptFlags InterruptSource()
	{
		return interruptFlags;
	}
	
	static Error GetError()
	{
		return error;
	}

	static void EnableTx()
	{
		
	}

	static void DisableTx()
	{
		
	}
};

bool TestUsart::txReady = false;
bool TestUsart::rxReady = false;
UsartBase::InterruptFlags TestUsart::interruptFlags = UsartBase::NoInterrupt;
UsartBase::Error TestUsart::error = UsartBase::NoError;
uint8_t TestUsart::data = 0;

}

#endif
