#include <stdint.h>
#include <avr/io.h>
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
//	Timer0::OutputCompare<0>::Set(100);
//	Timer0::OutputCompare<0>::EnableInterrupt();
	sei();

	while(1)
	{
	
	}

	return 0;
}
