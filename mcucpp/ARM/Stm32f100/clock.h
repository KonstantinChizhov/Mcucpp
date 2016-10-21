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
			
			static inline uint32_t SetClockFreq(uint32_t freq);
			
			static clock_freq_t SrcClockFreq()
			{
				if ((RCC->CFGR & RCC_CFGR_PLLSRC) == 0)
					return HsiClock::ClockFreq();
				else
					return HseClock::ClockFreq();
			}
			
			static clock_freq_t GetDivider()
			{
				if ((RCC->CFGR & RCC_CFGR_PLLSRC) == 0)
					return 2;
				else
				#if defined(STM32F10X_CL)
					return (RCC->CFGR2 & RCC_CFGR2_PREDIV1) + 1;
				#else
					return ((RCC->CFGR & RCC_CFGR_PLLXTPRE) >> 17) + 1;
				#endif
			}
			
			static clock_freq_t GetMultipler()
			{
			#if defined(STM32F10X_CL)
				clock_freq_t mul = ((RCC->CFGR & RCC_CFGR_PLLMULL) >> 18);
				if(mul == 13) return 6;
				return mul + 2;
			#else
				return ((RCC->CFGR & RCC_CFGR_PLLMULL) >> 18) + 2;
			#endif
			}
			
			static void SetMultipler(uint8_t multiler)
			{
	
			#if defined(STM32F10X_CL)
				if(multiler > 9)
					multiler = 9;
				if(multiler < 4)
					multiler = 4;
			#else
				if(multiler > 16)
					multiler = 16;
			#endif
				multiler-=2;
				RCC->CFGR = (RCC->CFGR & RCC_CFGR_PLLMULL) | (multiler << 18);
			}
			
			static void SetDivider(uint8_t divider)
			{
				#if defined(STM32F10X_CL)
					if(divider > 15)
						divider = 15;
					divider-=1;
					RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PREDIV1) | (divider);
				#else
					if(divider > 2)
						divider = 2;
					divider-=1;
					RCC->CFGR = (RCC->CFGR & RCC_CFGR_PLLXTPRE) | (divider << 17);
				#endif
			}
			
			static void SelectClockSource(ClockSource clockSource)
			{
				RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE) ) | clockSource;
			}
			
			static clock_freq_t ClockFreq()
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

	//=======================================================================================
	
	clock_freq_t PllClock::SetClockFreq(clock_freq_t freq)
	{
		if(freq > SysClock::MaxFreq())
			freq = SysClock::MaxFreq();
						
		uint32_t	resPllDiv = 0, 
					resPllMul = 0;
		
		uint32_t srcFreq = SrcClockFreq();
		
		#if defined(STM32F10X_CL)
		resPllDiv = 16;
		#else
		resPllDiv = 2;
		#endif
		
		while(resPllDiv > 1)
		{
			uint32_t pllMul = resPllDiv * freq / srcFreq;
			
			#if defined(STM32F10X_CL)
			if(pllMul <= 9 && pllMul >= 4)
			{
				resPllMul = pllMul;
				break;
			}
			#else
			if(pllMul <= 16)
			{
				resPllMul = pllMul;
				break;
			}
			#endif
			resPllDiv--;
		}
		
		SetDivider(resPllDiv);
		SetMultipler(resPllMul);
		
		return ClockFreq();
	}
	
	clock_freq_t SysClock::MaxFreq()
	{
		#if defined(STM32F10X_CL)
			return 72000000;
		#else
			return 24000000;
		#endif
	}

	SysClock::ErrorCode SysClock::SelectClockSource(ClockSource clockSource)
	{
		uint32_t clockStatusValue;
		uint32_t clockSelectMask;
		
		if(clockSource == Internal)
		{
			clockStatusValue = RCC_CFGR_SWS_HSI;
			clockSelectMask = RCC_CFGR_SW_HSI;
			if (!HsiClock::Enable())
				return ClockSourceFailed;
		}else if(clockSource == External)
		{
			clockStatusValue = RCC_CFGR_SWS_HSE;
			clockSelectMask = RCC_CFGR_SW_HSE;
			if (!HseClock::Enable())
				return ClockSourceFailed;
		}if(clockSource == Pll)
		{
			clockStatusValue = RCC_CFGR_SWS_PLL;
			clockSelectMask = RCC_CFGR_SW_PLL;
			if (!PllClock::Enable())
				return ClockSourceFailed;
		}else
			return InvalidClockSource;

		RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | clockSelectMask;
		
		uint32_t timeout = 10000;
		while ((RCC->CFGR & RCC_CFGR_SWS) != clockStatusValue  && --timeout)
		{
			;
		}
		if(timeout == 0)
		{
			return ClockSelectFailed;
		}
		return Success;
	}
	
	clock_freq_t SysClock::ClockFreq()
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
	
	
	clock_freq_t SysClock::SetClockFreq(clock_freq_t freq)
	{
		SelectClockSource(Internal);
		PllClock::Disable();
		PllClock::SelectClockSource(PllClock::External);
		PllClock::SetClockFreq(freq);
		SelectClockSource(Pll);
		return ClockFreq();
	}
	
}
}