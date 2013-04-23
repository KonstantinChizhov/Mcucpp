
#include <gtest/gtest.h>
#include <Dispatcher.h>


using namespace Mcucpp;

bool task1Called = false;
void Task1()
{
	task1Called = true;
}

bool task2Called = false;
void Task2()
{
	task2Called = true;
}

TEST(Dispatcher, SetTask)
{
	task1Called = false;
	task2Called = false;
	Dispatcher<10, 10> dispatcher;
	EXPECT_TRUE(dispatcher.SetTask(Task1));
	EXPECT_TRUE(dispatcher.SetTask(Task2));
	dispatcher.Poll();
	EXPECT_TRUE(task1Called);
	EXPECT_FALSE(task2Called);

	dispatcher.Poll();
	EXPECT_TRUE(task1Called);
	EXPECT_TRUE(task2Called);
}

TEST(Dispatcher, SetTimer)
{
	task1Called = false;
	task2Called = false;
	Dispatcher<10, 10> dispatcher;
	EXPECT_TRUE(dispatcher.SetTimer(Task1, 10));
	EXPECT_TRUE(dispatcher.SetTimer(Task2, 20));
	dispatcher.Poll();
	EXPECT_FALSE(task1Called);
	EXPECT_FALSE(task2Called);
	dispatcher.Poll();
	EXPECT_FALSE(task1Called);
	EXPECT_FALSE(task2Called);

	for(int i = 0; i < 10; i++)
		dispatcher.TimerHandler();
	dispatcher.Poll();
	EXPECT_TRUE(task1Called);
	EXPECT_FALSE(task2Called);

	for(int i = 0; i < 10; i++)
		dispatcher.TimerHandler();
	dispatcher.Poll();
	EXPECT_TRUE(task1Called);
	EXPECT_TRUE(task2Called);
}


TEST(Dispatcher, SetTaskOverflow)
{
	task1Called = false;
	task2Called = false;
	Dispatcher<10, 10> dispatcher;
	for(int i = 0; i < 10; i++)
		EXPECT_TRUE(dispatcher.SetTask(Task1));
	EXPECT_FALSE(dispatcher.SetTask(Task1));
	for(int i = 0; i < 10; i++)
	{
		dispatcher.Poll();
		EXPECT_TRUE(task1Called);
		task1Called = false;
	}
	dispatcher.Poll();
	EXPECT_FALSE(task1Called);
}