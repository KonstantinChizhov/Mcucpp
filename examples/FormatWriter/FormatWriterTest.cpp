#include <avr/io.h>
#include <usart.h>
#include <tiny_ostream.h>
#include <format_parser.h>
#include <flashptr.h>

FLASH char str1[] = "Str = %|-12|\nPORTA = %|-x10|\n";
FLASH char str2[] = "Hello world!!";
using namespace Mcucpp;

typedef Usart<16, 16> MyUsart;


ISR(USART_UDRE_vect)
{
	MyUsart::TxHandler();
}

template<class Src>
struct RawWriter
{
	void put(char c)
	{
		while(!Src::Putch(c));
	}
};

typedef basic_ostream<RawWriter<MyUsart> > ostream;


ostream& operator<<(ostream &s, ProgmemPtr<char> str)
{
	s.puts(str);
	return s;
}

ostream cout;
#include <iopins.h>
#include <pinlist.h>

int main()
{

	MyUsart::Init<115200>();
	sei ();
// Format string stored in flash
	cout % Format(MakeFlashPtr(str1)) % MakeFlashPtr(str2) % PORTA;
//  Format string stored in ram
//	cout % IO::Format("%|-20| -- %|10|\n") % "Hello world" % 12345;
	while(1)
	{
		
	}
}
