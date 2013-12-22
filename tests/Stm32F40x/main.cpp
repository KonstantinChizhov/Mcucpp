
//volatile uint32_t status = 0xffffffff;

#include <clock.h>
#include <iopins.h>
#include <power.h>
#include <dma.h>
#include <usart.h>
#include <adc.h>
#include <delay.h>

#include <tiny_ostream.h>
#include <tiny_iomanip.h>

using namespace Mcucpp;
using namespace Mcucpp::IO;
using namespace Mcucpp::Clock;


typedef Pd13 Led1;
typedef Pd12 Led2;
typedef Pd14 Led3;
typedef Pd15 Led4;

struct UsartOut
{
	void put(char c)
	{
		if(c == '\n')
			Usart1::Write('\r');
		Usart1::Write(c);
	}
};

typedef basic_ostream<UsartOut> ostream;

ostream cout;

void Hello(void *, size_t)
{
	Led2::Toggle();
	Usart1::SetTxCompleteCallback(Hello);
	Usart1::Write("Hello world!!!\r\n", 16, true);
}

int main()
{
	SysClock::SetClockFreq(168000000);
	Portd::Enable();
	Porta::Enable();
	Portb::Enable();
	Portc::Enable();
	
	Led1::SetConfiguration(NativePortBase::Out);
	Led2::SetConfiguration(NativePortBase::Out);
	
	Usart1::Init(115200);
	Usart1::SelectTxRxPins(1, 1);
	Usart1::EnableInterrupt(Usart1::RxNotEmptyInt);
	cout << "Hello, World!!\nSys Freq = " << SysClock::ClockFreq() << "\n";
	cout << "Usart1 Freq = " << Usart1Clock::ClockFreq() << "\n";
	Adc1::Init();
	
	Usart1::SetTxCompleteCallback(Hello);
	Usart1::Write("Hello world!!!\r\n", 16, true);
	
	Led1::Set();
	delay_ms<5000, 168000000>();
	cout << "\n" << hex << setw(9) << DMA2->LISR << setw(9) << DMA2->HISR << setw(9) << DMA2->LIFCR << setw(9) << DMA2->HIFCR << "\n";
	
	cout << hex << setw(9) << DMA2_Stream7->CR << setw(9) << DMA2_Stream7->NDTR 
		<< setw(9) << DMA2_Stream7->PAR << setw(9) << DMA2_Stream7->M0AR
		<< setw(9) << DMA2_Stream7->FCR << "\n";
		
	//uint16_t data[16] = {0};
	//uint8_t ch[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
	//0x40012000
	while(1)
	{
		//Adc1::ReadSequence(data, ch, 16);
		//for(int i = 0; i<19; i++)
			//cout << setw(6) << Adc1::Read(i);
		//cout << Adc1::Read(Adc1::TempSensorChannel);
		//cout << "\n";
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

