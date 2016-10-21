#include <usart.h>
#include <tiny_ostream.h>
#include <iopins.h>
#include <pinlist.h>
#include <drivers/servo.h>
#include <delay.h>
#include <compiler.h>
#include <timers.h>

using namespace Mcucpp;

typedef Usart1 Usart;

class UsartOut
{
	public:
	void put(char c)
	{
		Usart::Write(c);
	}
};

typedef basic_ostream<UsartOut> ostream;
ostream cout; 


typedef IO::Pc8 Led;
typedef Timers::Timer7 ServoTimer;
typedef IO::PinList<IO::Pb8, IO::Pb9, IO::Pb7> ServoPins;
Servo<ServoPins, ServoTimer> servo;

const int ServoTable[] = {
	128, 134, 140, 146, 153, 159, 165, 171, 
	177, 183, 188, 194, 199, 204, 209, 214, 
	219, 223, 227, 231, 234, 238, 241, 243, 
	246, 248, 250, 251, 253, 254, 254, 254, 
	254, 254, 253, 252, 251, 250, 248, 245, 
	243, 240, 237, 234, 230, 226, 222, 218, 
	213, 208, 204, 198, 193, 187, 182, 176, 
	170, 164, 158, 152, 145, 139, 133, 126, 
	120, 114, 107, 101, 95, 89, 83, 77, 
	71, 66, 60, 55, 50, 45, 40, 36, 
	31, 27, 24, 20, 17, 14, 11, 9, 
	7, 5, 3, 2, 1, 1, 1, 1, 
	1, 2, 3, 4, 6, 8, 10, 12, 
	15, 18, 22, 25, 29, 34, 38, 43, 
	47, 52, 58, 63, 68, 74, 80, 86, 
	92, 98, 104, 111, 117, 123
};

static const int Elements = sizeof(ServoTable) / sizeof(ServoTable[0]);
typedef IO::Pa0 Btn;
int main()
{
	Clock::SysClock::SetClockFreq(24000000);
	
	IO::Porta::Enable();
	IO::Portb::Enable();
	IO::Portc::Enable();
	IO::Portd::Enable();


	Led::SetConfiguration(IO::NativePortBase::Out);
	IO::Pc9::SetConfiguration(IO::NativePortBase::Out);
	Btn::SetConfiguration(IO::NativePortBase::In);
	Btn::SetPullUp(IO::NativePortBase::PullUp);
	
	Led::Set();
	
	Usart::Init(115200);
	Usart::SelectTxRxPins(0, 0);
	
	
	cout << "Hello, world!\r\n";
	cout << "Clock = " << Clock::SysClock::ClockFreq() << "\r\n";

	IO::Pc9::Set();
	
	servo.UpdateTimings();
	
	cout << "servo._pulseDivider = " << servo._pulseDivider << "\r\n";
	cout << "servo._pulseMaxReload = " << servo._pulseMaxTicks << "\r\n";
	cout << "servo._pulseMinReload = " << servo._pulseMinTicks << "\r\n";
	cout << "servo._pauseDivider = " << servo._pauseDivider << "\r\n";
	cout << "servo._pauseReload = " << servo._pauseTicks << "\r\n";
	
	servo.TimerHandler();
	ServoTimer::EnableInterrupt(ServoTimer::OverflowInt);
	
	int pos = 0, prev = 0;
	
	while(1)
	{
		if(Btn::IsSet())
		{
			servo.SetPosition(2, 0);
		}
		else
		{
			servo.SetPosition(2, 255);
		}
		delay_ms<20, 24000000>();
		IO::Pc9::Toggle();
		servo.SetPosition(0, (ServoTable[pos] + ServoTable[prev]) / 2);
		
		delay_ms<20, 24000000>();
		servo.SetPosition(0, ServoTable[pos]);
		
		servo.SetPosition(1, 0);
		
		prev = pos;
		pos++;
		if(pos >= Elements)
			pos = 0;
	}
	return 0;
}

extern "C" MCUCPP_INTERRUPT(TIM7_IRQHandler)
{
	ServoTimer::ClearInterruptFlag();
	servo.TimerHandler();
	ServoTimer::EnableInterrupt(ServoTimer::OverflowInt);
}

