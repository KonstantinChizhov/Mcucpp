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

#include <containers.h>
#include <atomic.h>

namespace Mcucpp
{
	typedef void (*task_t)();

	template<uint8_t TasksLenght, uint8_t TimersLenght, class Atomic=VoidAtomic>
	class Dispatcher
	{
		struct TimerData
		{
			task_t task;
			uint16_t period;
		};

	public:
		Dispatcher()
		{
			for(size_t i=0; i < TimersLenght; i++)
			{
				_timers[i].task = 0;
			}
		}

		bool SetTask(task_t task)
		{
			return _tasks.push_back(task);
		}

		bool SetTimer(task_t timerTask, uint16_t period)
		{
			for(size_t i=0; i <TimersLenght; i++)
			{
				task_t task = Atomic::Fetch(&_timers[i].task);
				if(task == 0)
				{
					_timers[i].task = timerTask;
					_timers[i].period = period;
					return true;
				}
				if(task == timerTask)
				{
					_timers[i].period = period;
					return true;
				}
			}
			return false;
		}

		void StopTimer(task_t taskToStop)
		{
			for(size_t i=0; i < TimersLenght; i++)
			{
				task_t task = Atomic::Fetch(&_timers[i].task);
				if(task == taskToStop)
				{
					_timers[i].task = 0;
					return;
				}
			}
		}

		void Poll()
		{
			if(!_tasks.empty())
			{
				task_t task = _tasks.front();
				_tasks.pop_front();
				task();
			}
		}

		void TimerHandler()
		{
			for(size_t i=0; i < TimersLenght; i++)
			{
				task_t task = Atomic::Fetch(&_timers[i].task);
				if(task != 0 &&	Atomic::SubAndFetch(&_timers[i].period, 1) == 0)
				{
					_tasks.push_back(task);
					_timers[i].task = 0;
				}
			}
		}

	private:
		Containers::RingBuffer<TasksLenght, task_t, Atomic> _tasks;
		TimerData _timers[TimersLenght];
	};
}

