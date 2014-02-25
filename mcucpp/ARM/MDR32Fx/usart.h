
#pragma once

#include <clock.h>
#include <ioreg.h>
#include <iopins.h>

namespace Mcucpp
{
	class UsartBase
	{
	public:
		enum UsartMode
		{
			FifoEnabled = UART_LCR_H_FEN,
			DataBits5 = 0 << UART_LCR_H_WLEN_Pos,
			DataBits6 = 1 << UART_LCR_H_WLEN_Pos,
			DataBits7 = 2 << UART_LCR_H_WLEN_Pos,
			DataBits8 = 3 << UART_LCR_H_WLEN_Pos,

			NoneParity = 0,
			EvenParity = UART_LCR_H_PEN ,
			OddParity  = UART_LCR_H_PEN | UART_LCR_H_EPS,
			OneParity  = UART_LCR_H_PEN | UART_LCR_H_SPS,
			ZeroParity = UART_LCR_H_PEN | UART_LCR_H_SPS | UART_LCR_H_EPS,
			
			OneStopBit  = 0,
			TwoStopBits = UART_LCR_H_STP2,

			Disabled   = 0,
			RxEnable   = (UART_CR_UARTEN | UART_CR_RXE) << 16,
			TxEnable   = (UART_CR_UARTEN | UART_CR_TXE) << 16,
			RxTxEnable = RxEnable | TxEnable,
			Default    = RxTxEnable | FifoEnabled | DataBits8 
		};
	};
	
	inline UsartBase::UsartMode operator|(UsartBase::UsartMode left, UsartBase::UsartMode right)
	{	return static_cast<UsartBase::UsartMode>(static_cast<int>(left) | static_cast<int>(right));}
	
	
	namespace Private
	{
		template<class Regs, class ClockControl, class TxPin, class RxPin>
		class Usart :public UsartBase
		{
		public:
			typedef ClockControl Clock;
			
			static bool Init(unsigned baud, UsartMode flags = Default)
			{
				TxPin:: template SetConfiguration<TxPin::Port::Alt2OutFastest>();
				RxPin:: template SetConfiguration<TxPin::Port::In>();
				
				ClockControl::Enable();
				uint32_t clockSpeed = ClockControl::GetFreq();
				uint32_t divider = clockSpeed / (baud >> 2);
				uint32_t intDivider = divider >> 6;
				uint32_t fractDivider = divider & 63;
				uint32_t realSpeed = clockSpeed * 4 / divider;
				uint32_t error = (realSpeed - baud) * 256 / baud;
				if (error > 4)
					return false;

				Regs()->IBRD = intDivider;
				Regs()->FBRD = fractDivider;
				
				Regs()->LCR_H = flags & 0xff;
				Regs()->CR = ((flags >> 16) & 0xffff);
				return true;
			}

			static uint8_t Read()
			{
				uint32_t c = 0;
				if(!ReadReady())
					return 0;
				c = Regs()->DR;
				Regs()->RSR_ECR = 0;
				return (uint8_t)c;
			}
			
			static bool Write(uint8_t c)
			{
				if(!WriteReady())
					return false;
				Regs()->DR = c;
				return true;
			}

			static bool WriteReady()
			{
				return (Regs()->FR & UART_FR_TXFF) == 0;
			}

			static bool ReadReady()
			{
				return (Regs()->FR & UART_FR_RXFE) == 0;
			}
		};
		
		IO_STRUCT_WRAPPER(MDR_UART1, Usart1Regs, MDR_UART_TypeDef);
		IO_STRUCT_WRAPPER(MDR_UART2, Usart2Regs, MDR_UART_TypeDef);
	}
	
	typedef Private::Usart<Private::Usart1Regs, Clock::Uart1Clock, IO::Pb5, IO::Pb6> Usart1;
	typedef Private::Usart<Private::Usart2Regs, Clock::Uart2Clock, IO::Pd1, IO::Pd0> Usart2;
	
}