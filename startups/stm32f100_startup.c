#include "stm32f100_vectors.h"
#include <stm32f10x.h>

void DefaultIrqHandler(void)
{
	while (1)
		;
}

void ResetISR(void) __attribute__((__interrupt__));

extern int main(void);

extern unsigned long _etext;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;
extern unsigned long _eheap;
extern unsigned long __ctors_start__;
extern unsigned long __ctors_end__;

/* stm32 vectors */
__attribute__ ((section(".isr_vectors"), used))
void (* const g_pfnVectors[])(void) =
{
	(void (*)(void))((unsigned long)&_eheap),
	ResetISR,
	NMIException,
	HardFaultException,
	MemManageException,
	BusFaultException,
	UsageFaultException,
	0, 0, 0, 0, /* Reserved */
	SVCHandler,
	DebugMonitor,
	0, /* Reserved */
	PendSVC,
	SysTickHandler,
	WWDG_IRQHandler,
	PVD_IRQHandler,
	TAMPER_IRQHandler,
	RTC_IRQHandler,
	FLASH_IRQHandler,
	RCC_IRQHandler,
	EXTI0_IRQHandler,
	EXTI1_IRQHandler,
	EXTI2_IRQHandler,
	EXTI3_IRQHandler,
	EXTI4_IRQHandler,
	DMAChannel1_IRQHandler,
	DMAChannel2_IRQHandler,
	DMAChannel3_IRQHandler,
	DMAChannel4_IRQHandler,
	DMAChannel5_IRQHandler,
	DMAChannel6_IRQHandler,
	DMAChannel7_IRQHandler,
	ADC_IRQHandler,
	USB_HP_CAN_TX_IRQHandler,
	USB_LP_CAN_RX0_IRQHandler,
	CAN_RX1_IRQHandler,
	CAN_SCE_IRQHandler,
	EXTI9_5_IRQHandler,
	TIM1_BRK_IRQHandler,
	TIM1_UP_IRQHandler,
	TIM1_TRG_COM_IRQHandler,
	TIM1_CC_IRQHandler,
	TIM2_IRQHandler,
	TIM3_IRQHandler,
	TIM4_IRQHandler,
	I2C1_EV_IRQHandler,
	I2C1_ER_IRQHandler,
	I2C2_EV_IRQHandler,
	I2C2_ER_IRQHandler,
	SPI1_IRQHandler,
	SPI2_IRQHandler,
	USART1_IRQHandler,
	USART2_IRQHandler,
	USART3_IRQHandler,
	EXTI15_10_IRQHandler,
	RTCAlarm_IRQHandler,
	USBWakeUp_IRQHandler
};


__attribute__((weak));
void __lowlevel_init()
{
	// Do nothing
}

__attribute__((noreturn, __interrupt__)) void ResetISR(void)
{
	unsigned long *pSrc, *pDest;

	// copy the data segment initializers from flash to SRAM
	pSrc = &_etext;
	for (pDest = &_sdata; pDest < &_edata;)
	{
		*pDest++ = *pSrc++;
	}

	// zero fill the bss segment
	for (pDest = &_sbss; pDest < &_ebss;)
	{
		*pDest++ = 0;
	}
	
	__lowlevel_init();

	// call global object ctors
	typedef void (*voidFunc)();
	for (pDest = &__ctors_start__; pDest < &__ctors_end__; pDest++)
	{
		((voidFunc) (*pDest))();
	}

	// call the application entry point

	main();

	for (;;)
		;
}



