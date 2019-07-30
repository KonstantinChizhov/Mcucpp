//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2016
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
#include "clock.h"

namespace Mcucpp
{
namespace Timers
{
namespace Private
{
template <class Regs, class ClockEnReg, IRQn_Type IQRNumber>
class BaseTimer
{
  public:
	typedef uint16_t DataT;
	static const uint16_t MaxValue = 0xffff;
	static const uint16_t MaxDivider = 0xffff;

	typedef uint32_t ClockDivider;
	static const ClockDivider Div1 = 0;
	static const ClockDivider Div2 = 1;
	static const ClockDivider Div4 = 3;
	static const ClockDivider Div8 = 7;
	static const ClockDivider Div16 = 15;
	static const ClockDivider Div32 = 31;
	static const ClockDivider Div64 = 63;
	static const ClockDivider Div128 = 127;
	static const ClockDivider Div256 = 255;
	static const ClockDivider Div1024 = 1023;
	static const ClockDivider Div2048 = 2047;
	static const ClockDivider Div4096 = 4095;
	static const ClockDivider Div8192 = 8191;
	static const ClockDivider Div16384 = 16383;
	static const ClockDivider Div32768 = 32767;

	static ClockDivider DividerValue(unsigned number)
	{
		return (ClockDivider)number;
	}

	static uint32_t DividerCoeff(unsigned number)
	{
		return number + 1;
	}

	enum InterruptFlags
	{
		OverflowInt = TIM_SR_UIF,
		UpdateInt = TIM_SR_UIF,
	};

	static clock_freq_t ClockFreq() { return ClockEnReg::ClockFreq(); }
	static void Enable() { ClockEnReg::Enable(); }
	static void Disable() { ClockEnReg::Disable(); }
	static void Set(DataT val) { Regs()->CNT = val; }
	static void Clear() { Regs()->CNT = 0; }
	static void SetDivider(ClockDivider divider) { Regs()->PSC = divider; }
	static DataT Get() { return Regs()->CNT; }

	static void Stop()
	{
		Regs()->CR1 = 0;
		Regs()->CR2 = 0;
	}

	static void Start(ClockDivider divider = Div1, DataT reloadValue = BaseTimer::MaxValue)
	{
		Enable();
		Regs()->CR1 = 0;
		Regs()->CR2 = 0;
		Regs()->DIER = 0;
		Regs()->PSC = divider;
		Regs()->ARR = reloadValue;
		Regs()->EGR |= TIM_EGR_UG;
		Regs()->CR1 = (TIM_CR1_CEN | TIM_CR1_URS);
	}

	static void EnableInterrupt(InterruptFlags interrupt = UpdateInt)
	{
		if (interrupt & UpdateInt)
			Regs()->DIER |= TIM_DIER_UIE;
		if (interrupt)
			NVIC_EnableIRQ(IQRNumber);
	}

	static void DisableInterrupt(InterruptFlags interrupt = UpdateInt)
	{
		if (interrupt & UpdateInt)
			Regs()->DIER &= ~TIM_DIER_UIE;
	}

	static InterruptFlags IsInterrupt()
	{
		return (InterruptFlags)(Regs()->SR & TIM_SR_UIF);
	}

	static void ClearInterruptFlag(InterruptFlags interrupt = UpdateInt)
	{
		(void)interrupt;
		Regs()->SR = 0;
		NVIC_ClearPendingIRQ(IQRNumber);
	}

	static void SetAutoReload(DataT value)
	{
		Regs()->ARR = value;
		Regs()->EGR |= TIM_EGR_UG;
	}
};

IO_STRUCT_WRAPPER(TIM1, Tim1Regs, TIM_TypeDef);
IO_STRUCT_WRAPPER(TIM2, Tim2Regs, TIM_TypeDef);
IO_STRUCT_WRAPPER(TIM3, Tim3Regs, TIM_TypeDef);
IO_STRUCT_WRAPPER(TIM4, Tim4Regs, TIM_TypeDef);
#if defined(TIM6)
IO_STRUCT_WRAPPER(TIM6, Tim6Regs, TIM_TypeDef);
#endif
#if defined(TIM7)
IO_STRUCT_WRAPPER(TIM7, Tim7Regs, TIM_TypeDef);
#endif
#if defined(TIM8)
IO_STRUCT_WRAPPER(TIM8, Tim8Regs, TIM_TypeDef);
#endif
#if defined(TIM9)
IO_STRUCT_WRAPPER(TIM9, Tim9Regs, TIM_TypeDef);
#endif
#if defined(TIM10)
IO_STRUCT_WRAPPER(TIM10, Tim10Regs, TIM_TypeDef);
#endif
#if defined(TIM11)
IO_STRUCT_WRAPPER(TIM11, Tim11Regs, TIM_TypeDef);
#endif
#if defined(TIM12)
IO_STRUCT_WRAPPER(TIM12, Tim12Regs, TIM_TypeDef);
#endif
#if defined(TIM13)
IO_STRUCT_WRAPPER(TIM13, Tim13Regs, TIM_TypeDef);
#endif
#if defined(TIM14)
IO_STRUCT_WRAPPER(TIM14, Tim14Regs, TIM_TypeDef);
#endif
#if defined(TIM15)
IO_STRUCT_WRAPPER(TIM15, Tim15Regs, TIM_TypeDef);
#endif
#if defined(TIM16)
IO_STRUCT_WRAPPER(TIM16, Tim16Regs, TIM_TypeDef);
#endif
#if defined(TIM17)
IO_STRUCT_WRAPPER(TIM17, Tim17Regs, TIM_TypeDef);
#endif
} // namespace Private

#if defined(TIM1)
class Timer1 : public Private::BaseTimer<Private::Tim1Regs, Clock::Tim1Clock, TIM1_UP_TIM10_IRQn>
{
};
#endif

#if defined(TIM2)
class Timer2 : public Private::BaseTimer<Private::Tim2Regs, Clock::Tim2Clock, TIM2_IRQn>
{
};
#endif

#if defined(TIM6)
class Timer6 : public Private::BaseTimer<Private::Tim6Regs, Clock::Tim6Clock, TIM6_DAC_IRQn>
{
};
#endif
#if defined(TIM7)
class Timer7 : public Private::BaseTimer<Private::Tim7Regs, Clock::Tim7Clock, TIM7_IRQn>
{
};
#endif

} // namespace Timers
} // namespace Mcucpp
#undef DECLARE_STM32_BASIC_TIMER