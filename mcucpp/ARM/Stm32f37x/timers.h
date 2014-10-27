#pragma once

#include <ioreg.h>
#include <stm32f37x.h>
#include <clock.h>

namespace Mcucpp
{
	namespace Timers
	{
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
			static const ClockDivider Div512   = 512;
			static const ClockDivider Div1024  = 1024;
			static const ClockDivider Div2048  = 2048;
			static const ClockDivider Div4096  = 4096;
			static const ClockDivider Div8192  = 8192;
			static const ClockDivider Div16384 = 16384;
			static const ClockDivider Div32768 = 32768;
			
			template<unsigned Number> struct Divider;
			
			enum InterruptFlags
			{
				OverflowInt = TIM_SR_UIF,
				UpdateInt = TIM_SR_UIF,
			};
		};
		
			template<> struct BaseTimer::Divider <0>  { static const ClockDivider value = Div1;     enum {Div = 1}; };
			template<> struct BaseTimer::Divider <1>  { static const ClockDivider value = Div2;     enum {Div = 2}; };
			template<> struct BaseTimer::Divider <2>  { static const ClockDivider value = Div4;     enum {Div = 4}; };
			template<> struct BaseTimer::Divider <3>  { static const ClockDivider value = Div8;     enum {Div = 8}; };
			template<> struct BaseTimer::Divider <4>  { static const ClockDivider value = Div16;    enum {Div = 16}; };
			template<> struct BaseTimer::Divider <5>  { static const ClockDivider value = Div32;    enum {Div = 32}; };
			template<> struct BaseTimer::Divider <6>  { static const ClockDivider value = Div64;    enum {Div = 64}; };
			template<> struct BaseTimer::Divider <7>  { static const ClockDivider value = Div128;   enum {Div = 128}; };
			template<> struct BaseTimer::Divider <8>  { static const ClockDivider value = Div256;   enum {Div = 256}; };
			template<> struct BaseTimer::Divider <9>  { static const ClockDivider value = Div512;   enum {Div = 512}; };
			template<> struct BaseTimer::Divider <10> { static const ClockDivider value = Div1024;  enum {Div = 1024}; };
			template<> struct BaseTimer::Divider <11> { static const ClockDivider value = Div2048;  enum {Div = 2048}; };
			template<> struct BaseTimer::Divider <12> { static const ClockDivider value = Div4096;  enum {Div = 40961}; };
			template<> struct BaseTimer::Divider <13> { static const ClockDivider value = Div8192;  enum {Div = 8192}; };
			template<> struct BaseTimer::Divider <14> { static const ClockDivider value = Div16384; enum {Div = 16384}; };
			template<> struct BaseTimer::Divider <15> { static const ClockDivider value = Div32768; enum {Div = 32768}; };
		
		namespace Private
		{
			
			template<class Regs, class ClockEnReg, IRQn_Type IQRNumber>
			class BasicTimer : public BaseTimer
			{
			public:
				
				using BaseTimer::ClockDivider;
				using BaseTimer::DataT;
				
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

				static void Start(ClockDivider divider = Div1, DataT reloadValue = BaseTimer::MaxValue)
				{
					Enable();
					Regs()->CR2 = 0;
					Regs()->DIER = 0;
					Regs()->PSC = divider - 1;
					Regs()->ARR = reloadValue;
					Regs()->CR1 = (TIM_CR1_CEN | TIM_CR1_URS);
				}

				static void EnableInterrupt(InterruptFlags interrupt = UpdateInt)
				{
					if(interrupt & UpdateInt)
						Regs()->DIER |= TIM_DIER_UIE;
					if(interrupt)
						NVIC_EnableIRQ(IQRNumber);
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
			
			
			template<class Regs>
			class RepetitionControl
			{
			public:
				static void RepetitionCount(uint8_t count)
				{
					Regs()->RCR = count;
				}
			};
			
			template<class Regs>
			class OutputCompare1
			{
			public:
				typedef uint16_t DataT;
				static void Set(DataT val)
				{
					
				}

				static DataT Get()
				{
					return 0;
				}

				static void EnableInterrupt()
				{
					
				}

				static bool IsInterrupt()
				{
					return false;
				}
			
				static void ClearInterruptFlag()
				{
					
				}
				
				static void OutputEnable()
				{
				
				}
				
				
			};
		
			IO_STRUCT_WRAPPER(TIM2,  Tim2Regs,  TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM3,  Tim3Regs,  TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM4,  Tim4Regs,  TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM5,  Tim5Regs,  TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM6,  Tim6Regs,  TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM7,  Tim7Regs,  TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM12, Tim12Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM13, Tim13Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM14, Tim14Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM15, Tim15Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM16, Tim16Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM17, Tim17Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM18, Tim18Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM19, Tim19Regs, TIM_TypeDef);
		}
		
		class Timer2 :public Private::BasicTimer<Private::Tim2Regs, Clock::Tim2Clock, TIM2_IRQn>
		{
		};
		class Timer3 :public Private::BasicTimer<Private::Tim3Regs, Clock::Tim3Clock, TIM3_IRQn>
		{
		};
		class Timer4 :public Private::BasicTimer<Private::Tim4Regs, Clock::Tim4Clock, TIM4_IRQn>
		{
		};
		
		class Timer16 :public Private::BasicTimer<Private::Tim16Regs, Clock::Tim16Clock, TIM16_IRQn>
		{
		};
		class Timer17 :public Private::BasicTimer<Private::Tim17Regs, Clock::Tim17Clock, TIM17_IRQn>
		{
		};
		
		class Timer13 :public Private::BasicTimer<Private::Tim13Regs, Clock::Tim13Clock, TIM13_IRQn>
		{
		};
	}
}
