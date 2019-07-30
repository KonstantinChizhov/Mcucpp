
#ifndef __VECTORS_H
#define __VECTORS_H

#ifdef __cplusplus
extern "C" {
#endif

	void DefaultIrqHandler(void) __attribute__((weak, __interrupt__));

	void NMIException              (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void HardFaultException        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void MemManageException        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void BusFaultException         (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void UsageFaultException       (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DebugMonitor              (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SVCHandler                (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void PendSVC                   (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SysTickHandler            (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void WWDG_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void PVD_IRQHandler            (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TAMPER_IRQHandler         (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void RTC_IRQHandler            (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void FLASH_IRQHandler          (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void RCC_IRQHandler            (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI0_IRQHandler          (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI1_IRQHandler          (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI2_IRQHandler          (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI3_IRQHandler          (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI4_IRQHandler          (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA1_Channel1_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA1_Channel2_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA1_Channel3_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA1_Channel4_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA1_Channel5_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA1_Channel6_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA1_Channel7_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void ADC1_2_IRQHandler         (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USB_HP_CAN1_TX_IRQHandler (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USB_LP_CAN1_RX0_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void CAN1_RX1_IRQHandler       (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void CAN1_SCE_IRQHandler       (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI9_5_IRQHandler        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	
#if defined(STM32F103XG)
	void TIM1_BRK_TIM9_IRQHandler       (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM1_UP_TIM10_IRQHandler       (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM1_TRG_COM_TIM11_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
#else
	void TIM1_BRK_IRQHandler       (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM1_UP_IRQHandler        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM1_TRG_COM_IRQHandler   (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
#endif
	void TIM1_CC_IRQHandler        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM2_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM3_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM4_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void I2C1_EV_IRQHandler        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void I2C1_ER_IRQHandler        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void I2C2_EV_IRQHandler        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void I2C2_ER_IRQHandler        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SPI1_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SPI2_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USART1_IRQHandler         (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USART2_IRQHandler         (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USART3_IRQHandler         (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI15_10_IRQHandler      (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void RTC_Alarm_IRQHandler      (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USBWakeUp_IRQHandler      (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	
#if defined(STM32F103XE)
	void TIM8_BRK_IRQHandler       (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM8_UP_IRQHandler        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM8_TRG_COM_IRQHandler   (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM8_CC_IRQHandler        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void ADC3_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void FSMC_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SDIO_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM5_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SPI3_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void UART4_IRQHandler          (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void UART5_IRQHandler          (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM6_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM7_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA2_Channel1_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA2_Channel2_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA2_Channel3_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA2_Channel4_5_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));

#elif defined(STM32F103XG)
	void TIM8_BRK_TIM12_IRQHandler       (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM8_UP_TIM13_IRQHandler        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM8_TRG_COM_TIM14_IRQHandler   (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM8_CC_IRQHandler        (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void ADC3_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void FSMC_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SDIO_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM5_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SPI3_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void UART4_IRQHandler          (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void UART5_IRQHandler          (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM6_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM7_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA2_Channel1_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA2_Channel2_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA2_Channel3_IRQHandler  (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMA2_Channel4_5_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
#elif defined(STM32F103XB)

#else
	#error "Unsupported device"
#endif
    extern void assert_failed(unsigned char* file, unsigned int line);

#ifdef __cplusplus
}
#endif

#endif 
