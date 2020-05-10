
#include <usart.h>
#include <compiler.h>

using namespace Mcucpp;

extern "C" MCUCPP_INTERRUPT(USART1_IRQHandler)
{
	if(USART1->ISR & USART_ISR_RTOF)
	{
		GetCurrentDispatcher().SetTask(Usart1::OnReadTimeout);
		USART1->ICR = USART_ICR_RTOCF;
	}
	if(USART1->ISR & USART_ISR_IDLE)
    {
        USART1->CR1 &= ~USART_CR1_IDLEIE;
        USART1->ICR = USART_ICR_IDLECF;
        GetCurrentDispatcher().SetTask(Usart1::OnReadTimeout);
    }
}

extern "C" MCUCPP_INTERRUPT(USART2_IRQHandler)
{
	if(USART2->ISR & USART_ISR_RTOF)
	{
		GetCurrentDispatcher().SetTask(Usart2::OnReadTimeout);
		USART2->ICR = USART_ICR_RTOCF;
	}
	if(USART2->ISR & USART_ISR_IDLE)
    {
        USART2->CR1 &= ~USART_CR1_IDLEIE;
        USART2->ICR = USART_ICR_IDLECF;
        GetCurrentDispatcher().SetTask(Usart2::OnReadTimeout);
    }
}

extern "C" MCUCPP_INTERRUPT(USART3_IRQHandler)
{
	if(USART3->ISR & USART_ISR_RTOF)
	{
		GetCurrentDispatcher().SetTask(Usart3::OnReadTimeout);
		USART3->ICR = USART_ICR_RTOCF;
	}
	if(USART3->ISR & USART_ISR_IDLE)
    {
        USART3->CR1 &= ~USART_CR1_IDLEIE;
        USART3->ICR = USART_ICR_IDLECF;
        GetCurrentDispatcher().SetTask(Usart3::OnReadTimeout);
    }
}

extern "C" MCUCPP_INTERRUPT(UART4_IRQHandler)
{
//	if(USART4->ISR & USART_ISR_RTOF)
//    {
//        GetCurrentDispatcher().SetTask(Usart4::OnReadTimeout);
//        USART4->ICR = USART_ICR_RTOCF;
//    }
}

extern "C" MCUCPP_INTERRUPT(UART5_IRQHandler)
{
//	if(USART5->ISR & USART_ISR_RTOF)
//    {
//        GetCurrentDispatcher().SetTask(Usart5::OnReadTimeout);
//        USART5->ICR = USART_ICR_RTOCF;
//    }
}

extern "C" MCUCPP_INTERRUPT(LPUART1_IRQHandler)
{
    if(LPUART1->ISR & USART_ISR_IDLE)
    {
        LPUART1->CR1 &= ~USART_CR1_IDLEIE;
        LPUART1->ICR = USART_ICR_IDLECF;
        GetCurrentDispatcher().SetTask(LpUsart1::OnReadTimeout);
    }
}
