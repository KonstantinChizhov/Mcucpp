#pragma once

#include "ioreg.h"
#include "LPC17xx.h"

#ifndef F_CPU
#error F_CPU must be defined to proper cpu frequency
#endif

namespace Mcucpp
{
	namespace Clock
	{

		IO_REG_WRAPPER(LPC_SC->PCONP, PowerControlPeripherals, uint32_t);
		IO_REG_WRAPPER(LPC_SC->PCLKSEL0, PeriphClkSel0, uint32_t);
		IO_REG_WRAPPER(LPC_SC->PCLKSEL1, PeriphClkSel1, uint32_t);

		class SysClock
		{
		public:
			static uint32_t ClockFreq()
			{
				return F_CPU;
			}
		};
		
		template<unsigned Mask>
		class PowerControl{
		public:
			static void Enable(){
				PowerControlPeripherals::Or(Mask);
			}
			static void Disable(){
				PowerControlPeripherals::And(~Mask);
			}
		};
		
		class NullPowerControl{};
		
		template<unsigned div00, unsigned div01, unsigned div10, unsigned div11>
		struct ClockDiv2Mask{
			static unsigned Get(unsigned div){
				switch( div ){
				case div00:
					return 0;
				case div01:
					return 1;
				case div10:
					return 2;
				case div11:
					return 3;
				default:
					while(true);
				}
			}
		};
		
		template<class Reg, unsigned bitpos, unsigned div00=4, unsigned div01=1, unsigned div10=2, unsigned div11=8>
		class ClockDivider
		{
		public:
			static unsigned GetDiv(){
				switch( (Reg::Get() >> bitpos)&3 ){
				case 0:
					return div00;
				case 1:
					return div01;
				case 2:
					return div10;
				case 3:
					return div11;
				default:
					while(true);
				}
			}

			static unsigned GetFreq(){
				return SysClock::ClockFreq() / GetDiv();
			}
			
			static void SetDiv(unsigned div){
				unsigned mask = ClockDiv2Mask<div00,div01,div10,div11>::Get(div)<<bitpos;
				unsigned val = Reg::Get() & ~(3<<bitpos) | mask;
				Reg::Set(val);
			}
		};
		
		class NullClockDivider{};
		
		template<class PowerControl, class ClockDivider>
		class ClockControl : public PowerControl, public ClockDivider
		{
		};
		


		
		// Power Control for Peripherals Definitions
		enum{
			CLKPWR_PCONP_PCTIM0		= 1<<1,		// Timer/Counter 0 power/clock control bit
			CLKPWR_PCONP_PCTIM1		= 1<<2,		// Timer/Counter 1 power/clock control bit
			CLKPWR_PCONP_PCUART0  	= 1<<3,		// UART0 power/clock control bit
			CLKPWR_PCONP_PCUART1  	= 1<<4,		// UART1 power/clock control bit
			CLKPWR_PCONP_PCPWM1		= 1<<6,		// PWM1 power/clock control bit
			CLKPWR_PCONP_PCI2C0		= 1<<7,		// The I2C0 interface power/clock control bit
			CLKPWR_PCONP_PCSPI  	= 1<<8,		// The SPI interface power/clock control bit
			CLKPWR_PCONP_PCRTC  	= 1<<9,		// The RTC power/clock control bit
			CLKPWR_PCONP_PCSSP1		= 1<<10,	// The SSP1 interface power/clock control bit
			CLKPWR_PCONP_PCAD  		= 1<<12,	// A/D converter 0 (ADC0) power/clock control bit
			CLKPWR_PCONP_PCAN1  	= 1<<13,	// CAN Controller 1 power/clock control bit
			CLKPWR_PCONP_PCAN2 		= 1<<14,	// CAN Controller 2 power/clock control bit
			CLKPWR_PCONP_PCGPIO 	= 1<<15,	// GPIO power/clock control bit
			CLKPWR_PCONP_PCRIT 		= 1<<16,	// Repetitive Interrupt Timer power/clock control bit
			CLKPWR_PCONP_PCMC 		= 1<<17,	// Motor Control PWM
			CLKPWR_PCONP_PCQEI 		= 1<<18,	// Quadrature Encoder Interface power/clock control bit
			CLKPWR_PCONP_PCI2C1  	= 1<<19,	// The I2C1 interface power/clock control bit
			CLKPWR_PCONP_PCSSP0		= 1<<21,	// The SSP0 interface power/clock control bit
			CLKPWR_PCONP_PCTIM2		= 1<<22,	// Timer 2 power/clock control bit
			CLKPWR_PCONP_PCTIM3		= 1<<23,	// Timer 3 power/clock control bit
			CLKPWR_PCONP_PCUART2  	= 1<<24,	// UART 2 power/clock control bit
			CLKPWR_PCONP_PCUART3  	= 1<<25,	// UART 3 power/clock control bit
			CLKPWR_PCONP_PCI2C2		= 1<<26,	// I2C interface 2 power/clock control bit
			CLKPWR_PCONP_PCI2S  	= 1<<27,	// I2S interface power/clock control bi
			CLKPWR_PCONP_PCGPDMA  	= 1<<29,	// GP DMA function power/clock control bi
			CLKPWR_PCONP_PCENET		= 1<<30,	// Ethernet block power/clock control bi
			CLKPWR_PCONP_PCUSB  	= 1u<<31,	// USB interface power/clock control bi
		};
		
		// Peripheral Clock Selection Definitions
		enum{
			//PCLKSEL0
			CLKPWR_PCLKSEL_WDT  		= 0,  // Peripheral clock divider bit position for WDT
			CLKPWR_PCLKSEL_TIMER0  		= 2,  // Peripheral clock divider bit position for TIMER0
			CLKPWR_PCLKSEL_TIMER1  		= 4,  // Peripheral clock divider bit position for TIMER1
			CLKPWR_PCLKSEL_UART0  		= 6,  // Peripheral clock divider bit position for UART0
			CLKPWR_PCLKSEL_UART1  		= 8,  // Peripheral clock divider bit position for UART1
			CLKPWR_PCLKSEL_PWM1  		= 12, // Peripheral clock divider bit position for PWM1
			CLKPWR_PCLKSEL_I2C0  		= 14, // Peripheral clock divider bit position for I2C0
			CLKPWR_PCLKSEL_SPI  		= 16, // Peripheral clock divider bit position for SPI
			CLKPWR_PCLKSEL_SSP1  		= 20, // Peripheral clock divider bit position for SSP1
			CLKPWR_PCLKSEL_DAC  		= 22, // Peripheral clock divider bit position for DAC
			CLKPWR_PCLKSEL_ADC  		= 24, // Peripheral clock divider bit position for ADC
			CLKPWR_PCLKSEL_CAN1 		= 26, // Peripheral clock divider bit position for CAN1
			CLKPWR_PCLKSEL_CAN2 		= 28, // Peripheral clock divider bit position for CAN2
			CLKPWR_PCLKSEL_ACF  		= 30, // Peripheral clock divider bit position for ACF
			
			//PCLKSEL1
			CLKPWR_PCLKSEL_QEI  		= 0,  // Peripheral clock divider bit position for QEI
			CLKPWR_PCLKSEL_GPIOINT		= 2,  // Peripheral clock divider bit position for GPIOINT
			CLKPWR_PCLKSEL_PCB  		= 4,  // Peripheral clock divider bit position for PCB
			CLKPWR_PCLKSEL_I2C1  		= 6,  // Peripheral clock divider bit position for I2C1
			CLKPWR_PCLKSEL_SSP0  		= 10, // Peripheral clock divider bit position for SSP0
			CLKPWR_PCLKSEL_TIMER2  		= 12, // Peripheral clock divider bit position for TIMER2
			CLKPWR_PCLKSEL_TIMER3  		= 14, // Peripheral clock divider bit position for TIMER3
			CLKPWR_PCLKSEL_UART2  		= 16, // Peripheral clock divider bit position for UART2
			CLKPWR_PCLKSEL_UART3  		= 18, // Peripheral clock divider bit position for UART3
			CLKPWR_PCLKSEL_I2C2  		= 20, // Peripheral clock divider bit position for I2C2
			CLKPWR_PCLKSEL_I2S  		= 22, // Peripheral clock divider bit position for I2S
			CLKPWR_PCLKSEL_RIT  		= 26, // Peripheral clock divider bit position for RIT
			CLKPWR_PCLKSEL_SYSCON  		= 28, // Peripheral clock divider bit position for SYSCON
			CLKPWR_PCLKSEL_MC  			= 30, // Peripheral clock divider bit position for MC
		};

		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCAD    > , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_ADC    > > AdcClock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCAN1   > , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_CAN1 ,4,1,2,6  > > Can1Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCAN2   > , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_CAN2 ,4,1,2,6  > > Can2Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCI2C0  > , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_I2C0   > > I2c0Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCI2C1  > , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_I2C1   > > I2c1Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCI2C2  > , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_I2C2   > > I2c2Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCI2S   > , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_I2S    > > I2sClock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCMC    > , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_MC     > > McClock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCPWM1  > , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_PWM1   > > Pwm1Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCQEI   > , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_QEI    > > QeiClock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCRIT   > , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_RIT    > > RitClock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCSPI   > , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_SPI    > > SpiClock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCSSP0  > , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_SSP0   > > Ssp0Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCSSP1  > , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_SSP1   > > Ssp1Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCTIM0  > , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_TIMER0 > > Timer0Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCTIM1  > , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_TIMER1 > > Timer1Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCTIM2  > , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_TIMER2 > > Timer2Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCTIM3  > , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_TIMER3 > > Timer3Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCUART0 > , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_UART0  > > Usart0Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCUART1 > , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_UART1  > > Usart1Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCUART2 > , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_UART2  > > Usart2Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCUART3 > , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_UART3  > > Usart3Clock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCENET  > , NullClockDivider                         > EnetClock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCGPDMA > , NullClockDivider                         > GpdmaClock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCGPIO  > , NullClockDivider                         > GpioClock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCRTC   > , NullClockDivider                         > RtcClock;
		typedef ClockControl< PowerControl< CLKPWR_PCONP_PCUSB   > , NullClockDivider                         > UsbClock;
		typedef ClockControl< NullPowerControl                     , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_ACF ,4,1,2,6    > > AcfClock;
		typedef ClockControl< NullPowerControl                     , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_DAC     > > DacClock;
		typedef ClockControl< NullPowerControl                     , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_GPIOINT > > GpiointClock;
		typedef ClockControl< NullPowerControl                     , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_PCB     > > PcbClock;
		typedef ClockControl< NullPowerControl                     , ClockDivider<PeriphClkSel1, CLKPWR_PCLKSEL_SYSCON  > > SysconClock;
		typedef ClockControl< NullPowerControl                     , ClockDivider<PeriphClkSel0, CLKPWR_PCLKSEL_WDT     > > WdtClock;


	}
}

