#include <dma.h>
#include <compiler.h>

using namespace Mcucpp;

void VoidDmaCallback(void *, size_t )
{

}


extern "C" MCUCPP_INTERRUPT(DMA1_Channel1_IRQHandler)
{
	Dma1Channel1::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Channel2_IRQHandler)
{
	Dma1Channel2::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Channel3_IRQHandler)
{
	Dma1Channel3::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Channel4_IRQHandler)
{
	Dma1Channel4::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Channel5_IRQHandler)
{
	Dma1Channel5::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Channel6_IRQHandler)
{
	Dma1Channel6::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Channel7_IRQHandler)
{
	Dma1Channel7::IrqHandler();
}


extern "C" MCUCPP_INTERRUPT(DMA2_Channel1_IRQHandler)
{
	Dma2Channel1::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Channel2_IRQHandler)
{
	Dma2Channel2::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Channel3_IRQHandler)
{
	Dma2Channel3::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Channel4_IRQHandler)
{
	Dma2Channel4::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Channel5_IRQHandler)
{
	Dma2Channel5::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Channel6_IRQHandler)
{
	Dma2Channel6::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Channel7_IRQHandler)
{
	Dma2Channel7::IrqHandler();
}
