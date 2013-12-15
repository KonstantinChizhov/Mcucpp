
//volatile uint32_t status = 0xffffffff;

#include <clock.h>
#include <iopins.h>
#include <power.h>
#include <dma.h>
#include <usart.h>

using namespace Mcucpp;
using namespace Mcucpp::IO;
using namespace Mcucpp::Clock;

volatile uint32_t clock1 = 0;
volatile uint32_t clock2 = 0;
volatile uint32_t clock3 = 0;

typedef Pd13 Led1;
typedef Pd12 Led2;
typedef Pd14 Led3;
typedef Pd15 Led4;

uint8_t buffer [200];
uint8_t buffer2 [200];
int main()
{
	SysClock::SetClockFreq(168000000);
	Portd::Enable();
	Porta::Enable();
	Portb::Enable();
	
	Led1::SetConfiguration(NativePortBase::Out);
	Pa1:: SetConfiguration(NativePortBase::Out);
	
	for(unsigned i =0; i < sizeof(buffer); i++)
	{
		buffer[i] = (uint8_t)i;
	}
	
	//clock1 = Flash::ErasePage(4);
	//clock2 = Flash::WritePage(4, buffer, sizeof(buffer));
	//clock2 = Flash::WritePage(4, buffer, sizeof(buffer), 300);
	
	//Dma2Channel0::Transfer(DmaBase::Mem2Mem | DmaBase::MemIncriment | DmaBase::PeriphIncriment,
	//		buffer2, buffer, sizeof(buffer));
	
	Usart1::Init(115200);
	Usart1::SelectTxRxPins(1, 1);
	Usart1::EnableInterrupt(Usart1::RxNotEmptyInt);
	Usart1::Write("Hello, world!!!", 15, true);
	Usart1::Write("Hello, again!!!", 15, true);
	
	Led1::Set();
	while(1)
	{
		Pa1::Toggle();
		//Usart1::Write(0xAA);
	}
}

extern "C" void USART1_IRQHandler()
{
	if(Usart1::RxReady())
	{
		Usart1::Write(Usart1::Read()+1);
	}
	Usart1::ClearInterruptFlag(Usart1::RxNotEmptyInt);
}

