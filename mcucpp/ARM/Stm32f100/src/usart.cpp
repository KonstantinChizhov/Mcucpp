
#include <usart.h>
#include <compiler.h>
#include <dispatcher.h>

using namespace Mcucpp;

extern "C" MCUCPP_INTERRUPT(USART1_IRQHandler)
{
	if(USART1->SR & USART_SR_IDLE)
	{
		(void)USART1->DR;
		USART1->CR1 &= ~USART_CR1_IDLEIE;
		GetCurrentDispatcher().SetTask(Usart1::OnReadTimeout);
	}
}

extern "C" MCUCPP_INTERRUPT(USART2_IRQHandler)
{
	if(USART2->SR & USART_SR_IDLE)
	{
		(void)USART2->DR;
		USART2->CR1 &= ~USART_CR1_IDLEIE;
		GetCurrentDispatcher().SetTask(Usart2::OnReadTimeout);
	}
}

extern "C" MCUCPP_INTERRUPT(USART3_IRQHandler)
{
	if(USART3->SR & USART_SR_IDLE)
	{
		USART3->CR1 &= ~USART_CR1_IDLEIE;
		(void)USART3->DR;
		GetCurrentDispatcher().SetTask(Usart3::OnReadTimeout);
	}
}