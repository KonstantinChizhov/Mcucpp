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
#include <clock.h>

namespace Mcucpp
{
	namespace Timers
	{
		template<int number>
		class BaseTimer
		{
		public:
			typedef uint16_t DataT;
			static const uint16_t MaxValue = 0xffff;
					
			typedef uint16_t ClockDivider;
			static const ClockDivider DivStop     = 0;
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
		
			enum InterruptFlags
			{
				OverflowInt = 0,
				UpdateInt = 0
			};
		
			template<unsigned Number> struct Divider
			{ 
				static const ClockDivider value = (ClockDivider)(Number+1);
				enum {Div = Number+1 }; 
			};
			
			enum {MaxDivider = 0xffff};
			
			static ClockDivider DividerValue(unsigned num)
			{
				return (ClockDivider)(num+1);
			}
			
			static uint32_t DividerCoeff(unsigned num)
			{
				return num + 1;
			}
			
			static uint32_t ClockFreq()      { return Clock::SysClock::ClockFreq(); }
			
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
		
			static void Start(ClockDivider divider, DataT reloadValue = 0)
			{
				timerData.CurrentDivider = divider;
				timerData.Counter = reloadValue;
			}
		
			static void EnableInterrupt(InterruptFlags interrupt = UpdateInt)
			{
				
			}
			
			static void DisableInterrupt(InterruptFlags interrupt = UpdateInt)
			{
				
			}
		
			static InterruptFlags IsInterrupt()
			{
				return (InterruptFlags)0;
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
