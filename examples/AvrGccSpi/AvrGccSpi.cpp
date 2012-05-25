
#include <spi.h>

using namespace Mcucpp;

int main()
{
	Spi::Init(Spi::Fast, Spi::Master | Spi::LsbFirst);
	Spi::ClearSS();
	while(1)
	{
		Spi::Write(0x55);
	}
	return 0;
}
