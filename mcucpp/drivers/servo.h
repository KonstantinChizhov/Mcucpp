#include <clock.h>
#include <DividerSelector.h>

namespace Mcucpp
{
	template<class ServoPins, class Timer, int PositionMin = 0, int PositionMax = 255>
	class Servo
	{
	public:
		enum
		{ 
			ServoCount = ServoPins::Length, 
			NeutralCode = (PositionMin + PositionMax) / 2, 
			DefaultMinPulseMkSec = 800, 
			DefaultMaxPulseMkSec = 2200, 
			DefaultPauseMkSec = 18000
		};
		
		typename Timer::ClockDivider _pulseDivider;
		typename Timer::DataT _pulseMaxTicks;
		typename Timer::DataT _pulseMinTicks;
		
		typename Timer::ClockDivider _pauseDivider;
		typename Timer::DataT _pauseTicks;
		
		volatile typename Timer::DataT _position[ServoCount];
		volatile typename Timer::DataT _positionCache[ServoCount];
		volatile typename Timer::DataT _phase;
		
		void SyncCache();
	public:
	
		Servo();
	
		int Count() { return ServoCount; }
		
		void SetPosition(int n, int value);
		
		void Stop(int n);
		
		void StopAll();
		
		void UpdateTimings() { UpdateTimings(DefaultMinPulseMkSec, DefaultMaxPulseMkSec, DefaultPauseMkSec); }
		
		void UpdateTimings(unsigned minPulseMkSec, unsigned maxPulseMkSec, unsigned pauseMkSec);
		
		unsigned PositionToTimer(int pos);
		
		void TimerHandler();
	};
	
	
	template<class ServoPins, class Timer, int PositionMin, int PositionMax>
	Servo<ServoPins, Timer, PositionMin, PositionMax>::Servo()
		:_pulseDivider(Timer::DividerValue(0)),
		_pulseMaxTicks(0),
		_pulseMinTicks(0),
		_pauseDivider(Timer::DividerValue(0)),
		_pauseTicks(Timer::MaxValue),
		_phase(0)
	{
		for(unsigned i = 0; i < ServoPins::Length; i++)
		{
			_position[i] = 0;
			_positionCache[i] = 0;
		}
	}
	
	template<class ServoPins, class Timer, int PositionMin, int PositionMax>
	void Servo<ServoPins, Timer, PositionMin, PositionMax>::SetPosition(int n, int value)
	{
		if(n < ServoCount)
		{
			if(value < PositionMin)
				value = PositionMin;
			if(value > PositionMax)
				value = PositionMax;
			_positionCache[n] = PositionToTimer(value);
		}
	}
	
	template<class ServoPins, class Timer, int PositionMin, int PositionMax>
	void Servo<ServoPins, Timer, PositionMin, PositionMax>::Stop(int n)
	{
		if(n < ServoCount)
		{
			_positionCache[n] = 0;
		}
	}
	
	template<class ServoPins, class Timer, int PositionMin, int PositionMax>
	void Servo<ServoPins, Timer, PositionMin, PositionMax>::StopAll()
	{
		for(unsigned i = 0; i < ServoPins::Length; i++)
		{
			_positionCache[i] = 0;
		}
	}
	
	template<class ServoPins, class Timer, int PositionMin, int PositionMax>
	void Servo<ServoPins, Timer, PositionMin, PositionMax>::UpdateTimings(unsigned minPulseMkSec, unsigned maxPulseMkSec, unsigned pauseMkSec)
	{
		uint32_t fCpu = Timer::ClockFreq();
		Timers::DividerSelector<Timer> pulseSelector;
		pulseSelector.ProcessMkSek(fCpu, maxPulseMkSec);
		_pulseDivider = pulseSelector.DividerValue();
		
		_pulseMaxTicks = pulseSelector.GetTicks();
		_pulseMinTicks = (typename Timer::DataT)
				(((uint32_t)pulseSelector.GetTicks()) * minPulseMkSec / maxPulseMkSec);
		
		Timers::DividerSelector<Timer> pauseSelector;
		pauseSelector.ProcessMkSek(fCpu, pauseMkSec);
		_pauseDivider = pauseSelector.DividerValue();
		_pauseTicks = pauseSelector.GetTicks();
		
		for(unsigned i = 0; i < ServoPins::Length; i++)
		{
			_positionCache[i] = 0;
		}
		
		ServoPins::SetConfiguration(ServoPins::Out);
	}
	
	template<class ServoPins, class Timer, int PositionMin, int PositionMax>
	unsigned Servo<ServoPins, Timer, PositionMin, PositionMax>::PositionToTimer(int pos)
	{
		return _pulseMinTicks + (uint32_t)(_pulseMaxTicks - _pulseMinTicks) * (pos - PositionMin) / (PositionMax - PositionMin);
	}
	
	template<class ServoPins, class Timer, int PositionMin, int PositionMax>
	void Servo<ServoPins, Timer, PositionMin, PositionMax>::TimerHandler()
	{
		Timer::Stop(); 
		unsigned portValue = 0, mask = 1;
		unsigned next = Timer::MaxValue;
		unsigned phase = _phase;
		for(unsigned i = 0; i < ServoPins::Length; i++)
		{
			unsigned pos = _position[i];
			if(pos > phase)
			{
				if(pos < next)
				{
					next = pos;
				}
				portValue |= mask;
			}
			mask <<= 1;
		}
		unsigned ticks;
		typename Timer::ClockDivider divider;
		
		if(portValue)
		{
			ticks = next - phase;
			divider = _pulseDivider;
		}
		else
		{
			next = 0;
			ticks = _pauseTicks;
			divider = _pauseDivider;
			SyncCache();
		}
		_phase = next;
		Timer::Start(divider, ( typename Timer::DataT)ticks);
		ServoPins::Write((typename ServoPins::ValueType)portValue);
	}
	
	template<class ServoPins, class Timer, int PositionMin, int PositionMax>
	void Servo<ServoPins, Timer, PositionMin, PositionMax>::SyncCache()
	{
		for(unsigned i = 0; i < ServoPins::Length; i++)
		{
			_position[i] = _positionCache[i];
		}
	}
}