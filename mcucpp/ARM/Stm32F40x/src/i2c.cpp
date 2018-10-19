
#include <i2c.h>
#include <compiler.h>

using namespace Mcucpp;

extern "C"
{
	MCUCPP_INTERRUPT(I2C1_EV_IRQHandler)
	{
		I2c1::EventIrqHandler();
	}

	MCUCPP_INTERRUPT( I2C1_ER_IRQHandler)
	{
		I2c1::ErrorIrqHandler();
	}
}
