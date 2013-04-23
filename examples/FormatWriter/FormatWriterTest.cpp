#include <avr/io.h>
#include <usart.h>
#include <tiny_ostream.h>
#include <format_parser.h>
#include <flashptr.h>

const FLASH_STORAGE char str1[] = "Str = %|-12|\nPORTA = %|-x10|\n";
const FLASH_STORAGE char str2[] = "Hello world!!";
using namespace Mcucpp;

typedef Usart<Usart0Regs> MyUsart;
struct UsartWriter
{
	void put(char c)
	{
		if(c == '\n')
			Usart<Usart0Regs>::Write('\r');
		Usart<Usart0Regs>::Write(c);
	}
};

typedef basic_ostream<UsartWriter> ostream;

ostream cout;

ostream& operator<<(ostream &s, ProgmemPtr<char> str)
{
	s.puts(str);
	return s;
}

#include <iopins.h>
#include <pinlist.h>

int main()
{
	MyUsart::Init<9600>();
// Format string stored in flash
	cout % Format(MakeFlashPtr(str1)) % MakeFlashPtr(str2) % PORTA;
//  Format string stored in ram
	cout % Format("%|-20| -- %|10|\n") % "Hello world" % 12345;
	while(1)
	{
		
	}
}
