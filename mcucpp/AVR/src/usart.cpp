
#include <usart.h>

#if MCUCPP_HAS_USART1 && MCUCPP_HAS_USART2

ISR(USART0_RX_vect)
{
	Mcucpp::Usart1::RxIntHandler();
}

ISR(USART0_UDRE_vect)
{
	Mcucpp::Usart1::TxIntHandler();
}

ISR(USART1_RX_vect)
{
	Mcucpp::Usart2::RxIntHandler();
}

ISR(USART1_UDRE_vect)
{
	Mcucpp::Usart2::TxIntHandler();
}

#elif MCUCPP_HAS_USART1

#if defined(__ATmega16A__) || defined( __ATmega16__) || defined( __ATmega323__) || \
	defined( __ATmega32A__) || defined( __ATmega32__) || defined( __ATmega8515__) || \
	defined( __ATmega8535__) || defined( __ATmega8A__) || defined( __ATmega8__) || \
	defined(__AVR_ATmega16__) || defined( __AVR_ATmega323__) || defined( __AVR_ATmega32__) || defined( __AVR_ATmega8__)
 
ISR(USART_RXC_vect)
{
	Mcucpp::Usart1::RxIntHandler();
}
#else
ISR(USART_RX_vect)
{
	Mcucpp::Usart1::RxIntHandler();
}
#endif

ISR(USART_UDRE_vect)
{
	Mcucpp::Usart1::TxIntHandler();
}

#endif