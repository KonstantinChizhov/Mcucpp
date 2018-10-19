#include <adc.h>
#include <compiler.h>

using namespace Mcucpp;

extern "C" MCUCPP_INTERRUPT(ADC1_2_IRQHandler)
{
	Adc1::IrqHandler();
	Adc2::IrqHandler();
}


extern "C" MCUCPP_INTERRUPT(ADC3_IRQHandler)
{
	Adc3::IrqHandler();
}