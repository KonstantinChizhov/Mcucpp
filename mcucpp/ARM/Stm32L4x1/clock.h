//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2018
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************

#pragma once

#include "ioreg.h"
#include "mcu_header.h"

#ifndef F_OSC
#warning F_OSC is not defined. F_OSC is in its default value 8 MHZ. Verify that external cristal freq is correct.
#define F_OSC 8000000u
#endif

#include "flash.h"
#include "template_utils.h"

namespace Mcucpp
{
	namespace Clock
	{
		enum class PllClockSource
		{
			None = 0,
			Msi = 1,
			Internal = 2,
			External = 3
		};

		enum class SysClockSource
		{
			Msi = 0,
			Internal = 1,
			External = 2,
			Pll = 3
		};

		enum class ClockErrorCode
		{
			Success = 0,
			ClockSourceFailed = 1,
			InvalidClockSource = 2,
			ClockSelectFailed = 3
		};

		enum class AhbPrescaller
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

		enum class ApbPrescaller
		{
			Div1 = 0,
			Div2 = 0x04,
			Div4 = 0x05,
			Div8 = 0x06,
			Div16 = 0x07,
		};

		enum class AdcClockSel
		{
			None,
			PllSai1,
			PllSai2,
			SysClock
		};

		IO_BITFIELD_WRAPPER(RCC->CFGR, SysClockSwitch, SysClockSource, 0, 2);
		IO_BITFIELD_WRAPPER(RCC->CFGR, SysClockStatus, SysClockSource, 2, 2);

		IO_BITFIELD_WRAPPER(RCC->CFGR, AhbPrescalerBitField, AhbPrescaller, 4, 4);
		IO_BITFIELD_WRAPPER(RCC->CFGR, Apb1PrescalerBitField, ApbPrescaller, 8, 3);
		IO_BITFIELD_WRAPPER(RCC->CFGR, Apb2PrescalerBitField, ApbPrescaller, 11, 3);

		IO_BITFIELD_WRAPPER(RCC->CFGR, McoSelBitField, uint32_t, 24, 3);
		IO_BITFIELD_WRAPPER(RCC->CFGR, McoPreBitField, uint32_t, 28, 3);

		IO_BITFIELD_WRAPPER(RCC->CR, MsiPange, uint32_t, 4, 4);

		IO_BITFIELD_WRAPPER(RCC->PLLCFGR, PllSrc, PllClockSource, 0, 2);
		IO_BITFIELD_WRAPPER(RCC->PLLCFGR, PllM, uint32_t, 4, 3);
		IO_BITFIELD_WRAPPER(RCC->PLLCFGR, PllN, uint32_t, 8, 7);
		IO_BITFIELD_WRAPPER(RCC->PLLCFGR, PllP, uint32_t, 17, 1);
		IO_BITFIELD_WRAPPER(RCC->PLLCFGR, PllQ, uint32_t, 21, 2);
		IO_BITFIELD_WRAPPER(RCC->PLLCFGR, PllR, uint32_t, 25, 2);


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

		static const uint16_t msiFreqTable[] = {1, 2, 4, 8, 10, 20, 40, 80, 160, 240, 320, 480};

		class MsiClock :public ClockBase
		{
		public:
			static uint32_t SrcClockFreq()
			{
				uint32_t range = MsiPange::Get();
				return msiFreqTable[range] * 100000;
			}

			static uint32_t SetClockFreq(uint32_t clockFreq)
			{
				if((RCC->CR & (RCC_CR_MSION | RCC_CR_MSIRGSEL)) != (RCC_CR_MSION | RCC_CR_MSIRGSEL))
				{
					RCC->CR |= RCC_CR_MSION | RCC_CR_MSIRGSEL;
				}
				while(RCC->CR & RCC_CR_MSIRDY)
				{
					;
				}
				clockFreq /= 100000;
				uint32_t range = 6;
				for(unsigned i = 0; i < sizeof(msiFreqTable) / sizeof(msiFreqTable[0]); i++)
				{
					if(msiFreqTable[i] >= clockFreq)
					{
						range = i;
						break;
					}
				}
				MsiPange::Set(range);
				return ClockFreq();
			}

			static uint32_t ClockFreq() { return SrcClockFreq(); }

			static bool Enable()
			{
				return ClockBase::EnableClockSource(RCC_CR_MSION, RCC_CR_MSIRDY);
			}

			static bool Disable()
			{
				return ClockBase::DisablelockSource(RCC_CR_MSION, RCC_CR_MSIRDY);
			}
		};


		class PllClock :public ClockBase
		{
			static const uint32_t  VcoMaxFreq  = 344000000ul;
			static const uint32_t  VcoMinFreq  = 64000000ul;
			static const uint32_t  PllnMax     = 86ul;
			static const uint32_t  PllnMin     = 8ul;
			static const uint32_t  PllmMax     = 8ul;
			static const uint32_t  PllmMin     = 1ul;
			static const uint32_t  PllrMax     = 8ul;
			static const uint32_t  PllrMin     = 2ul;

			static const uint32_t  Freq48Mhz   = 48000000ul;
			static const uint32_t  PllMaxFreq  = 80000000;
			static const uint32_t  PllnMaxFreq = 16000000ul;
			static const uint32_t  PllnMinFreq = 4000000ul;

			static inline uint32_t CalcVco(uint32_t vco, uint32_t &resPllm, uint32_t &resPlln);
		public:

			static inline uint32_t SrcClockFreq();
			static inline void SelectClockSource(PllClockSource clockSource);
			static inline uint32_t SetClockFreq(uint32_t freq);
			static inline uint32_t ClockFreq();
			static inline bool Enable();
			static inline void Disable();
		};


		class SysClock
		{
		public:
			static uint32_t MaxFreq() {return F_CPU;}
			static inline ClockErrorCode SelectClockSource(SysClockSource clockSource);
			static inline uint32_t SetClockFreq(uint32_t freq);
			static inline uint32_t ClockFreq();
			static inline uint32_t SrcClockFreq();
		};


		template<class Reg, unsigned Mask, class ResetReg, unsigned ResetMask, class ClockSrc>
		class ClockResetControl :public ClockSrc
		{
		public:
			static inline void Enable() { Reg::Or(Mask); __DSB(); }
			static inline void Disable() { Reg::And(~Mask); __DSB(); }
			static inline void Reset() { ResetReg::Or(ResetMask); __DSB(); ResetReg::And(~ResetMask); }
			static inline void SelectClockSource(SysClockSource clockSource);
		};



		class AhbClock
		{
		public:

			static uint32_t MaxFreq() {return F_CPU;}

			static uint32_t SrcClockFreq()
			{
				return SysClock::ClockFreq();
			}

			static uint32_t ClockFreq()
			{
				uint32_t clock = SysClock::ClockFreq();
				static const uint8_t clockPrescShift[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 6, 7, 8, 9};
				uint8_t shiftBits = clockPrescShift[(unsigned)AhbPrescalerBitField::Get()];
				clock >>= shiftBits;
				return clock;
			}

			static void SetPrescaller(AhbPrescaller prescaller)
			{
				AhbPrescalerBitField::Set(prescaller);
			}
		};

		class Apb1Clock
		{
		public:

			static uint32_t MaxFreq() {return 80000000u;}

			static uint32_t SrcClockFreq(){ return AhbClock::ClockFreq(); }
			static uint32_t ClockFreq()
			{
				uint32_t clock = AhbClock::ClockFreq();
				uint8_t clockPrescShift[] = {0, 0, 0, 0, 1, 2, 3, 4};
				uint8_t shiftBits = clockPrescShift[(unsigned)Apb1PrescalerBitField::Get()];
				clock >>= shiftBits;
				return clock;
			}

			static void SetPrescaller(ApbPrescaller prescaller) { Apb1PrescalerBitField::Set(prescaller); }
			static ApbPrescaller GetPrescaller() { return Apb2PrescalerBitField::Get(); }

			static void AdjustMaxFreq(uint32_t targetFreq)
			{
				ApbPrescaller presc = ApbPrescaller::Div1;
				if(targetFreq > MaxFreq()*4)
					presc = ApbPrescaller::Div8;
				else if(targetFreq > MaxFreq()*2)
					presc = ApbPrescaller::Div4;
				else if(targetFreq > MaxFreq())
					presc = ApbPrescaller::Div2;
				SetPrescaller(presc);
			}
		};

		class Apb2Clock
		{
		public:

			static uint32_t MaxFreq() {return 80000000u;}

			static uint32_t SrcClockFreq() { return AhbClock::ClockFreq(); }

			static uint32_t ClockFreq()
			{
				uint32_t clock = AhbClock::ClockFreq();
				const uint8_t clockPrescShift[] = {0, 0, 0, 0, 1, 2, 3, 4};
				uint8_t shiftBits = clockPrescShift[(unsigned)Apb2PrescalerBitField::Get()];
				clock >>= shiftBits;
				return clock;
			}

			static void SetPrescaller(ApbPrescaller prescaller) { Apb2PrescalerBitField::Set(prescaller); }
			static ApbPrescaller GetPrescaller() { return Apb2PrescalerBitField::Get(); }

			static void AdjustMaxFreq(uint32_t targetFreq)
			{
				ApbPrescaller presc = ApbPrescaller::Div1;
				if(targetFreq > MaxFreq()*2)
					presc = ApbPrescaller::Div4;
				else if(targetFreq > MaxFreq())
					presc = ApbPrescaller::Div2;
				SetPrescaller(presc);
			}
		};


		IO_REG_WRAPPER(RCC->APB1ENR1, Apb1ClockReg1, uint32_t);
		IO_REG_WRAPPER(RCC->APB1ENR2, Apb1ClockReg2, uint32_t);
		IO_REG_WRAPPER(RCC->APB2ENR, Apb2ClockReg, uint32_t);

		IO_REG_WRAPPER(RCC->AHB1ENR, Ahb1ClockEnableReg, uint32_t);
		IO_REG_WRAPPER(RCC->AHB2ENR, Ahb2ClockEnableReg, uint32_t);
		IO_REG_WRAPPER(RCC->AHB3ENR, Ahb3ClockEnableReg, uint32_t);

		IO_REG_WRAPPER(RCC->APB1RSTR1, Apb1ResetReg1, uint32_t);
		IO_REG_WRAPPER(RCC->APB1RSTR2, Apb1ResetReg2, uint32_t);

		IO_REG_WRAPPER(RCC->APB2RSTR, Apb2ResetReg, uint32_t);

		IO_REG_WRAPPER(RCC->AHB1RSTR, Ahb1ResetReg, uint32_t);
		IO_REG_WRAPPER(RCC->AHB2RSTR, Ahb2ResetReg, uint32_t);
		IO_REG_WRAPPER(RCC->AHB3RSTR, Ahb3ResetReg, uint32_t);

		enum class LpUsartClockSrc
		{
			Pclk,
			SysClock,
			Hsi,
			Lse
		};
		
		enum class I2cClockSrc
		{
			Pclk,
			SysClock,
			Hsi
		};

		IO_BITFIELD_WRAPPER(RCC->CCIPR, Usart1Sel,   uint32_t, 0, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, Usart2Sel,   uint32_t, 2, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, Usart3Sel,   uint32_t, 4, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, Usart4Sel,   uint32_t, 6, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, Usart5Sel,   uint32_t, 8, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, LpUsart1Sel, LpUsartClockSrc, 10, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, I2c1Sel,     I2cClockSrc, 12, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, I2c2Sel,     I2cClockSrc, 14, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, I2c3Sel,     I2cClockSrc, 16, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, LpTim1Sel,   uint32_t, 18, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, LpTim2Sel,   uint32_t, 20, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, Sai1Sel,     uint32_t, 22, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, Sai2Sel,     uint32_t, 24, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, Clk48Sel,    uint32_t, 26, 2);
		IO_BITFIELD_WRAPPER(RCC->CCIPR, AdcSel,      AdcClockSel, 28, 2);


		typedef ClockResetControl<Ahb1ClockEnableReg, RCC_AHB1ENR_DMA1EN      , Ahb1ResetReg, RCC_AHB1RSTR_DMA1RST, AhbClock> Dma1Clock;
		typedef ClockResetControl<Ahb1ClockEnableReg, RCC_AHB1ENR_DMA2EN      , Ahb1ResetReg, RCC_AHB1RSTR_DMA2RST, AhbClock> Dma2Clock;
		typedef ClockResetControl<Ahb1ClockEnableReg, RCC_AHB1ENR_CRCEN       , Ahb1ResetReg, RCC_AHB1RSTR_CRCRST, AhbClock> CrcClock;
		typedef ClockResetControl<Ahb1ClockEnableReg, RCC_AHB1ENR_FLASHEN     , Ahb1ResetReg, RCC_AHB1RSTR_FLASHRST, AhbClock> FlashClock;
		typedef ClockResetControl<Ahb1ClockEnableReg, RCC_AHB1ENR_TSCEN       , Ahb1ResetReg, RCC_AHB1RSTR_TSCRST, AhbClock> TscClock;


		typedef ClockResetControl<Ahb2ClockEnableReg, RCC_AHB2ENR_GPIOAEN, Ahb2ResetReg, RCC_AHB2RSTR_GPIOARST, AhbClock> GpioaClock;
		typedef ClockResetControl<Ahb2ClockEnableReg, RCC_AHB2ENR_GPIOBEN, Ahb2ResetReg, RCC_AHB2RSTR_GPIOBRST, AhbClock> GpiobClock;
		typedef ClockResetControl<Ahb2ClockEnableReg, RCC_AHB2ENR_GPIOCEN, Ahb2ResetReg, RCC_AHB2RSTR_GPIOCRST, AhbClock> GpiocClock;
		typedef ClockResetControl<Ahb2ClockEnableReg, RCC_AHB2ENR_GPIODEN, Ahb2ResetReg, RCC_AHB2RSTR_GPIODRST, AhbClock> GpiodClock;
		typedef ClockResetControl<Ahb2ClockEnableReg, RCC_AHB2ENR_GPIOEEN, Ahb2ResetReg, RCC_AHB2RSTR_GPIOERST, AhbClock> GpioeClock;
		typedef ClockResetControl<Ahb2ClockEnableReg, RCC_AHB2ENR_GPIOFEN, Ahb2ResetReg, RCC_AHB2RSTR_GPIOFRST, AhbClock> GpiofClock;
		typedef ClockResetControl<Ahb2ClockEnableReg, RCC_AHB2ENR_GPIOGEN, Ahb2ResetReg, RCC_AHB2RSTR_GPIOGRST, AhbClock> GpiogClock;
		typedef ClockResetControl<Ahb2ClockEnableReg, RCC_AHB2ENR_GPIOHEN, Ahb2ResetReg, RCC_AHB2RSTR_GPIOHRST, AhbClock> GpiohClock;
		typedef ClockResetControl<Ahb2ClockEnableReg, RCC_AHB2ENR_RNGEN  , Ahb2ResetReg, RCC_AHB2RSTR_RNGRST, AhbClock> RngClock;
		typedef ClockResetControl<Ahb2ClockEnableReg, RCC_AHB2ENR_ADCEN  , Ahb2ResetReg, RCC_AHB2RSTR_ADCRST, AhbClock> AdcClock;

		typedef ClockResetControl<Ahb3ClockEnableReg, RCC_AHB3ENR_QSPIEN , Ahb3ResetReg, RCC_AHB3RSTR_QSPIRST, AhbClock> QspiClock;
		typedef ClockResetControl<Ahb3ClockEnableReg, RCC_AHB3ENR_FMCEN  , Ahb3ResetReg, RCC_AHB3RSTR_FMCRST, AhbClock>  FmcClock;


		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_LPTIM1EN, Apb1ResetReg1, RCC_APB1RSTR1_LPTIM1RST, Apb1Clock>  LpTim1Clock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_OPAMPEN , Apb1ResetReg1, RCC_APB1RSTR1_OPAMPRST , Apb1Clock>  OpampClock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_DAC1EN  , Apb1ResetReg1, RCC_APB1RSTR1_DAC1RST  , Apb1Clock>  Dac1Clock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_PWREN   , Apb1ResetReg1, RCC_APB1RSTR1_PWRRST   , Apb1Clock>  PwrClock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_CAN1EN  , Apb1ResetReg1, RCC_APB1RSTR1_CAN1RST  , Apb1Clock>  Can1Clock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_I2C3EN  , Apb1ResetReg1, RCC_APB1RSTR1_I2C3RST  , Apb1Clock>  I2c3ClockBase;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_I2C2EN  , Apb1ResetReg1, RCC_APB1RSTR1_I2C2RST  , Apb1Clock>  I2c2ClockBase;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_I2C1EN  , Apb1ResetReg1, RCC_APB1RSTR1_I2C1RST  , Apb1Clock>  I2c1ClockBase;
		//typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_USART5EN, Apb1ResetReg1, RCC_APB1RSTR1_USART5RST, Apb1Clock>  Usart5Clock;
		//typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_USART4EN, Apb1ResetReg1, RCC_APB1RSTR1_USART4RST, Apb1Clock>  Usart4Clock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_USART3EN, Apb1ResetReg1, RCC_APB1RSTR1_USART3RST, Apb1Clock>  Usart3Clock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_USART2EN, Apb1ResetReg1, RCC_APB1RSTR1_USART2RST, Apb1Clock>  Usart2Clock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_SPI3EN  , Apb1ResetReg1, RCC_APB1RSTR1_SPI3RST  , Apb1Clock>  Spi3Clock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_SPI2EN  , Apb1ResetReg1, RCC_APB1RSTR1_SPI2RST  , Apb1Clock>  Spi2Clock;
		//typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_WWDGEN  , Apb1ResetReg1, RCC_APB1RSTR1_WWDGERST , Apb1Clock>  WwdgClock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_TIM7EN  , Apb1ResetReg1, RCC_APB1RSTR1_TIM7RST  , Apb1Clock>  Tim7Clock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_TIM6EN  , Apb1ResetReg1, RCC_APB1RSTR1_TIM6RST  , Apb1Clock>  Tim6Clock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_TIM5EN  , Apb1ResetReg1, RCC_APB1RSTR1_TIM5RST  , Apb1Clock>  Tim5Clock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_TIM4EN  , Apb1ResetReg1, RCC_APB1RSTR1_TIM4RST  , Apb1Clock>  Tim4Clock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_TIM3EN  , Apb1ResetReg1, RCC_APB1RSTR1_TIM3RST  , Apb1Clock>  Tim3Clock;
		typedef ClockResetControl<Apb1ClockReg1, RCC_APB1ENR1_TIM2EN  , Apb1ResetReg1, RCC_APB1RSTR1_TIM2RST  , Apb1Clock>  Tim2Clock;


		typedef ClockResetControl<Apb1ClockReg2, RCC_APB1ENR2_LPTIM2EN  , Apb1ResetReg2, RCC_APB1RSTR2_LPTIM2RST  , Apb1Clock>  LpTim2Clock;
		typedef ClockResetControl<Apb1ClockReg2, RCC_APB1ENR2_SWPMI1EN  , Apb1ResetReg2, RCC_APB1RSTR2_SWPMI1RST  , Apb1Clock>  Swpmi1Clock;
		typedef ClockResetControl<Apb1ClockReg2, RCC_APB1ENR2_LPUART1EN  , Apb1ResetReg2, RCC_APB1RSTR2_LPUART1RST  , Apb1Clock>  LpUart1ClockBase;

		typedef ClockResetControl<Apb2ClockReg, RCC_APB2ENR_DFSDM1EN, Apb2ResetReg, RCC_APB2RSTR_DFSDM1RST, Apb1Clock>  Dfsdm1Clock;
		typedef ClockResetControl<Apb2ClockReg, RCC_APB2ENR_SAI2EN  , Apb2ResetReg, RCC_APB2RSTR_SAI2RST  , Apb1Clock>  Sai2Clock;
		typedef ClockResetControl<Apb2ClockReg, RCC_APB2ENR_SAI1EN  , Apb2ResetReg, RCC_APB2RSTR_SAI1RST  , Apb1Clock>  Sai1Clock;
		typedef ClockResetControl<Apb2ClockReg, RCC_APB2ENR_TIM17EN  , Apb2ResetReg, RCC_APB2RSTR_TIM17RST, Apb1Clock>  Tim17Clock;
		typedef ClockResetControl<Apb2ClockReg, RCC_APB2ENR_TIM16EN  , Apb2ResetReg, RCC_APB2RSTR_TIM16RST, Apb1Clock>  Tim16Clock;
		typedef ClockResetControl<Apb2ClockReg, RCC_APB2ENR_TIM15EN  , Apb2ResetReg, RCC_APB2RSTR_TIM15RST, Apb1Clock>  Tim15Clock;
		typedef ClockResetControl<Apb2ClockReg, RCC_APB2ENR_USART1EN  , Apb2ResetReg, RCC_APB2RSTR_USART1RST, Apb1Clock> Usart1Clock;
		typedef ClockResetControl<Apb2ClockReg, RCC_APB2ENR_TIM8EN  , Apb2ResetReg, RCC_APB2RSTR_TIM8RST, Apb1Clock>  Tim8Clock;
		typedef ClockResetControl<Apb2ClockReg, RCC_APB2ENR_SPI1EN  , Apb2ResetReg, RCC_APB2RSTR_SPI1RST, Apb1Clock>  Spi1Clock;
		typedef ClockResetControl<Apb2ClockReg, RCC_APB2ENR_TIM1EN  , Apb2ResetReg, RCC_APB2RSTR_TIM1RST, Apb1Clock>  Tim1Clock;
		typedef ClockResetControl<Apb2ClockReg, RCC_APB2ENR_SDMMC1EN  , Apb2ResetReg, RCC_APB2RSTR_SDMMC1RST, Apb1Clock>  Sdmmc1Clock;
		typedef ClockResetControl<Apb2ClockReg, RCC_APB2ENR_SYSCFGEN  , Apb2ResetReg, RCC_APB2RSTR_SYSCFGRST, Apb1Clock>  SyscfgClock;

		struct LpUart1Clock: public LpUart1ClockBase
		{
			static inline void SelectClockSource(LpUsartClockSrc clockSource)
			{
				LpUsart1Sel::Set(clockSource);
			}

			static inline uint32_t ClockFreq()
			{
				switch(LpUsart1Sel::Get())
				{
					case LpUsartClockSrc::Pclk:
						return Apb1Clock::ClockFreq();
					case LpUsartClockSrc::SysClock:
						return SysClock::ClockFreq();
					case LpUsartClockSrc::Hsi:
						return HsiClock::ClockFreq();
					case LpUsartClockSrc::Lse:
						// TODO: implement
						//return LseClock::ClockFreq();
						return 0;
				}
				return 0;
			}
		};
		
		template<class Base, class ClockSel>
		struct I2c1ClockTemplate: public Base
		{
			static inline void SelectClockSource(I2cClockSrc clockSource)
			{
				ClockSel::Set(clockSource);
			}

			static inline uint32_t ClockFreq()
			{
				switch(ClockSel::Get())
				{
					case I2cClockSrc::Pclk:
						return Apb1Clock::ClockFreq();
					case I2cClockSrc::SysClock:
						return SysClock::ClockFreq();
					case I2cClockSrc::Hsi:
						return HsiClock::ClockFreq();
				}
				return 0;
			}
		};
		
		typedef I2c1ClockTemplate<I2c1ClockBase, I2c1Sel> I2c1Clock;
		typedef I2c1ClockTemplate<I2c2ClockBase, I2c2Sel> I2c2Clock;
		typedef I2c1ClockTemplate<I2c3ClockBase, I2c3Sel> I2c3Clock;

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
			for(uint32_t pllm = PllmMin; pllm <= PllmMax; pllm++)
			{
				uint32_t pllnFreq = inputClock / pllm;
				if(pllnFreq < PllnMinFreq)
					break;
				if(pllnFreq > PllnMaxFreq)
					continue;
				uint32_t plln = (vco + pllnFreq/2) / pllnFreq;
				if(plln < PllnMin)
					continue;
				if(plln > PllnMax)
					break;
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
			switch(PllSrc::Get())
			{
				default:
				case PllClockSource::None: return 0;
				case PllClockSource::Msi: return MsiClock::ClockFreq();
				case PllClockSource::Internal: return HsiClock::ClockFreq();
				case PllClockSource::External: return HseClock::ClockFreq();
			}
		}

		void PllClock::SelectClockSource(PllClockSource clockSource)
		{
			PllSrc::Set(clockSource);
		}

		uint32_t PllClock::SetClockFreq(uint32_t freq)
		{
			if(freq > PllMaxFreq)
				freq = PllMaxFreq;

			uint32_t	resPllr = 0,
						resPllm = 0,
						resPlln = 0,
						minErr  = freq,
						bestFreq = 0;

			uint32_t vcoInc = SrcClockFreq() / PllmMax;

			for(uint32_t vco = Util::max(VcoMinFreq, freq*PllrMin); vco <= VcoMaxFreq; vco += vcoInc)
			{
				uint32_t plln, pllm;
				vco = CalcVco(vco, pllm, plln);
				if(vco == 0)
					continue;
				uint32_t pllr = (vco + freq/2) / freq;
				if((pllr & 1) != 0)
					continue;
				if(pllr < 2)
					continue;
				if(pllr > 8)
					break;
				uint32_t realFreq = vco / pllr;
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
					resPllr = pllr;
					resPlln = plln;
					resPllm = pllm;
					bestFreq = realFreq;
				}
				if(err == 0) break;
			}
			if(resPllr == 0 || resPlln == 0 || resPllm == 0 )
				return 0;
			PllN::Set(resPlln);
			PllM::Set(resPllm-1);
			PllR::Set((resPllr - 2) / 2);

			return bestFreq;
		}

		uint32_t PllClock::ClockFreq()
		{
			uint32_t plln = PllN::Get();
			uint32_t pllm = PllM::Get() + 1;
			uint32_t pllr = (PllR::Get() + 1 ) * 2;
			uint32_t div = pllm * pllr;
			if(div == 0)
				return 0;
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
            RCC->PLLCFGR |= RCC_PLLCFGR_PLLREN;

			return ClockBase::EnableClockSource(RCC_CR_PLLON, RCC_CR_PLLRDY);
		}

		void PllClock::Disable()
		{
			ClockBase::DisablelockSource(RCC_CR_PLLON, RCC_CR_PLLRDY);
		}

		ClockErrorCode SysClock::SelectClockSource(SysClockSource clockSource)
		{
			uint32_t currentFreq = ClockFreq();
			uint32_t targetFreq;

			if(clockSource == SysClockSource::Msi)
			{
				if (!MsiClock::Enable())
				{
					return ClockErrorCode::ClockSourceFailed;
				}
				targetFreq = MsiClock::ClockFreq();
			}else if(clockSource == SysClockSource::Internal)
			{
				if (!HsiClock::Enable())
				{
					return ClockErrorCode::ClockSourceFailed;
				}
				targetFreq = HsiClock::ClockFreq();
			}else if(clockSource == SysClockSource::External)
			{
				if (!HseClock::Enable())
				{
					return ClockErrorCode::ClockSourceFailed;
				}
				targetFreq = HseClock::ClockFreq();
			}else if(clockSource == SysClockSource::Pll)
			{
				if (!PllClock::Enable())
				{
					return ClockErrorCode::ClockSourceFailed;
				}
				targetFreq = PllClock::ClockFreq();
			}else
				return ClockErrorCode::InvalidClockSource;

			PwrClock::Enable();
			PWR->CR1 |= PWR_CR1_VOS;
			AhbClock::SetPrescaller(AhbPrescaller::Div1);
			Apb1Clock::SetPrescaller(ApbPrescaller::Div2);
			Apb2Clock::SetPrescaller(ApbPrescaller::Div2);

			if(currentFreq < targetFreq)
			{
				Flash::ConfigureFreq(targetFreq);
			}

			SysClockSwitch::Set(clockSource);

			uint32_t timeout = 100000;
			while (SysClockStatus::Get() != clockSource && --timeout)
			{
			}
			if(timeout == 0)
			{
				return ClockErrorCode::ClockSelectFailed;
			}
			if(currentFreq > targetFreq)
			{
				Flash::ConfigureFreq(targetFreq);
			}
			return ClockErrorCode::Success;
		}

		uint32_t SysClock::SetClockFreq(uint32_t freq)
		{
			SelectClockSource(SysClockSource::Internal);
			PllClock::Disable();
			PllClock::SelectClockSource(PllClockSource::External);
			if(PllClock::SetClockFreq(freq) > 0)
			{
				SelectClockSource(SysClockSource::Pll);
			}
			else
			{
				MsiClock::Disable();
				MsiClock::SetClockFreq(freq);
				SelectClockSource(SysClockSource::Msi);
			}

			return ClockFreq();
		}

		uint32_t SysClock::ClockFreq()
		{
			switch (SysClockStatus::Get())
			{
				case SysClockSource::Msi:      return MsiClock::ClockFreq();
				case SysClockSource::Internal: return HsiClock::ClockFreq();
				case SysClockSource::External: return HseClock::ClockFreq();
				case SysClockSource::Pll:      return PllClock::ClockFreq();
			}
			return 0;
		}

		uint32_t SysClock::SrcClockFreq()
		{
			return ClockFreq();
		}
	}
}
