#include <avr/io.h>
#include <util/delay.h>
#include "iopins.h"
#include "pinlist.h"
#include "latch.h"
#include "HD44780.h"

using namespace IO;

typedef ThreePinLatch<Pb0, Pb1, Pb2, 'L'> Latch1;

	typedef TPin<Latch1, 0> L0;
	typedef TPin<Latch1, 1> L1;
	typedef TPin<Latch1, 2> L2;
	typedef TPin<Latch1, 3> L3;
	typedef TPin<Latch1, 4> L4;
	typedef TPin<Latch1, 5> L5;
	typedef TPin<Latch1, 6> L6;
	typedef TPin<Latch1, 7> L7;

typedef Lcd<PinList<Pc0, Pc1, Pc2, Pa4, Pa5, Pa6, Pa7> > Lcd1;

typedef Lcd<PinList<L0, L1, L2, L3, L4, L5, L6> > Lcd2;


int main()
{

	//Lcd1::Init();
 	//Lcd1::Puts("Hello world!", 12);

	Lcd2::Init();
 	//Lcd2::Puts("Hello world!", 12);

	while(1)
	{	
	
	}

}
