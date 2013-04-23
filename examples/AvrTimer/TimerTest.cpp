#include <avr/interrupt.h>

#include <timers.h>
#include <iopins.h>

using namespace Mcucpp;
using namespace Timers;
typedef IO::Pb0 Led;

ISR(SIG_OVERFLOW0)
{
	Led::Toggle();
}

int main()
{
	Led::SetConfiguration(Led::Port::Out);
	Timer0::Start(Timer0::Div1);
	Timer0::EnableInterrupt();
	sei();

	while(1)
	{
	
	}

	return 0;
}
