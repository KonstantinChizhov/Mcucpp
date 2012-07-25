#include <iopins.h>
#include <pinlist.h>
#define USE_TIMER_0
#include <timers.h>
#include <drivers/7segments.h>
#include <intrinsics.h>

using namespace Mcucpp;
using namespace IO;
using namespace Timers;


typedef PinList<Pc0, Pc1, Pc2, Pc3, Pc4, Pc5, Pc6, Pc7> Segments;
typedef PinList<Pb0, Pb1, Pb2, Pb3> Commons;

typedef Display7Segment<Segments, Commons, Base7Segment::Seg7All> Display;

#pragma vector=0x14
__interrupt void Timer0Overflow(void)
{
	Display::Update();
}

int main()
{
	Timer0::Start(Timer0::Div8);
	Timer0::EnableInterrupt();
	__enable_interrupt();
	while(1)
	{
		Display::Write(123u);
	}
	return 0;
}
