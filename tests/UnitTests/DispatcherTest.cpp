
#include <gtest/gtest.h>
#include <Dispatcher.h>


using namespace Mcucpp;


bool task1Called = false;
void Task1(void *)
{
	task1Called = true;
}

bool task2Called = false;
void Task2(void *)
{
	task2Called = true;
}

TEST(Dispatcher, SetTask)
{
	task1Called = false;
	task2Called = false;
	int p = 0;
	
	TaskItem tasks[10];
	TimerData timers[10];
	Dispatcher dispatcher(tasks, 10, timers, 10);
	EXPECT_TRUE(dispatcher.SetTask(Task1, &p));
	EXPECT_TRUE(dispatcher.SetTask(Task2, &p));
	dispatcher.Poll();
	EXPECT_TRUE(task1Called);
	EXPECT_FALSE(task2Called);

	dispatcher.Poll();
	EXPECT_TRUE(task1Called);
	EXPECT_TRUE(task2Called);
}

class FooBar
{
public:
	FooBar()
	{
		called = 0;
	}
	
	int called;
	
	void Bar()
	{
		called++;
	}
	
	void operator()()
	{
		called++;
	}
};


TEST(Dispatcher, SetTaskClass)
{
	FooBar foo;
	
	TaskItem tasks[10];
	TimerData timers[10];
	
	Dispatcher dispatcher(tasks, 10, timers, 10);
	EXPECT_TRUE((dispatcher.SetTask<FooBar, &FooBar::Bar>(&foo)));
	dispatcher.Poll();
	EXPECT_EQ(1, foo.called);
}

TEST(Dispatcher, SetTaskFunctor)
{
	FooBar foo;
	
	TaskItem tasks[10];
	TimerData timers[10];
	
	Dispatcher dispatcher(tasks, 10, timers, 10);
	EXPECT_TRUE(dispatcher.SetTask(foo));
	dispatcher.Poll();
	EXPECT_EQ(1, foo.called);
}

TEST(Dispatcher, SetTaskLambda)
{
	bool called = false;
	
	TaskItem tasks[10];
	TimerData timers[10];
	
	Dispatcher dispatcher(tasks, 10, timers, 10);
	EXPECT_TRUE(dispatcher.SetTask( [&called] {called=true;} ));
	dispatcher.Poll();
	EXPECT_EQ(true, called);
}

static uint32_t ticks;
static uint32_t GetTicks(){return ++ticks;}

TEST(Dispatcher, SetTimer)
{
	task1Called = false;
	task2Called = false;
	int p = 0;
	TaskItem tasks[10];
	TimerData timers[10];
	Dispatcher dispatcher(tasks, 10, timers, 10);
	dispatcher.SetTimerFunc(GetTicks);
	EXPECT_TRUE(dispatcher.SetTimer(10, Task1, &p));
	EXPECT_TRUE(dispatcher.SetTimer(20, Task2, &p));
	dispatcher.Poll();
	EXPECT_FALSE(task1Called);
	EXPECT_FALSE(task2Called);
	dispatcher.Poll();
	EXPECT_FALSE(task1Called);
	EXPECT_FALSE(task2Called);

	for(int i = 0; i <= 10; i++)
	{
		dispatcher.Poll();
	}

	EXPECT_TRUE(task1Called);
	EXPECT_FALSE(task2Called);

	for(int i = 11; i <= 20; i++)
		dispatcher.Poll();
	dispatcher.Poll();
	EXPECT_TRUE(task1Called);
	EXPECT_TRUE(task2Called);
}


TEST(Dispatcher, SetTaskOverflow)
{
	task1Called = false;
	task2Called = false;
	int p = 0;
	TaskItem tasks[10];
	TimerData timers[10];
	Dispatcher dispatcher(tasks, 10, timers, 10);
	for(int i = 0; i < 10; i++)
		EXPECT_TRUE(dispatcher.SetTask(Task1, &p));
	EXPECT_FALSE(dispatcher.SetTask(Task1, &p));
	for(int i = 0; i < 10; i++)
	{
		dispatcher.Poll();
		EXPECT_TRUE(task1Called);
		task1Called = false;
	}
	dispatcher.Poll();
	EXPECT_FALSE(task1Called);
}