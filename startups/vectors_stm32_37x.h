
#ifndef __VECTORS_H
#define __VECTORS_H

#ifdef __cplusplus
extern "C" {
#endif

    void DefaultIrqHandler(void) __attribute__((weak, __interrupt__));

	void Reset_Handler                () __attribute__((noreturn, __interrupt__));
	
	void NMI_Handler                  () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void HardFault_Handler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void MemManage_Handler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void BusFault_Handler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void UsageFault_Handler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void SVC_Handler                  () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DebugMon_Handler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void PendSV_Handler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void SysTick_Handler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void WWDG_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void PVD_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	
	void TAMPER_STAMP_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void RTC_WKUP_IRQHandler          () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void FLASH_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void RCC_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void EXTI0_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void EXTI1_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void EXTI2_TS_IRQHandler          () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void EXTI3_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void EXTI4_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Channel1_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Channel2_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Channel3_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Channel4_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Channel5_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Channel6_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Channel7_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void ADC1_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void CAN1_TX_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void CAN1_RX0_IRQHandler          () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void CAN1_RX1_IRQHandler          () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void CAN1_SCE_IRQHandler          () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void EXTI9_5_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM15_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM16_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM17_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM18_DAC2_IRQHandler        () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM2_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM3_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM4_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void I2C1_EV_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void I2C1_ER_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void I2C2_EV_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void I2C2_ER_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void SPI1_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void SPI2_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void USART1_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void USART2_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void USART3_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void EXTI15_10_IRQHandler         () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void RTC_Alarm_IRQHandler         () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void CEC_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM12_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM13_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM14_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM5_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void SPI3_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM6_DAC1_IRQHandler         () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM7_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Channel1_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Channel2_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Channel3_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Channel4_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Channel5_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void SDADC1_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void SDADC2_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void SDADC3_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void COMP_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void USB_HP_IRQHandler                 () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void USB_LP_IRQHandler                 () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void USBWakeUp_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM19_IRQHandler                  () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void FPU_IRQHandler                    () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	
	extern void assert_failed(unsigned char* file, unsigned int line);
	
#ifdef __cplusplus
}
#endif

#endif
         
		 
		