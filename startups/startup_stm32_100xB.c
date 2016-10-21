#include "vectors_stm32_100xB.h"
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
#ifdef STM32F10X_LD
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
#endif
#if  defined(STM32F10X_LD_VL) || defined(STM32F10X_MD_VL)
	ADC1_IRQHandler,                // irq 18, 
	0, 0, 0, 0,
	EXTI9_5_IRQHandler,             // irq 23
	TIM1_BRK_TIM15_IRQHandler,      // irq 24
	TIM1_UP_TIM16_IRQHandler,       // irq 25
	TIM1_TRG_COM_TIM17_IRQHandler,  // irq 26
	TIM1_CC_IRQHandler,             // irq 27
	TIM2_IRQHandler,                // irq 28
	TIM3_IRQHandler,                // irq 29
	0,
	I2C1_EV_IRQHandler,             // irq 31
	I2C1_ER_IRQHandler,             // irq 32
	0, 0,
	SPI1_IRQHandler,                // irq 35
	0,
	USART1_IRQHandler,              // irq 37
	USART2_IRQHandler,              // irq 38
	0,
	EXTI15_10_IRQHandler,           // irq 40
	RTCAlarm_IRQHandler,            // irq 41
	CEC_IRQHandler,                 // irq 42
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	TIM6_DAC_IRQHandler,            // irq 54
	TIM7_IRQHandler                 // irq 55
#endif /* STM32F10X_LD_VL || STM32F10X_MD_VL */
};

/* stm32 reset code */


__attribute__((noreturn, __interrupt__)) void ResetISR(void)
{
	unsigned long volatile * pSrc;
	unsigned long volatile * pDest;

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



