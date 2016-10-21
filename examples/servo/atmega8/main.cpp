#include <usart.h>
#include <tiny_ostream.h>
#include <iopins.h>
#include <pinlist.h>
#include <delay.h>
#include <timers.h>
#include <drivers/servo.h>

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



typedef IO::Pc8 Led;
typedef Timers::Timer1 ServoTimer;
typedef IO::PinList<IO::Pc0, IO::Pc1> ServoPins;
Servo<ServoPins, ServoTimer> servo;


const int ServoTable[] = {
	128, 140, 153, 165,
	177, 188, 199, 209,
	219, 227, 234, 241,
	246, 250, 253, 254,
	254, 253, 251, 248,
	243, 237, 230, 222,
	213, 204, 193, 182,
	170, 158, 145, 133,
	120, 107, 95,  83, 
	71,  60,  50,  40, 
	31,  24,  17,  11, 
	7,   3,   1,   1,  
	1,   3,   6,   10, 
	15,  22,  29,  38, 
	47,  58,  68,  80, 
	92,  104, 117
};

static const int Elements = sizeof(ServoTable) / sizeof(ServoTable[0]);


int main()
{	
	MyUsart::Init(9600);
	MyUsart::SelectTxRxPins(0, 0);
	sei();
	
	cout << "Hello, world!\r\n";
	cout << "Clock = " << Clock::SysClock::ClockFreq() << "\r\n";
		
	servo.UpdateTimings();
	
	cout << "servo._pulseDivider = " << servo._pulseDivider << "\r\n";
	cout << "servo._pulseMaxReload = " << servo._pulseMaxTicks << "\r\n";
	cout << "servo._pulseMinReload = " << servo._pulseMinTicks << "\r\n";
	cout << "servo._pauseDivider = " << servo._pauseDivider << "\r\n";
	cout << "servo._pauseReload = " << servo._pauseTicks << "\r\n";
	
	servo.TimerHandler();
	ServoTimer::EnableInterrupt(ServoTimer::OverflowInt);
	
	int pos = 0, prev = 0;

	DDRC = 0xff;
	while(1)
	{
		delay_ms<10, F_CPU>();
		servo.SetPosition(0, (ServoTable[pos] + ServoTable[prev]) / 2);
		
		delay_ms<10, F_CPU>();
		servo.SetPosition(0, ServoTable[pos]);
		
		servo.SetPosition(1, 127);
		
		prev = pos;
		pos++;
		if(pos >= Elements)
			pos = 0; 
	}
	return 0;
}

ISR(TIMER1_OVF_vect)
{
	ServoTimer::ClearInterruptFlag();
	servo.TimerHandler();
	ServoTimer::EnableInterrupt(ServoTimer::OverflowInt);
}

