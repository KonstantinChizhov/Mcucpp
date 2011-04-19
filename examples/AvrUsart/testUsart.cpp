#include "Usart.h"

// 8 bytes tx fifo buffer, 
// 16 bytes rx fifo buffer
// interrupt driven USART
typedef Usart<8, 16> usart;

ISR(USART_UDRE_vect)
{	
	usart::TxHandler();
}


ISR(USART_RXC_vect)
{
	usart::RxHandler();
}


int main()
{
	usart::Init(115200);
	uint8_t c;
	while(1)
	{	
		// echo recived data back
		if(usart::Getch(c))
			usart::Putch(c);
	}
}
