//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2016
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
#include <stddef.h>
#include <atomic.h>

namespace Mcucpp
{
	typedef void (*task_t)(void *tag);
	typedef void (*simple_task_t)();

	struct TaskItem
	{
		TaskItem(task_t taskArg = 0, void *tagArg = 0)
		:task(taskArg), tag(tagArg)
		{

		}
		void Invoke()
		{
			if(task)
				task(tag);
		}
		task_t task;
		void *tag;
	};

	struct TimerData
	{
		TimerData()
		:time(0), id(0)
		{}
		TaskItem task;
		uint32_t time;
		uint32_t id;
	};

	typedef uint32_t (*GetTimerTicksFuncT)();

	class Dispatcher
	{
		template<class ObjectT, void (ObjectT::* Func)()>
		static void Invoke(void *object)
		{
			return (static_cast<ObjectT*>(object)->* Func)();
		}
		Dispatcher(const Dispatcher&)=delete;
		Dispatcher &operator=(const Dispatcher&)=delete;

		static void SimpleTaskAdapter(void *simple_task)
		{
			reinterpret_cast<simple_task_t>(simple_task)();
		}

		template<class FunctorT>
		static void FunctorAdapter(void *functorPtr)
		{
			(*reinterpret_cast<FunctorT*>(functorPtr))();
		}

	public:
		Dispatcher(TaskItem *taskStorage, size_t tasksCount, TimerData *timerStorage, size_t timersCount)
			:_timerSequence(0),
			_count(0),
			_first(0),
			_last(0),
			_tasksLen(tasksCount),
			_timersLen(timersCount),
			_tasks(taskStorage),
			_timers(timerStorage),
			GetTimerTicksFunc(0)
		{
		}

		void SetTimerFunc(GetTimerTicksFuncT timerFunc)
		{
			GetTimerTicksFunc = timerFunc;
		}

		template<class FunctorT>
		bool SetTask(const FunctorT &functor)
		{
			return SetTask(&FunctorAdapter<FunctorT>, (void*)&functor);
		}

		template<class FunctorT>
		bool SetTask(FunctorT &functor)
		{
			return SetTask(&FunctorAdapter<FunctorT>, (void*)&functor);
		}

		template<class ObjectT, void (ObjectT::*Func)()>
		bool SetTask(ObjectT * object)
		{
			return SetTask(&Invoke<ObjectT, Func>, object);
		}

		bool SetTask(simple_task_t task)
		{
			return SetTask(SimpleTaskAdapter, (void*)task);
		}

		bool SetTask(task_t task, void *tag)
		{
			if(_count >= _tasksLen)
				return false;
			DisableInterrupts di;
			TaskItem item(task, tag);
			_tasks[_last] = item;
			_last++;
			if(_last >= _tasksLen)
				_last = 0;
			_count++;
			return true;
		}

		template<class ObjectT, void (ObjectT::*Func)()>
		uint32_t SetTimer(uint32_t time, ObjectT * object)
		{
			return SetTimer(time, &Invoke<ObjectT, Func>, object);
		}

		uint32_t SetTimer(uint32_t period, simple_task_t timerTask)
		{
			return SetTimer(period, SimpleTaskAdapter, (void*)timerTask);
		}

		uint32_t SetTimer(uint32_t period, task_t timerTask, void *tag)
		{
			if(!GetTimerTicksFunc)
				return 0;
			uint32_t currentTime = GetTimerTicksFunc();
			TimerData *timer = 0;
			DisableInterrupts di;
			for(size_t i=0; i <_timersLen; i++)
			{
				task_t task = _timers[i].task.task;
				if(task == 0 && !timer)
				{
					timer = &_timers[i];
				}
				if(task == timerTask && _timers[i].task.tag == tag)
				{
					timer = &_timers[i];
					break;
				}
			}
			if(timer)
			{
				if(period == 0)
				{
					timer->task = 0;
					timer->time = 0;
				}
				else
				{
					timer->task.task = timerTask;
					timer->task.tag = tag;
					timer->time = currentTime + period;
				}

                timer->id = (_timerSequence++) & 0x00ffffff;
				if(timer->id == 0)
                {
                    timer->id = (_timerSequence++) & 0x00ffffff;
                }
                uint32_t index = uint32_t(timer - &_timers[0]);
                timer->id |=  index << 24;
				return  timer->id;
			}
			return 0;
		}

		template<class ObjectT, void (ObjectT::*Func)()>
		void StopTimer(ObjectT * object)
		{
			StopTimer(&Invoke<ObjectT, Func>, object);
		}

		void StopTimer(task_t taskToStop, void *tag)
		{
			for(size_t i=0; i < _timersLen; i++)
			{
				task_t task = _timers[i].task.task;
				if(task == taskToStop && _timers[i].task.tag == tag)
				{
					_timers[i].task = 0;
					_timers[i].id = 0;
					return;
				}
			}
		}

		void StopTimer(uint32_t id)
		{
		    uint32_t index = id >> 24;
		    if(_timers[index].id == id)
            {
                _timers[index].task = 0;
                _timers[index].id = 0;
            }
		}

		void RestartTimer(uint32_t id, uint32_t period)
		{
		    uint32_t index = id >> 24;
		    if(_timers[index].id == id)
            {
                _timers[index].time = GetTimerTicksFunc() + period;
            }
		}

		bool Poll()
		{
			bool result = true;
			if(GetTimerTicksFunc)
			{
				uint32_t ticks = GetTimerTicksFunc();
				if(!TimerHandler(ticks))
					result = false;
			}
			if(_count > 0)
			{
				TaskItem &task = _tasks[_first];
				ATOMIC
				{
					_count--;
					_first++;
					if(_first >= _tasksLen)
					_first = 0;
				}
				task.Invoke();
			}
			return result;
		}

		bool TimerHandler(uint32_t time)
		{
			for(size_t i=0; i < _timersLen; i++)
			{
				TaskItem &task = _timers[i].task;
				if(task.task != 0 && _timers[i].time <= time)
				{
					if(!SetTask(task.task, task.tag))
						return false;
					task.task = 0;
					_timers[i].id = 0;
				}
			}
			return true;
		}
		uint32_t GetTicks(){return GetTimerTicksFunc ? GetTimerTicksFunc() : 0;}
	private:
		uint32_t _timerSequence;
		size_t _count;
		size_t _first;
		size_t _last;
		size_t _tasksLen;
		size_t _timersLen;
		TaskItem *_tasks;
		TimerData *_timers;
		GetTimerTicksFuncT GetTimerTicksFunc;
	};


	Dispatcher &GetCurrentDispatcher();
}

