
#include <usart.h>
#include <compiler.h>
#include <dispatcher.h>

using namespace Mcucpp;

extern "C" MCUCPP_INTERRUPT(USART1_IRQHandler)
{
    uint32_t sr = USART1->SR;
    if (sr & USART_SR_IDLE)
    {
        USART1->CR1 &= ~USART_CR1_IDLEIE;
        Usart1::OnReadTimeout();
    }
}

extern "C" MCUCPP_INTERRUPT(USART2_IRQHandler)
{
    uint32_t sr = USART2->SR;
    if (sr & USART_SR_IDLE)
    {
        USART2->CR1 &= ~USART_CR1_IDLEIE;
        Usart2::OnReadTimeout();
    }
}

extern "C" MCUCPP_INTERRUPT(USART3_IRQHandler)
{
    uint32_t sr = USART3->SR;
    if (sr & USART_SR_IDLE)
    {
        USART3->CR1 &= ~USART_CR1_IDLEIE;
        Usart3::OnReadTimeout();
    }
}

extern "C" MCUCPP_INTERRUPT(UART4_IRQHandler)
{
}

extern "C" MCUCPP_INTERRUPT(UART5_IRQHandler)
{
}
