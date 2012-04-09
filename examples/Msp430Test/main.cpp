
#include <msp430.h>

#include "pinlist.h"
#include "iopins.h"

using namespace Mcucpp::IO;

typedef PinList<P1_0, P1_1, P1_2, P1_3> Pins;
volatile uint8_t v = 100;
int main()
{
    Pins::Write(v);

    return 0;
}
