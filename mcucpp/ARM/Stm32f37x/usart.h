
#pragma once
#include <clock.h>
#include <ioreg.h>
#include <static_assert.h>
#include <stm32f37x.h>
#include <iopins.h>
#include <pinlist.h>
#include <stddef.h>
#include <dma.h>

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
			
			ParityErrorInt = USART_ISR_PE,
			TxEmptyInt     = USART_ISR_TXE,
			TxCompleteInt  = USART_ISR_TC,
			RxNotEmptyInt  = USART_ISR_RXNE,
			IdleInt        = USART_ISR_IDLE,
			
			LineBreakInt   = USART_ISR_LBD,
			
			ErrorInt       = USART_ISR_FE | USART_ISR_NE | USART_ISR_ORE,
			CtsInt         = USART_ISR_CTSIF,
			AllInterrupts  = ParityErrorInt | TxEmptyInt | TxCompleteInt | RxNotEmptyInt | IdleInt | LineBreakInt | ErrorInt | CtsInt
		};
		
		enum Error
		{
			NoError = 0,
			OverrunError = USART_ISR_ORE,
			NoiseError = USART_ISR_NE,
			FramingError = USART_ISR_FE,
			ParityError = USART_ISR_PE
		};

	protected:
	
		static const unsigned ErrorMask = USART_ISR_ORE | USART_ISR_NE | USART_ISR_FE | USART_ISR_PE;
		static const unsigned InterruptMask = USART_ISR_ORE | USART_ISR_CTSIF |
				USART_ISR_PE | USART_ISR_TXE | USART_ISR_TC | USART_ISR_RXNE |
				USART_ISR_IDLE | USART_ISR_LBD | USART_ISR_FE | USART_ISR_NE;
		
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
		
		static const uint8_t UsartAltFuncNumber = 7;
	};

	inline UsartBase::UsartMode operator|(UsartBase::UsartMode left, UsartBase::UsartMode right)
	{	return static_cast<UsartBase::UsartMode>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}
	
	inline UsartBase::InterrupFlags operator|(UsartBase::InterrupFlags left, UsartBase::InterrupFlags right)
	{	return static_cast<UsartBase::InterrupFlags>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}

	inline UsartBase::Error operator|(UsartBase::Error left, UsartBase::Error right)
	{	return static_cast<UsartBase::Error>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}


	namespace Private
	{
		template<class Regs, IRQn_Type IQRNumber, class ClockCtrl, class TxPins, class RxPins, class DmaChannel>
		class Usart :public UsartBase
		{
		public:
			typedef DmaChannel Dma;
			
			template<unsigned long baud>
			static inline void Init(UsartMode usartMode = Default)
			{
				Init(baud, usartMode);
			}
			
			static void Init(unsigned baud, UsartMode usartMode = Default)
			{
				ClockCtrl::Enable();
				Regs()->CR1 = 0;
				Regs()->CR2 = 0;
				//SelectTxPin<0>();
				//SelectRxPin<0>();
				unsigned brr = ClockCtrl::ClockFreq() / baud;
				Regs()->BRR = brr;
				Regs()->CR1 = (((usartMode >> CR1ModeShift) & CR1ModeMask) | USART_CR1_UE );
				Regs()->CR2 = ((usartMode >> CR2ModeShift) & CR1ModeMask);
			}

			static void Write(uint8_t c)
			{
				while(!WriteReady())
					;
				Regs()->TDR = c;
			}
			
			static uint8_t Read()
			{
				while(!ReadReady())
					;
				return Regs()->RDR;
			}

			static bool WriteReady()
			{
				bool dmaActive = (Regs()->CR3 & USART_CR3_DMAT) && !DmaChannel::Ready();
				
				if(dmaActive)
					return false;
				
				return ((Regs()->ISR & USART_ISR_TXE) != 0);
			}

			static bool ReadReady()
			{
				return (Regs()->ISR & USART_ISR_RXNE) != 0;
			}
			
			static void EnableInterrupt(InterrupFlags interruptFlags)
			{
				uint32_t cr1Mask = 0;
				uint32_t cr2Mask = 0;
				uint32_t cr3Mask = 0;
				
				if(interruptFlags & ParityErrorInt) 
					cr1Mask |= USART_CR1_PEIE;
					
				STATIC_ASSERT(
						USART_CR1_TXEIE  == USART_ISR_TXE &&
						USART_CR1_TCIE   == USART_ISR_TC &&
						USART_CR1_RXNEIE == USART_ISR_RXNE &&
						USART_CR1_IDLEIE == USART_ISR_IDLE
						);
			
				cr1Mask |= interruptFlags & (USART_CR1_TXEIE | USART_CR1_TCIE | USART_CR1_RXNEIE | USART_CR1_IDLEIE);
				
				if(interruptFlags & LineBreakInt) 
					cr2Mask |= USART_CR2_LBDIE;
				
				if(interruptFlags & ErrorInt)
					cr3Mask |= USART_CR3_EIE;
					
				if(interruptFlags & CtsInt)
					cr3Mask |= USART_CR3_CTSIE;
					
				Regs()->CR1 |= cr1Mask;
				Regs()->CR2 |= cr2Mask;
				Regs()->CR3 |= cr3Mask;
					
				if(interruptFlags)
					NVIC_EnableIRQ(IQRNumber);
			}
			
			static void DisableInterrupt(InterrupFlags interruptFlags)
			{
				uint32_t cr1Mask = 0;
				uint32_t cr2Mask = 0;
				uint32_t cr3Mask = 0;
				
				if(interruptFlags & ParityErrorInt) 
					cr1Mask |= USART_CR1_PEIE;
					
				STATIC_ASSERT(
						USART_CR1_TXEIE  == USART_ISR_TXE &&
						USART_CR1_TCIE   == USART_ISR_TC &&
						USART_CR1_RXNEIE == USART_ISR_RXNE &&
						USART_CR1_IDLEIE == USART_ISR_IDLE
						);
			
				cr1Mask |= interruptFlags & (USART_CR1_TXEIE | USART_CR1_TCIE | USART_CR1_RXNEIE | USART_CR1_IDLEIE);
				
				if(interruptFlags & LineBreakInt) 
					cr2Mask |= USART_CR2_LBDIE;
				
				if(interruptFlags & ErrorInt)
					cr3Mask |= USART_CR3_EIE;
					
				if(interruptFlags & CtsInt)
					cr3Mask |= USART_CR3_CTSIE;
					
				Regs()->CR1 &= ~cr1Mask;
				Regs()->CR2 &= ~cr2Mask;
				Regs()->CR3 &= ~cr3Mask;
			}
			
			static InterrupFlags InterruptSource()
			{
				return static_cast<InterrupFlags>(Regs()->ISR & InterruptMask);
			}
			
			static Error GetError()
			{
				return static_cast<Error>(Regs()->ISR & ErrorMask);
			}
			
			static void ClearInterruptFlag(InterrupFlags interruptFlags)
			{
				STATIC_ASSERT(
					USART_ICR_PECF   == USART_ISR_PE && 
					USART_ICR_FECF   == USART_ISR_FE &&
					USART_ICR_NCF    == USART_ISR_NE &&
					USART_ICR_ORECF  == USART_ISR_ORE &&
					USART_ICR_IDLECF == USART_ISR_IDLE &&
					USART_ICR_TCCF   == USART_ISR_TC &&
					USART_ICR_LBDCF  == USART_ISR_LBD &&
					USART_ICR_CTSCF  == USART_ISR_CTSIF);
				Regs()->ICR |= interruptFlags;
			}
			
			template<uint8_t TxPinNumber, uint8_t RxPinNumber>
			static void SelectTxRxPins()
			{
				typedef typename TxPins:: template Pin<TxPinNumber> TxPin;
				TxPin::Port::Enable();
				TxPin::SetConfiguration(TxPin::Port::AltFunc);
				TxPin::AltFuncNumber(UsartAltFuncNumber);
				
				typedef typename RxPins:: template Pin<RxPinNumber> RxPin;
				RxPin::Port::Enable();
				RxPin::SetConfiguration(RxPin::Port::AltFunc);
				RxPin::AltFuncNumber(UsartAltFuncNumber);
			}

			static void SelectTxRxPins(uint8_t TxPinNumber, uint8_t RxPinNumber)
			{
				typedef typename TxPins::ValueType Type;
				Type maskTx (1 << TxPinNumber);
				TxPins::SetConfiguration(maskTx, TxPins::AltFunc);
				TxPins::AltFuncNumber(maskTx, UsartAltFuncNumber);

				Type maskRx (1 << RxPinNumber);
				RxPins::SetConfiguration(maskRx, RxPins::AltFunc);
				RxPins::AltFuncNumber(maskRx, UsartAltFuncNumber);
			}
			
			static void Write(const void *data, size_t size, bool async = false)
			{
				if(async && size > 1)
				{
					while(!WriteReady())
						;
					DmaChannel::ClearFlags();
					Regs()->CR3 |= USART_CR3_DMAT;
					Regs()->ISR &= ~USART_ISR_TC;
					DmaChannel::Init(DmaChannel::Mem2Periph | DmaChannel::MemIncriment, data, &Regs()->TDR, size);
				}
				else
				{
					uint8_t *ptr = (uint8_t*)data;
					while(size--)
					{
						Write(*ptr);
						ptr++;
					}
				}
			}

		};
		
		typedef IO::PinList<IO::Pa9,  IO::Pb6, IO::Pc4, IO::Pe0> Usart1TxPins;
		typedef IO::PinList<IO::Pa10, IO::Pb7, IO::Pc5, IO::Pe1> Usart1RxPins;
		
		typedef IO::PinList<IO::Pa2, IO::Pb3, IO::Pd5> Usart2TxPins;
		typedef IO::PinList<IO::Pa3, IO::Pb4, IO::Pd6> Usart2RxPins;
		
		typedef IO::PinList<IO::Pb8, IO::Pc10, IO::Pd8, IO::Pb10> Usart3TxPins;
		typedef IO::PinList<IO::Pb9, IO::Pa11, IO::Pd9, IO::Pe15> Usart3RxPins;
		
	}

#define DECLARE_USART(REGS, IRQ, CLOCK, className, DmaChannel) \
	namespace Private \
	{\
		IO_STRUCT_WRAPPER(REGS, className ## _REGS, USART_TypeDef);\
	}\
	typedef Private::Usart<\
		Private::className ## _REGS, \
		IRQ,\
		CLOCK,\
		Private::className ## TxPins, \
		Private::className ## RxPins, \
		DmaChannel\
		> className

		DECLARE_USART(USART1, USART1_IRQn, Clock::Usart1Clock, Usart1, Dma1Channel4);
		DECLARE_USART(USART2, USART2_IRQn, Clock::Usart2Clock, Usart2, Dma1Channel7);
		DECLARE_USART(USART3, USART3_IRQn, Clock::Usart3Clock, Usart3, Dma1Channel2);
		
		#define MCUCPP_HAS_USART1 1
		#define MCUCPP_HAS_USART2 1
		#define MCUCPP_HAS_USART3 1
}
