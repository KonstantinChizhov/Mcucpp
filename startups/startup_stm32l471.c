
#include "vectors_stm32l471.h"
#include <stm32l471xx.h>

void DefaultIrqHandler(void)
{
	while (1)
		;
}

static inline void FpuInit()
{
	SCB->CPACR |= (0xF << 20);
	asm
	(
		"DSB\n"
		"ISB\n"
	);
}

const uint32_t BootRAM = 0xF1E0F85F;

extern int main(void);

extern unsigned long _etext;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;
extern unsigned long _s_ram2_data;
extern unsigned long _e_ram2_data;
extern unsigned long _eheap;
extern unsigned long __ctors_start__;
extern unsigned long __ctors_end__;
extern unsigned long _estack;

/* stm32 vectors */
__attribute__ ((section(".isr_vectors"), used))
void (* const g_pfnVectors[])(void) =
{
	(void (*)(void))((unsigned long)&_estack),
	Reset_Handler,
	NMI_Handler,
	HardFault_Handler,
	MemManage_Handler,
	BusFault_Handler,
	UsageFault_Handler,
	0,
	0,
	0,
	0,
	SVC_Handler,
	DebugMon_Handler,
	0,
	PendSV_Handler,
	SysTick_Handler,
	WWDG_IRQHandler,
	PVD_PVM_IRQHandler,
	TAMP_STAMP_IRQHandler,
	RTC_WKUP_IRQHandler,
	FLASH_IRQHandler,
	RCC_IRQHandler,
	EXTI0_IRQHandler,
	EXTI1_IRQHandler,
	EXTI2_IRQHandler,
	EXTI3_IRQHandler,
	EXTI4_IRQHandler,
	DMA1_Channel1_IRQHandler,
	DMA1_Channel2_IRQHandler,
	DMA1_Channel3_IRQHandler,
	DMA1_Channel4_IRQHandler,
	DMA1_Channel5_IRQHandler,
	DMA1_Channel6_IRQHandler,
	DMA1_Channel7_IRQHandler,
	ADC1_2_IRQHandler,
	CAN1_TX_IRQHandler,
	CAN1_RX0_IRQHandler,
	CAN1_RX1_IRQHandler,
	CAN1_SCE_IRQHandler,
	EXTI9_5_IRQHandler,
	TIM1_BRK_TIM15_IRQHandler,
	TIM1_UP_TIM16_IRQHandler,
	TIM1_TRG_COM_TIM17_IRQHandler,
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
	RTC_Alarm_IRQHandler,
	DFSDM1_FLT3_IRQHandler,
	TIM8_BRK_IRQHandler,
	TIM8_UP_IRQHandler,
	TIM8_TRG_COM_IRQHandler,
	TIM8_CC_IRQHandler,
	ADC3_IRQHandler,
	FMC_IRQHandler,
	SDMMC1_IRQHandler,
	TIM5_IRQHandler,
	SPI3_IRQHandler,
	UART4_IRQHandler,
	UART5_IRQHandler,
	TIM6_DAC_IRQHandler,
	TIM7_IRQHandler,
	DMA2_Channel1_IRQHandler,
	DMA2_Channel2_IRQHandler,
	DMA2_Channel3_IRQHandler,
	DMA2_Channel4_IRQHandler,
	DMA2_Channel5_IRQHandler,
	DFSDM1_FLT0_IRQHandler,
	DFSDM1_FLT1_IRQHandler,
	DFSDM1_FLT2_IRQHandler,
	COMP_IRQHandler,
	LPTIM1_IRQHandler,
	LPTIM2_IRQHandler,
	0,
	DMA2_Channel6_IRQHandler,
	DMA2_Channel7_IRQHandler,
	LPUART1_IRQHandler,
	QUADSPI_IRQHandler,
	I2C3_EV_IRQHandler,
	I2C3_ER_IRQHandler,
	SAI1_IRQHandler,
	SAI2_IRQHandler,
	SWPMI1_IRQHandler,
	TSC_IRQHandler,
	0,
	0,
	RNG_IRQHandler,
	FPU_IRQHandler
};

/* stm32 reset code */


__attribute__((noreturn, __interrupt__)) void Reset_Handler(void)
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
	
	// zero fill the ram2_data segment
	for (pDest = &_s_ram2_data; pDest < &_e_ram2_data;)
	{
		*pDest++ = 0;
	}
	
	FpuInit();

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

