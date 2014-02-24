
#pragma once
#include <clock.h>
#include <ioreg.h>
#include <static_assert.h>
#include "stm32f4xx.h"
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
			
			ParityErrorInt = USART_SR_PE,
			TxEmptyInt     = USART_SR_TXE,
			TxCompleteInt  = USART_SR_TC,
			RxNotEmptyInt  = USART_SR_RXNE,
			IdleInt        = USART_SR_IDLE,
			
			LineBreakInt   = USART_SR_LBD,
			
			ErrorInt       = USART_SR_FE | USART_SR_NE | USART_SR_ORE,
			CtsInt         = USART_SR_CTS,
			AllInterrupts  = ParityErrorInt | TxEmptyInt | TxCompleteInt | RxNotEmptyInt | IdleInt | LineBreakInt | ErrorInt | CtsInt
		};
		
		enum Error
		{
			NoError = 0,
			OverrunError = USART_SR_ORE,
			NoiseError = USART_SR_NE,
			FramingError = USART_SR_FE,
			ParityError = USART_SR_PE
		};

	protected:
	
		static const unsigned ErrorMask = USART_SR_ORE | USART_SR_NE | USART_SR_FE | USART_SR_PE;
		static const unsigned InterruptMask = USART_SR_ORE | USART_SR_CTS |
				USART_SR_PE | USART_SR_TXE | USART_SR_TC | USART_SR_RXNE |
				USART_SR_IDLE | USART_SR_LBD | USART_SR_FE | USART_SR_NE;
		
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
		
	};
	
	inline UsartBase::UsartMode operator|(UsartBase::UsartMode left, UsartBase::UsartMode right)
	{	return static_cast<UsartBase::UsartMode>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}
	
	inline UsartBase::InterrupFlags operator|(UsartBase::InterrupFlags left, UsartBase::InterrupFlags right)
	{	return static_cast<UsartBase::InterrupFlags>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}

	inline UsartBase::Error operator|(UsartBase::Error left, UsartBase::Error right)
	{	return static_cast<UsartBase::Error>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}


	namespace Private
	{
		template
		<
			class Regs, 
			IRQn_Type IQRNumber, 
			class ClockCtrl, 
			class TxPins, 
			class RxPins, 
			class DmaTxChannel, 
			class DmaRxChannel, 
			uint8_t DmaTxChannelNum,
			uint8_t DmaRxChannelNum,
			uint8_t UsartAltFuncNumber
		>
		class Usart :public UsartBase
		{
		public:
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
				Regs()->CR2 = ((usartMode >> CR2ModeShift) & CR1ModeMask);
				Regs()->CR1 = (((usartMode >> CR1ModeShift) & CR1ModeMask) | USART_CR1_UE );
			}

			static void Write(uint8_t c)
			{
				while(!TxReady())
					;
				Regs()->DR = c;
			}
			
			static uint8_t Read()
			{
				while(!RxReady())
					;
				return Regs()->DR;
			}

			static bool TxReady()
			{
				bool dmaActive = (Regs()->CR3 & USART_CR3_DMAT) && DmaTxChannel::Enabled();
				return (!dmaActive || DmaTxChannel::TrasferComplete()) && ((Regs()->SR & USART_SR_TXE) != 0);
			}

			static bool RxReady()
			{
				return (Regs()->SR & USART_SR_RXNE) != 0;
			}
			
			static void EnableInterrupt(InterrupFlags interruptFlags)
			{
				uint32_t cr1Mask = 0;
				uint32_t cr2Mask = 0;
				uint32_t cr3Mask = 0;
				
				if(interruptFlags & ParityErrorInt) 
					cr1Mask |= USART_CR1_PEIE;
					
				STATIC_ASSERT(
						USART_CR1_TXEIE  == USART_SR_TXE &&
						USART_CR1_TCIE   == USART_SR_TC &&
						USART_CR1_RXNEIE == USART_SR_RXNE &&
						USART_CR1_IDLEIE == USART_SR_IDLE
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
						USART_CR1_TXEIE  == USART_SR_TXE &&
						USART_CR1_TCIE   == USART_SR_TC &&
						USART_CR1_RXNEIE == USART_SR_RXNE &&
						USART_CR1_IDLEIE == USART_SR_IDLE
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
				return static_cast<InterrupFlags>(Regs()->SR & InterruptMask);
			}
			
			static Error GetError()
			{
				return static_cast<Error>(Regs()->SR & ErrorMask);
			}
			
			static void ClearInterruptFlag(InterrupFlags interruptFlags)
			{
				Regs()->SR &= ~interruptFlags;
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
					while(!TxReady())
						;
					DmaTxChannel::ClearFlags();
					Regs()->CR3 |= USART_CR3_DMAT;
					Regs()->SR &= ~USART_SR_TC;
					DmaTxChannel::Transfer(DmaTxChannel::Mem2Periph | DmaTxChannel::MemIncriment, data, &Regs()->DR, size, DmaTxChannelNum);
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
			
			static void Read(void *data, size_t size, bool async = false)
			{
				uint8_t *ptr = (uint8_t*)data;
				if(async && size > 1)
				{
					if(RxReady())
					{
						*ptr = Read();
						ptr++;
						size--;
					}
					Regs()->CR3 |= USART_CR3_DMAR;
					Regs()->SR &= ~USART_SR_RXNE;
					DmaRxChannel::Transfer(DmaRxChannel::Periph2Mem | DmaRxChannel::MemIncriment, ptr, &Regs()->DR, size, DmaRxChannelNum);
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
			
			static void SetTxCompleteCallback(TransferCallback callback)
			{
				DmaTxChannel::SetTransferCallback(callback);
			}
			
			static void SetRxCompleteCallback(TransferCallback callback)
			{
				DmaRxChannel::SetTransferCallback(callback);
			}
			
			static void Break()
			{
				while(!TxReady())
					;
				Regs()->CR1 |= USART_CR1_SBK;
			}

		};
	#if defined(STM32F40_41xxx)
		typedef IO::PinList<IO::Pa9,  IO::Pb6> Usart1TxPins;
		typedef IO::PinList<IO::Pa10, IO::Pb7> Usart1RxPins;
		
		typedef IO::PinList<IO::Pa2,  IO::Pd5> Usart2TxPins;
		typedef IO::PinList<IO::Pa3,  IO::Pd6> Usart2RxPins;
		
		typedef IO::PinList<IO::Pb10, IO::Pc10, IO::Pd8> Usart3TxPins;
		typedef IO::PinList<IO::Pb11, IO::Pc11, IO::Pd9> Usart3RxPins;
		
		typedef IO::PinList<IO::Pa0,  IO::Pc10> Uart4TxPins;
		typedef IO::PinList<IO::Pa1,  IO::Pc11> Uart4RxPins;
		
		typedef IO::PinList<IO::Pc12> Uart5TxPins;
		typedef IO::PinList<IO::Pd2> Uart5RxPins;
		
		typedef IO::PinList<IO::Pc6, IO::Pg14> Usart6TxPins;
		typedef IO::PinList<IO::Pc7, IO::Pg9> Usart6RxPins;
		
		
	#else
	#error TODO: add USART pins description
	#endif
	
		IO_STRUCT_WRAPPER(USART1, Usart1Regs, USART_TypeDef);
		IO_STRUCT_WRAPPER(USART2, Usart2Regs, USART_TypeDef);
		IO_STRUCT_WRAPPER(USART3, Usart3Regs, USART_TypeDef);
		IO_STRUCT_WRAPPER(UART4, Uart4Regs, USART_TypeDef);
		IO_STRUCT_WRAPPER(UART5, Uart5Regs, USART_TypeDef);
		IO_STRUCT_WRAPPER(USART6, Usart6Regs, USART_TypeDef);
	}
	
	typedef Private::Usart<Private::Usart1Regs, USART1_IRQn, Clock::Usart1Clock, Private::Usart1TxPins, Private::Usart1RxPins, Dma2Channel7, Dma2Channel2, 4, 4, 7> Usart1;
	typedef Private::Usart<Private::Usart2Regs, USART2_IRQn, Clock::Usart2Clock, Private::Usart2TxPins, Private::Usart2RxPins, Dma1Channel6, Dma1Channel5, 4, 4, 7> Usart2;
	typedef Private::Usart<Private::Usart3Regs, USART3_IRQn, Clock::Usart3Clock, Private::Usart3TxPins, Private::Usart3RxPins, Dma1Channel3, Dma1Channel1, 4, 4, 7> Usart3;
	typedef Private::Usart<Private::Uart4Regs,  UART4_IRQn,  Clock::Uart4Clock,  Private::Uart4TxPins,  Private::Uart4RxPins,  Dma1Channel4, Dma1Channel2, 4, 4, 8> Uart4;
	typedef Private::Usart<Private::Uart5Regs,  UART5_IRQn,  Clock::Uart5Clock,  Private::Uart5TxPins,  Private::Uart5RxPins,  Dma2Channel7, Dma2Channel0, 4, 4, 8> Uart5;
	typedef Private::Usart<Private::Usart6Regs, USART6_IRQn, Clock::Usart6Clock, Private::Usart6TxPins, Private::Usart6RxPins, Dma2Channel7, Dma2Channel2, 5, 5, 8> Usart6;
	
	#define MCUCPP_HAS_USART1 1
	#define MCUCPP_HAS_USART2 1
	#define MCUCPP_HAS_USART3 1
	#define MCUCPP_HAS_USART4 1
	#define MCUCPP_HAS_USART5 1
	#define MCUCPP_HAS_USART6 1
}
