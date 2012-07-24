
#include <iopins.h>
#include <pinlist.h>
#include <timers.h>
#include <drivers/7segments.h>

#include <avr/interrupt.h>

using namespace Mcucpp;
using namespace IO;
using namespace Timers;


typedef PinList<Pc0, Pc1, Pc2, Pc3, Pc4, Pc5, Pc6, Pc7> Segments;
typedef PinList<Pb0, Pb1, Pb2, Pb3> Commons;

typedef Display7Segment<Segments, Commons, Base7Segment::Seg7All> Display;

ISR(TIMER0_OVF_vect)
{
	Display::Update();
}

int main()
{
	Timer0::Start(Timer0::Div8);
	Timer0::EnableInterrupt();
	sei();
	while(1)
	{
		Display::Write(123u);
	}
	return 0;
}
