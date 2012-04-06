#include <avr/io.h>
#include <spi.h>
#include <tiny_ostream.h>
#include <format_parser.h>
#include <usart.h>
#include <drivers/Rfm70.h>

using namespace Mcucpp;
using namespace Mcucpp::IO;

typedef Rfm70<Spi, Pc4, Pc5, Pc0> transceiver;

typedef Usart<16, 16> MyUsart;

ISR(USART_RXC_vect)
{
	MyUsart::RxHandler();
}

ISR(USART_UDRE_vect)
{
	MyUsart::TxHandler();
}

template<class Src>
class MyStream
{
    public:
    void put(char c)
    {
    	if(c == '\n')
			Src::Putch('\r');
        Src::Putch(c);
    }

    void write(const char *ptr, size_t size)
    {
        for(size_t i=0; i<size; i++)
            put(ptr[i]);
    }
};

typedef basic_ostream<MyStream<MyUsart> > Output;
Output out;


uint16_t temp;

__attribute__((OS_main))
int main()
{
	MyUsart::Init(19200);
	Spi::Init(Spi::Div16);

	sei();

	out << "Rfm70 module at AtMega16\n";
	transceiver::Init();
	transceiver::SetRfChannel(10);
	transceiver::SwitchToRxMode();
	
	//Transiver2::DumpRegs(out);

	transceiver::SetRxAddress<0>(0x3456789a, 0x12);

	while(1)
	{
		if(transceiver::Recive(&temp))
			out % Format("Temp = % C\n") % temp;
		delay_ms<100, F_CPU>();
	}
}
