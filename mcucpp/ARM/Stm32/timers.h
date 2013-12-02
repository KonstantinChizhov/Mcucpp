#pragma once

#include "ioreg.h"
#include "stm32f10x.h"
#include "clock.h"

namespace Mcucpp
{
	namespace Timers
	{
		namespace Private
		{
			template<class Regs, class ClockEnReg>
			class BaseTimer
			{
			public:
				typedef uint16_t DataT;
				static const uint16_t MaxValue = 0xffff;
				
				typedef uint16_t ClockDivider;
				static const ClockDivider Div1     = 1;
				static const ClockDivider Div2     = 2;
				static const ClockDivider Div4     = 4;
				static const ClockDivider Div8     = 8;
				static const ClockDivider Div16    = 16;
				static const ClockDivider Div32    = 32;
				static const ClockDivider Div64    = 64;
				static const ClockDivider Div128   = 128;
				static const ClockDivider Div256   = 256;
				static const ClockDivider Div1024  = 1024;
				static const ClockDivider Div2048  = 2048;
				static const ClockDivider Div4096  = 4096;
				static const ClockDivider Div8192  = 8192;
				static const ClockDivider Div16384 = 16384;
				static const ClockDivider Div32768 = 32768;
				
				enum InterruptFlags
				{
					OverflowInt = TIM_SR_UIF,
					UpdateInt = TIM_SR_UIF,
				};
				
				static void Enable()                 { ClockEnReg::Enable(); }
				static void Disable()                { ClockEnReg::Disable(); }
				static void Set(DataT val)           { Regs()->CNT = val; }
				static void Clear()                  { Regs()->CNT = 0; }
				static void SetDivider(ClockDivider divider) { Regs()->PSC = divider - 1; }
				static DataT Get()                   { return Regs()->CNT; }
				
				static void Stop()
				{
					Regs()->CR1 = 0;
					Regs()->CR2 = 0;
				}

				static void Start(ClockDivider divider = Div1)
				{
					Enable();
					Regs()->CR2 = 0;
					Regs()->DIER = 0;
					Regs()->PSC = divider - 1;
					Regs()->ARR = MaxValue;
					Regs()->CR1 = (TIM_CR1_CEN);
				}

				static void EnableInterrupt(InterruptFlags interrupt = UpdateInt)
				{
					if(interrupt & UpdateInt)
						Regs()->DIER |= TIM_DIER_UIE;
				}
				
				static void DisableInterrupt(InterruptFlags interrupt = UpdateInt)
				{
					if(interrupt & UpdateInt)
						Regs()->DIER &= ~TIM_DIER_UIE;
				}

				static InterruptFlags IsInterrupt()
				{
					return (InterruptFlags)(Regs()->SR & TIM_SR_UIF);
				}
				
				static void ClearInterruptFlag(InterruptFlags interrupt = UpdateInt)
				{
					Regs()->SR &= ~(uint32_t)interrupt;
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
			IO_STRUCT_WRAPPER(TIM6, Tim6Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM7, Tim7Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM8, Tim8Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM9, Tim9Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM10, Tim10Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM11, Tim11Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM12, Tim12Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM13, Tim13Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM14, Tim14Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM15, Tim15Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM16, Tim16Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM17, Tim17Regs, TIM_TypeDef);
		}
		
		class Timer7 :public Private::BaseTimer<Private::Tim6Regs, Clock::Tim6Clock> {};
		class Timer6 :public Private::BaseTimer<Private::Tim6Regs, Clock::Tim6Clock> {};
	}
}
#undef DECLARE_STM32_BASIC_TIMER