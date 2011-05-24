
#define USE_PORTA
#define USE_PORTC
#define USE_PORTB

#include <iopins.h>
#include <pinlist.h>
#include <delay.h>
#include <timers.h>
#include <atomic.h>


using namespace IO;

typedef IO::Pc8 led;
typedef IO::Pc9 led2;

volatile uint32_t value = 0;

void Foo()
{
	Atomic::FetchAndAdd(&value, 5);
}

int main()
{
	Porta::Enable();
	Portb::Enable();
	Portc::Enable();
	led::SetConfiguration(led::Port::Out);
	led2::SetConfiguration(led2::Port::Out);
	
	while(1)
	{
		led::Toggle();
		Util::delay_ms<50, 8000000>();
	}
}

