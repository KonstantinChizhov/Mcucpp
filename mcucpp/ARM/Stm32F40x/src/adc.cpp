#include <adc.h>
#include <compiler.h>

using namespace Mcucpp;

extern "C" MCUCPP_INTERRUPT(ADC_IRQHandler)
{
	Adc1::IrqHandler();
	Adc2::IrqHandler();
	Adc3::IrqHandler();
}
