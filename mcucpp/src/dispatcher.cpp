#include <stddef.h>
#include <compiler.h>
#include <dispatcher.h>

namespace Mcucpp
{
	static const int TaskCount = 10;
	static const int TimersCount = 10;
	
	static TaskItem taskStorage[TaskCount];
	static TimerData timerStorage[TimersCount];
	
	static Dispatcher mainDispatcher(taskStorage, TaskCount, timerStorage, TimersCount);
	
	Dispatcher &GetCurrentDispatcher()
	{
		return mainDispatcher;
	}
}
