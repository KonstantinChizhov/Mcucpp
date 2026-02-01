/*
 *
 * PLIC header file bm310s6 core
 *
 * created by DVS
 *
 */

#ifndef _PLIC_H
#define _PLIC_H

#include "stdint.h"

#define __I volatile const /*!< defines 'read only' permissions      */
#define __O volatile       /*!< defines 'write only' permissions     */
#define __IO volatile      /*!< defines 'read / write' permissions   */

enum Plic_Target
{
    Plic_Mach_Target = 0x0,
    Plic_SuperVisor_Target
};

#define PLIC_WDT_VECTNUM 1
#define PLIC_CAN0_VECTNUM 2
#define PLIC_CAN1_VECTNUM 3
#define PLIC_USB_VECTNUM 4
#define PLIC_GPIO_VECTNUM 5
#define PLIC_TMR32_VECTNUM 6
#define PLIC_TMR0_VECTNUM 7
#define PLIC_TMR1_VECTNUM 8
#define PLIC_TMR2_VECTNUM 9
#define PLIC_QSPI0_VECTNUM 10
#define PLIC_QSPI1_VECTNUM 11
#define PLIC_QSPI2_VECTNUM 12
#define PLIC_DMA0_VECTNUM 13
#define PLIC_DMA1_VECTNUM 14
#define PLIC_DMA2_VECTNUM 15
#define PLIC_DMA3_VECTNUM 16
#define PLIC_DMA4_VECTNUM 17
#define PLIC_DMA5_VECTNUM 18
#define PLIC_DMA6_VECTNUM 19
#define PLIC_DMA7_VECTNUM 20
#define PLIC_I2C_VECTNUM 21
#define PLIC_UART0_VECTNUM 22
#define PLIC_UART1_VECTNUM 23
#define PLIC_UART2_VECTNUM 24
#define PLIC_UART3_VECTNUM 25
#define PLIC_UART4_VECTNUM 26
#define PLIC_CRYPTOHASH_VECTNUM 27
#define PLIC_TRNG_VECTNUM 28
#define PLIC_ADC_SAR_VECTNUM 29
#define PLIC_CMP_VECTNUM 30
#define PLIC_PMURTC_VECTNUM 31

typedef void irqfunc(void);

#ifdef __cplusplus
extern "C"
{
#endif

    void PLIC_SetIrqHandler(uint8_t target, uint32_t isr_num, irqfunc *func);
    void PLIC_SetPriority(uint32_t isr_num, uint8_t pri);
    // void PLIC_SetMode (uint32_t isr_num, Plic_IrqMode_TypeDef mode);
    void PLIC_IntEnable(uint8_t target, uint32_t isr_num);
    void PLIC_IntDisable(uint8_t target, uint32_t isr_num);
    void PLIC_MachHandler(void);
    void PLIC_SetThreshold(uint8_t target, uint32_t value);
    void trap_handler(void);

#ifdef __cplusplus
}
#endif
#endif
