
#include <iopins.h>
#include <usart.h>
#include <watchdog.h>
#include <flash_crc_check.h>
#include <tiny_ostream.h>
#include <delay.h>
using namespace Mcucpp;
using namespace Mcucpp::IO;

class DebugOut
{
	public:
	void put(char c)
	{
		if(c == '\n')
		{
			Usart1::Write('\r');
		}
		Usart1::Write(c);
	}
};

typedef basic_ostream<DebugOut> ostream;
ostream cout;

typedef Pg13 Led;

int main()
{
	Watchdog::Start(1000);
	Porta::Enable();
	Led::SetConfiguration(Led::Port::Out);
	
	if(FlashCrcExpected() != FlashCrcComputed())
	{
		while(true) // cause watchdog to reset MCU after 1 sec
		{
			Led::Toggle();
			delay_ms<50, F_CPU>();
		}
	}
	
	Usart1::Init(9600);
	Usart1::SelectTxRxPins(0, 0);

	cout << hex << "FlashCrcExpected=" << FlashCrcExpected() << ", FlashCrcComputed=" << FlashCrcComputed();

	while(1)
	{
		Led::Toggle();
		delay_ms<200, F_CPU>();
		Watchdog::Reset();
	}
	return 0;
}


