#pragma once

#include "ioreg.h"
#include "stm32f30x.h"

#ifndef F_OSC
#warning F_OSC is not defined. F_OSC is in its default value 8 MHZ. Verify that external cristal freq is correct.  
#define F_OSC 8000000u
#endif


namespace Mcucpp
{
	namespace Clock
	{
		class ClockBase
		{
		protected:
			static const uint32_t ClockStartTimeout = 4000;
			
			static bool EnableClockSource(unsigned turnOnMask,  unsigned waitReadyMask)
			{
				uint32_t timeoutCounter = 0;
				RCC->CR |= turnOnMask;
				while(((RCC->CR & waitReadyMask) == 0) && (timeoutCounter < ClockStartTimeout))
				{
					timeoutCounter++;
				}
				return (RCC->CR & waitReadyMask) != 0;
			}
			
			static bool DisablelockSource(unsigned turnOnMask,  unsigned waitReadyMask)
			{
				uint32_t timeoutCounter = 0;
				RCC->CR &= ~turnOnMask;
				while(((RCC->CR & waitReadyMask) != 0) && (timeoutCounter < ClockStartTimeout))
				{
					timeoutCounter++;
				}
				return (RCC->CR & waitReadyMask) == 0;
			}
		};
		
		class HseClock :public ClockBase
		{
		public:
			static uint32_t SrcClockFreq()
			{
				return F_OSC;
			}
			
			static uint32_t GetDivider() { return 1; }
			
			static uint32_t GetMultipler() { return 1; }
			
			static uint32_t ClockFreq()
			{
				return SrcClockFreq();
			}
			
			static bool Enable()
			{
				return ClockBase::EnableClockSource(RCC_CR_HSEON, RCC_CR_HSERDY);
			}
			
			static bool Disable()
			{
				return ClockBase::DisablelockSource(RCC_CR_HSEON, RCC_CR_HSERDY);
			}
		};
		
		class HsiClock :public ClockBase
		{
		public:
			static uint32_t SrcClockFreq()
			{
				return 8000000u;
			}
			
			static uint32_t GetDivider() { return 1; }
			
			static uint32_t GetMultipler() { return 1; }
			
			static uint32_t ClockFreq()
			{
				return SrcClockFreq();
			}
			
			static bool Enable()
			{
				return ClockBase::EnableClockSource(RCC_CR_HSION, RCC_CR_HSIRDY);
			}
			
			static bool Disable()
			{
				return ClockBase::DisablelockSource(RCC_CR_HSION, RCC_CR_HSIRDY);
			}
		};
		
		class LseClock :public ClockBase
		{
		public:
			static uint32_t SrcClockFreq()
			{
				return 32768;
			}
			
			static uint32_t GetDivider() { return 1; }
			
			static uint32_t GetMultipler() { return 1; }
			
			static uint32_t ClockFreq()
			{
				return SrcClockFreq();
			}
			
			static bool Enable()
			{
				return false; // TODO
			}
			
			static bool Disable()
			{
				return false; // TODO
			}
		};
		
		class PllClock :public ClockBase
		{
		public:
			enum ClockSource
			{
				Internal = RCC_CFGR_PLLSRC_HSI_Div2,
				External = RCC_CFGR_PLLSRC_PREDIV1,
			};
			
			static uint32_t SrcClockFreq()
			{
				if ((RCC->CFGR & RCC_CFGR_PLLSRC) == 0)
					return HsiClock::ClockFreq();
				else
					return HseClock::ClockFreq();
			}
			
			static uint32_t GetDivider()
			{
				if ((RCC->CFGR & RCC_CFGR_PLLSRC) == 0)
					return 2;
				else
					return (RCC->CFGR2 & RCC_CFGR2_PREDIV1) + 1;
			}
			
			static uint32_t GetMultipler()
			{
				return ((RCC->CFGR & RCC_CFGR_PLLMULL) >> 18) + 2;
			}
			
			static void SetMultipler(uint8_t multiler)
			{
				multiler-=2;
				if(multiler > 15)
					multiler = 15;
				RCC->CFGR = (RCC->CFGR & RCC_CFGR_PLLMULL) | (multiler << 18);
			}
			
			static void SetDivider(uint8_t divider)
			{
				divider-=1;
				if(divider > 15)
					divider = 15;
				RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PREDIV1) | (divider);
			}
			
			static void SelectClockSource(ClockSource clockSource)
			{
				RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE) ) | clockSource;
			}
			
			static uint32_t ClockFreq()
			{
				return SrcClockFreq() / GetDivider() * GetMultipler();
			}
			
			static bool Enable()
			{
				if ((RCC->CFGR & RCC_CFGR_PLLSRC) == 0)
				{
					if (!HsiClock::Enable())
						return false;
				}
				else
					if (!HseClock::Enable())
						return false;
				return ClockBase::EnableClockSource(RCC_CR_PLLON, RCC_CR_PLLRDY);
			}
			
			static void Disable()
			{
				ClockBase::DisablelockSource(RCC_CR_PLLON, RCC_CR_PLLRDY);
			}
		};
		
		
		class SysClock
		{
		public:
			enum ClockSource
			{
				Internal = 0,
				External = 1,
				Pll = 2
			};
			
		public:
			static bool SelectClockSource(ClockSource clockSource)
			{
				uint32_t clockStatusValue;
				uint32_t clockSelectMask;
				
				if(clockSource == Internal)
				{
					clockStatusValue = RCC_CFGR_SWS_HSI;
					clockSelectMask = RCC_CFGR_SW_HSI;
					if (!HsiClock::Enable())
						return false;
				}else if(clockSource == External)
				{
					clockStatusValue = RCC_CFGR_SWS_HSE;
					clockSelectMask = RCC_CFGR_SW_HSE;
					if (!HseClock::Enable())
						return false;
				}if(clockSource == Pll)
				{
					clockStatusValue = RCC_CFGR_SWS_PLL;
					clockSelectMask = RCC_CFGR_SW_PLL;
					if (!PllClock::Enable())
						return false;
				}else
					return false;
	
				RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | clockSelectMask;
				
				while ((RCC->CFGR & RCC_CFGR_SWS) != clockStatusValue)
				{
				}
				return true;
			}
			
			static uint32_t ClockFreq()
			{
				uint32_t clockSrc = RCC->CFGR & RCC_CFGR_SWS;
				switch (clockSrc)
				{
					case 0:              return HsiClock::ClockFreq();
					case RCC_CFGR_SWS_0: return HseClock::ClockFreq();
					case RCC_CFGR_SWS_1: return PllClock::ClockFreq();
				}
				return 0;
			}
			
			static uint32_t SrcClockFreq()
			{
				return ClockFreq();
			}
		};
		
		
		template<class Reg, unsigned Mask, class ClockSrc>
		class ClockControl :public ClockSrc
		{
		public:
			static void Enable()
			{
				Reg::Or(Mask);
			}
			
			static void Disable()
			{
				Reg::And(~Mask);
			}
		};
		
		IO_BITFIELD_WRAPPER(RCC->CFGR, AhbPrescalerBitField, uint32_t, 4, 4);
		IO_BITFIELD_WRAPPER(RCC->CFGR, Apb1PrescalerBitField, uint32_t, 8, 3);
		IO_BITFIELD_WRAPPER(RCC->CFGR, Apb2PrescalerBitField, uint32_t, 11, 3);
		
		IO_BITFIELD_WRAPPER(RCC->CFGR, McoBitField, uint32_t, 24, 3);
		IO_BITFIELD_WRAPPER(RCC->CFGR3, Usart1ClockSwitch, uint32_t, 0, 2);
		
		IO_BITFIELD_WRAPPER(RCC->CFGR2, Adc12Prescaller, uint32_t, 4, 5);
		IO_BITFIELD_WRAPPER(RCC->CFGR2, Adc34Prescaller, uint32_t, 9, 5);
		
		class AhbClock
		{
		public:
			enum Prescaller
			{
				Div1 = 0,
				Div2 = 0x08,
				Div4 = 0x09,
				Div8 = 0x0A,
				Div16 = 0x0B,
				Div64 = 0x0C,
				Div128 = 0x0D,
				Div256 = 0x0E,
				Div512 = 0x0F
			};
			
			static uint32_t SrcClockFreq()
			{
				return SysClock::ClockFreq();
			}
			
			static uint32_t ClockFreq()
			{
				uint32_t clock = SysClock::ClockFreq();
				uint8_t clockPrescShift[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
				uint8_t shiftBits = clockPrescShift[AhbPrescalerBitField::Get()];
				clock >>= shiftBits;
				return clock;
			}
			
			static void SetPrescaller(Prescaller prescaller)
			{
				AhbPrescalerBitField::Set((uint32_t)prescaller);
			}
		};
		
		class Apb1Clock
		{
		public:
			enum Prescaller
			{
				Div1 = 0,
				Div2 = 0x04,
				Div4 = 0x05,
				Div8 = 0x06,
				Div16 = 0x07,
			};
			
			static uint32_t SrcClockFreq()
			{
				return AhbClock::ClockFreq();
			}
			
			static uint32_t ClockFreq()
			{
				uint32_t clock = AhbClock::ClockFreq();
				uint8_t clockPrescShift[] = {0, 0, 0, 0, 1, 2, 3, 4};
				uint8_t shiftBits = clockPrescShift[Apb1PrescalerBitField::Get()];
				clock >>= shiftBits;
				return clock;
			}
			
			static void SetPrescaller(Prescaller prescaller)
			{
				Apb1PrescalerBitField::Set((uint32_t)prescaller);
			}
		};
		
		class Apb2Clock
		{
		public:
			enum Prescaller
			{
				Div1 = 0,
				Div2 = 0x04,
				Div4 = 0x05,
				Div8 = 0x06,
				Div16 = 0x07,
			};
			
			static uint32_t SrcClockFreq()
			{
				return AhbClock::ClockFreq();
			}
			
			static uint32_t ClockFreq()
			{
				uint32_t clock = AhbClock::ClockFreq();
				const uint8_t clockPrescShift[] = {0, 0, 0, 0, 1, 2, 3, 4};
				uint8_t shiftBits = clockPrescShift[Apb2PrescalerBitField::Get()];
				clock >>= shiftBits;
				return clock;
			}
			
			static void SetPrescaller(Prescaller prescaller)
			{
				Apb2PrescalerBitField::Set((uint32_t)prescaller);
			}
		};
		
		class Adc12ClockSource 
		{
		public:
		
			enum ClockSource
			{
				Async = 0,
				AhbDiv1 = ADC12_CCR_CKMODE_0,
				AhbDiv2 = ADC12_CCR_CKMODE_1,
				AhbDiv4 = ADC12_CCR_CKMODE_0 | ADC12_CCR_CKMODE_1
			};
			
			enum Prescaller
			{
				Div1 = 16,
				Div2 = 17,
				Div4 = 18,
				Div6 = 19,
				Div8 = 20,
				Div10 = 21,
				Div12 = 22,
				Div16 = 23,
				Div32 = 24,
				Div64 = 25,
				Div128 = 26,
				Div256 = 27
			};
			
			static bool SelectClockSource(ClockSource clockSource)
			{
				ADC1_2->CCR = (ADC1_2->CCR & ~ADC12_CCR_CKMODE) | clockSource;
				return true;
			}
			
			static void SetPrescaller(Prescaller prescaller)
			{
				Adc12Prescaller::Set((uint32_t)prescaller);
			}
			
			static uint32_t SrcClockFreq()
			{
				const uint16_t clockPresc[] = {1, 2, 4, 6, 8, 10, 12, 16, 32, 64, 128, 256};
				switch(ADC1_2->CCR & ADC12_CCR_CKMODE)
				{
					case 0: 
					{
						uint16_t divIndex = Adc12Prescaller::Get() - 16;
						if(divIndex > 11)
							divIndex = 11;
						return PllClock::ClockFreq() / clockPresc[divIndex];
					}
					case AhbDiv1: return AhbClock::ClockFreq();
					case AhbDiv2: return AhbClock::ClockFreq() >> 1;
					case AhbDiv4: return AhbClock::ClockFreq() >> 2;
				}
				return 0;
			}
			
			static uint32_t ClockFreq()
			{
				return SrcClockFreq();
			}
		};
		
		class Adc34ClockSource 
		{
		public:
		
			enum ClockSource
			{
				AdcClock = 0,
				AhbDiv1 = ADC34_CCR_CKMODE_0,
				AhbDiv2 = ADC34_CCR_CKMODE_1,
				AhbDiv4 = ADC34_CCR_CKMODE_0 | ADC34_CCR_CKMODE_1
			};
			
			enum Prescaller
			{
				Div1 = 16,
				Div2 = 17,
				Div4 = 18,
				Div6 = 19,
				Div8 = 20,
				Div10 = 21,
				Div12 = 22,
				Div16 = 23,
				Div32 = 24,
				Div64 = 25,
				Div128 = 26,
				Div256 = 27
			};
			
			static bool SelectClockSource(ClockSource clockSource)
			{
				ADC3_4->CCR = (ADC3_4->CCR & ~ADC34_CCR_CKMODE) | clockSource;
				return true;
			}
			
			static void SetPrescaller(Prescaller prescaller)
			{
				Adc34Prescaller::Set((uint32_t)prescaller);
			}
			
			static uint32_t SrcClockFreq()
			{
				const uint16_t clockPresc[] = {1, 2, 4, 6, 8, 10, 12, 16, 32, 64, 128, 256};
				switch(ADC3_4->CCR & ADC12_CCR_CKMODE)
				{
					case 0: 
					{
						uint16_t divIndex = Adc34Prescaller::Get() - 16;
						if(divIndex > 11)
							divIndex = 11;
						return PllClock::ClockFreq() / clockPresc[divIndex];
					}
					case AhbDiv1: return AhbClock::ClockFreq();
					case AhbDiv2: return AhbClock::ClockFreq() >> 1;
					case AhbDiv4: return AhbClock::ClockFreq() >> 2;
				}
				return 0;
			}
			
			static uint32_t ClockFreq()
			{
				return SrcClockFreq();
			}
		};
		
		class Usart1ClockSource
		{
		public:
			enum ClockSource
			{
				Ahb = 0,
				System = 1,
				Lse = 2,
				Hsi = 3,
			};
			
			static bool SelectClockSource(ClockSource source)
			{
				Usart1ClockSwitch::Set((uint32_t)source);
				return true;
			}
			
			static uint32_t SrcClockFreq()
			{
				switch(Usart1ClockSwitch::Get())
				{
					case 0: return AhbClock::ClockFreq();
					case 1: return SysClock::ClockFreq();
					case 2: return LseClock::ClockFreq();
					case 3: return HsiClock::ClockFreq();
				}
				return 0;
			}
			
			static uint32_t ClockFreq()
			{
				return SrcClockFreq();
			}
		};
		
		IO_REG_WRAPPER(RCC->APB2ENR, PeriphClockEnable2, uint32_t);
		IO_REG_WRAPPER(RCC->APB1ENR, PeriphClockEnable1, uint32_t);
		IO_REG_WRAPPER(RCC->AHBENR, AhbClockEnableReg, uint32_t);
		
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_DMA1EN , AhbClock> Dma1Clock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_DMA2EN , AhbClock> Dma2Clock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_SRAMEN , AhbClock> SramClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_FLITFEN, AhbClock> FlitfClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_CRCEN  , AhbClock> CrcClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_GPIOAEN, AhbClock> GpioaClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_GPIOBEN, AhbClock> GpiobClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_GPIOCEN, AhbClock> GpiocClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_GPIODEN, AhbClock> GpiodClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_GPIOEEN, AhbClock> GpioeClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_GPIOFEN, AhbClock> GpiofClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_TSEN   , AhbClock> TsClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_ADC12EN, Adc12ClockSource> Adc12Clock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_ADC34EN, Adc12ClockSource> Adc34Clock;
		
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_SYSCFGEN, Apb2Clock> SyscfgClock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM1EN  , Apb2Clock> Tim1Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_SPI1EN  , Apb2Clock> Spi1Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM8EN  , Apb2Clock> Tim8Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_USART1EN, Usart1ClockSource> Usart1Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM15EN , Apb2Clock> Tim15Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM16EN , Apb2Clock> Tim16Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM17EN , Apb2Clock> Tim17Clock;
		
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM2EN  , Apb1Clock> Tim2Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM3EN  , Apb1Clock> Tim3Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM4EN  , Apb1Clock> Tim4Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM6EN  , Apb1Clock> Tim6Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM7EN  , Apb1Clock> Tim7Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_WWDGEN  , Apb1Clock> WwdgClock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_SPI2EN  , Apb1Clock> Spi2Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_SPI3EN  , Apb1Clock> Spi3Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_USART2EN, Apb1Clock> Usart2Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_USART3EN, Apb1Clock> Usart3Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART3EN , Apb1Clock> Uart3Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART4EN , Apb1Clock> Uart4Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_I2C1EN  , Apb1Clock> I2c1Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_I2C2EN  , Apb1Clock> I2c2Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_USBEN   , Apb1Clock> UsbClock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_CAN1EN  , Apb1Clock> CanClock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_PWREN   , Apb1Clock> PwrClock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_DACEN   , Apb1Clock> DacClock;
	}
}