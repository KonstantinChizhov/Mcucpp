
//#include <clock.h>
//#include <iopins.h>
//#include <power.h>
//#include <dma.h>
//#include <usart.h>
//#include <adc.h>
//#include <delay.h>
//#include <hw_random.h>
//#include <sys_tick.h>

//#include <tiny_ostream.h>
//#include <tiny_iomanip.h>
#include <mcu_header.h>
#include <compiler.h>

#include <functional>

//using namespace Mcucpp;
//using namespace Mcucpp::IO;
//using namespace Mcucpp::Clock;


struct SwoOut
{
	void put(char c)
	{
		if(c == '\n')
			ITM_SendChar('\r');
		ITM_SendChar(c);
	}
};

//typedef basic_ostream<SwoOut> ostream;

//ostream cout;
SwoOut out;

std::function<void(char)> f = [](char c){out.put(c);};

int main()
{
	GPIOA->ODR = sizeof(std::function<void(SwoOut&, char)>);
	
	// SysClock::SetClockFreq(72000000);
	// Porta::Enable();
	
	// Pa1::Set();
	// cout << "Hello, World!!\nSys Freq = " << SysClock::ClockFreq() << "\n";
	// cout << "Usart1 Freq = " << Usart1Clock::ClockFreq() << "\n";
	// cout << "SysTick->CALIB = " << hex << SysTick->CALIB << "\n";
		
	// while(1)
	// {
		// delay_ms<100, F_CPU>();
	// }
	return 0;
}

extern "C" MCUCPP_INTERRUPT(SysTick_Handler)
{
	f('.');
}

void* __dso_handle;
namespace std
{
	void __throw_bad_function_call()
	{
		while(1);
	}
}
