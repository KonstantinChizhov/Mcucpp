#pragma once

#include "ioreg.h"
#include "stm32f10x.h"

#define DECLARE_STM32_BASIC_TIMER(NUMBER, CLASS_NAME, ClockEnableReg, ClockEnableMask)\
namespace Private{\
	IO_REG_WRAPPER(TIM ## NUMBER ->CR1,	Tim ## NUMBER ## Cr1, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->CR2,	Tim ## NUMBER ## Cr2, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->SMCR,	Tim ## NUMBER ## Smcr, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->DIER,	Tim ## NUMBER ## Dier, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->SR,	Tim ## NUMBER ## Sr, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->EGR,	Tim ## NUMBER ## Egr, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->CCMR1,	Tim ## NUMBER ## Ccmr1, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->CCMR2,	Tim ## NUMBER ## Ccmr2, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->CCER,	Tim ## NUMBER ## Ccer, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->CNT,	Tim ## NUMBER ## Cnt, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->PSC,	Tim ## NUMBER ## Psc, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->ARR,	Tim ## NUMBER ## Arr, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->RCR,	Tim ## NUMBER ## Rcr, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->CCR1,	Tim ## NUMBER ## Ccr1, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->CCR2,	Tim ## NUMBER ## Ccr2, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->CCR3,	Tim ## NUMBER ## Ccr3, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->CCR4,	Tim ## NUMBER ## Ccr4, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->BDTR,	Tim ## NUMBER ## Bdtr, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->DCR,	Tim ## NUMBER ## Dcr, uint16_t);\
	IO_REG_WRAPPER(TIM ## NUMBER ->DMAR,	Tim ## NUMBER ## Dmar, uint16_t);\
}\
	typedef TimerImp<\
		Private::Tim ## NUMBER ## Cr1,\
		Private::Tim ## NUMBER ## Cr2,\
		Private::Tim ## NUMBER ## Smcr,\
		Private::Tim ## NUMBER ## Dier,\
		Private::Tim ## NUMBER ## Sr,\
		Private::Tim ## NUMBER ## Egr,\
		Private::Tim ## NUMBER ## Ccmr1,\
		Private::Tim ## NUMBER ## Ccmr2,\
		Private::Tim ## NUMBER ## Ccer,\
		Private::Tim ## NUMBER ## Cnt,\
		Private::Tim ## NUMBER ## Psc,\
		Private::Tim ## NUMBER ## Arr,\
		Private::Tim ## NUMBER ## Rcr,\
		Private::Tim ## NUMBER ## Ccr1,\
		Private::Tim ## NUMBER ## Ccr2,\
		Private::Tim ## NUMBER ## Ccr3,\
		Private::Tim ## NUMBER ## Ccr4,\
		Private::Tim ## NUMBER ## Bdtr,\
		Private::Tim ## NUMBER ## Dcr,\
		Private::Tim ## NUMBER ## Dmar,\
		ClockEnableReg,\
		ClockEnableMask\
	> CLASS_NAME;

namespace Timers
{
	namespace Private
	{
		IO_REG_WRAPPER(RCC->APB2ENR,	ClockEnableReg2, uint32_t);
		IO_REG_WRAPPER(RCC->APB1ENR,	ClockEnableReg1, uint32_t);
	}
	
template<
	class Cr1, 
	class Cr2, 
	class Smcr, 
	class Dier, 
	class Sr, 
	class Egr, 
	class Ccmr1, 
	class Ccmr2, 
	class Ccer, 
	class Cnt, 
	class Psc, 
	class Arr, 
	class Rcr, 
	class Ccr1, 
	class Ccr2, 
	class Ccr3, 
	class Ccr4, 
	class Bdtr, 
	class Dcr, 
	class Dmar,
	class ClockEnReg,
	unsigned ClockEnMask>
	class TimerImp
	{
		public:
		typedef unsigned short DataT;
		static const unsigned short MaxValue = 0xffff;
		
		enum ClockDivider
		{
			Div1 = 0, 
			Div2 = TIM_CR1_CKD_0, 
			Div4 = TIM_CR1_CKD_1, 
			DivReserved = TIM_CR1_CKD
		};
		
		enum TimerMode
		{
			UpMode		= 0,
			NormalMode	= 0,
			DownMode	= TIM_CR1_DIR,
			CenterAligned1	= TIM_CR1_CMS_0,
			CenterAligned2	= TIM_CR1_CMS_1,
			CenterAligned3	= TIM_CR1_CMS
		};
		
		enum TimerDir
		{
			Up = 0,
			Down = 1
		};
		
		enum ClockSrc
		{
			MainClock = 0
		};
		
		static void Set(DataT val)
		{
			Cnt::Set(val);
		}

		static DataT Get()
		{
			return Cnt::Get();
		}
		
		static void SetPeriod(DataT val)
		{
			Arr::Set(val);
			Egr::Or(TIM_EGR_UG);
		}

		static void Stop()
		{
		
		}

		static void Clear()
		{
			
		}
		
		static TimerDir Direction()
		{
		  return Cr1::Get() & TIM_CR1_DIR ? Down : Up;
		}

		static void Start(ClockDivider divider, TimerMode mode = NormalMode, ClockSrc clockSrc = MainClock)
		{
			ClockEnReg::Or(ClockEnMask);
			Cr2::Set(0);
			Psc::Set(0);
			SetPeriod(MaxValue);
			Cr1::Set(TIM_CR1_CEN | divider | mode);
		}

		static void EnableInterrupt()
		{
			Dier::Or(TIM_DIER_UIE);
		}

		static bool IsInterrupt()
		{
			return 0;
		}
		
		static void ClearInterruptFlag()
		{
			
		}
		
		static void SetMode(TimerMode mode)
		{
			
		}

		template<int number> class OutputCompare;
		template<int number> class InputCapture;
	};
	
#ifdef USE_TIM1
	DECLARE_STM32_BASIC_TIMER(1, Timer1, Private::ClockEnableReg2, RCC_APB2ENR_TIM1EN)
#endif

#ifdef USE_TIM2
	DECLARE_STM32_BASIC_TIMER(2, Timer2, Private::ClockEnableReg2, 1 << 0)
#endif

#ifdef USE_TIM3
	DECLARE_STM32_BASIC_TIMER(3, Timer3, Private::ClockEnableReg2, 1 << 1)
#endif

#ifdef USE_TIM4
	DECLARE_STM32_BASIC_TIMER(4, Timer4, Private::ClockEnableReg2, 1 << 2)
#endif

// #ifdef USE_TIM5
	// DECLARE_STM32_BASIC_TIMER(5, Timer5)
// #endif

// #ifdef USE_TIM6
	// DECLARE_STM32_BASIC_TIMER(6, Timer6)
// #endif

// #ifdef USE_TIM7
	// DECLARE_STM32_BASIC_TIMER(7, Timer7)
// #endif

// #ifdef USE_TIM15
	// DECLARE_STM32_BASIC_TIMER(15, Timer15)
// #endif

// #ifdef USE_TIM16
	// DECLARE_STM32_BASIC_TIMER(16, Timer16)
// #endif

// #ifdef USE_TIM17
	// DECLARE_STM32_BASIC_TIMER(17, Timer17)
// #endif

}
#undef DECLARE_STM32_BASIC_TIMER