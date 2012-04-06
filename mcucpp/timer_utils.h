
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
			static const unsigned long _ticks = Fcpu / Divider::Div / Freq;
			static const unsigned long ticks = Timer::MaxValue > _ticks ? _ticks : Timer::MaxValue;
			static const unsigned long value = Timer::MaxValue - ticks;
			static const unsigned long realFreq = Fcpu / Divider::Div / ticks;
			static const unsigned long error = realFreq > Freq ? realFreq - Freq : Freq - realFreq;
		};

		template<class Timer, unsigned long Freq, unsigned long Fcpu, int CurrentDividerN, int BestDividerN>
		class DividerSelector
		{
			typedef typename Timer::template Divider<CurrentDividerN> CurrentDivider;
			typedef ReloadValue<Timer, CurrentDivider, Freq, Fcpu> CurrentValue;

			typedef typename Timer::template Divider<BestDividerN> BestDivider;
			typedef ReloadValue<Timer, BestDivider, Freq, Fcpu> BestValue;

			enum{NewBestDivider = (CurrentValue::error < BestValue::error ||
									(CurrentValue::error == BestValue::error &&
										CurrentValue::ticks > BestValue::ticks)) ?
				CurrentDividerN	: BestDividerN};

			public:
			typedef typename DividerSelector<
				Timer, Freq, Fcpu,
				CurrentDividerN-1, NewBestDivider>::Result Result;
		};

		template<class Timer, unsigned long Freq, unsigned long Fcpu, int BestDivider>
		struct DividerSelector<Timer, Freq, Fcpu, -1, BestDivider>
		{
			typedef typename Timer::template Divider<BestDivider> Result;
		};
	}

	template<class Timer, unsigned long Freq, unsigned long Fcpu DEFAULT_TIMER_CLOCK_FREQ>
	class TimerFreqSetup
	{
		typedef Private::DividerSelector<Timer, Freq, Fcpu, Timer::MaxDivider, Timer::MaxDivider> DivSelector;
		typedef Private::ReloadValue<Timer, typename DivSelector::Result, Freq, Fcpu> ReloadValueHolder;
	public:
		static const typename Timer::ClockDivider divider = DivSelector::Result::value;
		static const unsigned dividerValue = DivSelector::Result::Div;
		static const typename Timer::DataT reloadValue = ReloadValueHolder::value;
		static const unsigned long realFreq = ReloadValueHolder::realFreq;
		static const unsigned long tickFreq = Fcpu / dividerValue;
		static const unsigned long error = ReloadValueHolder::error;
	};

}
}