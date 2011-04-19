
#include "io430.h"
#include "iopins.h"
#include "pinlist.h"

void delay_loop(unsigned long count)
{
  do{}while(count--);
}

inline void
_delay_ms(unsigned __ms)
{
  delay_loop(__ms*1000);
}

inline void
_delay_us(unsigned __us)
{
  delay_loop(__us);
}

#include "HD44780.h"

using namespace IO;
typedef PinList<P1_0, NullPin, P1_1, P1_2, P1_3, P1_4, P1_5> LcdBus;
typedef Lcd<LcdBus> MyLcd;

int main( void )
{ 
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW | WDTHOLD;

	MyLcd::Init();
	MyLcd::Puts("Hello msp430", 8);
	MyLcd::Goto(0x40);
	MyLcd::Puts("Hello msp430"+8, 4);
	Port1::Set<0xff>();

  
  while(1)
  { 
   
  }
  return 0;
}
