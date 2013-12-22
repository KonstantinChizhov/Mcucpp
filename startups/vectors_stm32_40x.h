
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
	void DMA1_Stream0_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Stream1_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Stream2_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Stream3_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Stream4_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Stream5_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Stream6_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void ADC_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	
	void CAN1_TX_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void CAN1_RX0_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void CAN1_RX1_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void CAN1_SCE_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void EXTI9_5_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM1_BRK_TIM9_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM1_UP_TIM10_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM1_TRG_COM_TIM11_IRQHandler () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM1_CC_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM2_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM3_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM4_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void I2C1_EV_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void I2C1_ER_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void I2C2_EV_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void I2C2_ER_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void SPI1_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void SPI2_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void USART1_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void USART2_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void USART3_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void EXTI15_10_IRQHandler          () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void RTC_Alarm_IRQHandler          () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void OTG_FS_WKUP_IRQHandler        () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM8_BRK_TIM12_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM8_UP_TIM13_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM8_TRG_COM_TIM14_IRQHandler () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM8_CC_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA1_Stream7_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void FSMC_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void SDIO_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM5_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void SPI3_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void UART4_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void UART5_IRQHandler              () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM6_DAC_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void TIM7_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Stream0_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Stream1_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Stream2_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Stream3_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Stream4_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void ETH_IRQHandler                () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void ETH_WKUP_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void CAN2_TX_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void CAN2_RX0_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void CAN2_RX1_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
    void CAN2_SCE_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void OTG_FS_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Stream5_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Stream6_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DMA2_Stream7_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void USART6_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void I2C3_EV_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void I2C3_ER_IRQHandler            () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void OTG_HS_EP1_OUT_IRQHandler     () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void OTG_HS_EP1_IN_IRQHandler      () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void OTG_HS_WKUP_IRQHandler        () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void OTG_HS_IRQHandler             () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void DCMI_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void CRYP_IRQHandler               () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void HASH_RNG_IRQHandler           () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	void FPU_IRQHandler                () __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler"))); 
	
	extern void assert_failed(unsigned char* file, unsigned int line);

#ifdef __cplusplus
}
#endif

#endif 
