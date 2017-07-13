#pragma once

#include "ioreg.h"
#include "stm32f10x.h"

#ifndef F_OSC
#warning F_OSC is not defined. F_OSC is defaulted to HSE_VALUE. Verify that external cristal freq is correct.  
#define F_OSC HSE_VALUE
#endif

namespace Mcucpp
{
	typedef uint32_t clock_freq_t;
	namespace Clock
	{
		
		class ClockBase
		{
		protected:
			static const uint32_t ClockStartTimeout = 4000;
			
			static bool EnableClockSource(unsigned turnOnMask,  unsigned waitReadyMask);
			
			static bool DisablelockSource(unsigned turnOnMask,  unsigned waitReadyMask);
		};
		
		class HseClock :public ClockBase
		{
		public:
			static clock_freq_t SrcClockFreq()
			{
				return F_OSC;
			}
			
			static clock_freq_t GetDivider() { return 1; }
			
			static clock_freq_t GetMultipler() { return 1; }
			
			static clock_freq_t ClockFreq()
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
			static clock_freq_t SrcClockFreq()
			{
				return 8000000u;
			}
			
			static clock_freq_t GetDivider() { return 1; }
			
			static clock_freq_t GetMultipler() { return 1; }
			
			static clock_freq_t ClockFreq()
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
		
		class PllClock :public ClockBase
		{
		public:
			enum ClockSource
			{
				Internal = RCC_CFGR_PLLSRC_HSI_Div2,
				External = RCC_CFGR_PLLSRC_PREDIV1,
			};
			
			static uint32_t SetClockFreq(uint32_t freq);
			
			static clock_freq_t SrcClockFreq();
			
			static clock_freq_t GetDivider();
			
			static clock_freq_t GetMultipler();
			
			static void SetMultipler(uint8_t multiler);
			
			static void SetDivider(uint8_t divider);
			
			static void SelectClockSource(ClockSource clockSource);
			
			static clock_freq_t ClockFreq();
			
			static bool Enable();
			
			static void Disable();
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
			
			enum ErrorCode
			{
				Success = 0,
				ClockSourceFailed = 1,
				InvalidClockSource = 2,
				ClockSelectFailed = 3
			};
			
		public:
			static clock_freq_t MaxFreq();
			static ErrorCode SelectClockSource(ClockSource clockSource);
			static clock_freq_t ClockFreq();
			
			static clock_freq_t SrcClockFreq()
			{
				return ClockFreq();
			}
			
			static clock_freq_t SetClockFreq(clock_freq_t freq);
		};
		
		IO_REG_WRAPPER(RCC->APB2ENR, PeriphClockEnable2, uint32_t);
		IO_REG_WRAPPER(RCC->APB1ENR, PeriphClockEnable1, uint32_t);
		IO_REG_WRAPPER(RCC->AHBENR, AhbClockEnableReg, uint32_t);
		
		IO_BITFIELD_WRAPPER(RCC->CFGR, AhbPrescalerBitField, uint32_t, 4, 4);
		IO_BITFIELD_WRAPPER(RCC->CFGR, Apb1PrescalerBitField, uint32_t, 8, 3);
		IO_BITFIELD_WRAPPER(RCC->CFGR, Apb2PrescalerBitField, uint32_t, 11, 3);
		IO_BITFIELD_WRAPPER(RCC->CFGR, AdcPrescaller, uint32_t, 14, 2);
		
		IO_BITFIELD_WRAPPER(RCC->CFGR, McoBitField, uint32_t, 24, 3);
		
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
			
			static clock_freq_t SrcClockFreq()
			{
				return SysClock::ClockFreq();
			}
			
			static clock_freq_t ClockFreq()
			{
				clock_freq_t clock = SysClock::ClockFreq();
				uint8_t clockPrescShift[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
				uint8_t shiftBits = clockPrescShift[AhbPrescalerBitField::Get()];
				clock >>= shiftBits;
				return clock;
			}
			
			static void SetPrescaller(Prescaller prescaller)
			{
				AhbPrescalerBitField::Set((clock_freq_t)prescaller);
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
			
			static clock_freq_t SrcClockFreq()
			{
				return AhbClock::ClockFreq();
			}
			
			static clock_freq_t ClockFreq()
			{
				clock_freq_t clock = AhbClock::ClockFreq();
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
			
			static clock_freq_t SrcClockFreq()
			{
				return AhbClock::ClockFreq();
			}
			
			static clock_freq_t ClockFreq()
			{
				clock_freq_t clock = AhbClock::ClockFreq();
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
		
		class AdcClockSource 
		{
		public:
		
			enum ClockSource
			{
				Apb2 = 0,
			};
			
			enum Prescaller
			{
				Div2 = 0,
				Div4 = 1,
				Div6 = 2,
				Div8 = 3,
			};
			
			static bool SelectClockSource(ClockSource)
			{
				return true;
			}
			
			static void SetPrescaller(Prescaller prescaller)
			{
				AdcPrescaller::Set((uint32_t)prescaller);
			}
			
			static clock_freq_t SrcClockFreq()
			{
				return Apb2Clock::ClockFreq();
			}
			
			static clock_freq_t ClockFreq()
			{
				return SrcClockFreq() / ((AdcPrescaller::Get() + 1) * 2);
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
		
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_DMA1EN, AhbClock> Dma1Clock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_SRAMEN, AhbClock> SramClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_FLITFEN, AhbClock> FlitfClock;
	#if defined (STM32F10X_HD) || defined  (STM32F10X_CL) || defined  (STM32F10X_HD_VL)
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_DMA2EN, AhbClock> Dma2Clock;
	#endif

	#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_FSMCEN, AhbClock> FsmcClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_SDIOEN, AhbClock> SdioClock;
	#endif

	#if defined (STM32F10X_HD_VL)
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_FSMCEN, AhbClock> FsmcClock;
	#endif

	#ifdef STM32F10X_CL
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_OTGFSEN, AhbClock> OtgFsClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_ETHMACEN, AhbClock> MacClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_ETHMACTXEN, AhbClock> MacTxClock;
		typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_ETHMACRXEN, AhbClock> MacRxClock;
	#endif /* STM32F10X_CL */

		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_AFIOEN, Apb2Clock> AfioClock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPAEN, Apb2Clock> PortaClock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPBEN, Apb2Clock> PortbClock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPCEN, Apb2Clock> PortcClock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPDEN, Apb2Clock> PortdClock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_ADC1EN, AdcClockSource> Adc1Clock;

	#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_ADC2EN, Apb2Clock> Adc2Clock;
	#endif

		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM1EN, Apb2Clock> Tim1Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_SPI1EN, Apb2Clock> Spi1Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_USART1EN, Apb2Clock> Usart1Clock;

	#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM15EN, Apb2Clock> Tim15Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM16EN, Apb2Clock> Tim16Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM17EN, Apb2Clock> Tim17Clock;
	#endif

	#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPEEN, Apb2Clock> PorteClock;
	#endif /* STM32F10X_LD && STM32F10X_LD_VL */

	#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPFEN, Apb2Clock> PortfClock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPGEN, Apb2Clock> PortgClock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM8EN, Apb2Clock> Tim8Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_ADC3EN, Apb2Clock> Adc3Clock;
	#endif

	#if defined (STM32F10X_HD_VL)
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPFEN, Apb2Clock> PortfClock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPGEN, Apb2Clock> PortgClock;
	#endif

	#ifdef STM32F10X_XL
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM9EN, Apb2Clock> Tim9Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM10EN, Apb2Clock> Tim10Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM11EN, Apb2Clock> Tim11Clock;
	#endif

		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM2EN, Apb1Clock> Tim2Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM3EN, Apb1Clock> Tim3Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_WWDGEN, Apb1Clock> WatchDogClock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_USART2EN, Apb1Clock> Usart2Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_I2C1EN, Apb1Clock> I2c1Clock;


	#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_CAN1EN, Apb1Clock> Can1Clock;
	#endif

		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_BKPEN, Apb1Clock> BackupClock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_PWREN, Apb1Clock> PowerClock;

	#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM4EN, Apb1Clock> Tim4Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_SPI2EN, Apb1Clock> Spi2Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_USART3EN, Apb1Clock> Usart3Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_I2C2EN, Apb1Clock> I2c2Clock;
	#endif /* STM32F10X_LD && STM32F10X_LD_VL */

	#if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined  (STM32F10X_LD)
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_USBEN, Apb1Clock> UsbClock;
	#endif

	#if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM5EN, Apb1Clock> Tim5Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM6EN, Apb1Clock> Tim6Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM7EN, Apb1Clock> Tim7Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_SPI3EN, Apb1Clock> Spi3Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART4EN, Apb1Clock> Usart4Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART5EN, Apb1Clock> Usart5Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_DACEN, Apb1Clock> DacClock;
	#endif

	#if defined (STM32F10X_LD_VL) || defined  (STM32F10X_MD_VL) || defined  (STM32F10X_HD_VL)
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM6EN, Apb1Clock> Tim6Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM7EN, Apb1Clock> Tim7Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_DACEN, Apb1Clock> DacClock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_CECEN, Apb1Clock> CecClock;
	#endif

	#ifdef STM32F10X_HD_VL
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM5EN, Apb1Clock> Tim5Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM12EN, Apb1Clock> Tim12Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM13EN, Apb1Clock> Tim13Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM14EN, Apb1Clock> Tim14Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_SPI3EN, Apb1Clock> Spi3Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART4EN, Apb1Clock> Usart4Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART5EN, Apb1Clock> Usart5Clock;
	#endif /* STM32F10X_HD_VL */

	#ifdef STM32F10X_CL
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_CAN2EN, Apb1Clock> Can2Clock;
	#endif /* STM32F10X_CL */

	#ifdef STM32F10X_XL
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM12EN, Apb1Clock> Tim12Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM13EN, Apb1Clock> Tim13Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM14EN, Apb1Clock> Tim14Clock;
	#endif /* STM32F10X_XL */
	

}
}