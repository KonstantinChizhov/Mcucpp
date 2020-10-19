
#pragma once
#include <clock.h>
#include <ioreg.h>
#include <static_assert.h>
#include <iopins.h>
#include <pinlist.h>
#include <debug.h>
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

		static const unsigned InterruptMask = ParityErrorInt | TxEmptyInt |
				TxCompleteInt | RxNotEmptyInt | IdleInt | LineBreakInt |
				ErrorInt | CtsInt;

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
		class UsartData
		{
		public:
			UsartData()
				:rxTimeoutChars(0),
				data(nullptr),
				rxSize(0)
			{
			}

			//TransferCallbackFunc txCallback;
			TransferCallbackFunc rxCallback;
			unsigned rxTimeoutChars;
			void *data;
			size_t rxSize;
		};
		
		#define USART_TEMPLATE_ARGS	template<class Regs, IRQn_Type IQRNumber, class ClockCtrl, class TxPins, class RxPins, class RemapField, class DmaChannel, class DmaRxChannel>

		#define USART_TEMPLATE_QUALIFIER	Usart<\
			Regs, IQRNumber, ClockCtrl, TxPins, RxPins, RemapField, DmaChannel, DmaRxChannel>
			
		template<class Regs, IRQn_Type IQRNumber, class ClockCtrl, class TxPins, class RxPins, class RemapField, class DmaChannel, class DmaRxChannel>
		class Usart :public UsartBase
		{
			static UsartData _data;
		public:
			
			template<unsigned long baud>
			static inline void Init(UsartMode usartMode = Default)
			{
				Init(baud, usartMode);
			}

			static void Init(unsigned baud, UsartMode usartMode = Default)
			{
				ClockCtrl::Enable();
				unsigned brr = ClockCtrl::ClockFreq() / baud;
				Regs()->BRR = brr;
				Regs()->CR1 = (((usartMode >> CR1ModeShift) & CR1ModeMask) | USART_CR1_UE );
				Regs()->CR2 = ((usartMode >> CR2ModeShift) & CR2ModeMask);
			}

			static void Write(uint8_t c)
			{
				while(!WriteReady())
					;
				Regs()->DR = c;
			}

			static uint8_t Read()
			{
				while(!ReadReady())
					;
				return Regs()->DR;
			}

			static bool WriteReady()
			{
				bool dmaActive = (Regs()->CR3 & USART_CR3_DMAT) && DmaChannel::Enabled();
				return (!dmaActive || DmaChannel::TrasferComplete()) && (Regs()->SR & USART_SR_TXE);
			}

			static bool ReadReady()
			{
				return Regs()->SR & USART_SR_RXNE;
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

				if(interruptFlags != NoInterrupt)
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

				typedef typename RxPins:: template Pin<RxPinNumber> RxPin;
				RxPin::Port::Enable();
				RxPin::SetConfiguration(RxPin::Port::In);
				if(TxPins::Length == 3 && TxPinNumber == 2) // Usart3
				{
					RemapField::Set(3);
				}
				else
				{
					RemapField::Set(TxPinNumber);
				}
			}

			static void SelectTxRxPins(uint8_t TxPinNumber, uint8_t RxPinNumber)
			{
				typedef typename TxPins::ValueType Type;
				Type maskTx (1 << TxPinNumber);
				TxPins::SetConfiguration(maskTx, TxPins::AltFunc);

				Type maskRx (1 << RxPinNumber);

				RxPins::SetConfiguration(maskRx, RxPins::In);
				if(TxPins::Length == 3 && TxPinNumber == 2) // Usart3
				{
					RemapField::Set(3);
				}
				else
				{
					RemapField::Set(TxPinNumber);
				}
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

			static void Write(const void *data, size_t size, bool async = false)
			{
				if(async && size > 1)
				{
					while(!WriteReady())
						;
					DmaChannel::ClearTrasferComplete();
					Regs()->CR3 |= USART_CR3_DMAT;
					Regs()->SR &= ~USART_SR_TC;
					DmaChannel::Init(DmaChannel::Mem2Periph | DmaChannel::MemIncriment, data, &Regs()->DR, size);
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

			bool WriteAsync(const void *data, size_t size, TransferCallbackFunc callback)
			{
				if(!size || !data)
				{
				   return false;
				}
				if(!DmaTxChannel::Ready())
					return false;

				DmaChannel::ClearTrasferComplete();
				Regs()->CR3 |= USART_CR3_DMAT;
				Regs()->SR &= ~USART_SR_TC;
				DmaChannel::Init(DmaChannel::Mem2Periph | DmaChannel::MemIncriment, data, &Regs()->DR, size);
				return true;
			}

			bool ReadAsync(void *data, size_t size, TransferCallbackFunc callback)
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
				
				if(ReadReady())
				{
					*ptr = Read();
					ptr++;
					size--;
				}
				Regs()->CR3 |= USART_CR3_DMAR;
				_data.rxCallback = callback;
				_data.rxSize = size;
				_data.data = data;
				DmaRxChannel::SetTransferCallback(callback);
				DmaRxChannel::Init(DmaChannel::Periph2Mem | DmaChannel::MemIncriment, ptr, &Regs()->DR, size);
				if(_data.rxTimeoutChars > 0)
				{
					Regs()->CR1 |= USART_CR1_IDLEIE;
					NVIC_EnableIRQ(IQRNumber);
				}
				return true;
			}

			void OnReadTimeout()
			{
				DmaRxChannel::Disable();
				DmaRxChannel::ClearFlags();
				if(_data.rxCallback && _data.data)
				{
					size_t bytesTransfered = _data.rxSize - DmaRxChannel::RemainingTransfers();
					_data.rxCallback(_data.data, bytesTransfered, false);
				}
			}
		};

		USART_TEMPLATE_ARGS
		UsartData USART_TEMPLATE_QUALIFIER::_data;
		
	#undef USART_TEMPLATE_ARGS
	#undef USART_TEMPLATE_QUALIFIER
	
		typedef IO::PinList<IO::Pa9,  IO::Pb6> Usart1TxPins;
		typedef IO::PinList<IO::Pa10, IO::Pb7> Usart1RxPins;

		typedef IO::PinList<IO::Pa2, IO::Pd5> Usart2TxPins;
		typedef IO::PinList<IO::Pa3, IO::Pd6> Usart2RxPins;

		typedef IO::PinList<IO::Pb10, IO::Pd8, IO::Pc10> Usart3TxPins;
		typedef IO::PinList<IO::Pb11, IO::Pa9, IO::Pc11> Usart3RxPins;

		IO_BITFIELD_WRAPPER(AFIO->MAPR, Usart1Remap, uint32_t, 2, 1);
		IO_BITFIELD_WRAPPER(AFIO->MAPR, Usart2Remap, uint32_t, 3, 1);
		IO_BITFIELD_WRAPPER(AFIO->MAPR, Usart3Remap, uint32_t, 4, 2);
	}

#define DECLARE_USART(REGS, IRQ, CLOCK, className, DmaChannel, DmaRxChannnel) \
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
		Private::className ## Remap, \
		DmaChannel,\
		DmaRxChannnel\
		> className

		DECLARE_USART(USART1, USART1_IRQn, Clock::Usart1Clock, Usart1, Dma1Channel4, Dma1Channel5);
		DECLARE_USART(USART2, USART2_IRQn, Clock::Usart2Clock, Usart2, Dma1Channel7, Dma1Channel6);
		DECLARE_USART(USART3, USART3_IRQn, Clock::Usart3Clock, Usart3, Dma1Channel2, Dma1Channel3);

		#define MCUCPP_HAS_USART1 1
		#define MCUCPP_HAS_USART2 1
		#define MCUCPP_HAS_USART3 1
}
