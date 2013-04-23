
#include <spi.h>

using namespace Mcucpp;

int main()
{
	Spi::Init(Spi::Fast, Spi::Master | Spi::LsbFirst);
	
	while(1)
	{
		Spi::ClearSS();
		Spi::Write(0x55);
		Spi::SetSS();
	}
	return 0;
}
