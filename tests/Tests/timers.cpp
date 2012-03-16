#include <iostream>
#include <gtest.h>
#include <cmath>

using namespace std;

#include <Timer.h>
#include <timer_utils.h>
using namespace Timers;

TEST(TimerTests, TimerSetupTest)
{
	const unsigned units=10;
	const unsigned freq=12345;
	const unsigned fcpu=16000000;

	typedef TimerFreqSetup<Timer1, freq*units, fcpu*units> TimerSetup1;

    cout << "TimerSetup1::dividerValue == " << TimerSetup1::dividerValue << endl;
    cout << "TimerSetup1::reloadValue == " << (int)TimerSetup1::reloadValue << endl;
    cout << "TimerSetup1::tickFreq == " << TimerSetup1::tickFreq << endl;
	cout << "TimerSetup1::realFreq == " << TimerSetup1::realFreq << endl;
	cout << "TimerSetup1::error == " << TimerSetup1::error << endl;

	double realFreq = (double)fcpu / TimerSetup1::dividerValue / (Timer1::MaxValue - TimerSetup1::reloadValue);
	cout << "realFreq == " << realFreq << endl;
	double realError = abs(realFreq - freq);
	cout << "realError == " << realError << endl;

	EXPECT_LE(realError, (double)TimerSetup1::error / units + 1);
	const unsigned cRealFreq=TimerSetup1::realFreq / units;
	EXPECT_EQ((unsigned)realFreq, cRealFreq );
}

