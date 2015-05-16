#pragma once

// Unimplemented: IrDA, DMA, Auto-baud, Modem mode

#include <clock.h>
#include <enum.h>

namespace Mcucpp
{
	struct UsartConsts
	{
		// UART interrupt enable register
		enum{
			UART_IER_RBRINT_EN		= 1<<0	, 	// RBR Interrupt enable
			UART_IER_THREINT_EN		= 1<<1	, 	// THR Interrupt enable
			UART_IER_RLSINT_EN		= 1<<2	, 	// RX line status interrupt enable
			UART1_IER_MSINT_EN		= 1<<3	,	// Modem status interrupt enable 
			UART1_IER_CTSINT_EN		= 1<<7	,	// CTS1 signal transition interrupt enable 
			UART_IER_ABEOINT_EN		= 1<<8	, 	// Enables the end of auto-baud interrupt 
			UART_IER_ABTOINT_EN		= 1<<9	, 	// Enables the auto-baud time-out interrupt 
			UART_IER_BITMASK		= 0x307	,	// UART interrupt enable register bit mask 
			UART1_IER_BITMASK		= 0x38F	,	// UART1 interrupt enable register bit mask 
		};
		
		// UART interrupt identification register
		enum{
			UART_IIR_INTSTAT_PEND	= 1<<0	,	// Interrupt Status - Active low 
			UART_IIR_INTID_RLS		= 3<<1	, 	// Interrupt identification: Receive line status
			UART_IIR_INTID_RDA		= 2<<1	, 	// Interrupt identification: Receive data available
			UART_IIR_INTID_CTI		= 6<<1	, 	// Interrupt identification: Character time-out indicator
			UART_IIR_INTID_THRE		= 1<<1	, 	// Interrupt identification: THRE interrupt
			UART1_IIR_INTID_MODEM	= 0<<1	, 	// Interrupt identification: Modem interrupt
			UART_IIR_INTID_MASK		= 7<<1	,	// Interrupt identification: Interrupt ID mask 
			UART_IIR_FIFO_EN		= 3<<6	, 	// These bits are equivalent to UnFCR[0] 
			UART_IIR_ABEO_INT		= 1<<8	, 	// End of auto-baud interrupt 
			UART_IIR_ABTO_INT		= 1<<9	, 	// Auto-baud time-out interrupt 
			UART_IIR_BITMASK		= 0x3CF	,	// UART interrupt identification register bit mask 
		};
		
		// UART FIFO control register
		enum{
			UART_FCR_FIFO_EN		= 1<<0,	// UART FIFO enable
			UART_FCR_RX_RS			= 1<<1,	// UART FIFO RX reset
			UART_FCR_TX_RS			= 1<<2,	// UART FIFO TX reset
			UART_FCR_DMAMODE_SEL 	= 1<<3,	// UART DMA mode selection
			UART_FCR_TRG_LEV0		= 0   ,	// UART FIFO trigger level 0: 1 character
			UART_FCR_TRG_LEV1		= 1<<6,	// UART FIFO trigger level 1: 4 character
			UART_FCR_TRG_LEV2		= 2<<6,	// UART FIFO trigger level 2: 8 character
			UART_FCR_TRG_LEV3		= 3<<6,	// UART FIFO trigger level 3: 14 character
			UART_FCR_BITMASK		= 0xCF,	// UART FIFO control bit mask
			UART_TX_FIFO_SIZE		= 16  ,
		};

		// UART line control register
		enum{
			UART_LCR_WLEN5     		= 0	,		// UART 5 bit data mode
			UART_LCR_WLEN6     		= 1<<0,   	// UART 6 bit data mode 
			UART_LCR_WLEN7     		= 2<<0,   	// UART 7 bit data mode
			UART_LCR_WLEN8     		= 3<<0,   	// UART 8 bit data mode
			UART_LCR_STOPBIT_SEL	= 1<<2,   	// UART Two Stop Bits Select
			UART_LCR_PARITY_EN		= 1<<3,		// UART Parity Enable
			UART_LCR_PARITY_ODD		= 0	,      	// UART Odd Parity Select
			UART_LCR_PARITY_EVEN	= 1<<4,		// UART Even Parity Select
			UART_LCR_PARITY_F_1		= 2<<4,		// UART force 1 stick parity
			UART_LCR_PARITY_F_0		= 3<<4,		// UART force 0 stick parity
			UART_LCR_BREAK_EN		= 1<<6,		// UART Transmission Break enable
			UART_LCR_DLAB_EN		= 1<<7,    	// UART Divisor Latches Access bit enable
			UART_LCR_BITMASK		= 0xFF,		// UART line control bit mask
		};

		// UART line status register
		enum{
			UART_LSR_RDR		= 1<<0, 	// Line status register: Receive data ready
			UART_LSR_OE			= 1<<1, 	// Line status register: Overrun error
			UART_LSR_PE			= 1<<2, 	// Line status register: Parity error
			UART_LSR_FE			= 1<<3, 	// Line status register: Framing error
			UART_LSR_BI			= 1<<4, 	// Line status register: Break interrupt
			UART_LSR_THRE		= 1<<5, 	// Line status register: Transmit holding register empty
			UART_LSR_TEMT		= 1<<6, 	// Line status register: Transmitter empty
			UART_LSR_RXFE		= 1<<7, 	// Error in RX FIFO
			UART_LSR_BITMASK	= 0xFF, 	// UART Line status bit mask 
		};

		// UART Tx Enable register
		enum{
			UART_TER_TXEN		= 1<<7, 	// Transmit enable bit
			UART_TER_BITMASK	= 0x80,		// UART Transmit Enable Register bit mask
		};
		
		// UART1 RS485 Control register
		enum{
			UART1_RS485CTRL_NMM_EN		= 1<<0,	// RS-485/EIA-485 Normal Multi-drop Mode (NMM)is disabled
			UART1_RS485CTRL_RX_DIS		= 1<<1,	// The receiver is disabled
			UART1_RS485CTRL_AADEN		= 1<<2,	// Auto Address Detect (AAD) is enabled
			UART1_RS485CTRL_SEL_DTR		= 1<<3,	// If direction control is enabled(bit DCTRL = 1), pin DTR is used for direction control
			UART1_RS485CTRL_DCTRL_EN	= 1<<4,	// Enable Auto Direction Control
			UART1_RS485CTRL_OINV_1		= 1<<5,	// This bit reverses the polarity of the directioncontrol signal on the RTS (or DTR) pin. 
												// The direction control pin will be driven to logic "1" when the transmitter has data to be sent
			UART1_RS485CTRL_BITMASK		= 0x3F,	// RS485 control bit-mask value
		};


		enum InterrupFlags
		{
			TxEmptyInt     = UART_IER_THREINT_EN ,
			RxNotEmptyInt  = UART_IER_RBRINT_EN  ,
			StatusInt      = UART_IER_RLSINT_EN  ,
		};
		
		enum InterruptId
		{
			IIdTxEmpty            = UART_IIR_INTID_THRE  ,
			IIdRxNotEmpty         = UART_IIR_INTID_RDA   ,
			IIdLineStatus         = UART_IIR_INTID_RLS   ,
			IIdCharTimeout        = UART_IIR_INTID_CTI   ,
			IIdModem              = UART1_IIR_INTID_MODEM,
		};
		
		enum UsartDataFormat
		{
			WordLen5	= UART_LCR_WLEN5,
			WordLen6	= UART_LCR_WLEN6,
			WordLen7	= UART_LCR_WLEN7,
			WordLen8	= UART_LCR_WLEN8,

			OneStopBit 	= 0,
			TwoStopBits	= UART_LCR_STOPBIT_SEL,

			NoneParity	= 0,
			OddParity 	= UART_LCR_PARITY_EN | UART_LCR_PARITY_ODD,
			EvenParity	= UART_LCR_PARITY_EN | UART_LCR_PARITY_EVEN,

			DataFormatDefault = WordLen8 | OneStopBit | NoneParity,
		};

		// When I tried to set FiFo buffer level to 1, 4 or 8, it always became 14!
		enum UsartFiFo
		{
			FiFoDisable = 0,
			FiFoBuf1    = UART_FCR_TRG_LEV0 | UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS,
			FiFoBuf4    = UART_FCR_TRG_LEV1 | UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS,
			FiFoBuf8    = UART_FCR_TRG_LEV2 | UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS,
			FiFoBuf14   = UART_FCR_TRG_LEV3 | UART_FCR_FIFO_EN | UART_FCR_RX_RS | UART_FCR_TX_RS,
		};

		enum UsartAutoDirectionControl
		{
			AutoDirDisable	= 0,
			
			Rts	= UART1_RS485CTRL_DCTRL_EN,
			Dtr	= UART1_RS485CTRL_DCTRL_EN | UART1_RS485CTRL_SEL_DTR,
			
			Inv	= UART1_RS485CTRL_OINV_1, 	// If set, drive pin Rts or Dtr to    1 at Sending and    0 at Receiving
											// else    drive pin Rts or Dtr to    0 at Sending and    1 at Receiving
											// therefore for MAX485, ADM485 etc. should be set
		};
	
		enum Status
		{
			RxNotEmpty = UART_LSR_RDR,
			TxEmpty = UART_LSR_THRE,
			TxDone = UART_LSR_TEMT,
			BreakInterrupt = UART_LSR_BI,
			OverrunError = UART_LSR_OE,
			ParityError = UART_LSR_PE,
			FramingError = UART_LSR_FE,
			FiFoError = UART_LSR_RXFE
		};

	};
	
	DECLARE_ENUM_OPERATIONS(UsartConsts::InterrupFlags)
	DECLARE_ENUM_OPERATIONS(UsartConsts::UsartDataFormat)
	DECLARE_ENUM_OPERATIONS(UsartConsts::UsartAutoDirectionControl)
	DECLARE_ENUM_OPERATIONS(UsartConsts::Status)


	namespace Private
	{
	
		class Baudrate
		{
		protected:
			static bool CalcDivisors( uint32_t uClk, uint32_t baudrate, uint32_t& DL,  uint32_t& FDR ){
				uint32_t d, m, bestd, bestm, tmp;
				uint64_t divisor;
				uint32_t best_divisor;
				uint32_t current_error, best_error;
				uint32_t recalcbaud;

				/* In the Usart IP block, baud rate is calculated using FDR and DLL-DLM registers
				* The formula is :
				* BaudRate= uClk * mulFracDiv/(mulFracDiv+dividerAddFracDiv) / (16 * DLL)
				* It involves floating point calculations. That's the reason the formulae are adjusted with
				* Multiply and divide method.*/
				/* The value of mulFracDiv and dividerAddFracDiv should comply to the following expressions:
				* 0 < mulFracDiv <= 15, 0 <= dividerAddFracDiv <= 15 */
				best_error = 0xFFFFFFFF; /* Worst case */
				bestd = 0;
				bestm = 0;
				best_divisor = 0;
				for (m = 1 ; m <= 15 ;m++)
				{
					for (d = 0 ; d < m ; d++)
					{
						divisor = ((uint64_t)uClk<<28)*m/(baudrate*(m+d));
						current_error = divisor & 0xFFFFFFFF;
		
						tmp = divisor>>32;
		
						// Adjust error
						if(current_error > ((uint32_t)1<<31)){
							current_error = -current_error;
							tmp++;
						}

						if(tmp<1 || tmp>65536) // Out of range
							continue;

						if( current_error < best_error){
							best_error = current_error;
							best_divisor = tmp;
							bestd = d;
							bestm = m;
							if(best_error == 0) break;
						}
					}

					if (best_error == 0) break;
				}

				if(best_divisor == 0) return false; // can not find best match

				recalcbaud = (uClk>>4) * bestm/(best_divisor * (bestm + bestd));

				// reuse best_error to evaluate baud error
				if(baudrate>recalcbaud) best_error = baudrate - recalcbaud;
				else best_error = recalcbaud -baudrate;

				best_error = best_error * 100 / baudrate;

				if (best_error >= 3) // Acceptable UART baudrate error
					return false;
				
				DL = best_divisor;
				FDR = bestm<<4 | bestd;
				return true;
			}
		};

	
		template<class Regs, class ClockCtrl>
		class UsartBase :public UsartConsts, public Baudrate
		{
		public:
			static void Init(	unsigned baudrate, 
								UsartConsts::UsartDataFormat flags_data = DataFormatDefault,
								UsartConsts::UsartFiFo flags_fifo = FiFoDisable )
			{
				ClockCtrl::Enable();
				
				FiFoReset(flags_fifo);
				
				Regs()->TER = UsartConsts::UART_TER_TXEN;	// Software flow control: Transmit Enable
				Regs()->IER = 0;							// Disable interrupt

				// Set baudrate and data format
				uint32_t dl_, fdr_;
				if( !Baudrate::CalcDivisors( ClockCtrl::GetFreq(), baudrate, dl_, fdr_ ) )
					while(true)
						;
				Regs()->FDR = fdr_;
				Regs()->LCR = UsartConsts::UART_LCR_DLAB_EN | flags_data;
				Regs()->DLM = dl_ >> 8;
				Regs()->DLL = dl_ & 0xff;
				Regs()->LCR = flags_data;
				
				Regs()->ACR = 0;		// Set Auto-baud Control Register to default state
			}
			
			static bool Putch(uint8_t c){
				if( Regs()->LSR & UsartConsts::UART_LSR_THRE ){
					Regs()->THR = c;
					return true;
				}
				return false;
			}

			static bool Getch(uint8_t &c){
				if(Regs()->LSR & UsartConsts::UART_LSR_RDR ){
					c = Regs()->RBR;
					return true;
				}
				return false;
			}

			inline static void Write(uint8_t c){
				Regs()->THR = c;
			}

			inline static uint8_t Read(){
				return Regs()->RBR;
			}
			
			
			inline static void EnableInterrupt(InterrupFlags interruptFlags)
			{
				Regs()->IER |= interruptFlags;
			}
			
			inline static void DisableInterrupt(InterrupFlags interruptFlags)
			{
				Regs()->IER &= ~interruptFlags;
			}
			
			inline static InterruptId InterruptSource(){
				return static_cast<InterruptId>(Regs()->IIR & UsartConsts::UART_IIR_INTID_MASK);
			}

			inline static Status GetStatus()
			{
				return static_cast<Status>(Regs()->LSR);
			}
			
			/*
			
			static void ClearInterruptFlag(InterrupFlags interruptFlags)
			{
			}
			
			template<uint8_t TxPinNumber, uint8_t RxPinNumber>
			static void SelectTxRxPins()
			{
			}

			static void SelectTxRxPins(uint8_t TxPinNumber, uint8_t RxPinNumber)
			{
			}
*/
/*
			static inline void EnableTxRx(){
				Regs()->IER |= UsartConsts::UART_IER_RBRINT_EN|UsartConsts::UART_IER_THREINT_EN; // rx,tx interrupt enable
			}
			
			static inline void DisableTxRx(){
				Regs()->IER &= ~(UsartConsts::UART_IER_RBRINT_EN|UsartConsts::UART_IER_THREINT_EN);
			}

			static inline void EnableTx(){
				Regs()->IER |= UsartConsts::UART_IER_THREINT_EN;
			}
			
			static inline void DisableTx(){
				Regs()->IER &= ~UsartConsts::UART_IER_THREINT_EN;
			}

			static inline void EnableRx(){
				Regs()->IER |= UsartConsts::UART_IER_RBRINT_EN;
			}
			
			static inline void DisableRx(){
				Regs()->IER &= ~UsartConsts::UART_IER_RBRINT_EN;
			}
*/			
			static inline void FiFoReset( UsartConsts::UsartFiFo flags_fifo ){
				Regs()->FCR = flags_fifo;
			}
			
			static inline void FiFoFlush(){
				Regs()->FCR |= UART_FCR_RX_RS | UART_FCR_TX_RS;
			}
			
			// FIFOLVL is buggy
			//static unsigned TxBufLvl(){
			//	return Regs()->FIFOLVL>>8 & 0x1f;	// !May be incorrect
			//}
			//
			//static unsigned RxBufLvl(){
			//	return Regs()->FIFOLVL & 0x1f;	// !May be incorrect
			//}
			
		};

		template<class Regs, class ClockCtrl>
		class Usart1 :public UsartBase<Regs,ClockCtrl>
		{
		typedef UsartBase<Regs,ClockCtrl> Base;
		public:
			static void Init(	unsigned baudrate, 
								UsartConsts::UsartDataFormat flags_data = UsartConsts::DataFormatDefault, 
								UsartConsts::UsartFiFo flags_fifo = UsartConsts::FiFoDisable,
								UsartConsts::UsartAutoDirectionControl flags_dir = UsartConsts::AutoDirDisable)
			{
				Base::Init(baudrate, flags_data, flags_fifo);

				Regs()->MCR = 0;		// Set Modem Control to default state
				
				// Set RS485 control to default state
				//Regs()->RS485CTRL = 0;
				Regs()->RS485CTRL = UsartConsts::UART1_RS485CTRL_NMM_EN | flags_dir; //RxEn bit: UART1_RS485CTRL_RX_DIS
				// Set RS485 delay timer to default state
				Regs()->RS485DLY = 0;
				// Set RS485 addr match to default state
				//Regs()->ADRMATCH = 0;
			}
		};

	}

#define DECLARE_UART(REGS, REGS_TypeDef, CLOCK, BASECLASS, className) \
	namespace Private \
	{\
		IO_STRUCT_WRAPPER(REGS, className ## _REGS, REGS_TypeDef);\
	}\
	typedef Private::BASECLASS<\
		Private::className ## _REGS,\
		CLOCK\
		> className

		DECLARE_UART( LPC_UART0, LPC_UART_TypeDef,  Clock::Usart0Clock, UsartBase, Usart0);
		DECLARE_UART( LPC_UART1, LPC_UART1_TypeDef, Clock::Usart1Clock, Usart1,    Usart1);
		DECLARE_UART( LPC_UART2, LPC_UART_TypeDef,  Clock::Usart2Clock, UsartBase, Usart2);
		DECLARE_UART( LPC_UART3, LPC_UART_TypeDef,  Clock::Usart3Clock, UsartBase, Usart3);
}



