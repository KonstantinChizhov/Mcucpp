#include <usart.h>
#include <tiny_ostream.h>
#include <iopins.h>
#include <pinlist.h>
#include <delay.h>
#include <timers.h>
#include <timer_utils.h>
#include <Dispatcher.h>
#include <atomic.h>

using namespace Mcucpp;

typedef Usart1 MyUsart;

class UsartOut
{
	public:
	void put(char c)
	{
		MyUsart::Write(c);
	}
 };

typedef basic_ostream<UsartOut> ostream;
ostream cout; 


typedef IO::Pc0 Led;
typedef Timers::Timer1 TheTimer;
	                 // timer type, target freq, timer src freq
typedef Timers::TimerFreqSetup<TheTimer, 1000, 8000000> TimerSetup;

uint32_t GetTicks();

void Blink()
{
	Led::Toggle();
	GetCurrentDispatcher().SetTimer(1000, &Blink);
}

class Print
{
	int i;
public:
	Print()
		:i(0)
	{
		
	}
	
	void Do()
	{
		cout << i << "\r\n";
		GetCurrentDispatcher().SetTimer<Print, &Print::Do>(2000, this);
	}
};

int main()
{
	Led::SetConfiguration(Led::Port::Out);
	Led::Set();
	MyUsart::Init(9600);
	MyUsart::SelectTxRxPins(0, 0);
	
	sei();
	
	cout << "Hello, world!\r\n";
	GetCurrentDispatcher().SetTimerFunc(GetTicks);
	
	TheTimer::Start(TimerSetup::divider, TimerSetup::reloadValue);
	TheTimer::EnableInterrupt(TheTimer::OverflowInt);
	
	Blink();
	
	Print print;
	GetCurrentDispatcher().SetTimer<Print, &Print::Do>(2000, &print);
	
	while(1)
	{
		GetCurrentDispatcher().Poll();
	}
	return 0;
}

static uint32_t ms_ticks = 0;

uint32_t GetTicks()
{
	return Atomic::Fetch(&ms_ticks);
}

ISR(TIMER1_OVF_vect)
{
	ms_ticks++;
	TheTimer::Set(TimerSetup::reloadValue);
}

