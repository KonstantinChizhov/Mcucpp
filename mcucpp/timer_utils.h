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

#ifdef F_CPU
#define DEFAULT_TIMER_CLOCK_FREQ =F_CPU
#else
#define DEFAULT_TIMER_CLOCK_FREQ
#endif

namespace Mcucpp
{
	namespace Timers
	{
		namespace Private
		{
			template<class Timer, class Divider, unsigned long Freq, unsigned long Fcpu>
			struct ReloadValue
			{
				static const unsigned long ticks_ = Fcpu / Divider::Div / Freq;
				static const unsigned long ticks = ticks_ == 0 ? 1 : ticks_;
				static const unsigned long value = Timer::MaxValue - ticks;
				static const unsigned long realFreq = Fcpu / Divider::Div / ticks;
				static const unsigned long error = realFreq > Freq ? realFreq - Freq : Freq - realFreq;
			};

			template<class Timer, unsigned long Freq, unsigned long Fcpu, int MinDividerNumber, int MaxDividerNumber>
			class DividerSelector
			{
				static const unsigned long CurrentDividerN = (MinDividerNumber + MaxDividerNumber) / 2;
				typedef typename Timer::template Divider<CurrentDividerN> CurrentDivider;
				typedef ReloadValue<Timer, CurrentDivider, Freq, Fcpu> CurrentValue;
				
				static const unsigned long NewMinDividerNumber = 
							CurrentValue::ticks > Timer::MaxValue ? 
							CurrentDividerN + 1:
							MinDividerNumber;
							
				static const unsigned long NewMaxDividerNumber = 
							CurrentValue::ticks > Timer::MaxValue ?
							MaxDividerNumber:
							CurrentDividerN;
			public:
				typedef typename DividerSelector<
					Timer, Freq, Fcpu,
					NewMinDividerNumber, NewMaxDividerNumber>::Result Result;
			};

			template<class Timer, unsigned long Freq, unsigned long Fcpu, int BestDivider>
			struct DividerSelector<Timer, Freq, Fcpu, BestDivider, BestDivider>
			{
				typedef typename Timer::template Divider<BestDivider> Result;
			};
		}

		template<class Timer, unsigned long Freq, unsigned long Fcpu DEFAULT_TIMER_CLOCK_FREQ>
		class TimerFreqSetup
		{
			typedef Private::DividerSelector<Timer, Freq, Fcpu, 0, Timer::MaxDivider> DivSelector;
			typedef Private::ReloadValue<Timer, typename DivSelector::Result, Freq, Fcpu> ReloadValueHolder;
		public:
			static const typename Timer::ClockDivider divider = DivSelector::Result::value;
			static const unsigned dividerValue = DivSelector::Result::Div;
			static const typename Timer::DataT reloadValue = ReloadValueHolder::value;
			static const typename Timer::DataT ticks = ReloadValueHolder::ticks;
			
			static const unsigned long realFreq = ReloadValueHolder::realFreq;
			static const unsigned long tickFreq = Fcpu / dividerValue;
			static const unsigned long error = ReloadValueHolder::error;
		};
	}
}