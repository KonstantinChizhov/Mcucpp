
//volatile uint32_t status = 0xffffffff;

#include <clock.h>
#include <iopins.h>
#include <power.h>
#include <dma.h>
#include <usart.h>
//#include <adc.h>
#include <delay.h>
#include <hw_random.h>
#include <sys_tick.h>

#include <tiny_ostream.h>
#include <tiny_iomanip.h>


using namespace Mcucpp;
using namespace Mcucpp::IO;
using namespace Mcucpp::Clock;


typedef Pc13 Led1;


struct UsartOut
{
	void put(char c)
	{
		if(c == '\n')
			Usart1::Write('\r');
		Usart1::Write(c);
	}
};

struct SwoOut
{
	void put(char c)
	{
		if(c == '\n')
			ITM_SendChar('\r');
		ITM_SendChar(c);
	}
};

typedef basic_ostream<UsartOut> ostream;

ostream cout;

namespace Mcucpp
{
	size_t HeapBytesUsed();
}

int main()
{
	SysClock::SetClockFreq(F_CPU);
	Portd::Enable();
	Porta::Enable();
	Portb::Enable();
	Portc::Enable();
	
	Led1::SetConfiguration(NativePortBase::Out);
	
	Usart1::Init(115200);
	Usart1::SelectTxRxPins(1, 1);
	Usart1::EnableInterrupt(Usart1::RxNotEmptyInt);
	cout << "Hello, World!!\nSys Freq = " << SysClock::ClockFreq() << "\n";
	cout << "Usart1 Freq = " << Usart1Clock::ClockFreq() << "\n";
	cout << "SysTick->CALIB = " << hex << SysTick->CALIB << "\n";
	cout << "SysTick->CTRL = " << hex << SysTick->CTRL << "\n";

	cout << "HeapBytesUsed=" << HeapBytesUsed() << "\n";
	int *arr = new int[ 10];
	cout << hex << (uint32_t) arr << dec << " HeapBytesUsed=" << HeapBytesUsed() << "\n";


	if(!Usart1::WriteAsync("Hello world!!!\r\n", 16, [](auto a1, auto a2, bool r){ } ))
	{
		cout << "failed WriteAsync\r\n"; 
	}
	
	Led1::Set();
	cout << "SysTick->CTRL = " << hex << SysTick->CTRL << "\n";
	while(1)
	{
		Led1::Toggle();
		delay_ms<1000, F_CPU>();
	}
}


