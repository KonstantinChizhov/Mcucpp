

#include <clock.h>

using namespace Mcucpp;

volatile uint32_t clock1 = 0;
volatile uint32_t clock2 = 0;

int main()
{
	//clock1 = Clock::HseClock::Enable();
	//clock1 = Clock::SysClock::SelectClockSource(Clock::SysClock::External);
	// 0x40023800
	clock1 = Clock::SysClock::SetClockFreq(168000000);
	clock2 = Clock::SysClock::ClockFreq();
	while(1)
	{
		
	}
}


