#include <iopins.h>
#include <pinlist.h>
#include <drivers/HD44780.h>

using namespace Mcucpp;
using namespace Mcucpp::IO;

typedef Lcd<Pc0, Pc1, Pc2, Pa4, Pa5, Pa6, Pa7 > Lcd1;


int main()
{

	Lcd1::Init();
 	Lcd1::Puts("Hello world!", 12);

	while(1)
	{	
	
	}

}
