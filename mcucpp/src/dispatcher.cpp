#include <stddef.h>
#include <compiler.h>
#include <dispatcher.h>

namespace Mcucpp
{
#if defined(DISPATCHER_MAX_TASKS) 
	static const int TaskCount = DISPATCHER_MAX_TASKS;
#else
	static const int TaskCount = 10;
#endif

#if defined(DISPATCHER_MAX_TIMERS) 
	static const int TimersCount = DISPATCHER_MAX_TIMERS;
#else
	static const int TimersCount = 10;
#endif
	
	static TaskItem taskStorage[TaskCount];
	static TimerData timerStorage[TimersCount];
	
	static Dispatcher mainDispatcher(taskStorage, TaskCount, timerStorage, TimersCount);
	
	Dispatcher &GetCurrentDispatcher()
	{
		return mainDispatcher;
	}
}
