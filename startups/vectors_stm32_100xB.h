
#ifndef __VECTORS_H
#define __VECTORS_H

#ifdef __cplusplus
extern "C" {
#endif

	void DefaultIrqHandler(void) __attribute__((weak, __interrupt__));

	void NMIException(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void HardFaultException(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void MemManageException(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void BusFaultException(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void UsageFaultException(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DebugMonitor(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SVCHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void PendSVC(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SysTickHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void WWDG_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void PVD_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TAMPER_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void RTC_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void FLASH_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void RCC_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI0_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI1_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI2_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI3_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI4_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMAChannel1_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMAChannel2_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMAChannel3_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMAChannel4_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMAChannel5_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMAChannel6_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void DMAChannel7_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	
#ifdef STM32F10X_LD
	void ADC_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USB_HP_CAN_TX_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USB_LP_CAN_RX0_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void CAN_RX1_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void CAN_SCE_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI9_5_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM1_BRK_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM1_UP_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM1_TRG_COM_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM1_CC_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM2_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM3_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM4_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void I2C1_EV_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void I2C1_ER_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void I2C2_EV_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void I2C2_ER_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SPI1_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SPI2_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USART1_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USART2_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USART3_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI15_10_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void RTCAlarm_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USBWakeUp_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
#endif /* STM32F10X_LD */  
	
	
#if  defined(STM32F10X_LD_VL) || defined(STM32F10X_MD_VL)
	void ADC1_IRQHandler              (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI9_5_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM1_BRK_TIM15_IRQHandler    (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM1_UP_TIM16_IRQHandler     (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM1_TRG_COM_TIM17_IRQHandler(void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM1_CC_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM2_IRQHandler              (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM3_IRQHandler              (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void I2C1_EV_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void I2C1_ER_IRQHandler           (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void SPI1_IRQHandler              (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USART1_IRQHandler            (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void USART2_IRQHandler            (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void EXTI15_10_IRQHandler         (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void RTCAlarm_IRQHandler          (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void CEC_IRQHandler               (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM6_DAC_IRQHandler          (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
	void TIM7_IRQHandler              (void) __attribute__((weak, __interrupt__, alias ("DefaultIrqHandler")));
#endif /* STM32F10X_LD_VL || STM32F10X_MD_VL */
	

    extern void assert_failed(unsigned char* file, unsigned int line);

#ifdef __cplusplus
}
#endif

#endif 
