//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
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

#include <stdint.h>
#include <template_utils.h>

namespace Mcucpp
{
	namespace Timers
	{
		template<int number>
		class BaseTimer
		{
		public:
			typedef uint16_t DataT;
			enum {MaxValue = 0xff};
			enum ClockDivider
			{
				DivStop=0,
				Div1,
				Div16,
				Div64,
				Div256,
				Div1024,
				ExtFalling,
				ExtRising
			};

			enum InterruptFlags
			{
				OverflowInt = 0,
				UpdateInt = 0
			};

			template<unsigned Number> struct Divider
			{ 
				static const ClockDivider value = (ClockDivider)Number;
				enum {Div = Util::Pow<4, Number>::value }; 
			};
			enum {MaxDivider = 5};
			
			static void Enable(){  }
			static void Disable(){  }

			static void Set(DataT val)
			{
				timerData.Counter = val;
			}

			static DataT Get()
			{
				return timerData.Counter;
			}

			static void Stop()
			{
				timerData.CurrentDivider = DivStop;
			}

			static void Clear()
			{
				timerData.Counter = 0;
			}

			static void Start(ClockDivider divider)
			{
				timerData.CurrentDivider = divider;
			}

			static void EnableInterrupt(InterruptFlags interrupt = UpdateInt)
			{
				
			}
			
			static void DisableInterrupt(InterruptFlags interrupt = UpdateInt)
			{
				
			}

			static InterruptFlags IsInterrupt()
			{
				return 0;
			}
			
			static void ClearInterruptFlag(InterruptFlags interrupt = UpdateInt)
			{
				
			}
			
			static void SetAutoReload(DataT value)
			{
				
			}

			enum TimerMode
			{
				Normal = 0,
				PwmPhaseCorrect8Bit,
				PwmPhaseCorrect9Bit,
				PwmPhaseCorrect10Bit,
				ClearOnMatchOcr1,
				PwmFast8Bit,
				PwmFast9Bit,
				PwmFast10Bit,
				ClearOnMatchOcr2 ,
				PwmFastToOcr1,
				PwmFastToOcr2
			};

			static void SetMode(TimerMode mode)
			{

			}

			struct TimerData
			{
				volatile DataT Counter;
				ClockDivider CurrentDivider;
			};
			static TimerData timerData;
		};

		
		template<int number>
		typename BaseTimer<number>::TimerData BaseTimer<number>::timerData;

		typedef BaseTimer<1> Timer1;
		typedef BaseTimer<2> Timer2;
		typedef BaseTimer<3> Timer3;
		typedef BaseTimer<4> Timer4;
		typedef BaseTimer<5> Timer5;
		typedef BaseTimer<6> Timer6;
		typedef BaseTimer<7> Timer7;
		typedef BaseTimer<8> Timer8;
		typedef BaseTimer<9> Timer9;
		typedef BaseTimer<10> Timer10;
		typedef BaseTimer<11> Timer11;
		typedef BaseTimer<12> Timer12;
		typedef BaseTimer<13> Timer13;
	} // namespace Timers
}
