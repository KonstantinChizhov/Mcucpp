#pragma once

#include "ioreg.h"
#include "stm32f10x.h"

namespace Clock
{
	class SysClock
	{
	public:
		static unsigned FCore()
		{
			// TODO: implement this
			return 8000000;
		}
		
		static unsigned FPeriph()
		{
			// TODO: implement this
			return 8000000;
		}
	};
	
	IO_REG_WRAPPER(RCC->APB2ENR, PeriphClockEnable2, uint32_t);
	IO_REG_WRAPPER(RCC->APB1ENR, PeriphClockEnable1, uint32_t);
	IO_REG_WRAPPER(RCC->AHBENR, AhbClockEnableReg, uint32_t);
	
	
	template<class Reg, unsigned Mask>
	class ClockControl
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
	
	typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_DMA1EN> Dma1Clock;
	typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_SRAMEN> SramClock;
	typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_FLITFEN> FlitfClock;
#if defined (STM32F10X_HD) || defined  (STM32F10X_CL) || defined  (STM32F10X_HD_VL)
	typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_DMA2EN> Dma2Clock;
#endif

#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
	typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_FSMCEN> FsmcClock;
	typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_SDIOEN> SdioClock;
#endif

#if defined (STM32F10X_HD_VL)
	typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_FSMCEN> FsmcClock;
#endif

#ifdef STM32F10X_CL
	typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_OTGFSEN> OtgFsClock;
	typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_ETHMACEN> MacClock;
	typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_ETHMACTXEN> MacTxClock;
	typedef ClockControl<AhbClockEnableReg, RCC_AHBENR_ETHMACRXEN> MacRxClock;
#endif /* STM32F10X_CL */

	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_AFIOEN> AfioClock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPAEN> PortaClock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPBEN> PortbClock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPCEN> PortcClock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPDEN> PortdClock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_ADC1EN> Adc1Clock;

#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_ADC2EN> Adc2Clock;
#endif

	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM1EN> Tim1Clock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_SPI1EN> Spi1Clock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_USART1EN> Usart1Clock;

#if defined (STM32F10X_LD_VL) || defined (STM32F10X_MD_VL) || defined (STM32F10X_HD_VL)
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM15EN> Tim15Clock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM16EN> Tim16Clock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM17EN> Tim17Clock;
#endif

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPEEN> PorteClock;
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

#if defined (STM32F10X_HD) || defined (STM32F10X_XL)
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPFEN> PortfClock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPGEN> PortgClock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM8EN> Tim8Clock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_ADC3EN> Adc3Clock;
#endif

#if defined (STM32F10X_HD_VL)
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPFEN> PortfClock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_IOPGEN> PortgClock;
#endif

#ifdef STM32F10X_XL
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM9EN> Tim9Clock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM10EN> Tim10Clock;
	typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM11EN> Tim11Clock;
#endif

	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM2EN> Tim2Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM3EN> Tim3Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_WWDGEN> WatchDogClock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_USART2EN> Usart2Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_I2C1EN> I2c1Clock;


#if !defined (STM32F10X_LD_VL) && !defined (STM32F10X_MD_VL) && !defined (STM32F10X_HD_VL)
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_CAN1EN> Can1Clock;
#endif

	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_BKPEN> BackupClock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_PWREN> PowerClock;

#if !defined (STM32F10X_LD) && !defined (STM32F10X_LD_VL)
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM4EN> Tim4Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_SPI2EN> Spi2Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_USART3EN> Usart3Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_I2C2EN> I2c2Clock;
#endif /* STM32F10X_LD && STM32F10X_LD_VL */

#if defined (STM32F10X_HD) || defined (STM32F10X_MD) || defined  (STM32F10X_LD)
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_USBEN> UsbClock;
#endif

#if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM5EN> Tim5Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM6EN> Tim6Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM7EN> Tim7Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_SPI3EN> Spi3Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART4EN> Usart4Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART5EN> Usart5Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_DACEN> DacClock;
#endif

#if defined (STM32F10X_LD_VL) || defined  (STM32F10X_MD_VL) || defined  (STM32F10X_HD_VL)
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM6EN> Tim6Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM7EN> Tim7Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_DACEN> DacClock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_CECEN> CecClock;
#endif

#ifdef STM32F10X_HD_VL
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM5EN> Tim5Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM12EN> Tim12Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM13EN> Tim13Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM14EN> Tim14Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_SPI3EN> Spi3Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART4EN> Usart4Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART5EN> Usart5Clock;
#endif /* STM32F10X_HD_VL */

#ifdef STM32F10X_CL
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_CAN2EN> Can2Clock;
#endif /* STM32F10X_CL */

#ifdef STM32F10X_XL
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM12EN> Tim12Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM13EN> Tim13Clock;
	typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM14EN> Tim14Clock;
#endif /* STM32F10X_XL */

}