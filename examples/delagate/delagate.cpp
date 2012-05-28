
#include <avr/io.h>
#include <delegate.h>

class Foo
{
public:
	void Func(uint32_t v)
	{
		union
		{
			uint32_t u32;
			uint8_t u8[4];
		}Conv;

		Conv.u32 = v;
		DDRA = Conv.u8[0];
		PORTA = Conv.u8[1];
		DDRB = Conv.u8[2];
		PORTB = Conv.u8[3];
	}
};

class Bar
{
public:
	int Func()
	{
		return DDRB;
	}
};


void Hello(uint32_t v)
{
	(void)v;
}

using namespace Mcucpp;

Foo foo;
Bar bar;


template<class Func>
void Fee(const Func &d)
{
	for(int i=0; i<10;i++)
		d(i);
}
	

__attribute__((OS_main))
int main()
{

	delegate1<void, uint32_t> d(foo, &Foo::Func);
	delegate1<void, uint32_t> d2(Hello);
	delegate<int> d3(bar, &Bar::Func);

	d(123);
	d2(987);
	d3();

	Fee(Hello);
	Fee(d);
	Fee(d2);


	while(1)
	{

	}
	return 0;
}
