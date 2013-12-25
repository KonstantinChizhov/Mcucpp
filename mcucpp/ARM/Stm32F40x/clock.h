#pragma once

#include "ioreg.h"
#include "stm32f4xx.h"

#ifndef F_OSC
#warning F_OSC is not defined. F_OSC is in its default value 8 MHZ. Verify that external cristal freq is correct.  
#define F_OSC 8000000u
#endif

#include "flash.h"

namespace Mcucpp
{
	namespace Clock
	{
		class ClockBase
		{
		protected:
			static const uint32_t ClockStartTimeout = 100000;
			static inline bool EnableClockSource(unsigned turnOnMask,  unsigned waitReadyMask);
			static inline bool DisablelockSource(unsigned turnOnMask,  unsigned waitReadyMask);
		};
		
		class HseClock :public ClockBase
		{
		public:
			static uint32_t SrcClockFreq(){ return F_OSC; }
			static uint32_t SetClockFreq(uint32_t) { return  ClockFreq(); }
			static uint32_t ClockFreq() { return SrcClockFreq(); }
			
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
			static uint32_t SrcClockFreq() { return 16000000u; }
			static uint32_t SetClockFreq(uint32_t) { return  ClockFreq(); }
			static uint32_t ClockFreq() { return SrcClockFreq(); }
			
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
			static uint32_t SrcClockFreq() { return 32768; }
			static uint32_t SetClockFreq(uint32_t) { return  ClockFreq(); }
			static uint32_t ClockFreq() { return SrcClockFreq();}
			
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
			static const uint32_t  VcoMaxFreq  = 432000000ul;
			static const uint32_t  VcoMinFreq  = 192000000ul;
			static const uint32_t  PllnMax     = 432ul;
			static const uint32_t  PllnMin     = 192ul;
			static const uint32_t  UsbFreq     = 48000000ul;
			static const uint32_t  PllMaxFreq  = 168000000ul;
			static const uint32_t  PllnMaxFreq = 2000000ul;
			static const uint32_t  PllnMinFreq = 1000000ul;
			static inline uint32_t CalcVco(uint32_t vco, uint32_t &resPllm, uint32_t &resPlln);
		public:
			enum ClockSource
			{
				Internal = RCC_PLLCFGR_PLLSRC_HSI,
				External = RCC_PLLCFGR_PLLSRC_HSE,
			};
			
			static inline uint32_t SrcClockFreq();
			static inline void SelectClockSource(ClockSource clockSource);
			static inline uint32_t SetClockFreq(uint32_t freq);
			static inline uint32_t ClockFreq();
			static inline bool Enable();
			static inline void Disable();
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
			static uint32_t MaxFreq() {return 168000000u;}
			static inline ErrorCode SelectClockSource(ClockSource clockSource);
			static inline uint32_t SetClockFreq(uint32_t freq);
			static inline uint32_t ClockFreq();
			static inline uint32_t SrcClockFreq();
		};
				
		
		template<class Reg, unsigned Mask, class ClockSrc>
		class ClockControl :public ClockSrc
		{
		public:
			static inline void Enable() { Reg::Or(Mask); }
			static inline void Disable() { Reg::And(~Mask); }
		};
		
		
		IO_BITFIELD_WRAPPER(RCC->CFGR, AhbPrescalerBitField, uint32_t, 4, 4);
		IO_BITFIELD_WRAPPER(RCC->CFGR, Apb1PrescalerBitField, uint32_t, 10, 3);
		IO_BITFIELD_WRAPPER(RCC->CFGR, Apb2PrescalerBitField, uint32_t, 13, 3);
		
		IO_BITFIELD_WRAPPER(RCC->CFGR, McoBitField, uint32_t, 21, 2);
		IO_BITFIELD_WRAPPER(RCC->CFGR, Mco2BitField, uint32_t, 30, 2);
		
		IO_BITFIELD_WRAPPER(RCC->CFGR, McoPreBitField, uint32_t, 24, 3);
		IO_BITFIELD_WRAPPER(RCC->CFGR, Mco2PreBitField, uint32_t, 27, 3);
		IO_BITFIELD_WRAPPER(RCC->CFGR, RtcPreBitField, uint32_t, 16, 5);
		
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
			
			static uint32_t MaxFreq() {return 168000000u;}
			
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
			
			static uint32_t MaxFreq() {return 42000000u;}
			
			static uint32_t SrcClockFreq(){ return AhbClock::ClockFreq(); }
			static uint32_t ClockFreq()
			{
				uint32_t clock = AhbClock::ClockFreq();
				uint8_t clockPrescShift[] = {0, 0, 0, 0, 1, 2, 3, 4};
				uint8_t shiftBits = clockPrescShift[Apb1PrescalerBitField::Get()];
				clock >>= shiftBits;
				return clock;
			}
			
			static void SetPrescaller(Prescaller prescaller) { Apb1PrescalerBitField::Set((uint32_t)prescaller); }
			static Prescaller GetPrescaller() { return (Prescaller)Apb2PrescalerBitField::Get(); }
			
			static void AdjustMaxFreq(uint32_t targetFreq)
			{
				Prescaller presc = Div1;
				if(targetFreq > MaxFreq()*4)
					presc = Div8;
				else if(targetFreq > MaxFreq()*2)
					presc = Div4;
				else if(targetFreq > MaxFreq())
					presc = Div2;
				SetPrescaller(presc);
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
			
			static uint32_t MaxFreq() {return 84000000u;}
			
			static uint32_t SrcClockFreq() { return AhbClock::ClockFreq(); }
			
			static uint32_t ClockFreq()
			{
				uint32_t clock = AhbClock::ClockFreq();
				const uint8_t clockPrescShift[] = {0, 0, 0, 0, 1, 2, 3, 4};
				uint8_t shiftBits = clockPrescShift[Apb2PrescalerBitField::Get()];
				clock >>= shiftBits;
				return clock;
			}
			
			static void SetPrescaller(Prescaller prescaller) { Apb2PrescalerBitField::Set((uint32_t)prescaller); }
			static Prescaller GetPrescaller() { return (Prescaller)Apb2PrescalerBitField::Get(); }
			
			static void AdjustMaxFreq(uint32_t targetFreq)
			{
				Prescaller presc = Div1;
				if(targetFreq > MaxFreq()*2)
					presc = Div4;
				else if(targetFreq > MaxFreq())
					presc = Div2;
				SetPrescaller(presc);
			}
		};
		
		
		IO_REG_WRAPPER(RCC->APB2ENR, PeriphClockEnable2, uint32_t);
		IO_REG_WRAPPER(RCC->APB1ENR, PeriphClockEnable1, uint32_t);
		
		IO_REG_WRAPPER(RCC->AHB1ENR, Ahb1ClockEnableReg, uint32_t);
		IO_REG_WRAPPER(RCC->AHB2ENR, Ahb2ClockEnableReg, uint32_t);
		IO_REG_WRAPPER(RCC->AHB3ENR, Ahb3ClockEnableReg, uint32_t);
		
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_GPIOAEN     , AhbClock> GpioaClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_GPIOBEN     , AhbClock> GpiobClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_GPIOCEN     , AhbClock> GpiocClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_GPIODEN     , AhbClock> GpiodClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_GPIOEEN     , AhbClock> GpioeClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_GPIOFEN     , AhbClock> GpiofClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_GPIOGEN     , AhbClock> GpiogClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_GPIOHEN     , AhbClock> GpiohClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_GPIOIEN     , AhbClock> GpioiClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_GPIOJEN     , AhbClock> GpiojClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_GPIOKEN     , AhbClock> GpiokClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_CRCEN       , AhbClock> CrcClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_BKPSRAMEN   , AhbClock> BkpsramClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_CCMDATARAMEN, AhbClock> CcmdataraClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_DMA1EN      , AhbClock> Dma1Clock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_DMA2EN      , AhbClock> Dma2Clock; 
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_DMA2DEN     , AhbClock> Dma2dClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_ETHMACEN    , AhbClock> EthMacClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_ETHMACTXEN  , AhbClock> EthMacTxClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_ETHMACRXEN  , AhbClock> EthMacRxClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_ETHMACPTPEN , AhbClock> EthMacPtpClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_OTGHSEN     , AhbClock> OtgHsClock;
		typedef ClockControl<Ahb1ClockEnableReg, RCC_AHB1ENR_OTGHSULPIEN , AhbClock> OtgHsUlpiClock;
		
		typedef ClockControl<Ahb2ClockEnableReg, RCC_AHB2ENR_DCMIEN      , AhbClock> DcmiClock;
		typedef ClockControl<Ahb2ClockEnableReg, RCC_AHB2ENR_CRYPEN      , AhbClock> CrypClock;
		typedef ClockControl<Ahb2ClockEnableReg, RCC_AHB2ENR_HASHEN      , AhbClock> HashClock;
		typedef ClockControl<Ahb2ClockEnableReg, RCC_AHB2ENR_RNGEN       , AhbClock> RngClock;
		typedef ClockControl<Ahb2ClockEnableReg, RCC_AHB2ENR_OTGFSEN     , AhbClock> OtgFsClock;

		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM1EN      , Apb2Clock> Tim1Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM8EN      , Apb2Clock> Tim8Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_USART1EN    , Apb2Clock> Usart1Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_USART6EN    , Apb2Clock> Usart6Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_ADC1EN      , Apb2Clock> Adc1Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_ADC2EN      , Apb2Clock> Adc2Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_ADC3EN      , Apb2Clock> Adc3Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_SDIOEN      , Apb2Clock> SdioClock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_SPI1EN      , Apb2Clock> Spi1Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_SPI4EN      , Apb2Clock> Spi4Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_SYSCFGEN    , Apb2Clock> SyscfgClock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM9EN      , Apb2Clock> Tim9Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM10EN     , Apb2Clock> Tim10Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_TIM11EN     , Apb2Clock> Tim11Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_SPI5EN      , Apb2Clock> Spi5Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_SPI6EN      , Apb2Clock> Spi6Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_SAI1EN      , Apb2Clock> Sai1Clock;
		typedef ClockControl<PeriphClockEnable2, RCC_APB2ENR_LTDCEN      , Apb2Clock> LtdcClock;
		
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM2EN      , Apb1Clock> Tim2Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM3EN      , Apb1Clock> Tim3Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM4EN      , Apb1Clock> Tim4Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM5EN      , Apb1Clock> Tim5Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM6EN      , Apb1Clock> Tim6Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM7EN      , Apb1Clock> Tim7Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM12EN     , Apb1Clock> Tim12Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM13EN     , Apb1Clock> Tim13Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_TIM14EN     , Apb1Clock> Tim14Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_WWDGEN      , Apb1Clock> WwdgClock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_SPI2EN      , Apb1Clock> Spi2Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_SPI3EN      , Apb1Clock> Spi3Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_USART2EN    , Apb1Clock> Usart2Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_USART3EN    , Apb1Clock> Usart3Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART4EN     , Apb1Clock> Uart4Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART5EN     , Apb1Clock> Uart5Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_I2C1EN      , Apb1Clock> I2c1Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_I2C2EN      , Apb1Clock> I2c2Clock;
        typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_I2C3EN      , Apb1Clock> I2c3Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_CAN1EN      , Apb1Clock> Can1Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_CAN2EN      , Apb1Clock> Can2Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_PWREN       , Apb1Clock> PwrClock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_DACEN       , Apb1Clock> DacClock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART7EN     , Apb1Clock> Uart7Clock;
		typedef ClockControl<PeriphClockEnable1, RCC_APB1ENR_UART8EN     , Apb1Clock> Uart8Clock;
		
		
		
		IO_BITFIELD_WRAPPER(RCC->PLLCFGR, PllM, uint32_t, 0, 6);
		IO_BITFIELD_WRAPPER(RCC->PLLCFGR, PllN, uint32_t, 6, 9);
		IO_BITFIELD_WRAPPER(RCC->PLLCFGR, PllP, uint32_t, 16, 2);
		IO_BITFIELD_WRAPPER(RCC->PLLCFGR, PllQ, uint32_t, 24, 4);
		
		bool ClockBase::EnableClockSource(unsigned turnOnMask,  unsigned waitReadyMask)
		{
			uint32_t timeoutCounter = 0;
			RCC->CR |= turnOnMask;
			while(((RCC->CR & waitReadyMask) == 0) && (timeoutCounter < ClockStartTimeout))
			{
				timeoutCounter++;
			}
			return (RCC->CR & waitReadyMask) != 0;
		}
		
		bool ClockBase::DisablelockSource(unsigned turnOnMask,  unsigned waitReadyMask)
		{
			uint32_t timeoutCounter = 0;
			RCC->CR &= ~turnOnMask;
			while(((RCC->CR & waitReadyMask) != 0) && (timeoutCounter < ClockStartTimeout))
			{
				timeoutCounter++;
			}
			return (RCC->CR & waitReadyMask) == 0;
		}
		
		uint32_t PllClock::CalcVco(uint32_t vco, uint32_t &resPllm, uint32_t &resPlln)
		{
			const uint32_t inputClock = SrcClockFreq();
			uint32_t vcoMinErr = vco;
			uint32_t bestVco = 0;
			for(uint32_t pllm = 2; pllm < 64; pllm++)
			{
				uint32_t pllnFreq = inputClock / pllm;
				if(pllnFreq < PllnMinFreq || pllnFreq > PllnMaxFreq)
					continue;
				uint32_t plln = (vco + pllnFreq/2) / pllnFreq;
				if(plln < PllnMin || plln > PllnMax)
					continue;
				uint32_t realVco = pllnFreq * plln;
				if(realVco < VcoMinFreq || realVco > VcoMaxFreq)
					continue;
				uint32_t vcoErr;
				if(realVco > vco)
					vcoErr = realVco - vco;
				else
					vcoErr = vco - realVco;
				if(vcoErr < vcoMinErr)
				{
					vcoMinErr = vcoErr;
					bestVco = realVco;
					resPllm = pllm;
					resPlln = plln;
				}
				if(vcoErr == 0)
					break;
			}
			return bestVco;
		}
		
		uint32_t PllClock::SrcClockFreq()
		{
			if ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == 0)
				return HsiClock::ClockFreq();
			else
				return HseClock::ClockFreq();
		}
		
		void PllClock::SelectClockSource(ClockSource clockSource)
		{
			RCC->PLLCFGR = (RCC->PLLCFGR & ~(RCC_PLLCFGR_PLLSRC)) | clockSource;
		}
		
		uint32_t PllClock::SetClockFreq(uint32_t freq)
		{
			if(freq > PllMaxFreq)
				freq = PllMaxFreq;
							
			uint32_t	resPllp = 0, 
						resPllq = 0, 
						resPllm = 0,
						resPlln = 0,
						minErr  = freq,
						bestFreq = 0;
				
			for(uint32_t pllq = 2; pllq < 16; pllq++)
			{
				uint32_t vco = UsbFreq * pllq;
				if(vco < VcoMinFreq || vco > VcoMaxFreq)
					continue;
				uint32_t plln, pllm;
				vco = CalcVco(vco, pllm, plln);
				if(vco == 0)
					continue;
				uint32_t pllp = (vco + freq/2) / freq;
				if(pllp == 0 || ((pllp & 1) != 0))
					continue;
				uint32_t realFreq = vco / pllp;
				if(realFreq > PllMaxFreq)
					continue;
				uint32_t err;
				if(realFreq > freq)
					err = realFreq - freq;
				else 
					err = freq - realFreq;
				if(err < minErr)
				{
					minErr = err;
					resPllp = pllp;
					resPllq = pllq;
					resPlln = plln;
					resPllm = pllm;
					bestFreq = realFreq;
				}
				if(err == 0) break;
			}
			if(resPllp == 0 || resPlln == 0 || resPllm == 0 || resPllq == 0)
				return 0;
			PllN::Set(resPlln);
			PllM::Set(resPllm);
			PllP::Set((resPllp - 2) / 2);
			PllQ::Set(resPllq);
			return bestFreq;
		}
		
		uint32_t PllClock::ClockFreq()
		{
			uint32_t plln = PllN::Get();
			uint32_t pllm = PllM::Get();
			uint32_t pllp = (PllP::Get() + 1) * 2;
			if(pllm == 0)
				return 0;
			uint32_t div = pllm * pllp;
			return SrcClockFreq() / div * plln;
		}
		
		bool PllClock::Enable()
		{
			if ((RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC) == 0)
			{
				if (!HsiClock::Enable())
					return false;
			}
			else
				if (!HseClock::Enable())
					return false;
			return ClockBase::EnableClockSource(RCC_CR_PLLON, RCC_CR_PLLRDY);
		}
		
		void PllClock::Disable()
		{
			ClockBase::DisablelockSource(RCC_CR_PLLON, RCC_CR_PLLRDY);
		}
		
		SysClock::ErrorCode SysClock::SelectClockSource(ClockSource clockSource)
		{
			uint32_t clockStatusValue;
			uint32_t clockSelectMask;
			uint32_t currentFreq = ClockFreq();
			uint32_t targetFreq;
			
			if(clockSource == Internal)
			{
				clockStatusValue = RCC_CFGR_SWS_HSI;
				clockSelectMask = RCC_CFGR_SW_HSI;
				if (!HsiClock::Enable())
				{
					return ClockSourceFailed;
				}
				targetFreq = HsiClock::ClockFreq();
			}else if(clockSource == External)
			{
				clockStatusValue = RCC_CFGR_SWS_HSE;
				clockSelectMask = RCC_CFGR_SW_HSE;
				if (!HseClock::Enable())
				{
					return ClockSourceFailed;
				}
				targetFreq = HseClock::ClockFreq();
			}else if(clockSource == Pll)
			{
				clockStatusValue = RCC_CFGR_SWS_PLL;
				clockSelectMask = RCC_CFGR_SW_PLL;
				if (!PllClock::Enable())
				{
					return ClockSourceFailed;
				}
				targetFreq = PllClock::ClockFreq();
			}else
				return InvalidClockSource;
				
			RCC->APB1ENR |= RCC_APB1ENR_PWREN;
			PWR->CR |= PWR_CR_VOS;
			RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
			RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
			RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

			if(currentFreq < targetFreq)
				Flash::ConfigureFreq(targetFreq);
				
			Apb1Clock::AdjustMaxFreq(targetFreq);
			Apb2Clock::AdjustMaxFreq(targetFreq);
				
			RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | clockSelectMask;
			uint32_t timeout = 10000;
			while ((RCC->CFGR & RCC_CFGR_SWS) != clockStatusValue && --timeout)
			{
			}
			if(timeout == 0)
			{
				return ClockSelectFailed;
			}
			if(currentFreq > targetFreq)
				Flash::ConfigureFreq(targetFreq);
			return Success;
		}
		
		uint32_t SysClock::SetClockFreq(uint32_t freq)
		{
			SelectClockSource(Internal);
			PllClock::Disable();
			PllClock::SelectClockSource(PllClock::External);
			PllClock::SetClockFreq(freq);
			SelectClockSource(Pll);
			return ClockFreq();
		}
		
		uint32_t SysClock::ClockFreq()
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
		
		uint32_t SysClock::SrcClockFreq()
		{
			return ClockFreq();
		}
	}
}