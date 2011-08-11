#include <avr/io.h>
#include <usart.h>
#include <tiny_ostream.h>
#include <format_parser.h>

//#include <stdio.h>
#include <flashptr.h>

FLASH char str1[] = "Str = %|-12|\nPORTA = %|-x10|\n";
FLASH char str2[] = "Hello world!!";

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


ostream& operator<<(ostream &s, ProgmemPtr<char> str)
{
	s.puts(str);
	return s;
}

ostream cout;

int main()
{

	MyUsart::Init<115200>();
// Format string stored in flash
	cout % IO::Format(FLASH_PTR(str1)) % FLASH_PTR(str2) % PORTA;
// Format string stored in ram
	//cout % IO::Format("% -- %\n") % "Hello world" % PORTA;
	
	while(1)
	{
		
	}
}
