#pragma once

#include <ioreg.h>
#include "MDR32Fx.h"

#ifndef F_CPU
#error F_CPU must be defined to proper cpu frequency
#endif

namespace Mcucpp
{
	namespace Clock
	{
		class SysClock
		{
		public:
			static unsigned long FCore()
			{
				// TODO: implement this
				return F_CPU;
			}
			
			static unsigned long FPeriph()
			{
				// TODO: implement this
				return 8000000;
			}
			
			static const unsigned long CpuFreq = F_CPU;
		};
		
		namespace Private
		{
			IO_REG_WRAPPER(MDR_RST_CLK->CLOCK_STATUS,  ClockStatus, uint32_t);
			IO_REG_WRAPPER(MDR_RST_CLK->PLL_CONTROL,   PllControl,  uint32_t);
			IO_REG_WRAPPER(MDR_RST_CLK->HS_CONTROL,    HsControl,   uint32_t);
			IO_REG_WRAPPER(MDR_RST_CLK->CPU_CLOCK,     CpuClock,    uint32_t);
			IO_REG_WRAPPER(MDR_RST_CLK->USB_CLOCK,     UsbClock,    uint32_t);
			IO_REG_WRAPPER(MDR_RST_CLK->ADC_MCO_CLOCK, AdcMcoClock, uint32_t);
			IO_REG_WRAPPER(MDR_RST_CLK->RTC_CLOCK,     RtcClock,    uint32_t);
			IO_REG_WRAPPER(MDR_RST_CLK->PER_CLOCK,     PerClock,    uint32_t);
			IO_REG_WRAPPER(MDR_RST_CLK->CAN_CLOCK,     CanClock,    uint32_t);
			IO_REG_WRAPPER(MDR_RST_CLK->TIM_CLOCK,     TimClock,    uint32_t);
			IO_REG_WRAPPER(MDR_RST_CLK->UART_CLOCK,    UartClock,   uint32_t);
			IO_REG_WRAPPER(MDR_RST_CLK->SSP_CLOCK,     SspClock,    uint32_t);
			
			// UART clock control
			IO_BITFIELD_WRAPPER(MDR_RST_CLK->UART_CLOCK, Usart1ClockDiv, uint32_t, RST_CLK_UART_CLOCK_UART1_BRG_Pos, 8);
			IO_BITFIELD_WRAPPER(MDR_RST_CLK->UART_CLOCK, Usart2ClockDiv, uint32_t, RST_CLK_UART_CLOCK_UART2_BRG_Pos, 8);
			IO_BITFIELD_WRAPPER(MDR_RST_CLK->UART_CLOCK, Usart1ClockEnable, uint32_t, RST_CLK_UART_CLOCK_UART1_CLK_EN_Pos, 1);
			IO_BITFIELD_WRAPPER(MDR_RST_CLK->UART_CLOCK, Usart2ClockEnable, uint32_t, RST_CLK_UART_CLOCK_UART2_CLK_EN_Pos, 1);
		}
		
		template<class Reg, unsigned Mask>
		struct ClockControl
		{
			static void Enable()
			{
				Reg::Or(Mask);
			}
			
			static void Disable()
			{
				Reg::And(~Mask);
			}
		};
		
		typedef ClockControl<Private::PerClock, 1 << 0> Can1Clock;
		typedef ClockControl<Private::PerClock, 1 << 1> Can2Clock;
		typedef ClockControl<Private::PerClock, 1 << 2> UsbClock;
		typedef ClockControl<Private::PerClock, 1 << 3> EepromClock;
		typedef ClockControl<Private::PerClock, 1 << 4> RstClock;
		typedef ClockControl<Private::PerClock, 1 << 5> DmaClock;
		//typedef ClockControl<Private::PerClock, 1 << 6> Uart1Clock;
		//typedef ClockControl<Private::PerClock, 1 << 7> Uart2Clock;
		
		typedef ClockControl<Private::PerClock, 1 << 8> Spi1Clock;
		//typedef ClockControl<Private::PerClock, 1 << 9> Reserved;
		typedef ClockControl<Private::PerClock, 1 << 10> I2C1Clock;
		typedef ClockControl<Private::PerClock, 1 << 11> PowerClock;
		typedef ClockControl<Private::PerClock, 1 << 12> WwdtClock;
		typedef ClockControl<Private::PerClock, 1 << 13> IwdtClock;
		typedef ClockControl<Private::PerClock, 1 << 14> Timer1Clock;
		typedef ClockControl<Private::PerClock, 1 << 15> Timer2Clock;
		
		typedef ClockControl<Private::PerClock, 1 << 16> Timer3Clock;
		typedef ClockControl<Private::PerClock, 1 << 17> AdcClock;
		typedef ClockControl<Private::PerClock, 1 << 18> DacClock;
		typedef ClockControl<Private::PerClock, 1 << 19> CompClock;
		typedef ClockControl<Private::PerClock, 1 << 20> Spi2Clock;
		typedef ClockControl<Private::PerClock, 1 << 21> PortaClock;
		typedef ClockControl<Private::PerClock, 1 << 22> PortbClock;
		typedef ClockControl<Private::PerClock, 1 << 23> PortcClock;
		
		typedef ClockControl<Private::PerClock, 1 << 24> PortdClock;
		typedef ClockControl<Private::PerClock, 1 << 25> PorteClock;
		//typedef ClockControl<Private::PerClock, 1 << 26> Reserved;
		typedef ClockControl<Private::PerClock, 1 << 27> BkpClock;
		//typedef ClockControl<Private::PerClock, 1 << 28> Reserved;
		typedef ClockControl<Private::PerClock, 1 << 29> PortfClock;
		typedef ClockControl<Private::PerClock, 1 << 30> ExtBusClock;
		//typedef ClockControl<Private::PerClock, 1 << 31> Reserved;
		
		///////////////////////////////////////////////////////////////////////////
		/// Uart1Clock
		///////////////////////////////////////////////////////////////////////////
		struct Uart1Clock
		{
			enum ClockDivider
			{
				Div1 =   0,
				Div2 =   1,
				Div4 =   2,
				Div8 =   3,
				Div16 =  4,
				Div32 =  5,
				Div64 =  6,
				Div128 = 7
			};
			
			static void Enable()
			{
				Private::PerClock::Or( 1 << 6);
				Private::Usart1ClockEnable::Set(1);
			}
			
			static void Disable()
			{
				Private::PerClock::And(~(1 << 6));
				Private::Usart1ClockEnable::Set(0);
			}
			
			static void SetDivider(ClockDivider divider)
			{
				Private::Usart1ClockDiv::Set(divider);
			}
			
			static uint32_t GetFreq()
			{
				return SysClock::FCore() / (1 << Private::Usart1ClockDiv::Get());
			}
		};
		///////////////////////////////////////////////////////////////////////////
		/// Uart2Clock
		///////////////////////////////////////////////////////////////////////////
		struct Uart2Clock
		{
			enum ClockDivider
			{
				Div1 =   0,
				Div2 =   1,
				Div4 =   2,
				Div8 =   3,
				Div16 =  4,
				Div32 =  5,
				Div64 =  6,
				Div128 = 7
			};
			
			static void Enable()
			{
				Private::PerClock::Or( 1 << 7);
				Private::Usart2ClockEnable::Set(1);
			}
			
			static void Disable()
			{
				Private::PerClock::And(~(1 << 7));
				Private::Usart2ClockEnable::Set(0);
			}
			
			static void SetDivider(ClockDivider divider)
			{
				Private::Usart2ClockDiv::Set(divider);
			}
			
			static uint32_t GetFreq()
			{
				return SysClock::FCore() / (1 << Private::Usart2ClockDiv::Get());
			}
		};
		
	}
}