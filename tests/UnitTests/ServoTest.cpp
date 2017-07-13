

#include <gtest.h>
#include <drivers/servo.h>
#include <timers.h>
#include <iopins.h>
#include <pinlist.h>
#include <clock.h>
#include <DividerSelector.h>

using namespace Mcucpp;
using namespace Mcucpp::IO;
using namespace Mcucpp::Timers;
using namespace Mcucpp::Clock;

typedef PinList<Pa0, Pa1, Pa2, Pa3> Pins;
uint32_t SysClock::CpuFreq;
typedef Timer1 ServoTestTimer;

TEST(TestTimer, DividerSelection)
{
	DividerSelector<ServoTestTimer> selector;
	for(int j = 100; j < 20000; j+= 100)
	{
		bool result = selector.ProcessMkSek(24000000, j);
		EXPECT_TRUE(result);
		int t = selector.GetTicks();
		EXPECT_GE(0xffff, t);
	}
}

TEST(Servo, AllTurnedOff)
{
	SysClock::SetClockFreq(24000000);
	
	Servo<Pins, ServoTestTimer> servo;

	int expectedPauseDivider = 7;
	
	servo.UpdateTimings();

	
	for(int i = 0; i < Pins::Length + 2; i++)
	{
		servo.TimerHandler();
		EXPECT_EQ(expectedPauseDivider, ServoTestTimer::timerData.CurrentDivider);
		EXPECT_NE(0, ServoTestTimer::timerData.Counter);
		EXPECT_EQ(Pins::Read(), 0);
	}
}

TEST(Servo, OneTurnedOn)
{
	SysClock::SetClockFreq(24000000);
	Servo<Pins, ServoTestTimer> servo;

	int expectedPauseDivider = 7;
	
	servo.UpdateTimings();
	
	servo.SetPosition(0, 1);
		
	servo.TimerHandler();
	EXPECT_EQ(expectedPauseDivider, ServoTestTimer::timerData.CurrentDivider);
	EXPECT_NE(0, ServoTestTimer::timerData.Counter);
	EXPECT_EQ(Pins::Read(), 0);
	
	servo.TimerHandler();
	EXPECT_EQ(1, ServoTestTimer::timerData.CurrentDivider);
	EXPECT_NE(0, ServoTestTimer::timerData.Counter);
	EXPECT_EQ(Pins::Read(), 1);
	
	servo.TimerHandler();
	EXPECT_EQ(expectedPauseDivider, ServoTestTimer::timerData.CurrentDivider);
	EXPECT_NE(0, ServoTestTimer::timerData.Counter);
	EXPECT_EQ(Pins::Read(), 0);
}

TEST(Servo, AllTurnedOnSamePosition)
{
	SysClock::SetClockFreq(24000000);
	Servo<Pins, ServoTestTimer> servo;

	int expectedPauseDivider = 7;
	
	servo.UpdateTimings();
	
	servo.SetPosition(0, 1);
	servo.SetPosition(1, 1);
	servo.SetPosition(2, 1);
	servo.SetPosition(3, 1);
	
	servo.TimerHandler();
	EXPECT_EQ(expectedPauseDivider, ServoTestTimer::timerData.CurrentDivider);
	EXPECT_NE(0, ServoTestTimer::timerData.Counter);
	EXPECT_EQ(Pins::Read(), 0);
	
	servo.TimerHandler();
	EXPECT_EQ(1, ServoTestTimer::timerData.CurrentDivider);
	EXPECT_NE(0, ServoTestTimer::timerData.Counter);
	EXPECT_EQ(Pins::Read(), 15);
	
	servo.TimerHandler();
	EXPECT_EQ(expectedPauseDivider, ServoTestTimer::timerData.CurrentDivider);
	EXPECT_NE(0, ServoTestTimer::timerData.Counter);
	EXPECT_EQ(Pins::Read(), 0);
}

TEST(Servo, AllTurnedOnDifferentPosition)
{
	SysClock::SetClockFreq(24000000);
	Servo<Pins, ServoTestTimer> servo;

	int expectedPauseDivider = 7;
	
	servo.UpdateTimings();
	
	servo.SetPosition(0, 10);
	servo.SetPosition(1, 20);
	servo.SetPosition(2, 30);
	servo.SetPosition(3, 40);
	
	servo.TimerHandler();
	EXPECT_EQ(expectedPauseDivider, ServoTestTimer::timerData.CurrentDivider);
	EXPECT_NE(0, ServoTestTimer::timerData.Counter);
	EXPECT_EQ(Pins::Read(), 0);
	
	servo.TimerHandler();
	EXPECT_EQ(1, ServoTestTimer::timerData.CurrentDivider);
	EXPECT_NE(0, ServoTestTimer::timerData.Counter);
	EXPECT_EQ(Pins::Read(), 15);
	
	servo.TimerHandler();
	EXPECT_EQ(1, ServoTestTimer::timerData.CurrentDivider);
	EXPECT_NE(0, ServoTestTimer::timerData.Counter);
	EXPECT_EQ(Pins::Read(), 14);
	
	servo.TimerHandler();
	EXPECT_EQ(1, ServoTestTimer::timerData.CurrentDivider);
	EXPECT_NE(0, ServoTestTimer::timerData.Counter);
	EXPECT_EQ(Pins::Read(), 12);
	
	servo.TimerHandler();
	EXPECT_EQ(1, ServoTestTimer::timerData.CurrentDivider);
	EXPECT_NE(0, ServoTestTimer::timerData.Counter);
	EXPECT_EQ(Pins::Read(), 8);
	
	servo.TimerHandler();
	EXPECT_EQ(expectedPauseDivider, ServoTestTimer::timerData.CurrentDivider);
	EXPECT_NE(0, ServoTestTimer::timerData.Counter);
	EXPECT_EQ(Pins::Read(), 0);
}