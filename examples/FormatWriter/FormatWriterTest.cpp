#include <avr/io.h>
#include <usart.h>
#include <tiny_ostream.h>
//#include <stdio.h>
//#include <flashptr.h>


typedef Usart<16, 16> MyUsart;

ISR(USART_UDRE_vect)
{
	MyUsart::TxHandler();
}

template<class Src>
struct RawWriter
{
	static void put(char c)
	{
		while(!Src::Putch(c));
	}

	static void write(const char *str, unsigned size)
	{
		const char * end = str + size;
		while(str < end)
		{
			put(*str++);
		}
	}
};

typedef IO::FormatWriter<RawWriter<MyUsart> > ostream;

ostream cout;

int main()
{

	MyUsart::Init<115200>();

	cout.Format("Str = %|-12|\nPORTA = %|-x10|\n") % "Hello world" % PORTA;

	
	while(1)
	{
		
	}
}

