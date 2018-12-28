//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2018
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
#include <clock.h>
#include <ioreg.h>
#include <static_assert.h>
#include <mcu_header.h>
#include <iopins.h>
#include <pinlist.h>
#include <stddef.h>
#include <dma.h>
#include <dispatcher.h>

namespace Mcucpp
{
	class UsartBase
	{
	public:
		enum UsartMode
		{
			DataBits7 = USART_CR1_M1,
			DataBits8 = 0,
			DataBits9 = USART_CR1_M0,

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
			HalfStopBit        = (uint64_t)USART_CR2_STOP_0 << 32,
			TwoStopBits        = (uint64_t)USART_CR2_STOP_1 << 32,
			OneAndHalfStopBits = (uint64_t)(USART_CR2_STOP_0 | USART_CR2_STOP_1) << 32
		};

		enum InterrupFlags
		{
			NoInterrupt = 0,

			ParityErrorInt = USART_ISR_PE,
			TxEmptyInt     = USART_ISR_TXE,
			TxCompleteInt  = USART_ISR_TC,
			RxNotEmptyInt  = USART_ISR_RXNE,
			IdleInt        = USART_ISR_IDLE,
			LineBreakInt   = USART_ISR_LBDF,

			ErrorInt       = USART_ISR_FE | USART_ISR_NE | USART_ISR_ORE | USART_ISR_PE,
			CtsInt         = USART_ISR_CTS,
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
		static const unsigned InterruptMask = USART_ISR_ORE | USART_ISR_CTS |
				USART_ISR_PE | USART_ISR_TXE | USART_ISR_TC | USART_ISR_RXNE |
				USART_ISR_IDLE | USART_ISR_LBDF | USART_ISR_FE | USART_ISR_NE;

		static const unsigned CR2ModeMask = USART_CR2_STOP_0 | USART_CR2_STOP_1;
		enum
		{
			CR1ModeShift = 0,
			CR2ModeShift = 32
		};

	};

	inline UsartBase::UsartMode operator|(UsartBase::UsartMode left, UsartBase::UsartMode right)
	{	return static_cast<UsartBase::UsartMode>(static_cast<uint64_t>(left) | static_cast<uint64_t>(right));	}

	inline UsartBase::InterrupFlags operator|(UsartBase::InterrupFlags left, UsartBase::InterrupFlags right)
	{	return static_cast<UsartBase::InterrupFlags>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}

	inline UsartBase::Error operator|(UsartBase::Error left, UsartBase::Error right)
	{	return static_cast<UsartBase::Error>(static_cast<unsigned>(left) | static_cast<unsigned>(right));	}


	namespace Private
	{
		class UsartData
		{
        public:
			UsartData()
				:rxTimeoutMs(0),
				rxSize(0)
			{
			}

			//TransferCallbackFunc txCallback;
			TransferCallbackFunc rxCallback;
			unsigned rxTimeoutMs;
			size_t rxSize;
		};

	#define USART_TEMPLATE_ARGS	template \
		<\
			class Regs, \
			IRQn_Type IQRNumber, \
			class ClockCtrl, \
			class TxPins, \
			class RxPins, \
			class DmaTxChannel, \
			class DmaRxChannel, \
			typename DmaTxChannel::RequestType DmaTxChannelNum,\
			typename DmaRxChannel::RequestType DmaRxChannelNum,\
			uint8_t UsartAltFuncNumber\
		>

		#define USART_TEMPLATE_QUALIFIER	Usart<\
			Regs, \
			IQRNumber, \
			ClockCtrl, \
			TxPins, \
			RxPins, \
			DmaTxChannel, \
			DmaRxChannel, \
			DmaTxChannelNum,\
			DmaRxChannelNum,\
			UsartAltFuncNumber\
		>

		USART_TEMPLATE_ARGS
		class Usart :public UsartBase
		{
			static UsartData _data;
			static void OnReadTimeout(void *);

		public:
			template<unsigned long baud>
			static inline void Init(UsartMode usartMode = Default)
			{
				Init(baud, usartMode);
			}

			static void Init(unsigned baud, UsartMode usartMode = Default);

			static void Write(uint8_t c);

			static uint8_t Read();

			static bool WriteReady();

			static bool ReadReady();

			static void EnableInterrupt(InterrupFlags interruptFlags);

			static void DisableInterrupt(InterrupFlags interruptFlags);

			static InterrupFlags InterruptSource();

			static Error GetError();

			static bool Write(const void *data, size_t size);

			static bool Read(void *data, size_t size);

			static bool WriteAsync(const void *data, size_t size, TransferCallbackFunc callback);

			static bool ReadAsync(void *data, size_t size, TransferCallbackFunc callback);

			static void Break();

			static void SetRxTimeout(unsigned rxTimeoutMs) { _data.rxTimeoutMs = rxTimeoutMs; }

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
				TxPins::Enable();
				Type maskTx (1 << TxPinNumber);
				TxPins::SetConfiguration(maskTx, TxPins::AltFunc);
				TxPins::AltFuncNumber(maskTx, UsartAltFuncNumber);

				RxPins::Enable();
				Type maskRx (1 << RxPinNumber);
				RxPins::SetConfiguration(maskRx, RxPins::AltFunc);
				RxPins::AltFuncNumber(maskRx, UsartAltFuncNumber);
			}

			template<class TxPin, class RxPin>
			static void SelectTxRxPins()
			{
				const int txPinIndex = TxPins::template PinIndex<TxPin>::Value;
				const int rxPinIndex = RxPins::template PinIndex<RxPin>::Value;
				STATIC_ASSERT(txPinIndex >= 0);
				STATIC_ASSERT(rxPinIndex >= 0);
				SelectTxRxPins<txPinIndex, rxPinIndex>();
			}
		};


		USART_TEMPLATE_ARGS
		void USART_TEMPLATE_QUALIFIER::Init(unsigned baud, UsartMode usartMode)
		{
			ClockCtrl::Enable();
			Regs()->CR1 = 0;
			Regs()->CR2 = 0;
			if(&(Regs()->CR1) == &LPUART1->CR1)
			{
				Regs()->BRR = 256*ClockCtrl::ClockFreq() / baud;
			}
			else
			{
				Regs()->BRR = ClockCtrl::ClockFreq() / baud;
			}
			Regs()->CR2 = (usartMode >> CR2ModeShift);
			Regs()->CR1 = ((usartMode >> CR1ModeShift) | USART_CR1_UE );
		}

		USART_TEMPLATE_ARGS
		void USART_TEMPLATE_QUALIFIER::Write(uint8_t c)
		{
			while(!WriteReady())
				;
			Regs()->TDR = c;
		}

		USART_TEMPLATE_ARGS
		uint8_t USART_TEMPLATE_QUALIFIER::Read()
		{
			while(!ReadReady())
				;
			return Regs()->RDR;
		}

		USART_TEMPLATE_ARGS
		bool USART_TEMPLATE_QUALIFIER::WriteReady()
		{
			bool dmaActive = (Regs()->CR3 & USART_CR3_DMAT) && DmaTxChannel::Enabled();
			return (!dmaActive || DmaTxChannel::TrasferComplete()) && ((Regs()->ISR & USART_ISR_TXE) != 0);
		}

		USART_TEMPLATE_ARGS
		 bool USART_TEMPLATE_QUALIFIER::ReadReady()
		{
			return (Regs()->ISR & USART_ISR_RXNE) != 0;
		}

		USART_TEMPLATE_ARGS
		void USART_TEMPLATE_QUALIFIER::EnableInterrupt(InterrupFlags interruptFlags)
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

			if(interruptFlags != NoInterrupt)
				NVIC_EnableIRQ(IQRNumber);
		}

		USART_TEMPLATE_ARGS
		void USART_TEMPLATE_QUALIFIER::DisableInterrupt(InterrupFlags interruptFlags)
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

		USART_TEMPLATE_ARGS
		UsartBase::InterrupFlags USART_TEMPLATE_QUALIFIER::InterruptSource()
		{
			return static_cast<InterrupFlags>(Regs()->ISR & InterruptMask);
		}

		USART_TEMPLATE_ARGS
		UsartBase::Error USART_TEMPLATE_QUALIFIER::GetError()
		{
			return static_cast<Error>(Regs()->ISR & ErrorMask);
		}

		USART_TEMPLATE_ARGS
		void USART_TEMPLATE_QUALIFIER::Break()
		{
			while(!WriteReady())
				;
			Regs()->RQR |= USART_RQR_SBKRQ;
		}

		USART_TEMPLATE_ARGS
		bool USART_TEMPLATE_QUALIFIER::WriteAsync(const void *data, size_t size, TransferCallbackFunc callback)
		{
            if(!size || !data)
            {
               return false;
            }
			if(!DmaTxChannel::Ready())
				return false;

			DmaTxChannel::ClearFlags();

			Regs()->CR3 |= USART_CR3_DMAT;
			DmaTxChannel::SetTransferCallback(callback);
			DmaTxChannel::SetRequest(DmaTxChannelNum);
			Regs()->ICR = USART_ICR_TCCF;
			DmaTxChannel::Transfer(DmaMode::Mem2Periph | DmaMode::MemIncriment, data, &Regs()->TDR, size);
			Regs()->ICR = USART_ICR_TCCF;
			return true;
		}

		USART_TEMPLATE_ARGS
		bool USART_TEMPLATE_QUALIFIER::ReadAsync(void *data, size_t size, TransferCallbackFunc callback)
		{
            if(!size || !data)
            {
               return false;
            }
            if(!DmaRxChannel::Ready())
            {
                return false;
            }
            uint8_t *ptr = static_cast<uint8_t*>(data);
            Regs()->ICR = USART_ICR_PECF | USART_ICR_FECF | USART_ICR_NCF | USART_ICR_ORECF;
		    if(ReadReady())
            {
                *ptr = Read();
                ptr++;
                size--;
            }
			Regs()->CR3 |= USART_CR3_DMAR;
			_data.rxCallback = callback;
			_data.rxSize = size;
			DmaRxChannel::SetTransferCallback(callback);
            DmaRxChannel::SetRequest(DmaRxChannelNum);
            DmaRxChannel::Transfer(DmaMode::Periph2Mem | DmaMode::MemIncriment, ptr, &Regs()->RDR, size);
            if(_data.rxTimeoutMs > 0)
            {
                GetCurrentDispatcher().SetTimer(_data.rxTimeoutMs, OnReadTimeout, data);
            }

            return true;
		}

		USART_TEMPLATE_ARGS
		void USART_TEMPLATE_QUALIFIER::OnReadTimeout(void *data)
		{
		    DmaRxChannel::Disable();
		    DmaRxChannel::ClearFlags();
            if(_data.rxCallback)
            {
                size_t bytesTransfered = _data.rxSize - DmaRxChannel::RemainingTransfers();
                _data.rxCallback(data, bytesTransfered, false);
            }
		}

		USART_TEMPLATE_ARGS
		UsartData USART_TEMPLATE_QUALIFIER::_data;

	#undef USART_TEMPLATE_ARGS
	#undef USART_TEMPLATE_QUALIFIER

	#if defined(STM32L471xx)
		typedef IO::PinList<IO::Pa9,  IO::Pb6> Usart1TxPins;
		typedef IO::PinList<IO::Pa10, IO::Pb7> Usart1RxPins;

		typedef IO::PinList<IO::Pb11, IO::Pc1, IO::Pg7> LpUsart1TxPins;
		typedef IO::PinList<IO::Pb10, IO::Pc0, IO::Pg8> LpUsart1RxPins;

	#else
	#error TODO: add USART pins description
	#endif

		IO_STRUCT_WRAPPER(USART1, Usart1Regs, USART_TypeDef);
		IO_STRUCT_WRAPPER(LPUART1, LpUsartRegs, USART_TypeDef);
	}

	typedef Private::Usart<
		Private::Usart1Regs,
		USART1_IRQn,
		Clock::Usart1Clock,
		Private::Usart1TxPins,
		Private::Usart1RxPins,
		Dma2Channel6,
		Dma2Channel7,
		Dma2Channel6Request::Usart1_Tx,
		Dma2Channel7Request::Usart1_Rx,
		7>
			Usart1;

	typedef Private::Usart<
		Private::LpUsartRegs,
		LPUART1_IRQn,
		Clock::LpUart1Clock,
		Private::LpUsart1TxPins,
		Private::LpUsart1RxPins,
		Dma2Channel6,
		Dma2Channel7,
		Dma2Channel6Request::LpUart_Tx,
		Dma2Channel7Request::LpUart_Rx,
		8> LpUsart1;

	#define MCUCPP_HAS_USART1 1

}
