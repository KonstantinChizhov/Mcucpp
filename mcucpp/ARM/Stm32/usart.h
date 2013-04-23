
#pragma once
#include <clock.h>
#include <ioreg.h>
#include <static_assert.h>

namespace Mcucpp
{
	class UsartBase
	{
		public:
		enum UsartMode
		{
			DataBits8 = 0,
			DataBits9 = USART_CR1_M,

			NoneParity = 0,
			EvenParity = USART_CR1_PCE,
			OddParity  = USART_CR1_PS | USART_CR1_PCE,

			NoClock = 0,

			Disabled = 0,
			RxEnable = USART_CR1_RE,
			TxEnable = USART_CR1_TE,
			RxTxEnable  = USART_CR1_RE | USART_CR1_TE,
			Default = RxTxEnable,

			OneStopBit         = 0,
			HalfStopBit        = USART_CR2_STOP_0 << 16,
			TwoStopBits        = USART_CR2_STOP_1 << 16,
			OneAndHalfStopBits = (USART_CR2_STOP_0 | USART_CR2_STOP_1) << 16
		};
		
		enum InterrupFlags
		{
			NoInterrupt = 0,
			
			ParityErrorInt = USART_CR1_PEIE,
			TxEmptyInt     = USART_CR1_TXEIE,
			TxCompleteInt  = USART_CR1_TCIE,
			RxNotEmptyInt  = USART_CR1_RXNEIE,
			IdleInt        = USART_CR1_IDLEIE,
			
			LineBreakInt   = USART_CR2_LBDIE << 4,
			
			ErrorInt       = USART_CR3_EIE << 16,
			CtsInt         = USART_CR3_CTSIE << 16
		};
		
		enum Error
		{
			NoError = 0,
			OverrunError = USART_SR_ORE,
			NoiseError = USART_SR_NE,
			FramingError = USART_SR_FE,
			ParityError = USART_SR_PE
		};

		static const int EOF = -1;
	protected:
	
		static const unsigned ErrorMask = USART_SR_ORE | USART_SR_NE | USART_SR_FE | USART_SR_PE;
		
		static const unsigned CR1ModeMask = 
			USART_CR1_M |
			USART_CR1_PCE |
			USART_CR1_PS |
			USART_CR1_RE |
			USART_CR1_TE;
		
		static const unsigned CR2ModeMask = USART_CR2_STOP_0 | USART_CR2_STOP_1;
		enum
		{
			CR1ModeShift = 0,
			CR2ModeShift = 16
		};
		
		static const unsigned CR1InterruptMask = 
			USART_CR1_PEIE | 
			USART_CR1_TXEIE | 
			USART_CR1_TCIE | 
			USART_CR1_RXNEIE |
			USART_CR1_IDLEIE;
			
		static const unsigned CR2InterruptMask = USART_CR2_LBDIE;
		static const unsigned CR3InterruptMask = USART_CR3_EIE | USART_CR3_CTSIE;
		
		enum
		{
			CR1InterrupFlagsShift = 0,
			CR2InterrupFlagsShift = 4,
			CR3InterrupFlagsShift = 16
		};
			
		BOOST_STATIC_ASSERT(
			((CR1InterruptMask << CR1InterrupFlagsShift) & 
			(CR2InterruptMask << CR2InterrupFlagsShift) &
			(CR3InterruptMask << CR3InterrupFlagsShift)) == 0
			);
	};

	inline UsartBase::UsartMode operator|(UsartBase::UsartMode left, UsartBase::UsartMode right)
	{	return static_cast<UsartBase::UsartMode>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}
	
	inline UsartBase::InterrupFlags operator|(UsartBase::InterrupFlags left, UsartBase::InterrupFlags right)
	{	return static_cast<UsartBase::InterrupFlags>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}

	inline UsartBase::Error operator|(UsartBase::Error left, UsartBase::Error right)
	{	return static_cast<UsartBase::Error>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}


	namespace Private
	{
		template<class Regs, IRQn_Type IQRNumber, class ClockCtrl>
		class Usart :public UsartBase
		{
		public:
			static void Init(unsigned baud, UsartMode flags = Default)
			{
				ClockCtrl::Enable();
				unsigned brr = Clock::SysClock::FPeriph() / baud;
				Regs()->BRR = brr;
				Regs()->CR1 = (((flags >> CR1ModeShift) & CR1ModeMask) | USART_CR1_UE );
				Regs()->CR2 = ((flags >> CR2ModeShift) & CR1ModeMask);
			}

			static void Write(uint8_t c)
			{
				while((Regs()->SR & USART_SR_TXE) == 0);
				Regs()->DR = c;
			}
			
			static uint8_t Read()
			{
				if(RxReady())
					return Regs()->DR;
				return 0;
			}

			static bool TxReady()
			{
				return (Regs()->SR & USART_SR_TXE) == 0;
			}

			static bool RxReady()
			{
				return Regs()->SR & USART_SR_RXNE;
			}
			
			static void EnableInterrupt(InterrupFlags flags)
			{
				Regs()->CR1 = ((flags >> CR1InterrupFlagsShift) & CR1InterruptMask) | (Regs()->CR1 & ~CR1InterruptMask);
				Regs()->CR2 = ((flags >> CR2InterrupFlagsShift) & CR2InterruptMask) | (Regs()->CR2 & ~CR2InterruptMask);
				Regs()->CR3 = ((flags >> CR3InterrupFlagsShift) & CR3InterruptMask) | (Regs()->CR3 & ~CR3InterruptMask);
				if(flags)
					NVIC_EnableIRQ(IQRNumber);
			}
			
			static InterrupFlags InterruptSource()
			{
				InterrupFlags result = NoInterrupt;
				if(GetError() != NoError)
					result = result | ErrorInt;
					
				if(Regs()->SR & USART_SR_CTS)
					result = result | CtsInt;
					
				if(Regs()->SR & USART_SR_LBD)
					result = result | LineBreakInt;
					
				if(Regs()->SR & USART_SR_TXE)
					result = result | TxEmptyInt;
					
				if(Regs()->SR & USART_SR_TC)
					result = result | TxCompleteInt;
					
				if(Regs()->SR & USART_SR_RXNE)
					result = result | RxNotEmptyInt;
					
				if(Regs()->SR & USART_SR_IDLE)
					result = result | IdleInt;
				return NoInterrupt;
			}
			
			static Error GetError()
			{
				return static_cast<Error>(Regs()->SR & ErrorMask);
			}
			

			class Dma
			{
				public:
				//typedef Dma1Channel4 ChannelTx;
				static void EnableTx()
				{

				}

			};
		};
	}

#define DECLARE_USART(REGS, IRQ, CLOCK, className) \
	namespace Private \
	{\
		IO_STRUCT_WRAPPER(REGS, className ## _REGS, USART_TypeDef);\
	}\
	typedef Private::Usart<\
		Private::className ## _REGS, \
		IRQ,\
		CLOCK\
		> className

		DECLARE_USART(USART1, USART1_IRQn, Clock::Usart1Clock, Usart1);
		DECLARE_USART(USART2, USART2_IRQn, Clock::Usart2Clock, Usart2);
}
