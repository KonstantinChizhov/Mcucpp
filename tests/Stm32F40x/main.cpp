

#include <clock.h>

using namespace Mcucpp;
using namespace Mcucpp::Clock;

volatile uint32_t clock1 = 0;
volatile uint32_t clock2 = 0;

int main()
{
	//clock1 = :HseClock::Enable();
	//clock1 = :SysClock::SelectClockSource(Clock::SysClock::External);
	// 0x40023800
	clock1 = SysClock::SetClockFreq(168000000);
	
	// HseClock::Enable();
	// PllClock::SelectClockSource(PllClock::External);
	// PllN::Set(200);
	// PllM::Set(2);
	// PllP::Set(0);
	// PllQ::Set(2);
	// SysClock::SelectClockSource(SysClock::Pll);
	
	clock2 = SysClock::ClockFreq();
	while(1)
	{
		
	}
}


