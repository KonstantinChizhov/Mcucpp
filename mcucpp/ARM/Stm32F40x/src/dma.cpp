#include <dma.h>
#include <compiler.h>

using namespace Mcucpp;

void VoidDmaCallback(void *, size_t )
{

}

extern "C" MCUCPP_INTERRUPT(DMA1_Stream0_IRQHandler)
{
	Dma1Channel0::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Stream1_IRQHandler)
{
	Dma1Channel1::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Stream2_IRQHandler)
{
	Dma1Channel2::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Stream3_IRQHandler)
{
	Dma1Channel3::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Stream4_IRQHandler)
{
	Dma1Channel4::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Stream5_IRQHandler)
{
	Dma1Channel5::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Stream6_IRQHandler)
{
	Dma1Channel6::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA1_Stream7_IRQHandler)
{
	Dma1Channel7::IrqHandler();
}

extern "C" MCUCPP_INTERRUPT(DMA2_Stream0_IRQHandler)
{
	Dma2Channel0::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Stream1_IRQHandler)
{
	Dma2Channel1::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Stream2_IRQHandler)
{
	Dma2Channel2::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Stream3_IRQHandler)
{
	Dma2Channel3::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Stream4_IRQHandler)
{
	Dma2Channel4::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Stream5_IRQHandler)
{
	Dma2Channel5::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Stream6_IRQHandler)
{
	Dma2Channel6::IrqHandler();
}
extern "C" MCUCPP_INTERRUPT(DMA2_Stream7_IRQHandler)
{
	Dma2Channel7::IrqHandler();
}
