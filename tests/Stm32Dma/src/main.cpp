
#define USE_PORTA
#define USE_PORTC
#define USE_PORTB

#include <iopins.h>
#include <pinlist.h>
#include <delay.h>
//#include <timers.h>
#include <dma_buffer.h>

using namespace Mcucpp::IO;
using namespace IO;

typedef Pc8 led;
typedef Pc9 led2;

extern "C" void DefaultIrqHandler(void)
{
	while(1)
	{
		led::Toggle();
		Util::delay_ms<50, 8000000>();
	}
}

inline void * operator new(unsigned int, void* ptr)
{
	return ptr;
}

DmaWriteBuffer<Dma1Channel4, char, 8, 8> buffer(&USART1->DR);


static bool Putch(uint8_t c)
{
	while(( USART1->SR & USART_SR_TXE) == 0);
	USART1->DR = c;
	return true;
}

static void UsartInit(unsigned baud, unsigned flags)
{
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
	// Configure PA9 as alternative output for usart
	//GPIOA->CRH = (AltOut << 1*4); 
	Pa9::SetConfiguration<Porta::AltOut>();
	
	unsigned brr = 8000000 / baud;
	USART1->BRR = brr;
	USART1->CR1 = (flags & 0xffff) | USART_CR1_UE;
	USART1->CR2 = (flags >> 16) & 0xffff;
	USART1->CR3 |= USART_CR3_DMAT;
}

enum UsartMode
{
	DataBits8 = 0,
	DataBits9 = USART_CR1_M,

	NoneParity = 0,
	EvenParity = USART_CR1_PCE,
	OddParity  = USART_CR1_PS | USART_CR1_PCE,

	NoClock = 0,

	Disabled = 0,
	RxEnable = USART_CR1_RE,
	TxEnable = USART_CR1_TE,
	RxTxEnable  = USART_CR1_RE | USART_CR1_TE,
	Default = RxTxEnable,

	OneStopBit         = 0,
	HalfStopBit        = USART_CR2_STOP_0 << 16,
	TwoStopBits        = USART_CR2_STOP_1 << 16,
	OneAndHalfStopBits = ((USART_CR2_STOP_0 << 16) | (USART_CR2_STOP_1 << 16))
};

void PutsDma(char *str)
{
	while(char c = *str++)
		buffer.Write(c);
}

void Puts(char *str)
{
	while(char c = *str++)
		Putch(c);
}

int main()
{
	Porta::Enable();
	Portb::Enable();
	Portc::Enable();
	led::SetConfiguration(led::Port::Out);
	led2::SetConfiguration(led2::Port::Out);
	UsartInit(9600, RxTxEnable | DataBits8 | TwoStopBits);
	Puts("Hello, world");
	while(1)
	{
		led::Toggle();
		Util::delay_ms<50, 8000000>();
		
	}
}

