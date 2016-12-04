
#include <ethernet.h>
#include <compiler.h>
#include <iopins.h>

namespace Mcucpp
{
	EthernetMac ethernet(ETH);
}


extern "C" MCUCPP_INTERRUPT(ETH_IRQHandler)
{
	Mcucpp::ethernet.InterruptHandler();
	Mcucpp::IO::Pd13::Toggle();
}

extern "C" MCUCPP_INTERRUPT(ETH_WKUP_IRQHandler)
{
	Mcucpp::IO::Pd13::Toggle();
}
