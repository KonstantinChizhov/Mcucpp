
#pragma once

#include "loki/TypeManip.h"
#include "static_assert.h"

#ifdef F_CPU
#define DEFAULT_TIMER_CLOCK_FREQ =F_CPU
#else
#define DEFAULT_TIMER_CLOCK_FREQ
#endif

namespace Timers
{
	namespace Private
	{
		template<class T>
		struct Wrap
		{
			typedef T Result;
		};

		template<class Timer, unsigned long Freq, unsigned long Fcpu, unsigned DividerNum = 0>
		struct ReloadValueIter
		{
			typedef typename Timer::template Divider<DividerNum> DividerType;

			static const unsigned Ticks = Fcpu/Freq/DividerType::Div;

			typedef typename Loki::Select<
				(Ticks >= Timer::MaxValue),
				ReloadValueIter<Timer, Freq, Fcpu, DividerNum+1>,
				Loki::Int2Type<Ticks>
				>::Result ValueSelector;
	
			enum{value = Timer::MaxValue - ValueSelector::value};
		};


		template<class Timer, unsigned long Freq, unsigned long Fcpu, unsigned DividerNum = 0>
		class DividerIter
		{
			typedef typename Timer::template Divider<DividerNum> DividerType;

			static const unsigned Ticks = Fcpu/Freq/DividerType::Div;
	
			typedef typename Loki::Select<
				(Ticks >= Timer::MaxValue),
				DividerIter<Timer, Freq, Fcpu, DividerNum+1>,
				Wrap<DividerType>
				>::Result SelectedDivider;
			public:
			typedef typename SelectedDivider::Result Result;
		};
	}

	enum {MinTimerClockCycles = 50};

	template<class Timer, unsigned long Freq, unsigned long Fcpu DEFAULT_TIMER_CLOCK_FREQ>
	struct TimerFreqSetup
	{
		static const typename Timer::DataT ReloadValue = Private::ReloadValueIter<Timer, Freq, Fcpu, 0>::value;
		static const typename Timer::ClockDivider Divider = Private::DividerIter<Timer, Freq, Fcpu, 0>::Result::value;
		static const unsigned DividerValue = Private::DividerIter<Timer, Freq, Fcpu, 0>::Result::Div;
		static const unsigned long TickFreq = Fcpu / DividerValue;

	private:
		BOOST_STATIC_ASSERT( uint32_t(Timer::MaxValue - ReloadValue ) * DividerValue > MinTimerClockCycles);
	};

}
