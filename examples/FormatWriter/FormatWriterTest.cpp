#include <avr/io.h>
#include <usart.h>
#include <tiny_ostream.h>
//#include <stdio.h>
//#include <flashptr.h>


typedef Usart<16, 16> MyUsart;

template<class Src>
struct RawWriter
{
	void put(char c)
	{
		while(!Src::Putch(c));
	}

	void write(const char *str, unsigned size)
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

	MyUsart::Init(115200);

	cout.Format("Str = %-12\n Int = %-10\n PORTA = %") % "Hello" % "world" % PORTA;

	cout << PORTB;
	
	while(1)
	{
		//cout.write("12345", 5);
		cout << "12345";
	}
}

