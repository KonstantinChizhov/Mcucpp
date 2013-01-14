/**
  ******************************************************************************
  * @file    MDR32Fx.h
  * @author  Phyton Application Team
  * @version V1.0.0
  * @date    22/06/2011
  * @brief   This file contains all the Special Function Registers definitions
  *          for the Milandr MDR32Fx microcontroller.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, PHYTON SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
  * OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 Phyton</center></h2>
  ******************************************************************************
  * FILE MDR32Fx.h
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MDR32FX_H
#define __MDR32FX_H

/** @addtogroup __CMSIS CMSIS
  * @{
  */

/** @defgroup __MDR32Fx MDR32F9Qx Peripheral
  * @{
  */

#ifdef __cplusplus
extern "C" {
#endif

#if defined ( __CC_ARM   )
  #pragma anon_unions
#endif

/* MDR32Fx Interrupt Number Definition */
typedef enum IRQn
{
/*-- Cortex-M3 Processor Exceptions Numbers ----------------------------------*/
  NonMaskableInt_IRQn     = -14,  /*!<  2 Non Maskable Interrupt              */
  HardFault_IRQn          = -13,  /*!<  3 Hard Fault Interrupt                */
  MemoryManagement_IRQn   = -12,  /*!<  4 Memory Management Interrupt         */
  BusFault_IRQn           = -11,  /*!<  5 Bus Fault Interrupt                 */
  UsageFault_IRQn         = -10,  /*!<  6 Usage Fault Interrupt               */
  SVCall_IRQn             = -5,   /*!< 11 SV Call Interrupt                   */
  PendSV_IRQn             = -2,   /*!< 14 Pend SV Interrupt                   */
  SysTick_IRQn            = -1,   /*!< 15 System Tick Timer Interrupt         */

/*-- MDR32Fx specific Interrupt Numbers --------------------------------------*/
  CAN1_IRQn               =  0,   /*!< CAN1 Interrupt                         */
  CAN2_IRQn               =  1,   /*!< CAN1 Interrupt                         */
  USB_IRQn                =  2,   /*!< USB Host Interrupt                     */
  DMA_IRQn                =  5,   /*!< DMA Interrupt                          */
  UART1_IRQn              =  6,   /*!< UART1 Interrupt                        */
  UART2_IRQn              =  7,   /*!< UART2 Interrupt                        */
  SSP1_IRQn               =  8,   /*!< SSP1 Interrupt                         */
  I2C_IRQn                =  10,  /*!< I2C Interrupt                          */
  POWER_IRQn              =  11,  /*!< POWER Detecor Interrupt                */
  WWDG_IRQn               =  12,  /*!< Window Watchdog Interrupt              */
  Timer1_IRQn             =  14,  /*!< Timer1 Interrupt                       */
  Timer2_IRQn             =  15,  /*!< Timer2 Interrupt                       */
  Timer3_IRQn             =  16,  /*!< Timer3 Interrupt                       */
  ADC_IRQn                =  17,  /*!< ADC Interrupt                          */
  COMPARATOR_IRQn         =  19,  /*!< COMPARATOR Interrupt                   */
  SSP2_IRQn               =  20,  /*!< SSP2 Interrupt                         */
  BACKUP_IRQn             =  27,  /*!< BACKUP Interrupt                       */
  EXT_INT1_IRQn           =  28,  /*!< EXT_INT1 Interrupt                     */
  EXT_INT2_IRQn           =  29,  /*!< EXT_INT2 Interrupt                     */
  EXT_INT3_IRQn           =  30,  /*!< EXT_INT3 Interrupt                     */
  EXT_INT4_IRQn           =  31   /*!< EXT_INT4 Interrupt                     */
}IRQn_Type;

/** @addtogroup __Configuration_of_CMSIS Configuration of CMSIS
  * @{
  */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __CM3_REV              0x0102   /*!< Cortex-M3 Core Revision                      */
#define __MPU_PRESENT             1     /*!< MPU present or not                           */
#define __NVIC_PRIO_BITS          3     /*!< Number of Bits used for Priority Levels      */
#define __Vendor_SysTickConfig    0     /*!< Set to 1 if different SysTick Config is used */

/** @} */ /* End of group __Configuration_of_CMSIS */

/* Includes ------------------------------------------------------------------*/
#include "core_cm3.h"
#include "system_MDR32F9Qx.h"

/** @defgroup __Exported_types Exported types
  * @{
  */

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

/** @} */ /* End of group __Exported_types */

/** @addtogroup __MDR32Fx_Peripheral_Units MDR32F9Qx Peripheral Units
  * @{
  */

/** @defgroup MDR32Fx_Periph_CAN CAN
  * @{
  */

/** @defgroup MDR32Fx_Periph_CAN_Data_Structures Data Structures
  * @{
  */

/* MDR_CAN_BUF_TypeDef structure */
typedef struct
{
  __IO uint32_t ID;
  __IO uint32_t DLC;
  __IO uint32_t DATAL;
  __IO uint32_t DATAH;
}MDR_CAN_BUF_TypeDef;

/* MDR_CAN_BUF_FILTER_TypeDef structure */
typedef struct
{
  __IO uint32_t MASK;
  __IO uint32_t FILTER;
}MDR_CAN_BUF_FILTER_TypeDef;

/* MDR_CAN_TypeDef structure */
typedef struct
{
  __IO uint32_t CONTROL;
  __IO uint32_t STATUS;
  __IO uint32_t BITTMNG;
       uint32_t RESERVED0;
  __IO uint32_t INT_EN;
       uint32_t RESERVED1[2];
  __IO uint32_t OVER;
  __IO uint32_t RXID;
  __IO uint32_t RXDLC;
  __IO uint32_t RXDATAL;
  __IO uint32_t RXDATAH;
  __IO uint32_t TXID;
  __IO uint32_t TXDLC;
  __IO uint32_t DATAL;
  __IO uint32_t DATAH;
  __IO uint32_t BUF_CON[32];
  __IO uint32_t INT_RX;
  __IO uint32_t RX;
  __IO uint32_t INT_TX;
  __IO uint32_t TX;
       uint32_t RESERVED2[76];
    MDR_CAN_BUF_TypeDef CAN_BUF[32];
       uint32_t RESERVED3[64];
    MDR_CAN_BUF_FILTER_TypeDef CAN_BUF_FILTER[32];
}MDR_CAN_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_CAN_Data_Structures */

/** @defgroup MDR32Fx_Periph_CAN_Defines Defines
  * @{
  */

/*-- CAN_CONTROL: CAN Control Register -----------------------------*/
/* Bit field positions: */
#define CAN_CONTROL_CAN_EN_Pos                  0
#define CAN_CONTROL_ROM_Pos                     1
#define CAN_CONTROL_STM_Pos                     2
#define CAN_CONTROL_SAP_Pos                     3
#define CAN_CONTROL_ROP_Pos                     4

/* Bit field masks: */
#define CAN_CONTROL_CAN_EN                      ((uint32_t)0x00000001)
#define CAN_CONTROL_ROM                         ((uint32_t)0x00000002)
#define CAN_CONTROL_STM                         ((uint32_t)0x00000004)
#define CAN_CONTROL_SAP                         ((uint32_t)0x00000008)
#define CAN_CONTROL_ROP                         ((uint32_t)0x00000010)

/*-- CAN_STATUS: CAN Status Register -------------------------------*/
/* Bit field positions: */
#define CAN_STATUS_RX_READY_Pos                 0
#define CAN_STATUS_TX_READY_Pos                 1
#define CAN_STATUS_ERROR_OVER_Pos               2
#define CAN_STATUS_BIT_ERR_Pos                  3
#define CAN_STATUS_BIT_STUFF_ERR_Pos            4
#define CAN_STATUS_CRC_ERR_Pos                  5
#define CAN_STATUS_FRAME_ERR_Pos                6
#define CAN_STATUS_ACK_ERR_Pos                  7
#define CAN_STATUS_IDLOWER_Pos                  8
#define CAN_STATUS_ERR_STATUS_Pos               9
#define CAN_STATUS_RX_ERR_CNT8_Pos              11
#define CAN_STATUS_TX_ERR_CNT8_Pos              12
#define CAN_STATUS_RX_ERR_CNT_Pos               16
#define CAN_STATUS_TX_ERR_CNT_Pos               24

/* Bit field masks: */
#define CAN_STATUS_RX_READY                     ((uint32_t)0x00000001)
#define CAN_STATUS_TX_READY                     ((uint32_t)0x00000002)
#define CAN_STATUS_ERROR_OVER                   ((uint32_t)0x00000004)
#define CAN_STATUS_BIT_ERR                      ((uint32_t)0x00000008)
#define CAN_STATUS_BIT_STUFF_ERR                ((uint32_t)0x00000010)
#define CAN_STATUS_CRC_ERR                      ((uint32_t)0x00000020)
#define CAN_STATUS_FRAME_ERR                    ((uint32_t)0x00000040)
#define CAN_STATUS_ACK_ERR                      ((uint32_t)0x00000080)
#define CAN_STATUS_IDLOWER                      ((uint32_t)0x00000100)
#define CAN_STATUS_ERR_STATUS_Msk               ((uint32_t)0x00000600)
#define CAN_STATUS_RX_ERR_CNT8                  ((uint32_t)0x00000800)
#define CAN_STATUS_TX_ERR_CNT8                  ((uint32_t)0x00001000)
#define CAN_STATUS_RX_ERR_CNT_Msk               ((uint32_t)0x00FF0000)
#define CAN_STATUS_TX_ERR_CNT_Msk               ((uint32_t)0xFF000000)

/*-- CAN_BITTMNG: CAN Bittiming Register ---------------------------*/
/* Bit field positions: */
#define CAN_BITTMNG_BRP_Pos                     0
#define CAN_BITTMNG_PSEG_Pos                    16
#define CAN_BITTMNG_SEG1_Pos                    19
#define CAN_BITTMNG_SEG2_Pos                    22
#define CAN_BITTMNG_SJW_Pos                     25
#define CAN_BITTMNG_SB_Pos                      27

/* Bit field masks: */
#define CAN_BITTMNG_BRP_Msk                     ((uint32_t)0x0000FFFF)
#define CAN_BITTMNG_PSEG_Msk                    ((uint32_t)0x00070000)
#define CAN_BITTMNG_SEG1_Msk                    ((uint32_t)0x00380000)
#define CAN_BITTMNG_SEG2_Msk                    ((uint32_t)0x01C00000)
#define CAN_BITTMNG_SJW_Msk                     ((uint32_t)0x06000000)
#define CAN_BITTMNG_SB                          ((uint32_t)0x08000000)

/*-- CAN_INT_EN: CAN Interrupt enable Register ---------------------*/
/* Bit field positions: */
#define CAN_INT_EN_GLB_INT_EN_Pos               0
#define CAN_INT_EN_RX_INT_EN_Pos                1
#define CAN_INT_EN_TX_INT_EN_Pos                2
#define CAN_INT_EN_ERR_INT_EN_Pos               3
#define CAN_INT_EN_ERR_OVER_INT_EN_Pos          4

/* Bit field masks: */
#define CAN_INT_EN_GLB_INT_EN                   ((uint32_t)0x00000001)
#define CAN_INT_EN_RX_INT_EN                    ((uint32_t)0x00000002)
#define CAN_INT_EN_TX_INT_EN                    ((uint32_t)0x00000004)
#define CAN_INT_EN_ERR_INT_EN                   ((uint32_t)0x00000008)
#define CAN_INT_EN_ERR_OVER_INT_EN              ((uint32_t)0x00000010)

/*-- CAN_RXID:   CAN Receive ID Register ---------------------------*/
/*-- CAN_TXID:   CAN Transmit ID Register --------------------------*/
/*-- CAN_BUF_ID: CAN Buffer ID Register ----------------------------*/
/* Bit field positions: */
#define CAN_ID_EID_Pos                          0
#define CAN_ID_SID_Pos                          18

/* Bit field masks: */
#define CAN_ID_EID_Msk                          ((uint32_t)0x0003FFFF)
#define CAN_ID_SID_Msk                          ((uint32_t)0x1FFC0000)

/*-- CAN_RXDLC:   CAN Receive DLC Register -------------------------*/
/*-- CAN_TXDLC:   CAN Transmit DLC Register ------------------------*/
/*-- CAN_BUF_DLC: CAN Buffer DLC Register --------------------------*/
/* Bit field positions: */
#define CAN_DLC_Pos                             0
#define CAN_DLC_RTR_Pos                         8
#define CAN_DLC_R1_Pos                          9
#define CAN_DLC_R0_Pos                          10
#define CAN_DLC_SSR_Pos                         11
#define CAN_DLC_IDE_Pos                         12

/* Bit field masks: */
#define CAN_DLC_Msk                             ((uint32_t)0x0000000F)
#define CAN_DLC_RTR                             ((uint32_t)0x00000100)
#define CAN_DLC_R1                              ((uint32_t)0x00000200)
#define CAN_DLC_R0                              ((uint32_t)0x00000400)
#define CAN_DLC_SSR                             ((uint32_t)0x00000800)
#define CAN_DLC_IDE                             ((uint32_t)0x00001000)

/*-- CAN_RXDATAL:   CAN Receive Data low Register ------------------*/
/*-- CAN_DATAL:     CAN Transmit Data low Register -----------------*/
/*-- CAN_BUF_DATAL: CAN Buffer Data low Register -------------------*/
/* Bit field positions: */
#define CAN_DATAL_DB0_Pos                       0
#define CAN_DATAL_DB1_Pos                       8
#define CAN_DATAL_DB2_Pos                       16
#define CAN_DATAL_DB3_Pos                       24

/* Bit field masks: */
#define CAN_DATAL_DB0_Msk                       ((uint32_t)0x000000FF)
#define CAN_DATAL_DB1_Msk                       ((uint32_t)0x0000FF00)
#define CAN_DATAL_DB2_Msk                       ((uint32_t)0x00FF0000)
#define CAN_DATAL_DB3_Msk                       ((uint32_t)0xFF000000)

/*-- CAN_RXDATAH:   CAN Receive Data high Register -----------------*/
/*-- CAN_DATAH:     CAN Transmit Data high Register ----------------*/
/*-- CAN_BUF_DATAH: CAN Buffer Data high Register ------------------*/
/* Bit field positions: */
#define CAN_DATAH_DB4_Pos                       0
#define CAN_DATAH_DB5_Pos                       8
#define CAN_DATAH_DB6_Pos                       16
#define CAN_DATAH_DB7_Pos                       24

/* Bit field masks: */
#define CAN_DATAH_DB4_Msk                       ((uint32_t)0x000000FF)
#define CAN_DATAH_DB5_Msk                       ((uint32_t)0x0000FF00)
#define CAN_DATAH_DB6_Msk                       ((uint32_t)0x00FF0000)
#define CAN_DATAH_DB7_Msk                       ((uint32_t)0xFF000000)

/*-- CAN_BUF_CON: CAN Buffer Connection Register -------------------*/
/* Bit field positions: */
#define CAN_BUF_CON_EN_Pos                      0
#define CAN_BUF_CON_RX_TXN_Pos                  1
#define CAN_BUF_CON_OVER_EN_Pos                 2
#define CAN_BUF_CON_RTR_EN_Pos                  3
#define CAN_BUF_CON_PRIOR_0_Pos                 4
#define CAN_BUF_CON_TX_REQ_Pos                  5
#define CAN_BUF_CON_RX_FULL_Pos                 6
#define CAN_BUF_CON_OVER_WR_Pos                 7

/* Bit field masks: */
#define CAN_BUF_CON_EN                          ((uint32_t)0x00000001)
#define CAN_BUF_CON_RX_TXN                      ((uint32_t)0x00000002)
#define CAN_BUF_CON_OVER_EN                     ((uint32_t)0x00000004)
#define CAN_BUF_CON_RTR_EN                      ((uint32_t)0x00000008)
#define CAN_BUF_CON_PRIOR_0                     ((uint32_t)0x00000010)
#define CAN_BUF_CON_TX_REQ                      ((uint32_t)0x00000020)
#define CAN_BUF_CON_RX_FULL                     ((uint32_t)0x00000040)
#define CAN_BUF_CON_OVER_WR                     ((uint32_t)0x00000080)

/** @} */ /* End of group MDR32Fx_Periph_CAN_Defines */

/** @} */ /* End of group MDR32Fx_Periph_CAN */

/** @defgroup MDR32Fx_Periph_USB USB
  * @{
  */

/** @defgroup MDR32Fx_Periph_USB_Data_Structures Data Structures
  * @{
  */

/* MDR_USB_SEP_TypeDef structure */
typedef struct
{
  __IO uint32_t CTRL;
  __IO uint32_t STS;
  __IO uint32_t TS;
  __IO uint32_t NTS;
}MDR_USB_SEP_TypeDef;

/* MDR_USB_SEP_FIFO_TypeDef structure */
typedef struct
{
  __IO uint32_t RXFD;
       uint32_t RESERVED0;
  __IO uint32_t RXFDC_L;
  __IO uint32_t RXFDC_H;
  __IO uint32_t RXFC;
       uint32_t RESERVED1[11];
  __IO uint32_t TXFD;
       uint32_t RESERVED2[3];
  __IO uint32_t TXFDC;
       uint32_t RESERVED3[11];
}MDR_USB_SEP_FIFO_TypeDef;

/* MDR_USB_TypeDef structure */
typedef struct
{
  __IO uint32_t HTXC;
  __IO uint32_t HTXT;
  __IO uint32_t HTXLC;
  __IO uint32_t HTXSE;
  __IO uint32_t HTXA;
  __IO uint32_t HTXE;
  __IO uint32_t HFN_L;
  __IO uint32_t HFN_H;
  __IO uint32_t HIS;
  __IO uint32_t HIM;
  __IO uint32_t HRXS;
  __IO uint32_t HRXP;
  __IO uint32_t HRXA;
  __IO uint32_t HRXE;
  __IO uint32_t HRXCS;
  __IO uint32_t HSTM;
       uint32_t RESERVED0[16];
  __IO uint32_t HRXFD;
       uint32_t RESERVED1;
  __IO uint32_t HRXFDC_L;
  __IO uint32_t HRXFDC_H;
  __IO uint32_t HRXFC;
       uint32_t RESERVED2[11];
  __IO uint32_t HTXFD;
       uint32_t RESERVED3[3];
  __IO uint32_t HTXFC;
       uint32_t RESERVED4[11];
    MDR_USB_SEP_TypeDef USB_SEP[4];
  __IO uint32_t SC;
  __IO uint32_t SLS;
  __IO uint32_t SIS;
  __IO uint32_t SIM;
  __IO uint32_t SA;
  __IO uint32_t SFN_L;
  __IO uint32_t SFN_H;
       uint32_t RESERVED5[9];
    MDR_USB_SEP_FIFO_TypeDef USB_SEP_FIFO[4];
  __IO uint32_t HSCR;
  __IO uint32_t HSVR;
}MDR_USB_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_USB_Data_Structures */

/** @defgroup MDR32Fx_Periph_USB_Defines Defines
  * @{
  */

/*-- USB_HTXC: USB HTXC Register -----------------------------------*/
/* Bit field positions: */
#define USB_HTXC_TREQ_Pos                       0
#define USB_HTXC_SOFS_Pos                       1
#define USB_HTXC_PREEN_Pos                      2
#define USB_HTXC_ISOEN_Pos                      3

/* Bit field masks: */
#define USB_HTXC_TREQ                           ((uint32_t)0x00000001)
#define USB_HTXC_SOFS                           ((uint32_t)0x00000002)
#define USB_HTXC_PREEN                          ((uint32_t)0x00000004)
#define USB_HTXC_ISOEN                          ((uint32_t)0x00000008)

/*-- USB_HTXLC: USB HTXLC Register ---------------------------------*/
/* Bit field positions: */
#define USB_HTXLC_Pos                           0
#define USB_HTXLC_DC_Pos                        2
#define USB_HTXLC_FSPL_Pos                      3
#define USB_HTXLC_FSLR_Pos                      4

/* Bit field masks: */
#define USB_HTXLC_Msk                           ((uint32_t)0x00000003)
#define USB_HTXLC_DC                            ((uint32_t)0x00000004)
#define USB_HTXLC_FSPL                          ((uint32_t)0x00000008)
#define USB_HTXLC_FSLR                          ((uint32_t)0x00000010)

/*-- USB_HIS: USB_HIS Register -------------------------------------*/
/* Bit field positions: */
#define USB_HIS_TDONE_Pos                       0
#define USB_HIS_RESUME_Pos                      1
#define USB_HIS_CONEV_Pos                       2
#define USB_HIS_SOFS_Pos                        3

/* Bit field masks: */
#define USB_HIS_TDONE                           ((uint32_t)0x00000001)
#define USB_HIS_RESUME                          ((uint32_t)0x00000002)
#define USB_HIS_CONEV                           ((uint32_t)0x00000004)
#define USB_HIS_SOFS                            ((uint32_t)0x00000008)

/*-- USB_HIM: USB_HIM Register -------------------------------------*/
/* Bit field positions: */
#define USB_HIM_TDONEIE_Pos                     0
#define USB_HIM_RESUMEIE_Pos                    1
#define USB_HIM_CONEVIE_Pos                     2
#define USB_HIM_SOFIE_Pos                       3

/* Bit field masks: */
#define USB_HIM_TDONEIE                         ((uint32_t)0x00000001)
#define USB_HIM_RESUMEIE                        ((uint32_t)0x00000002)
#define USB_HIM_CONEVIE                         ((uint32_t)0x00000004)
#define USB_HIM_SOFIE                           ((uint32_t)0x00000008)

/*-- USB_HRXS: USB_HRXS Register -----------------------------------*/
/* Bit field positions: */
#define USB_HRXS_CRCERR_Pos                     0
#define USB_HRXS_BSERR_Pos                      1
#define USB_HRXS_RXOF_Pos                       2
#define USB_HRXS_RXTO_Pos                       3
#define USB_HRXS_NAKRXED_Pos                    4
#define USB_HRXS_STALLRXED_Pos                  5
#define USB_HRXS_ACKRXED_Pos                    6
#define USB_HRXS_DATASEQ_Pos                    7

/* Bit field masks: */
#define USB_HRXS_CRCERR                         ((uint32_t)0x00000001)
#define USB_HRXS_BSERR                          ((uint32_t)0x00000002)
#define USB_HRXS_RXOF                           ((uint32_t)0x00000004)
#define USB_HRXS_RXTO                           ((uint32_t)0x00000008)
#define USB_HRXS_NAKRXED                        ((uint32_t)0x00000010)
#define USB_HRXS_STALLRXED                      ((uint32_t)0x00000020)
#define USB_HRXS_ACKRXED                        ((uint32_t)0x00000040)
#define USB_HRXS_DATASEQ                        ((uint32_t)0x00000080)

/*-- USB_SEP_CTRL: USB_SEP Control Register ------------------------*/
/* Bit field positions: */
#define USB_SEP_CTRL_EPEN_Pos                   0
#define USB_SEP_CTRL_EPRDY_Pos                  1
#define USB_SEP_CTRL_EPDATASEQ_Pos              2
#define USB_SEP_CTRL_EPSSTALL_Pos               3
#define USB_SEP_CTRL_EPISOEN_Pos                4

/* Bit field masks: */
#define USB_SEP_CTRL_EPEN                       ((uint32_t)0x00000001)
#define USB_SEP_CTRL_EPRDY                      ((uint32_t)0x00000002)
#define USB_SEP_CTRL_EPDATASEQ                  ((uint32_t)0x00000004)
#define USB_SEP_CTRL_EPSSTALL                   ((uint32_t)0x00000008)
#define USB_SEP_CTRL_EPISOEN                    ((uint32_t)0x00000010)

/*-- USB_SEP_STS: USB_SEP Status Register --------------------------*/
/* Bit field positions: */
#define USB_SEP_STS_SCCRCERR_Pos                0
#define USB_SEP_STS_SCBSERR_Pos                 1
#define USB_SEP_STS_SCRXOF_Pos                  2
#define USB_SEP_STS_SCRXTO_Pos                  3
#define USB_SEP_STS_SCNAKSENT_Pos               4
#define USB_SEP_STS_SCSTALLSENT_Pos             5
#define USB_SEP_STS_SCACKRXED_Pos               6
#define USB_SEP_STS_SCDATASEQ_Pos               7

/* Bit field masks: */
#define USB_SEP_STS_SCCRCERR                    ((uint32_t)0x00000001)
#define USB_SEP_STS_SCBSERR                     ((uint32_t)0x00000002)
#define USB_SEP_STS_SCRXOF                      ((uint32_t)0x00000004)
#define USB_SEP_STS_SCRXTO                      ((uint32_t)0x00000008)
#define USB_SEP_STS_SCNAKSENT                   ((uint32_t)0x00000010)
#define USB_SEP_STS_SCSTALLSENT                 ((uint32_t)0x00000020)
#define USB_SEP_STS_SCACKRXED                   ((uint32_t)0x00000040)
#define USB_SEP_STS_SCDATASEQ                   ((uint32_t)0x00000080)

/*-- USB_SC: USB_SC Register ---------------------------------------*/
/* Bit field positions: */
#define USB_SCGEN_Pos                           0
#define USB_SCTXLS_Pos                          1
#define USB_SCDC_Pos                            3
#define USB_SCFSP_Pos                           4
#define USB_SCFSR_Pos                           5

/* Bit field masks: */
#define USB_SCGEN                               ((uint32_t)0x00000001)
#define USB_SCTXLS_Msk                          ((uint32_t)0x00000006)
#define USB_SCDC                                ((uint32_t)0x00000008)
#define USB_SCFSP                               ((uint32_t)0x00000010)
#define USB_SCFSR                               ((uint32_t)0x00000020)

/*-- USB_SIS: USB_SIS Register -------------------------------------*/
/* Bit field positions: */
#define USB_SIS_SCTDONE_Pos                     0
#define USB_SIS_SCRESUME_Pos                    1
#define USB_SIS_SCRESETEV_Pos                   2
#define USB_SIS_SCSOFREC_Pos                    3
#define USB_SIS_SCNAKSENT_Pos                   4

/* Bit field masks: */
#define USB_SIS_SCTDONE                         ((uint32_t)0x00000001)
#define USB_SIS_SCRESUME                        ((uint32_t)0x00000002)
#define USB_SIS_SCRESETEV                       ((uint32_t)0x00000004)
#define USB_SIS_SCSOFREC                        ((uint32_t)0x00000008)
#define USB_SIS_SCNAKSENT                       ((uint32_t)0x00000010)

/*-- USB_SIM: USB_SIM Register -------------------------------------*/
/* Bit field positions: */
#define USB_SIM_SCTDONEIE_Pos                   0
#define USB_SIM_SCRESUMEIE_Pos                  1
#define USB_SIM_SCRESETEVIE_Pos                 2
#define USB_SIM_SCSOFRECIE_Pos                  3
#define USB_SIM_SCNAKSENTIE_Pos                 4

/* Bit field masks: */
#define USB_SIM_SCTDONEIE                       ((uint32_t)0x00000001)
#define USB_SIM_SCRESUMEIE                      ((uint32_t)0x00000002)
#define USB_SIM_SCRESETEVIE                     ((uint32_t)0x00000004)
#define USB_SIM_SCSOFRECIE                      ((uint32_t)0x00000008)
#define USB_SIM_SCNAKSENTIE                     ((uint32_t)0x00000010)

/*-- USB_HSCR: USB_HSCR Register -----------------------------------*/
/* Bit field positions: */
#define USB_HSCR_HOST_MODE_Pos                  0
#define USB_HSCR_RESET_CORE_Pos                 1
#define USB_HSCR_EN_TX_Pos                      2
#define USB_HSCR_EN_RX_Pos                      3
#define USB_HSCR_DP_PULLUP_Pos                  4
#define USB_HSCR_DP_PULLDOWN_Pos                5
#define USB_HSCR_DM_PULLUP_Pos                  6
#define USB_HSCR_DM_PULLDOWN_Pos                7

/* Bit field masks: */
#define USB_HSCR_HOST_MODE                      ((uint32_t)0x00000001)
#define USB_HSCR_RESET_CORE                     ((uint32_t)0x00000002)
#define USB_HSCR_EN_TX                          ((uint32_t)0x00000004)
#define USB_HSCR_EN_RX                          ((uint32_t)0x00000008)
#define USB_HSCR_DP_PULLUP                      ((uint32_t)0x00000010)
#define USB_HSCR_DP_PULLDOWN                    ((uint32_t)0x00000020)
#define USB_HSCR_DM_PULLUP                      ((uint32_t)0x00000040)
#define USB_HSCR_DM_PULLDOWN                    ((uint32_t)0x00000080)

/*-- USB_HSVR: USB_HSVR Register -----------------------------------*/
/* Bit field positions: */
#define USB_HSVR_VERSION_Pos                    0
#define USB_HSVR_REVISION_Pos                   4

/* Bit field masks: */
#define USB_HSVR_VERSION_Msk                    ((uint32_t)0x0000000F)
#define USB_HSVR_REVISION_Msk                   ((uint32_t)0x000000F0)

/** @} */ /* End of group MDR32Fx_Periph_USB_Defines */

/** @} */ /* End of group MDR32Fx_Periph_USB */

/** @defgroup MDR32Fx_Periph_EEPROM EEPROM
  * @{
  */

/** @defgroup MDR32Fx_Periph_EEPROM_Data_Structures Data Structures
  * @{
  */

/* MDR_EEPROM_TypeDef structure */
typedef struct
{
  __IO uint32_t CMD;
  __IO uint32_t ADR;
  __IO uint32_t DI;
  __IO uint32_t DO;
  __IO uint32_t KEY;
}MDR_EEPROM_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_EEPROM_Data_Structures */

/** @defgroup MDR32Fx_Periph_EEPROM_Defines Defines
  * @{
  */

/*-- EEPROM_CMD: EEPROM Command Register ---------------------------*/
/* Bit field positions: */
#define EEPROM_CMD_CON_Pos                      0
#define EEPROM_CMD_WR_Pos                       1
#define EEPROM_CMD_RD_Pos                       2
#define EEPROM_CMD_DELAY_Pos                    3
#define EEPROM_CMD_XE_Pos                       6
#define EEPROM_CMD_YE_Pos                       7
#define EEPROM_CMD_SE_Pos                       8
#define EEPROM_CMD_IFREN_Pos                    9
#define EEPROM_CMD_ERASE_Pos                    10
#define EEPROM_CMD_MAS1_Pos                     11
#define EEPROM_CMD_PROG_Pos                     12
#define EEPROM_CMD_NVSTR_Pos                    13

/* Bit field masks: */
#define EEPROM_CMD_CON                          ((uint32_t)0x00000001)
#define EEPROM_CMD_WR                           ((uint32_t)0x00000002)
#define EEPROM_CMD_RD                           ((uint32_t)0x00000004)
#define EEPROM_CMD_DELAY_Msk                    ((uint32_t)0x00000038)
#define EEPROM_CMD_XE                           ((uint32_t)0x00000040)
#define EEPROM_CMD_YE                           ((uint32_t)0x00000080)
#define EEPROM_CMD_SE                           ((uint32_t)0x00000100)
#define EEPROM_CMD_IFREN                        ((uint32_t)0x00000200)
#define EEPROM_CMD_ERASE                        ((uint32_t)0x00000400)
#define EEPROM_CMD_MAS1                         ((uint32_t)0x00000800)
#define EEPROM_CMD_PROG                         ((uint32_t)0x00001000)
#define EEPROM_CMD_NVSTR                        ((uint32_t)0x00002000)

/** @} */ /* End of group MDR32Fx_Periph_EEPROM_Defines */

/** @} */ /* End of group MDR32Fx_Periph_EEPROM */

/** @defgroup MDR32Fx_Periph_RST_CLK RST_CLK
  * @{
  */

/** @defgroup MDR32Fx_Periph_RST_CLK_Data_Structures Data Structures
  * @{
  */

/* MDR_RST_CLK_TypeDef structure */
typedef struct
{
  __IO uint32_t CLOCK_STATUS;
  __IO uint32_t PLL_CONTROL;
  __IO uint32_t HS_CONTROL;
  __IO uint32_t CPU_CLOCK;
  __IO uint32_t USB_CLOCK;
  __IO uint32_t ADC_MCO_CLOCK;
  __IO uint32_t RTC_CLOCK;
  __IO uint32_t PER_CLOCK;
  __IO uint32_t CAN_CLOCK;
  __IO uint32_t TIM_CLOCK;
  __IO uint32_t UART_CLOCK;
  __IO uint32_t SSP_CLOCK;
}MDR_RST_CLK_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_RST_CLK_Data_Structures */

/** @defgroup MDR32Fx_Periph_RST_CLK_Defines Defines
  * @{
  */

/*-- RST_CLK_CLOCK_STATUS: Clock Status Register -------------------*/
/* Bit field positions: */
#define RST_CLK_CLOCK_STATUS_PLL_USB_RDY_Pos    0
#define RST_CLK_CLOCK_STATUS_PLL_CPU_RDY_Pos    1
#define RST_CLK_CLOCK_STATUS_HSE_RDY_Pos        2

/* Bit field masks: */
#define RST_CLK_CLOCK_STATUS_PLL_USB_RDY        ((uint32_t)0x00000001)
#define RST_CLK_CLOCK_STATUS_PLL_CPU_RDY        ((uint32_t)0x00000002)
#define RST_CLK_CLOCK_STATUS_HSE_RDY            ((uint32_t)0x00000004)

/*-- RST_CLK_PLL_CONTROL: PLL Control Register ---------------------*/
/* Bit field positions: */
#define RST_CLK_PLL_CONTROL_PLL_USB_ON_Pos      0
#define RST_CLK_PLL_CONTROL_PLL_USB_RLD_Pos     1
#define RST_CLK_PLL_CONTROL_PLL_CPU_ON_Pos      2
#define RST_CLK_PLL_CONTROL_PLL_CPU_PLD_Pos     3
#define RST_CLK_PLL_CONTROL_PLL_USB_MUL_Pos     4
#define RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Pos     8

/* Bit field masks: */
#define RST_CLK_PLL_CONTROL_PLL_USB_ON          ((uint32_t)0x00000001)
#define RST_CLK_PLL_CONTROL_PLL_USB_RLD         ((uint32_t)0x00000002)
#define RST_CLK_PLL_CONTROL_PLL_CPU_ON          ((uint32_t)0x00000004)
#define RST_CLK_PLL_CONTROL_PLL_CPU_PLD         ((uint32_t)0x00000008)
#define RST_CLK_PLL_CONTROL_PLL_USB_MUL_Msk     ((uint32_t)0x000000F0)
#define RST_CLK_PLL_CONTROL_PLL_CPU_MUL_Msk     ((uint32_t)0x00000F00)

/*-- RST_CLK_HS_CONTROL: HS Control Register -----------------------*/
/* Bit field positions: */
#define RST_CLK_HS_CONTROL_HSE_ON_Pos           0
#define RST_CLK_HS_CONTROL_HSE_BYP_Pos          1

/* Bit field masks: */
#define RST_CLK_HS_CONTROL_HSE_ON               ((uint32_t)0x00000001)
#define RST_CLK_HS_CONTROL_HSE_BYP              ((uint32_t)0x00000002)

/*-- RST_CLK_CPU_CLOCK: CPU Clock Register -------------------------*/
/* Bit field positions: */
#define RST_CLK_CPU_CLOCK_CPU_C1_SEL_Pos        0
#define RST_CLK_CPU_CLOCK_CPU_C2_SEL_Pos        2
#define RST_CLK_CPU_CLOCK_CPU_C3_SEL_Pos        4
#define RST_CLK_CPU_CLOCK_HCLK_SEL_Pos          8

/* Bit field masks: */
#define RST_CLK_CPU_CLOCK_CPU_C1_SEL_Msk        ((uint32_t)0x00000003)
#define RST_CLK_CPU_CLOCK_CPU_C2_SEL            ((uint32_t)0x00000004)
#define RST_CLK_CPU_CLOCK_CPU_C3_SEL_Msk        ((uint32_t)0x000000F0)
#define RST_CLK_CPU_CLOCK_HCLK_SEL_Msk          ((uint32_t)0x00000300)

/*-- RST_CLK_USB_CLOCK: USB Clock Register -------------------------*/
/* Bit field positions: */
#define RST_CLK_USB_CLOCK_USB_C1_SEL_Pos        0
#define RST_CLK_USB_CLOCK_USB_C2_SEL_Pos        2
#define RST_CLK_USB_CLOCK_CPU_C3_SEL_Pos        4
#define RST_CLK_USB_CLOCK_USB_CLK_EN_Pos        8

/* Bit field masks: */
#define RST_CLK_USB_CLOCK_USB_C1_SEL_Msk        ((uint32_t)0x00000003)
#define RST_CLK_USB_CLOCK_USB_C2_SEL            ((uint32_t)0x00000004)
#define RST_CLK_USB_CLOCK_CPU_C3_SEL_Msk        ((uint32_t)0x000000F0)
#define RST_CLK_USB_CLOCK_USB_CLK_EN            ((uint32_t)0x00000100)

/*-- RST_CLK_ADC_MCO_CLOCK: ADC Clock Register ---------------------*/
/* Bit field positions: */
#define RST_CLK_ADC_MCO_CLOCK_ADC_C1_SEL_Pos    0
#define RST_CLK_ADC_MCO_CLOCK_ADC_C2_SEL_Pos    4
#define RST_CLK_ADC_MCO_CLOCK_ADC_C3_SEL_Pos    8
#define RST_CLK_ADC_MCO_CLOCK_MCO_EN_Pos        12
#define RST_CLK_ADC_MCO_CLOCK_ADC_CLK_EN_Pos    13

/* Bit field masks: */
#define RST_CLK_ADC_MCO_CLOCK_ADC_C1_SEL_Msk    ((uint32_t)0x00000003)
#define RST_CLK_ADC_MCO_CLOCK_ADC_C2_SEL_Msk    ((uint32_t)0x00000030)
#define RST_CLK_ADC_MCO_CLOCK_ADC_C3_SEL_Msk    ((uint32_t)0x00000F00)
#define RST_CLK_ADC_MCO_CLOCK_MCO_EN            ((uint32_t)0x00001000)
#define RST_CLK_ADC_MCO_CLOCK_ADC_CLK_EN        ((uint32_t)0x00002000)

/*-- RST_CLK_RTC_CLOCK: RTC Clock Register -------------------------*/
/* Bit field positions: */
#define RST_CLK_RTC_CLOCK_HSE_SEL_Pos           0
#define RST_CLK_RTC_CLOCK_HSI_SEL_Pos           4
#define RST_CLK_RTC_CLOCK_HSE_RTC_EN_Pos        8
#define RST_CLK_RTC_CLOCK_HSI_RTC_EN_Pos        9

/* Bit field masks: */
#define RST_CLK_RTC_CLOCK_HSE_SEL_Msk           ((uint32_t)0x0000000F)
#define RST_CLK_RTC_CLOCK_HSI_SEL_Msk           ((uint32_t)0x000000F0)
#define RST_CLK_RTC_CLOCK_HSE_RTC_EN            ((uint32_t)0x00000100)
#define RST_CLK_RTC_CLOCK_HSI_RTC_EN            ((uint32_t)0x00000200)

/*-- RST_CLK_CAN_CLOCK: CAN Clock Register -------------------------*/
/* Bit field positions: */
#define RST_CLK_CAN_CLOCK_CAN1_BRG_Pos          0
#define RST_CLK_CAN_CLOCK_CAN2_BRG_Pos          8
#define RST_CLK_CAN_CLOCK_CAN1_CLK_EN_Pos       24
#define RST_CLK_CAN_CLOCK_CAN2_CLK_EN_Pos       25

/* Bit field masks: */
#define RST_CLK_CAN_CLOCK_CAN1_BRG_Msk          ((uint32_t)0x000000FF)
#define RST_CLK_CAN_CLOCK_CAN2_BRG_Msk          ((uint32_t)0x0000FF00)
#define RST_CLK_CAN_CLOCK_CAN1_CLK_EN           ((uint32_t)0x01000000)
#define RST_CLK_CAN_CLOCK_CAN2_CLK_EN           ((uint32_t)0x02000000)

/*-- RST_CLK_TIM_CLOCK: Timer Clock Register -----------------------*/
/* Bit field positions: */
#define RST_CLK_TIM_CLOCK_TIM1_BRG_Pos          0
#define RST_CLK_TIM_CLOCK_TIM2_BRG_Pos          8
#define RST_CLK_TIM_CLOCK_TIM3_BRG_Pos          16
#define RST_CLK_TIM_CLOCK_TIM1_CLK_EN_Pos       24
#define RST_CLK_TIM_CLOCK_TIM2_CLK_EN_Pos       25
#define RST_CLK_TIM_CLOCK_TIM3_CLK_EN_Pos       26

/* Bit field masks: */
#define RST_CLK_TIM_CLOCK_TIM1_BRG_Msk          ((uint32_t)0x000000FF)
#define RST_CLK_TIM_CLOCK_TIM2_BRG_Msk          ((uint32_t)0x0000FF00)
#define RST_CLK_TIM_CLOCK_TIM3_BRG_Msk          ((uint32_t)0x00FF0000)
#define RST_CLK_TIM_CLOCK_TIM1_CLK_EN           ((uint32_t)0x01000000)
#define RST_CLK_TIM_CLOCK_TIM2_CLK_EN           ((uint32_t)0x02000000)
#define RST_CLK_TIM_CLOCK_TIM3_CLK_EN           ((uint32_t)0x04000000)

/*-- RST_CLK_UART_CLOCK: UART Clock Register -----------------------*/
/* Bit field positions: */
#define RST_CLK_UART_CLOCK_UART1_BRG_Pos        0
#define RST_CLK_UART_CLOCK_UART2_BRG_Pos        8
#define RST_CLK_UART_CLOCK_UART1_CLK_EN_Pos     24
#define RST_CLK_UART_CLOCK_UART2_CLK_EN_Pos     25

/* Bit field masks: */
#define RST_CLK_UART_CLOCK_UART1_BRG_Msk        ((uint32_t)0x000000FF)
#define RST_CLK_UART_CLOCK_UART2_BRG_Msk        ((uint32_t)0x0000FF00)
#define RST_CLK_UART_CLOCK_UART1_CLK_EN         ((uint32_t)0x01000000)
#define RST_CLK_UART_CLOCK_UART2_CLK_EN         ((uint32_t)0x02000000)

/*-- RST_CLK_SSP_CLOCK: SSP Clock Register -------------------------*/
/* Bit field positions: */
#define RST_CLK_SSP_CLOCK_SSP1_BRG_Pos          0
#define RST_CLK_SSP_CLOCK_SSP2_BRG_Pos          8
#define RST_CLK_SSP_CLOCK_SSP1_CLK_EN_Pos       24
#define RST_CLK_SSP_CLOCK_SSP2_CLK_EN_Pos       25

/* Bit field masks: */
#define RST_CLK_SSP_CLOCK_SSP1_BRG_Msk          ((uint32_t)0x000000FF)
#define RST_CLK_SSP_CLOCK_SSP2_BRG_Msk          ((uint32_t)0x0000FF00)
#define RST_CLK_SSP_CLOCK_SSP1_CLK_EN           ((uint32_t)0x01000000)
#define RST_CLK_SSP_CLOCK_SSP2_CLK_EN           ((uint32_t)0x02000000)

/** @} */ /* End of group MDR32Fx_Periph_RST_CLK_Defines */

/** @} */ /* End of group MDR32Fx_Periph_RST_CLK */

/** @defgroup MDR32Fx_Periph_DMA DMA
  * @{
  */

/** @defgroup MDR32Fx_Periph_DMA_Data_Structures Data Structures
  * @{
  */

/* MDR_DMA_TypeDef structure */
typedef struct
{
  __IO uint32_t STATUS;
  __IO uint32_t CFG;
  __IO uint32_t CTRL_BASE_PTR;
  __IO uint32_t ALT_CTRL_BASE_PTR;
  __IO uint32_t WAITONREQ_STATUS;
  __IO uint32_t CHNL_SW_REQUEST;
  __IO uint32_t CHNL_USEBURST_SET;
  __IO uint32_t CHNL_USEBURST_CLR;
  __IO uint32_t CHNL_REQ_MASK_SET;
  __IO uint32_t CHNL_REQ_MASK_CLR;
  __IO uint32_t CHNL_ENABLE_SET;
  __IO uint32_t CHNL_ENABLE_CLR;
  __IO uint32_t CHNL_PRI_ALT_SET;
  __IO uint32_t CHNL_PRI_ALT_CLR;
  __IO uint32_t CHNL_PRIORITY_SET;
  __IO uint32_t CHNL_PRIORITY_CLR;
       uint32_t RESERVED0[3];
  __IO uint32_t ERR_CLR;
}MDR_DMA_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_DMA_Data_Structures */

/** @defgroup MDR32Fx_Periph_DMA_Defines Defines
  * @{
  */

/*-- DMA_STATUS: DMA Status Register -------------------------------*/
/* Bit field positions: */
#define DMA_STATUS_MASTER_ENABLE_Pos            0
#define DMA_STATUS_STATE_Pos                    4
#define DMA_STATUS_CHNLS_MINUS1_Pos             16
#define DMA_STATUS_TEST_STATUS_Pos              28

/* Bit field masks: */
#define DMA_STATUS_MASTER_ENABLE                ((uint32_t)0x00000001)
#define DMA_STATUS_STATE_Msk                    ((uint32_t)0x000000F0)
#define DMA_STATUS_CHNLS_MINUS1_Msk             ((uint32_t)0x001F0000)
#define DMA_STATUS_TEST_STATUS_Msk              ((uint32_t)0xF0000000)

/*-- DMA_CFG: DMA Configuration Register ---------------------------*/
/* Bit field positions: */
#define DMA_CFG_MASTER_ENABLE_Pos               0
#define DMA_CFG_CHNL_PROT_CTRL_Pos              5

/* Bit field masks: */
#define DMA_CFG_MASTER_ENABLE                   ((uint32_t)0x00000001)
#define DMA_CFG_CHNL_PROT_CTRL_Msk              ((uint32_t)0x000000E0)

/** @} */ /* End of group MDR32Fx_Periph_DMA_Defines */

/** @} */ /* End of group MDR32Fx_Periph_DMA */

/** @defgroup MDR32Fx_Periph_UART UART
  * @{
  */

/** @defgroup MDR32Fx_Periph_UART_Data_Structures Data Structures
  * @{
  */

/* MDR_UART_TypeDef structure */
typedef struct
{
  __IO uint32_t DR;
  __IO uint32_t RSR_ECR;
       uint32_t RESERVED0[4];
  __IO uint32_t FR;
       uint32_t RESERVED1;
  __IO uint32_t ILPR;
  __IO uint32_t IBRD;
  __IO uint32_t FBRD;
  __IO uint32_t LCR_H;
  __IO uint32_t CR;
  __IO uint32_t IFLS;
  __IO uint32_t IMSC;
  __IO uint32_t RIS;
  __IO uint32_t MIS;
  __IO uint32_t ICR;
  __IO uint32_t DMACR;
}MDR_UART_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_UART_Data_Structures */

/** @defgroup MDR32Fx_Periph_UART_Defines Defines
  * @{
  */

/*-- UART_DR: UART Data Register -----------------------------------*/
/* Bit field positions: */
#define UART_DR_DATA_Pos                        0
#define UART_DR_FE_Pos                          8
#define UART_DR_PE_Pos                          9
#define UART_DR_BE_Pos                          10
#define UART_DR_OE_Pos                          11

/* Bit field masks: */
#define UART_DR_DATA_Msk                        ((uint32_t)0x000000FF)
#define UART_DR_FE                              ((uint32_t)0x00000100)
#define UART_DR_PE                              ((uint32_t)0x00000200)
#define UART_DR_BE                              ((uint32_t)0x00000400)
#define UART_DR_OE                              ((uint32_t)0x00000800)

/*-- UART_RSR_ECR: UART RSR Register -------------------------------*/
/* Bit field positions: */
#define UART_RSR_ECR_FE_Pos                     0
#define UART_RSR_ECR_PE_Pos                     1
#define UART_RSR_ECR_BE_Pos                     2
#define UART_RSR_ECR_OE_Pos                     3

/* Bit field masks: */
#define UART_RSR_ECR_FE                         ((uint32_t)0x00000001)
#define UART_RSR_ECR_PE                         ((uint32_t)0x00000002)
#define UART_RSR_ECR_BE                         ((uint32_t)0x00000004)
#define UART_RSR_ECR_OE                         ((uint32_t)0x00000008)

/*-- UART_FR: UART Flag Register -----------------------------------*/
/* Bit field positions: */
#define UART_FR_CTS_Pos                         0
#define UART_FR_DSR_Pos                         1
#define UART_FR_DCD_Pos                         2
#define UART_FR_BUSY_Pos                        3
#define UART_FR_RXFE_Pos                        4
#define UART_FR_TXFF_Pos                        5
#define UART_FR_RXFF_Pos                        6
#define UART_FR_TXFE_Pos                        7
#define UART_FR_RI_Pos                          8

/* Bit field masks: */
#define UART_FR_CTS                             ((uint32_t)0x00000001)
#define UART_FR_DSR                             ((uint32_t)0x00000002)
#define UART_FR_DCD                             ((uint32_t)0x00000004)
#define UART_FR_BUSY                            ((uint32_t)0x00000008)
#define UART_FR_RXFE                            ((uint32_t)0x00000010)
#define UART_FR_TXFF                            ((uint32_t)0x00000020)
#define UART_FR_RXFF                            ((uint32_t)0x00000040)
#define UART_FR_TXFE                            ((uint32_t)0x00000080)
#define UART_FR_RI                              ((uint32_t)0x00000100)

/*-- UART_LCR_H: UART LCR_H Register -------------------------------*/
/* Bit field positions: */
#define UART_LCR_H_BRK_Pos                      0
#define UART_LCR_H_PEN_Pos                      1
#define UART_LCR_H_EPS_Pos                      2
#define UART_LCR_H_STP2_Pos                     3
#define UART_LCR_H_FEN_Pos                      4
#define UART_LCR_H_WLEN_Pos                     5
#define UART_LCR_H_SPS_Pos                      7

/* Bit field masks: */
#define UART_LCR_H_BRK                          ((uint32_t)0x00000001)
#define UART_LCR_H_PEN                          ((uint32_t)0x00000002)
#define UART_LCR_H_EPS                          ((uint32_t)0x00000004)
#define UART_LCR_H_STP2                         ((uint32_t)0x00000008)
#define UART_LCR_H_FEN                          ((uint32_t)0x00000010)
#define UART_LCR_H_WLEN_Msk                     ((uint32_t)0x00000060)
#define UART_LCR_H_SPS                          ((uint32_t)0x00000080)

/*-- UART_CR: UART Command Register --------------------------------*/
/* Bit field positions: */
#define UART_CR_UARTEN_Pos                      0
#define UART_CR_SIREN_Pos                       1
#define UART_CR_SIRLP_Pos                       2
#define UART_CR_LBE_Pos                         7
#define UART_CR_TXE_Pos                         8
#define UART_CR_RXE_Pos                         9
#define UART_CR_DTR_Pos                         10
#define UART_CR_RTS_Pos                         11
#define UART_CR_OUT1_Pos                        12
#define UART_CR_OUT2_Pos                        13
#define UART_CR_RTSEN_Pos                       14
#define UART_CR_CTSEN_Pos                       15

/* Bit field masks: */
#define UART_CR_UARTEN                          ((uint32_t)0x00000001)
#define UART_CR_SIREN                           ((uint32_t)0x00000002)
#define UART_CR_SIRLP                           ((uint32_t)0x00000004)
#define UART_CR_LBE                             ((uint32_t)0x00000080)
#define UART_CR_TXE                             ((uint32_t)0x00000100)
#define UART_CR_RXE                             ((uint32_t)0x00000200)
#define UART_CR_DTR                             ((uint32_t)0x00000400)
#define UART_CR_RTS                             ((uint32_t)0x00000800)
#define UART_CR_OUT1                            ((uint32_t)0x00001000)
#define UART_CR_OUT2                            ((uint32_t)0x00002000)
#define UART_CR_RTSEN                           ((uint32_t)0x00004000)
#define UART_CR_CTSEN                           ((uint32_t)0x00008000)

/*-- UART_IFLS: UART IFLS Register ---------------------------------*/
/* Bit field positions: */
#define UART_IFLS_TXIFLSEL_Pos                  0
#define UART_IFLS_RXIFLSEL_Pos                  3

/* Bit field masks: */
#define UART_IFLS_TXIFLSEL_Msk                  ((uint32_t)0x00000007)
#define UART_IFLS_RXIFLSEL_Msk                  ((uint32_t)0x00000038)

/*-- UART_IMSC: UART Interrupt Mask Register -----------------------*/
/* Bit field positions: */
#define UART_IMSC_RIMIM_Pos                     0
#define UART_IMSC_CTSMIM_Pos                    1
#define UART_IMSC_DCDMIM_Pos                    2
#define UART_IMSC_DSRMIM_Pos                    3
#define UART_IMSC_RXIM_Pos                      4
#define UART_IMSC_TXIM_Pos                      5
#define UART_IMSC_RTIM_Pos                      6
#define UART_IMSC_FEIM_Pos                      7
#define UART_IMSC_PEIM_Pos                      8
#define UART_IMSC_BEIM_Pos                      9
#define UART_IMSC_OEIM_Pos                      10

/* Bit field masks: */
#define UART_IMSC_RIMIM                         ((uint32_t)0x00000001)
#define UART_IMSC_CTSMIM                        ((uint32_t)0x00000002)
#define UART_IMSC_DCDMIM                        ((uint32_t)0x00000004)
#define UART_IMSC_DSRMIM                        ((uint32_t)0x00000008)
#define UART_IMSC_RXIM                          ((uint32_t)0x00000010)
#define UART_IMSC_TXIM                          ((uint32_t)0x00000020)
#define UART_IMSC_RTIM                          ((uint32_t)0x00000040)
#define UART_IMSC_FEIM                          ((uint32_t)0x00000080)
#define UART_IMSC_PEIM                          ((uint32_t)0x00000100)
#define UART_IMSC_BEIM                          ((uint32_t)0x00000200)
#define UART_IMSC_OEIM                          ((uint32_t)0x00000400)

/*-- UART_RIS: UART Interrupt Pending Register ---------------------*/
/* Bit field positions: */
#define UART_RIS_RIRMIS_Pos                     0
#define UART_RIS_CTSRMIS_Pos                    1
#define UART_RIS_DCDRMIS_Pos                    2
#define UART_RIS_DSRRMIS_Pos                    3
#define UART_RIS_RXRIS_Pos                      4
#define UART_RIS_TXRIS_Pos                      5
#define UART_RIS_RTRIS_Pos                      6
#define UART_RIS_FERIS_Pos                      7
#define UART_RIS_PERIS_Pos                      8
#define UART_RIS_BERIS_Pos                      9
#define UART_RIS_OERIS_Pos                      10

/* Bit field masks: */
#define UART_RIS_RIRMIS                         ((uint32_t)0x00000001)
#define UART_RIS_CTSRMIS                        ((uint32_t)0x00000002)
#define UART_RIS_DCDRMIS                        ((uint32_t)0x00000004)
#define UART_RIS_DSRRMIS                        ((uint32_t)0x00000008)
#define UART_RIS_RXRIS                          ((uint32_t)0x00000010)
#define UART_RIS_TXRIS                          ((uint32_t)0x00000020)
#define UART_RIS_RTRIS                          ((uint32_t)0x00000040)
#define UART_RIS_FERIS                          ((uint32_t)0x00000080)
#define UART_RIS_PERIS                          ((uint32_t)0x00000100)
#define UART_RIS_BERIS                          ((uint32_t)0x00000200)
#define UART_RIS_OERIS                          ((uint32_t)0x00000400)

/*-- UART_MIS: UART Masked Interrupt Pending Register --------------*/
/* Bit field positions: */
#define UART_MIS_RIMMIS_Pos                     0
#define UART_MIS_CTSMMIS_Pos                    1
#define UART_MIS_DCDMMIS_Pos                    2
#define UART_MIS_DSRMMIS_Pos                    3
#define UART_MIS_RXMIS_Pos                      4
#define UART_MIS_TXMIS_Pos                      5
#define UART_MIS_RTMIS_Pos                      6
#define UART_MIS_FEMIS_Pos                      7
#define UART_MIS_PEMIS_Pos                      8
#define UART_MIS_BEMIS_Pos                      9
#define UART_MIS_OEMIS_Pos                      10

/* Bit field masks: */
#define UART_MIS_RIMMIS                         ((uint32_t)0x00000001)
#define UART_MIS_CTSMMIS                        ((uint32_t)0x00000002)
#define UART_MIS_DCDMMIS                        ((uint32_t)0x00000004)
#define UART_MIS_DSRMMIS                        ((uint32_t)0x00000008)
#define UART_MIS_RXMIS                          ((uint32_t)0x00000010)
#define UART_MIS_TXMIS                          ((uint32_t)0x00000020)
#define UART_MIS_RTMIS                          ((uint32_t)0x00000040)
#define UART_MIS_FEMIS                          ((uint32_t)0x00000080)
#define UART_MIS_PEMIS                          ((uint32_t)0x00000100)
#define UART_MIS_BEMIS                          ((uint32_t)0x00000200)
#define UART_MIS_OEMIS                          ((uint32_t)0x00000400)

/*-- UART_ICR: UART Interrupt Clear Register -----------------------*/
/* Bit field positions: */
#define UART_ICR_RIMIC_Pos                      0
#define UART_ICR_CTSMIC_Pos                     1
#define UART_ICR_DCDMIC_Pos                     2
#define UART_ICR_DSRMIC_Pos                     3
#define UART_ICR_RXIC_Pos                       4
#define UART_ICR_TXIC_Pos                       5
#define UART_ICR_RTIC_Pos                       6
#define UART_ICR_FEIC_Pos                       7
#define UART_ICR_PEIC_Pos                       8
#define UART_ICR_BEIC_Pos                       9
#define UART_ICR_OEIC_Pos                       10

/* Bit field masks: */
#define UART_ICR_RIMIC                          ((uint32_t)0x00000001)
#define UART_ICR_CTSMIC                         ((uint32_t)0x00000002)
#define UART_ICR_DCDMIC                         ((uint32_t)0x00000004)
#define UART_ICR_DSRMIC                         ((uint32_t)0x00000008)
#define UART_ICR_RXIC                           ((uint32_t)0x00000010)
#define UART_ICR_TXIC                           ((uint32_t)0x00000020)
#define UART_ICR_RTIC                           ((uint32_t)0x00000040)
#define UART_ICR_FEIC                           ((uint32_t)0x00000080)
#define UART_ICR_PEIC                           ((uint32_t)0x00000100)
#define UART_ICR_BEIC                           ((uint32_t)0x00000200)
#define UART_ICR_OEIC                           ((uint32_t)0x00000400)

/*-- UART_DMACR: UART DMA Control Register -------------------------*/
/* Bit field positions: */
#define UART_DMACR_RXDMAE_Pos                   0
#define UART_DMACR_TXDMAE_Pos                   1
#define UART_DMACR_DMAONERR_Pos                 2

/* Bit field masks: */
#define UART_DMACR_RXDMAE                       ((uint32_t)0x00000001)
#define UART_DMACR_TXDMAE                       ((uint32_t)0x00000002)
#define UART_DMACR_DMAONERR                     ((uint32_t)0x00000004)

/** @} */ /* End of group MDR32Fx_Periph_UART_Defines */

/** @} */ /* End of group MDR32Fx_Periph_UART */

/** @defgroup MDR32Fx_Periph_SSP SSP
  * @{
  */

/** @defgroup MDR32Fx_Periph_SSP_Data_Structures Data Structures
  * @{
  */

/* MDR_SSP_TypeDef structure */
typedef struct
{
  __IO uint32_t CR0;
  __IO uint32_t CR1;
  __IO uint32_t DR;
  __IO uint32_t SR;
  __IO uint32_t CPSR;
  __IO uint32_t IMSC;
  __IO uint32_t RIS;
  __IO uint32_t MIS;
  __IO uint32_t ICR;
  __IO uint32_t DMACR;
}MDR_SSP_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_SSP_Data_Structures */

/** @defgroup MDR32Fx_Periph_SSP_Defines Defines
  * @{
  */

/*-- SSP_CR0: SSP Control0 Register --------------------------------*/
/* Bit field positions: */
#define SSP_CR0_DSS_Pos                         0
#define SSP_CR0_FRF_Pos                         4
#define SSP_CR0_SPO_Pos                         6
#define SSP_CR0_SPH_Pos                         7
#define SSP_CR0_SCR_Pos                         8

/* Bit field masks: */
#define SSP_CR0_DSS_Msk                         ((uint32_t)0x0000000F)
#define SSP_CR0_FRF_Msk                         ((uint32_t)0x00000030)
#define SSP_CR0_SPO                             ((uint32_t)0x00000040)
#define SSP_CR0_SPH                             ((uint32_t)0x00000080)
#define SSP_CR0_SCR_Msk                         ((uint32_t)0x0000FF00)

/*-- SSP_CR1: SSP Control1 Register --------------------------------*/
/* Bit field positions: */
#define SSP_CR1_LBM_Pos                         0
#define SSP_CR1_SSE_Pos                         1
#define SSP_CR1_MS_Pos                          2
#define SSP_CR1_SOD_Pos                         3

/* Bit field masks: */
#define SSP_CR1_LBM                             ((uint32_t)0x00000001)
#define SSP_CR1_SSE                             ((uint32_t)0x00000002)
#define SSP_CR1_MS                              ((uint32_t)0x00000004)
#define SSP_CR1_SOD                             ((uint32_t)0x00000008)

/*-- SSP_SR: SSP Status Register -----------------------------------*/
/* Bit field positions: */
#define SSP_SR_TFE_Pos                          0
#define SSP_SR_TNF_Pos                          1
#define SSP_SR_RNE_Pos                          2
#define SSP_SR_RFF_Pos                          3
#define SSP_SR_BSY_Pos                          4

/* Bit field masks: */
#define SSP_SR_TFE                              ((uint32_t)0x00000001)
#define SSP_SR_TNF                              ((uint32_t)0x00000002)
#define SSP_SR_RNE                              ((uint32_t)0x00000004)
#define SSP_SR_RFF                              ((uint32_t)0x00000008)
#define SSP_SR_BSY                              ((uint32_t)0x00000010)

/*-- SSP_IMSC: SSP Interrupt Mask Register -------------------------*/
/* Bit field positions: */
#define SSP_IMSC_RORIM_Pos                      0
#define SSP_IMSC_RTIM_Pos                       1
#define SSP_IMSC_RXIM_Pos                       2
#define SSP_IMSC_TXIM_Pos                       3

/* Bit field masks: */
#define SSP_IMSC_RORIM                          ((uint32_t)0x00000001)
#define SSP_IMSC_RTIM                           ((uint32_t)0x00000002)
#define SSP_IMSC_RXIM                           ((uint32_t)0x00000004)
#define SSP_IMSC_TXIM                           ((uint32_t)0x00000008)

/*-- SSP_RIS: SSP Interrupt Pending Register -----------------------*/
/* Bit field positions: */
#define SSP_RIS_RORRIS_Pos                      0
#define SSP_RIS_RTRIS_Pos                       1
#define SSP_RIS_RXRIS_Pos                       2
#define SSP_RIS_TXRIS_Pos                       3

/* Bit field masks: */
#define SSP_RIS_RORRIS                          ((uint32_t)0x00000001)
#define SSP_RIS_RTRIS                           ((uint32_t)0x00000002)
#define SSP_RIS_RXRIS                           ((uint32_t)0x00000004)
#define SSP_RIS_TXRIS                           ((uint32_t)0x00000008)

/*-- SSP_MIS: SSP Masked Interrupt Pending Register ----------------*/
/* Bit field positions: */
#define SSP_MIS_RORMIS_Pos                      0
#define SSP_MIS_RTMIS_Pos                       1
#define SSP_MIS_RXMIS_Pos                       2
#define SSP_MIS_TXMIS_Pos                       3

/* Bit field masks: */
#define SSP_MIS_RORMIS                          ((uint32_t)0x00000001)
#define SSP_MIS_RTMIS                           ((uint32_t)0x00000002)
#define SSP_MIS_RXMIS                           ((uint32_t)0x00000004)
#define SSP_MIS_TXMIS                           ((uint32_t)0x00000008)

/*-- SSP_ICR: SSP Interrupt Clear Register -------------------------*/
/* Bit field positions: */
#define SSP_ICR_RORIC_Pos                       0
#define SSP_ICR_RTIC_Pos                        1

/* Bit field masks: */
#define SSP_ICR_RORIC                           ((uint32_t)0x00000001)
#define SSP_ICR_RTIC                            ((uint32_t)0x00000002)

/*-- SSP_DMACR: SSP DMA Control Register ---------------------------*/
/* Bit field positions: */
#define SSP_DMACR_RXDMAE_Pos                    0
#define SSP_DMACR_TXDMAE_Pos                    1

/* Bit field masks: */
#define SSP_DMACR_RXDMAE                        ((uint32_t)0x00000001)
#define SSP_DMACR_TXDMAE                        ((uint32_t)0x00000002)

/** @} */ /* End of group MDR32Fx_Periph_SSP_Defines */

/** @} */ /* End of group MDR32Fx_Periph_SSP */

/** @defgroup MDR32Fx_Periph_I2C I2C
  * @{
  */

/** @defgroup MDR32Fx_Periph_I2C_Data_Structures Data Structures
  * @{
  */

/* MDR_I2C_TypeDef structure */
typedef struct
{
  __IO uint32_t PRL;
  __IO uint32_t PRH;
  __IO uint32_t CTR;
  __IO uint32_t RXD;
  __IO uint32_t STA;
  __IO uint32_t TXD;
  __IO uint32_t CMD;
}MDR_I2C_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_I2C_Data_Structures */

/** @defgroup MDR32Fx_Periph_I2C_Defines Defines
  * @{
  */

/*-- I2C_CTR: I2C Control Register ---------------------------------*/
/* Bit field positions: */
#define I2C_CTR_S_I2C_Pos                       5
#define I2C_CTR_EN_INT_Pos                      6
#define I2C_CTR_EN_I2C_Pos                      7

/* Bit field masks: */
#define I2C_CTR_S_I2C                           ((uint32_t)0x00000020)
#define I2C_CTR_EN_INT                          ((uint32_t)0x00000040)
#define I2C_CTR_EN_I2C                          ((uint32_t)0x00000080)

/*-- I2C_STA: I2C Status Register ----------------------------------*/
/* Bit field positions: */
#define I2C_STA_INT_Pos                         0
#define I2C_STA_TR_PROG_Pos                     1
#define I2C_STA_LOST_ARB_Pos                    5
#define I2C_STA_BUSY_Pos                        6
#define I2C_STA_RX_ACK_Pos                      7

/* Bit field masks: */
#define I2C_STA_INT                             ((uint32_t)0x00000001)
#define I2C_STA_TR_PROG                         ((uint32_t)0x00000002)
#define I2C_STA_LOST_ARB                        ((uint32_t)0x00000020)
#define I2C_STA_BUSY                            ((uint32_t)0x00000040)
#define I2C_STA_RX_ACK                          ((uint32_t)0x00000080)

/*-- I2C_CMD: I2C Command Register ---------------------------------*/
/* Bit field positions: */
#define I2C_CMD_CLRINT_Pos                      0
#define I2C_CMD_ACK_Pos                         3
#define I2C_CMD_WR_Pos                          4
#define I2C_CMD_RD_Pos                          5
#define I2C_CMD_STOP_Pos                        6
#define I2C_CMD_START_Pos                       7

/* Bit field masks: */
#define I2C_CMD_CLRINT                          ((uint32_t)0x00000001)
#define I2C_CMD_ACK                             ((uint32_t)0x00000008)
#define I2C_CMD_WR                              ((uint32_t)0x00000010)
#define I2C_CMD_RD                              ((uint32_t)0x00000020)
#define I2C_CMD_STOP                            ((uint32_t)0x00000040)
#define I2C_CMD_START                           ((uint32_t)0x00000080)

/** @} */ /* End of group MDR32Fx_Periph_I2C_Defines */

/** @} */ /* End of group MDR32Fx_Periph_I2C */

/** @defgroup MDR32Fx_Periph_POWER POWER
  * @{
  */

/** @defgroup MDR32Fx_Periph_POWER_Data_Structures Data Structures
  * @{
  */

/* MDR_POWER_TypeDef structure */
typedef struct
{
  __IO uint32_t PVDCS;
}MDR_POWER_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_POWER_Data_Structures */

/** @defgroup MDR32Fx_Periph_POWER_Defines Defines
  * @{
  */

/*-- POWER_PVDCS: POWER Power Detector Control/Status Register -----*/
/* Bit field positions: */
#define POWER_PVDCS_PVDEN_Pos                   0
#define POWER_PVDCS_PBLS_Pos                    1
#define POWER_PVDCS_PLS_Pos                     3
#define POWER_PVDCS_PVBD_Pos                    6
#define POWER_PVDCS_PVD_Pos                     7
#define POWER_PVDCS_IEPVBD_Pos                  8
#define POWER_PVDCS_IEPVD_Pos                   9
#define POWER_PVDCS_INVB_Pos                    10
#define POWER_PVDCS_INV_Pos                     11

/* Bit field masks: */
#define POWER_PVDCS_PVDEN                       ((uint32_t)0x00000001)
#define POWER_PVDCS_PBLS_Msk                    ((uint32_t)0x00000006)
#define POWER_PVDCS_PLS_Msk                     ((uint32_t)0x00000038)
#define POWER_PVDCS_PVBD                        ((uint32_t)0x00000040)
#define POWER_PVDCS_PVD                         ((uint32_t)0x00000080)
#define POWER_PVDCS_IEPVBD                      ((uint32_t)0x00000100)
#define POWER_PVDCS_IEPVD                       ((uint32_t)0x00000200)
#define POWER_PVDCS_INVB                        ((uint32_t)0x00000400)
#define POWER_PVDCS_INV                         ((uint32_t)0x00000800)

/** @} */ /* End of group MDR32Fx_Periph_POWER_Defines */

/** @} */ /* End of group MDR32Fx_Periph_POWER */

/** @defgroup MDR32Fx_Periph_WWDG WWDG
  * @{
  */

/** @defgroup MDR32Fx_Periph_WWDG_Data_Structures Data Structures
  * @{
  */

/* MDR_WWDG_TypeDef structure */
typedef struct
{
  __IO uint32_t CR;
  __IO uint32_t CFR;
  __IO uint32_t SR;
}MDR_WWDG_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_WWDG_Data_Structures */

/** @defgroup MDR32Fx_Periph_WWDG_Defines Defines
  * @{
  */

/*-- WWDG_CR: WWDG Command Register --------------------------------*/
/* Bit field positions: */
#define WWDG_CR_T_Pos                           0
#define WWDG_CR_WDGA_Pos                        7

/* Bit field masks: */
#define WWDG_CR_T_Msk                           ((uint32_t)0x0000007F)
#define WWDG_CR_WDGA                            ((uint32_t)0x00000080)

/*-- WWDG_CFR: WWDG Configuration Register -------------------------*/
/* Bit field positions: */
#define WWDG_CFR_W_Pos                          0
#define WWDG_CFR_WGTB_Pos                       7
#define WWDG_CFR_EWI_Pos                        9

/* Bit field masks: */
#define WWDG_CFR_W_Msk                          ((uint32_t)0x0000007F)
#define WWDG_CFR_WGTB_Msk                       ((uint32_t)0x00000180)
#define WWDG_CFR_EWI                            ((uint32_t)0x00000200)

/** @} */ /* End of group MDR32Fx_Periph_WWDG_Defines */

/** @} */ /* End of group MDR32Fx_Periph_WWDG */

/** @defgroup MDR32Fx_Periph_IWDG IWDG
  * @{
  */

/** @defgroup MDR32Fx_Periph_IWDG_Data_Structures Data Structures
  * @{
  */

/* MDR_IWDG_TypeDef structure */
typedef struct
{
  __IO uint32_t KR;
  __IO uint32_t PR;
  __IO uint32_t RLR;
  __IO uint32_t SR;
}MDR_IWDG_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_IWDG_Data_Structures */

/** @defgroup MDR32Fx_Periph_IWDG_Defines Defines
  * @{
  */

/*-- IWDG_SR: IWDG Status Register ---------------------------------*/
/* Bit field positions: */
#define IWDG_SR_PVU_Pos                         0
#define IWDG_SR_RVU_Pos                         1

/* Bit field masks: */
#define IWDG_SR_PVU                             ((uint32_t)0x00000001)
#define IWDG_SR_RVU                             ((uint32_t)0x00000002)

/** @} */ /* End of group MDR32Fx_Periph_IWDG_Defines */

/** @} */ /* End of group MDR32Fx_Periph_IWDG */

/** @defgroup MDR32Fx_Periph_TIMER TIMER
  * @{
  */

/** @defgroup MDR32Fx_Periph_TIMER_Data_Structures Data Structures
  * @{
  */

/* MDR_TIMER_TypeDef structure */
typedef struct
{
  __IO uint32_t CNT;
  __IO uint32_t PSG;
  __IO uint32_t ARR;
  __IO uint32_t CNTRL;
  __IO uint32_t CCR1;
  __IO uint32_t CCR2;
  __IO uint32_t CCR3;
  __IO uint32_t CCR4;
  __IO uint32_t CH1_CNTRL;
  __IO uint32_t CH2_CNTRL;
  __IO uint32_t CH3_CNTRL;
  __IO uint32_t CH4_CNTRL;
  __IO uint32_t CH1_CNTRL1;
  __IO uint32_t CH2_CNTRL1;
  __IO uint32_t CH3_CNTRL1;
  __IO uint32_t CH4_CNTRL1;
  __IO uint32_t CH1_DTG;
  __IO uint32_t CH2_DTG;
  __IO uint32_t CH3_DTG;
  __IO uint32_t CH4_DTG;
  __IO uint32_t BRKETR_CNTRL;
  __IO uint32_t STATUS;
  __IO uint32_t IE;
  __IO uint32_t DMA_RE;
  __IO uint32_t CH1_CNTRL2;
  __IO uint32_t CH2_CNTRL2;
  __IO uint32_t CH3_CNTRL2;
  __IO uint32_t CH4_CNTRL2;
  __IO uint32_t CCR11;
  __IO uint32_t CCR21;
  __IO uint32_t CCR31;
  __IO uint32_t CCR41;
}MDR_TIMER_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_TIMER_Data_Structures */

/** @defgroup MDR32Fx_Periph_TIMER_Defines Defines
  * @{
  */

/*-- TIMER_CNTRL: Timer Control Register ---------------------------*/
/* Bit field positions: */
#define TIMER_CNTRL_CNT_EN_Pos                  0
#define TIMER_CNTRL_ARRB_EN_Pos                 1
#define TIMER_CNTRL_WR_CMPL_Pos                 2
#define TIMER_CNTRL_DIR_Pos                     3
#define TIMER_CNTRL_FDTS_Pos                    4
#define TIMER_CNTRL_CNT_MODE_Pos                6
#define TIMER_CNTRL_EVENT_SEL_Pos               8

/* Bit field masks: */
#define TIMER_CNTRL_CNT_EN                      ((uint32_t)0x00000001)
#define TIMER_CNTRL_ARRB_EN                     ((uint32_t)0x00000002)
#define TIMER_CNTRL_WR_CMPL                     ((uint32_t)0x00000004)
#define TIMER_CNTRL_DIR                         ((uint32_t)0x00000008)
#define TIMER_CNTRL_FDTS_Msk                    ((uint32_t)0x00000030)
#define TIMER_CNTRL_CNT_MODE_Msk                ((uint32_t)0x000000C0)
#define TIMER_CNTRL_EVENT_SEL_Msk               ((uint32_t)0x00000F00)

/*-- TIMER_CH_CNTRL: Timer Channel Control Register ----------------*/
/* Bit field positions: */
#define TIMER_CH_CNTRL_CHFLTR_Pos               0
#define TIMER_CH_CNTRL_CHSEL_Pos                4
#define TIMER_CH_CNTRL_CHPSC_Pos                6
#define TIMER_CH_CNTRL_OCCE_Pos                 8
#define TIMER_CH_CNTRL_OCCM_Pos                 9
#define TIMER_CH_CNTRL_BRKEN_Pos                12
#define TIMER_CH_CNTRL_ETREN_Pos                13
#define TIMER_CH_CNTRL_WR_CMPL_Pos              14
#define TIMER_CH_CNTRL_CAP_NPWM_Pos             15

/* Bit field masks: */
#define TIMER_CH_CNTRL_CHFLTR_Msk               ((uint32_t)0x0000000F)
#define TIMER_CH_CNTRL_CHSEL_Msk                ((uint32_t)0x00000030)
#define TIMER_CH_CNTRL_CHPSC_Msk                ((uint32_t)0x000000C0)
#define TIMER_CH_CNTRL_OCCE                     ((uint32_t)0x00000100)
#define TIMER_CH_CNTRL_OCCM_Msk                 ((uint32_t)0x00000E00)
#define TIMER_CH_CNTRL_BRKEN                    ((uint32_t)0x00001000)
#define TIMER_CH_CNTRL_ETREN                    ((uint32_t)0x00002000)
#define TIMER_CH_CNTRL_WR_CMPL                  ((uint32_t)0x00004000)
#define TIMER_CH_CNTRL_CAP_NPWM                 ((uint32_t)0x00008000)

/*-- TIMER_CH_CNTRL1: Timer Channel Control1 Register --------------*/
/* Bit field positions: */
#define TIMER_CH_CNTRL1_SELOE_Pos               0
#define TIMER_CH_CNTRL1_SELO_Pos                2
#define TIMER_CH_CNTRL1_INV_Pos                 4
#define TIMER_CH_CNTRL1_NSELOE_Pos              8
#define TIMER_CH_CNTRL1_NSELO_Pos               10
#define TIMER_CH_CNTRL1_NINV_Pos                12

/* Bit field masks: */
#define TIMER_CH_CNTRL1_SELOE_Msk               ((uint32_t)0x00000003)
#define TIMER_CH_CNTRL1_SELO_Msk                ((uint32_t)0x0000000C)
#define TIMER_CH_CNTRL1_INV                     ((uint32_t)0x00000010)
#define TIMER_CH_CNTRL1_NSELOE_Msk              ((uint32_t)0x00000300)
#define TIMER_CH_CNTRL1_NSELO_Msk               ((uint32_t)0x00000C00)
#define TIMER_CH_CNTRL1_NINV                    ((uint32_t)0x00001000)

/*-- TIMER_CH_DTG: Timer Channel DTG Register ----------------------*/
/* Bit field positions: */
#define TIMER_CH_DTGX_Pos                       0
#define TIMER_CH_DTG_EDTS_Pos                   4
#define TIMER_CH_DTG_Pos                        8

/* Bit field masks: */
#define TIMER_CH_DTGX_Msk                       ((uint32_t)0x0000000F)
#define TIMER_CH_DTG_EDTS                       ((uint32_t)0x00000010)
#define TIMER_CH_DTG_Msk                        ((uint32_t)0x0000FF00)

/*-- TIMER_BRKETR_CNTRL: Timer BRK/ETR Control Register ------------*/
/* Bit field positions: */
#define TIMER_BRKETR_CNTRL_BRK_INV_Pos          0
#define TIMER_BRKETR_CNTRL_ETR_INV_Pos          1
#define TIMER_BRKETR_CNTRL_ETR_PSC_Pos          2
#define TIMER_BRKETR_CNTRL_ETR_FILTER_Pos       4

/* Bit field masks: */
#define TIMER_BRKETR_CNTRL_BRK_INV              ((uint32_t)0x00000001)
#define TIMER_BRKETR_CNTRL_ETR_INV              ((uint32_t)0x00000002)
#define TIMER_BRKETR_CNTRL_ETR_PSC_Msk          ((uint32_t)0x0000000C)
#define TIMER_BRKETR_CNTRL_ETR_FILTER_Msk       ((uint32_t)0x000000F0)

/*-- TIMER_STATUS: Timer Status Register ---------------------------*/
/* Bit field positions: */
#define TIMER_STATUS_CNT_ZERO_EVENT_Pos         0
#define TIMER_STATUS_CNT_ARR_EVENT_Pos          1
#define TIMER_STATUS_ETR_RE_EVENT_Pos           2
#define TIMER_STATUS_ETR_FE_EVENT_Pos           3
#define TIMER_STATUS_BRK_EVENT_Pos              4
#define TIMER_STATUS_CCR_CAP_EVENT_Pos          5
#define TIMER_STATUS_CCR_REF_EVENT_Pos          9
#define TIMER_STATUS_CCR1_CAP_EVENT_Pos         13

/* Bit field masks: */
#define TIMER_STATUS_CNT_ZERO_EVENT             ((uint32_t)0x00000001)
#define TIMER_STATUS_CNT_ARR_EVENT              ((uint32_t)0x00000002)
#define TIMER_STATUS_ETR_RE_EVENT               ((uint32_t)0x00000004)
#define TIMER_STATUS_ETR_FE_EVENT               ((uint32_t)0x00000008)
#define TIMER_STATUS_BRK_EVENT                  ((uint32_t)0x00000010)
#define TIMER_STATUS_CCR_CAP_EVENT_Msk          ((uint32_t)0x000001E0)
#define TIMER_STATUS_CCR_REF_EVENT_Msk          ((uint32_t)0x00001E00)
#define TIMER_STATUS_CCR1_CAP_EVENT_Msk         ((uint32_t)0x0001E000)

/*-- TIMER_IE: Timer Interrupt Enable Register ---------------------*/
/* Bit field positions: */
#define TIMER_IE_CNT_ZERO_EVENT_IE_Pos          0
#define TIMER_IE_CNT_ARR_EVENT_IE_Pos           1
#define TIMER_IE_ETR_RE_EVENT_IE_Pos            2
#define TIMER_IE_ETR_FE_EVENT_IE_Pos            3
#define TIMER_IE_BRK_EVENT_IE_Pos               4
#define TIMER_IE_CCR_CAP_EVENT_IE_Pos           5
#define TIMER_IE_CCR_REF_EVENT_IE_Pos           9
#define TIMER_IE_CCR1_CAP_EVENT_IE_Pos          13

/* Bit field masks: */
#define TIMER_IE_CNT_ZERO_EVENT_IE              ((uint32_t)0x00000001)
#define TIMER_IE_CNT_ARR_EVENT_IE               ((uint32_t)0x00000002)
#define TIMER_IE_ETR_RE_EVENT_IE                ((uint32_t)0x00000004)
#define TIMER_IE_ETR_FE_EVENT_IE                ((uint32_t)0x00000008)
#define TIMER_IE_BRK_EVENT_IE                   ((uint32_t)0x00000010)
#define TIMER_IE_CCR_CAP_EVENT_IE_Msk           ((uint32_t)0x000001E0)
#define TIMER_IE_CCR_REF_EVENT_IE_Msk           ((uint32_t)0x00001E00)
#define TIMER_IE_CCR1_CAP_EVENT_IE_Msk          ((uint32_t)0x0001E000)

/*-- TIMER_DMA_RE: Timer DMA Request Enable Register ---------------*/
/* Bit field positions: */
#define TIMER_DMA_RE_CNT_ZERO_EVENT_RE_Pos      0
#define TIMER_DMA_RE_CNT_ARR_EVENT_RE_Pos       1
#define TIMER_DMA_RE_ETR_RE_EVENT_RE_Pos        2
#define TIMER_DMA_RE_ETR_FE_EVENT_RE_Pos        3
#define TIMER_DMA_RE_BRK_EVENT_RE_Pos           4
#define TIMER_DMA_RE_CCR_CAP_EVENT_RE_Pos       5
#define TIMER_DMA_RE_CCR_REF_EVENT_RE_Pos       9
#define TIMER_DMA_RE_CCR1_CAP_EVENT_RE_Pos      13

/* Bit field masks: */
#define TIMER_DMA_RE_CNT_ZERO_EVENT_RE          ((uint32_t)0x00000001)
#define TIMER_DMA_RE_CNT_ARR_EVENT_RE           ((uint32_t)0x00000002)
#define TIMER_DMA_RE_ETR_RE_EVENT_RE            ((uint32_t)0x00000004)
#define TIMER_DMA_RE_ETR_FE_EVENT_RE            ((uint32_t)0x00000008)
#define TIMER_DMA_RE_BRK_EVENT_RE               ((uint32_t)0x00000010)
#define TIMER_DMA_RE_CCR_CAP_EVENT_RE_Msk       ((uint32_t)0x000001E0)
#define TIMER_DMA_RE_CCR_REF_EVENT_RE_Msk       ((uint32_t)0x00001E00)
#define TIMER_DMA_RE_CCR1_CAP_EVENT_RE_Msk      ((uint32_t)0x0001E000)

/*-- TIMER_CH_CNTRL2: Timer Channel Control2 Register --------------*/
/* Bit field positions: */
#define TIMER_CH_CNTRL2_CHSEL1_Pos              0
#define TIMER_CH_CNTRL2_CCR1_EN_Pos             2
#define TIMER_CH_CNTRL2_CCRRLD_Pos              3

/* Bit field masks: */
#define TIMER_CH_CNTRL2_CHSEL1_Msk              ((uint32_t)0x00000003)
#define TIMER_CH_CNTRL2_CCR1_EN                 ((uint32_t)0x00000004)
#define TIMER_CH_CNTRL2_CCRRLD                  ((uint32_t)0x00000008)

/** @} */ /* End of group MDR32Fx_Periph_TIMER_Defines */

/** @} */ /* End of group MDR32Fx_Periph_TIMER */

/** @defgroup MDR32Fx_Periph_ADC ADC
  * @{
  */

/** @defgroup MDR32Fx_Periph_ADC_Data_Structures Data Structures
  * @{
  */

/* MDR_ADC_TypeDef structure */
typedef struct
{
  __IO uint32_t ADC1_CFG;
  __IO uint32_t ADC2_CFG;
  __IO uint32_t ADC1_H_LEVEL;
  __IO uint32_t ADC2_H_LEVEL;
  __IO uint32_t ADC1_L_LEVEL;
  __IO uint32_t ADC2_L_LEVEL;
  __IO uint32_t ADC1_RESULT;
  __IO uint32_t ADC2_RESULT;
  __IO uint32_t ADC1_STATUS;
  __IO uint32_t ADC2_STATUS;
  __IO uint32_t ADC1_CHSEL;
  __IO uint32_t ADC2_CHSEL;
}MDR_ADC_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_ADC_Data_Structures */

/** @defgroup MDR32Fx_Periph_ADC_Defines Defines
  * @{
  */

/*-- ADC1_CFG: ADC1 Configuration Register -------------------------*/
/* Bit field positions: */
#define ADC1_CFG_REG_ADON_Pos                   0
#define ADC1_CFG_REG_GO_Pos                     1
#define ADC1_CFG_REG_CLKS_Pos                   2
#define ADC1_CFG_REG_SAMPLE_Pos                 3
#define ADC1_CFG_REG_CHS_Pos                    4
#define ADC1_CFG_REG_CHCH_Pos                   9
#define ADC1_CFG_REG_RNGC_Pos                   10
#define ADC1_CFG_M_REF_Pos                      11
#define ADC1_CFG_REG_DIVCLK_Pos                 12
#define ADC1_CFG_SYNC_CONVER_Pos                16
#define ADC1_CFG_TS_EN_Pos                      17
#define ADC1_CFG_TS_BUF_EN_Pos                  18
#define ADC1_CFG_SEL_TS_Pos                     19
#define ADC1_CFG_SEL_VREF_Pos                   20
#define ADC1_CFG_TR_Pos                         21
#define ADC1_CFG_DELAY_GO_Pos                   25
#define ADC1_CFG_DELAY_ADC_Pos                  28

/* Bit field masks: */
#define ADC1_CFG_REG_ADON                       ((uint32_t)0x00000001)
#define ADC1_CFG_REG_GO                         ((uint32_t)0x00000002)
#define ADC1_CFG_REG_CLKS                       ((uint32_t)0x00000004)
#define ADC1_CFG_REG_SAMPLE                     ((uint32_t)0x00000008)
#define ADC1_CFG_REG_CHS_Msk                    ((uint32_t)0x000001F0)
#define ADC1_CFG_REG_CHCH                       ((uint32_t)0x00000200)
#define ADC1_CFG_REG_RNGC                       ((uint32_t)0x00000400)
#define ADC1_CFG_M_REF                          ((uint32_t)0x00000800)
#define ADC1_CFG_REG_DIVCLK_Msk                 ((uint32_t)0x0000F000)
#define ADC1_CFG_SYNC_CONVER                    ((uint32_t)0x00010000)
#define ADC1_CFG_TS_EN                          ((uint32_t)0x00020000)
#define ADC1_CFG_TS_BUF_EN                      ((uint32_t)0x00040000)
#define ADC1_CFG_SEL_TS                         ((uint32_t)0x00080000)
#define ADC1_CFG_SEL_VREF                       ((uint32_t)0x00100000)
#define ADC1_CFG_TR_Msk                         ((uint32_t)0x01E00000)
#define ADC1_CFG_DELAY_GO_Msk                   ((uint32_t)0x0E000000)
#define ADC1_CFG_DELAY_ADC_Msk                  ((uint32_t)0xF0000000)

/*-- ADC2_CFG: ADC2 Configuration Register -------------------------*/
/* Bit field positions: */
#define ADC2_CFG_REG_ADON_Pos                   0
#define ADC2_CFG_REG_GO_Pos                     1
#define ADC2_CFG_REG_CLKS_Pos                   2
#define ADC2_CFG_REG_SAMPLE_Pos                 3
#define ADC2_CFG_REG_CHS_Pos                    4
#define ADC2_CFG_REG_CHCH_Pos                   9
#define ADC2_CFG_REG_RNGC_Pos                   10
#define ADC2_CFG_M_REF_Pos                      11
#define ADC2_CFG_REG_DIVCLK_Pos                 12
#define ADC2_CFG_ADC1_OP_Pos                    17
#define ADC2_CFG_ADC2_OP_Pos                    18
#define ADC2_CFG_DELAY_GO_Pos                   25

/* Bit field masks: */
#define ADC2_CFG_REG_ADON                       ((uint32_t)0x00000001)
#define ADC2_CFG_REG_GO                         ((uint32_t)0x00000002)
#define ADC2_CFG_REG_CLKS                       ((uint32_t)0x00000004)
#define ADC2_CFG_REG_SAMPLE                     ((uint32_t)0x00000008)
#define ADC2_CFG_REG_CHS_Msk                    ((uint32_t)0x000001F0)
#define ADC2_CFG_REG_CHCH                       ((uint32_t)0x00000200)
#define ADC2_CFG_REG_RNGC                       ((uint32_t)0x00000400)
#define ADC2_CFG_M_REF                          ((uint32_t)0x00000800)
#define ADC2_CFG_REG_DIVCLK_Msk                 ((uint32_t)0x0000F000)
#define ADC2_CFG_ADC1_OP                        ((uint32_t)0x00020000)
#define ADC2_CFG_ADC2_OP                        ((uint32_t)0x00040000)
#define ADC2_CFG_DELAY_GO_Msk                   ((uint32_t)0x0E000000)

/*-- ADC1_RESULT: ADC1 Result Register -----------------------------*/
/*-- ADC2_RESULT: ADC2 Result Register -----------------------------*/
/* Bit field positions: */
#define ADC_RESULT_Pos                          0
#define ADC_RESULT_CHANNEL_Pos                  16

/* Bit field masks: */
#define ADC_RESULT_Msk                          ((uint32_t)0x00000FFF)
#define ADC_RESULT_CHANNEL_Msk                  ((uint32_t)0x0FFF0000)

/*-- ADC1_STATUS: ADC1 Status Register -----------------------------*/
/*-- ADC2_STATUS: ADC2 Status Register -----------------------------*/
/* Bit field positions: */
#define ADC_STATUS_FLG_REG_OVERWRITE_Pos        0
#define ADC_STATUS_FLG_REG_AWOIFEN_Pos          1
#define ADC_STATUS_FLG_REG_EOCIF_Pos            2
#define ADC_STATUS_AWOIF_IE_Pos                 3
#define ADC_STATUS_ECOIF_IE_Pos                 4

/* Bit field masks: */
#define ADC_STATUS_FLG_REG_OVERWRITE            ((uint32_t)0x00000001)
#define ADC_STATUS_FLG_REG_AWOIFEN              ((uint32_t)0x00000002)
#define ADC_STATUS_FLG_REG_EOCIF                ((uint32_t)0x00000004)
#define ADC_STATUS_AWOIF_IE                     ((uint32_t)0x00000008)
#define ADC_STATUS_ECOIF_IE                     ((uint32_t)0x00000010)

/** @} */ /* End of group MDR32Fx_Periph_ADC_Defines */

/** @} */ /* End of group MDR32Fx_Periph_ADC */

/** @defgroup MDR32Fx_Periph_DAC DAC
  * @{
  */

/** @defgroup MDR32Fx_Periph_DAC_Data_Structures Data Structures
  * @{
  */

/* MDR_DAC_TypeDef structure */
typedef struct
{
  __IO uint32_t CFG;
  __IO uint32_t DAC1_DATA;
  __IO uint32_t DAC2_DATA;
}MDR_DAC_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_DAC_Data_Structures */

/** @defgroup MDR32Fx_Periph_DAC_Defines Defines
  * @{
  */

/*-- DAC_CFG: DAC Configuration Register ---------------------------*/
/* Bit field positions: */
#define DAC_CFG_M_REF0_Pos                      0
#define DAC_CFG_M_REF1_Pos                      1
#define DAC_CFG_ON_DAC0_Pos                     2
#define DAC_CFG_ON_DAC1_Pos                     3
#define DAC_CFG_SYNC_A_Pos                      4

/* Bit field masks: */
#define DAC_CFG_M_REF0                          ((uint32_t)0x00000001)
#define DAC_CFG_M_REF1                          ((uint32_t)0x00000002)
#define DAC_CFG_ON_DAC0                         ((uint32_t)0x00000004)
#define DAC_CFG_ON_DAC1                         ((uint32_t)0x00000008)
#define DAC_CFG_SYNC_A                          ((uint32_t)0x00000010)

/*-- DAC1_DATA: DAC1 Data Register ---------------------------------*/
/* Bit field positions: */
#define DAC1_DATA_DAC0DATA_Pos                  0
#define DAC1_DATA_DAC1DATA_Pos                  16

/* Bit field masks: */
#define DAC1_DATA_DAC0DATA_Msk                  ((uint32_t)0x00000FFF)
#define DAC1_DATA_DAC1DATA_Msk                  ((uint32_t)0x0FFF0000)

/*-- DAC2_DATA: DAC2 Data Register ---------------------------------*/
/* Bit field positions: */
#define DAC2_DATA_DAC1DATA_Pos                  0
#define DAC2_DATA_DAC0DATA_Pos                  16

/* Bit field masks: */
#define DAC2_DATA_DAC1DATA_Msk                  ((uint32_t)0x00000FFF)
#define DAC2_DATA_DAC0DATA_Msk                  ((uint32_t)0x0FFF0000)

/** @} */ /* End of group MDR32Fx_Periph_DAC_Defines */

/** @} */ /* End of group MDR32Fx_Periph_DAC */

/** @defgroup MDR32Fx_Periph_COMP COMP
  * @{
  */

/** @defgroup MDR32Fx_Periph_COMP_Data_Structures Data Structures
  * @{
  */

/* MDR_COMP_TypeDef structure */
typedef struct
{
  __IO uint32_t CFG;
  __IO uint32_t RESULT;
  __IO uint32_t RESULT_LATCH;
}MDR_COMP_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_COMP_Data_Structures */

/** @defgroup MDR32Fx_Periph_COMP_Defines Defines
  * @{
  */

/*-- COMP_CFG: COMP Configuration Register -------------------------*/
/* Bit field positions: */
#define COMP_CFG_ON_Pos                         0
#define COMP_CFG_CVRR_Pos                       1
#define COMP_CFG_CVRSS_Pos                      2
#define COMP_CFG_CVREN_Pos                      3
#define COMP_CFG_CVR_Pos                        4
#define COMP_CFG_CREF_Pos                       8
#define COMP_CFG_CCH_Pos                        9
#define COMP_CFG_INV_Pos                        11
#define COMP_CFG_READY_Pos                      12
#define COMP_CFG_CMPIE_Pos                      13

/* Bit field masks: */
#define COMP_CFG_ON                             ((uint32_t)0x00000001)
#define COMP_CFG_CVRR                           ((uint32_t)0x00000002)
#define COMP_CFG_CVRSS                          ((uint32_t)0x00000004)
#define COMP_CFG_CVREN                          ((uint32_t)0x00000008)
#define COMP_CFG_CVR_Msk                        ((uint32_t)0x000000F0)
#define COMP_CFG_CREF                           ((uint32_t)0x00000100)
#define COMP_CFG_CCH_Msk                        ((uint32_t)0x00000600)
#define COMP_CFG_INV                            ((uint32_t)0x00000800)
#define COMP_CFG_READY                          ((uint32_t)0x00001000)
#define COMP_CFG_CMPIE                          ((uint32_t)0x00002000)

/*-- COMP_RESULT: COMP Result Register -----------------------------*/
/* Bit field positions: */
#define COMP_RESULT_RSLT_SY_Pos                 0
#define COMP_RESULT_RSLT_AS_Pos                 1
#define COMP_RESULT_RST_LCH_Pos                 2

/* Bit field masks: */
#define COMP_RESULT_RSLT_SY                     ((uint32_t)0x00000001)
#define COMP_RESULT_RSLT_AS                     ((uint32_t)0x00000002)
#define COMP_RESULT_RST_LCH                     ((uint32_t)0x00000004)

/** @} */ /* End of group MDR32Fx_Periph_COMP_Defines */

/** @} */ /* End of group MDR32Fx_Periph_COMP */

/** @defgroup MDR32Fx_Periph_PORT PORT
  * @{
  */

/** @defgroup MDR32Fx_Periph_PORT_Data_Structures Data Structures
  * @{
  */

/* MDR_PORT_TypeDef structure */
typedef struct
{
  __IO uint32_t RXTX;
  __IO uint32_t OE;
  __IO uint32_t FUNC;
  __IO uint32_t ANALOG;
  __IO uint32_t PULL;
  __IO uint32_t PD;
  __IO uint32_t PWR;
  __IO uint32_t GFEN;
}MDR_PORT_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_PORT_Data_Structures */

/** @defgroup MDR32Fx_Periph_PORT_Defines Defines
  * @{
  */

/*-- PORT_FUNC: PORT Function Register -----------------------------*/
/* Bit field positions: */
#define PORT_FUNC_MODE0_Pos                     0
#define PORT_FUNC_MODE1_Pos                     2
#define PORT_FUNC_MODE2_Pos                     4
#define PORT_FUNC_MODE3_Pos                     6
#define PORT_FUNC_MODE4_Pos                     8
#define PORT_FUNC_MODE5_Pos                     10
#define PORT_FUNC_MODE6_Pos                     12
#define PORT_FUNC_MODE7_Pos                     14
#define PORT_FUNC_MODE8_Pos                     16
#define PORT_FUNC_MODE9_Pos                     18
#define PORT_FUNC_MODE10_Pos                    20
#define PORT_FUNC_MODE11_Pos                    22
#define PORT_FUNC_MODE12_Pos                    24
#define PORT_FUNC_MODE13_Pos                    26
#define PORT_FUNC_MODE14_Pos                    28
#define PORT_FUNC_MODE15_Pos                    30

/* Bit field masks: */
#define PORT_FUNC_MODE0_Msk                     ((uint32_t)0x00000003)
#define PORT_FUNC_MODE1_Msk                     ((uint32_t)0x0000000C)
#define PORT_FUNC_MODE2_Msk                     ((uint32_t)0x00000030)
#define PORT_FUNC_MODE3_Msk                     ((uint32_t)0x000000C0)
#define PORT_FUNC_MODE4_Msk                     ((uint32_t)0x00000300)
#define PORT_FUNC_MODE5_Msk                     ((uint32_t)0x00000C00)
#define PORT_FUNC_MODE6_Msk                     ((uint32_t)0x00003000)
#define PORT_FUNC_MODE7_Msk                     ((uint32_t)0x0000C000)
#define PORT_FUNC_MODE8_Msk                     ((uint32_t)0x00030000)
#define PORT_FUNC_MODE9_Msk                     ((uint32_t)0x000C0000)
#define PORT_FUNC_MODE10_Msk                    ((uint32_t)0x00300000)
#define PORT_FUNC_MODE11_Msk                    ((uint32_t)0x00C00000)
#define PORT_FUNC_MODE12_Msk                    ((uint32_t)0x03000000)
#define PORT_FUNC_MODE13_Msk                    ((uint32_t)0x0C000000)
#define PORT_FUNC_MODE14_Msk                    ((uint32_t)0x30000000)
#define PORT_FUNC_MODE15_Msk                    ((uint32_t)0xC0000000)

/*-- PORT_PULL: PORT Pull Up/Down Register -------------------------*/
/* Bit field positions: */
#define PORT_PULL_DOWN_Pos                      0
#define PORT_PULL_UP_Pos                        16

/* Bit field masks: */
#define PORT_PULL_DOWN_Msk                      ((uint32_t)0x0000FFFF)
#define PORT_PULL_UP_Msk                        ((uint32_t)0xFFFF0000)

/*-- PORT_PD: PORT Driver Mode Register ----------------------------*/
/* Bit field positions: */
#define PORT_PD_Pos                             0
#define PORT_PD_SHM_Pos                         16

/* Bit field masks: */
#define PORT_PD_Msk                             ((uint32_t)0x0000FFFF)
#define PORT_PD_SHM_Msk                         ((uint32_t)0xFFFF0000)

/*-- PORT_PWR: PORT Power Register ---------------------------------*/
/* Bit field positions: */
#define PORT_PWR0_Pos                           0
#define PORT_PWR1_Pos                           2
#define PORT_PWR2_Pos                           4
#define PORT_PWR3_Pos                           6
#define PORT_PWR4_Pos                           8
#define PORT_PWR5_Pos                           10
#define PORT_PWR6_Pos                           12
#define PORT_PWR7_Pos                           14
#define PORT_PWR8_Pos                           16
#define PORT_PWR9_Pos                           18
#define PORT_PWR10_Pos                          20
#define PORT_PWR11_Pos                          22
#define PORT_PWR12_Pos                          24
#define PORT_PWR13_Pos                          26
#define PORT_PWR14_Pos                          28
#define PORT_PWR15_Pos                          30

/* Bit field masks: */
#define PORT_PWR0_Msk                           ((uint32_t)0x00000003)
#define PORT_PWR1_Msk                           ((uint32_t)0x0000000C)
#define PORT_PWR2_Msk                           ((uint32_t)0x00000030)
#define PORT_PWR3_Msk                           ((uint32_t)0x000000C0)
#define PORT_PWR4_Msk                           ((uint32_t)0x00000300)
#define PORT_PWR5_Msk                           ((uint32_t)0x00000C00)
#define PORT_PWR6_Msk                           ((uint32_t)0x00003000)
#define PORT_PWR7_Msk                           ((uint32_t)0x0000C000)
#define PORT_PWR8_Msk                           ((uint32_t)0x00030000)
#define PORT_PWR9_Msk                           ((uint32_t)0x000C0000)
#define PORT_PWR10_Msk                          ((uint32_t)0x00300000)
#define PORT_PWR11_Msk                          ((uint32_t)0x00C00000)
#define PORT_PWR12_Msk                          ((uint32_t)0x03000000)
#define PORT_PWR13_Msk                          ((uint32_t)0x0C000000)
#define PORT_PWR14_Msk                          ((uint32_t)0x30000000)
#define PORT_PWR15_Msk                          ((uint32_t)0xC0000000)

/** @} */ /* End of group MDR32Fx_Periph_PORT_Defines */

/** @} */ /* End of group MDR32Fx_Periph_PORT */

/** @defgroup MDR32Fx_Periph_BKP BKP
  * @{
  */

/** @defgroup MDR32Fx_Periph_BKP_Data_Structures Data Structures
  * @{
  */

/* MDR_BKP_TypeDef structure */
typedef struct
{
  __IO uint32_t REG_00;
  __IO uint32_t REG_01;
  __IO uint32_t REG_02;
  __IO uint32_t REG_03;
  __IO uint32_t REG_04;
  __IO uint32_t REG_05;
  __IO uint32_t REG_06;
  __IO uint32_t REG_07;
  __IO uint32_t REG_08;
  __IO uint32_t REG_09;
  __IO uint32_t REG_0A;
  __IO uint32_t REG_0B;
  __IO uint32_t REG_0C;
  __IO uint32_t REG_0D;
  __IO uint32_t REG_0E;
  __IO uint32_t REG_0F;
  __IO uint32_t RTC_CNT;
  __IO uint32_t RTC_DIV;
  __IO uint32_t RTC_PRL;
  __IO uint32_t RTC_ALRM;
  __IO uint32_t RTC_CS;
}MDR_BKP_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_BKP_Data_Structures */

/** @defgroup MDR32Fx_Periph_BKP_Defines Defines
  * @{
  */

/*-- BKP_REG_0E: Backup Register 14 --------------------------------*/
/* Bit field positions: */
#define BKP_REG_0E_LOW_Pos                      0
#define BKP_REG_0E_SELECTRI_Pos                 3
#define BKP_REG_0E_JTAGA_Pos                    6
#define BKP_REG_0E_JTAGB_Pos                    7
#define BKP_REG_0E_TRIM_Pos                     8
#define BKP_REG_0E_FPOR_Pos                     11
#define BKP_REG_0E_BKP_REG_Pos                  12

/* Bit field masks: */
#define BKP_REG_0E_LOW_Msk                      ((uint32_t)0x00000007)
#define BKP_REG_0E_SELECTRI_Msk                 ((uint32_t)0x00000038)
#define BKP_REG_0E_JTAGA                        ((uint32_t)0x00000040)
#define BKP_REG_0E_JTAGB                        ((uint32_t)0x00000080)
#define BKP_REG_0E_TRIM_Msk                     ((uint32_t)0x00000700)
#define BKP_REG_0E_FPOR                         ((uint32_t)0x00000800)
#define BKP_REG_0E_BKP_REG_Msk                  ((uint32_t)0xFFFFF000)

/*-- BKP_REG_0F: Backup Register 15 --------------------------------*/
/* Bit field positions: */
#define BKP_REG_0F_LSE_ON_Pos                   0
#define BKP_REG_0F_LSE_BYP_Pos                  1
#define BKP_REG_0F_RTC_SEL_Pos                  2
#define BKP_REG_0F_RTC_EN_Pos                   4
#define BKP_REG_0F_CAL_Pos                      5
#define BKP_REG_0F_LSE_RDY_Pos                  13
#define BKP_REG_0F_BKP_REG_Pos                  14
#define BKP_REG_0F_LSI_ON_Pos                   15
#define BKP_REG_0F_LSI_TRIM_Pos                 16
#define BKP_REG_0F_LSI_RDY_Pos                  21
#define BKP_REG_0F_HSI_ON_Pos                   22
#define BKP_REG_0F_HSI_RDY_Pos                  23
#define BKP_REG_0F_HSI_TRIM_Pos                 24
#define BKP_REG_0F_STANDBY_Pos                  30
#define BKP_REG_0F_RTC_RESET_Pos                31

/* Bit field masks: */
#define BKP_REG_0F_LSE_ON                       ((uint32_t)0x00000001)
#define BKP_REG_0F_LSE_BYP                      ((uint32_t)0x00000002)
#define BKP_REG_0F_RTC_SEL_Msk                  ((uint32_t)0x0000000C)
#define BKP_REG_0F_RTC_EN                       ((uint32_t)0x00000010)
#define BKP_REG_0F_CAL_Msk                      ((uint32_t)0x00001FE0)
#define BKP_REG_0F_LSE_RDY                      ((uint32_t)0x00002000)
#define BKP_REG_0F_BKP_REG                      ((uint32_t)0x00004000)
#define BKP_REG_0F_LSI_ON                       ((uint32_t)0x00008000)
#define BKP_REG_0F_LSI_TRIM_Msk                 ((uint32_t)0x001F0000)
#define BKP_REG_0F_LSI_RDY                      ((uint32_t)0x00200000)
#define BKP_REG_0F_HSI_ON                       ((uint32_t)0x00400000)
#define BKP_REG_0F_HSI_RDY                      ((uint32_t)0x00800000)
#define BKP_REG_0F_HSI_TRIM_Msk                 ((uint32_t)0x3F000000)
#define BKP_REG_0F_STANDBY                      ((uint32_t)0x40000000)
#define BKP_REG_0F_RTC_RESET                    ((uint32_t)0x80000000)

/*-- BKP_RTC_CS: Backup Realtime clock Register --------------------*/
/* Bit field positions: */
#define BKP_RTC_CS_OWF_Pos                      0
#define BKP_RTC_CS_SECF_Pos                     1
#define BKP_RTC_CS_ALRF_Pos                     2
#define BKP_RTC_CS_OWF_IE_Pos                   3
#define BKP_RTC_CS_SECF_IE_Pos                  4
#define BKP_RTC_CS_ALRF_IE_Pos                  5
#define BKP_RTC_CS_WEC_Pos                      6

/* Bit field masks: */
#define BKP_RTC_CS_OWF                          ((uint32_t)0x00000001)
#define BKP_RTC_CS_SECF                         ((uint32_t)0x00000002)
#define BKP_RTC_CS_ALRF                         ((uint32_t)0x00000004)
#define BKP_RTC_CS_OWF_IE                       ((uint32_t)0x00000008)
#define BKP_RTC_CS_SECF_IE                      ((uint32_t)0x00000010)
#define BKP_RTC_CS_ALRF_IE                      ((uint32_t)0x00000020)
#define BKP_RTC_CS_WEC                          ((uint32_t)0x00000040)

/** @} */ /* End of group MDR32Fx_Periph_BKP_Defines */

/** @} */ /* End of group MDR32Fx_Periph_BKP */

/** @defgroup MDR32Fx_Periph_EBC EBC
  * @{
  */

/** @defgroup MDR32Fx_Periph_EBC_Data_Structures Data Structures
  * @{
  */

/* MDR_EBC_TypeDef structure */
typedef struct
{
       uint32_t RESERVED0[20];
  __IO uint32_t NAND_CYCLES;
  __IO uint32_t CONTROL;
}MDR_EBC_TypeDef;

/** @} */ /* End of group MDR32Fx_Periph_EBC_Data_Structures */

/** @defgroup MDR32Fx_Periph_EBC_Defines Defines
  * @{
  */

/*-- EBC_NAND_CYCLES: EBC NAND Timing Register ---------------------*/
/* Bit field positions: */
#define EBC_NAND_CYCLES_TRC_Pos                 0
#define EBC_NAND_CYCLES_TWC_Pos                 4
#define EBC_NAND_CYCLES_TREA_Pos                8
#define EBC_NAND_CYCLES_TWP_Pos                 12
#define EBC_NAND_CYCLES_TWHR_Pos                16
#define EBC_NAND_CYCLES_TALEA_Pos               20
#define EBC_NAND_CYCLES_TRR_Pos                 24

/* Bit field masks: */
#define EBC_NAND_CYCLES_TRC_Msk                 ((uint32_t)0x0000000F)
#define EBC_NAND_CYCLES_TWC_Msk                 ((uint32_t)0x000000F0)
#define EBC_NAND_CYCLES_TREA_Msk                ((uint32_t)0x00000F00)
#define EBC_NAND_CYCLES_TWP_Msk                 ((uint32_t)0x0000F000)
#define EBC_NAND_CYCLES_TWHR_Msk                ((uint32_t)0x000F0000)
#define EBC_NAND_CYCLES_TALEA_Msk               ((uint32_t)0x00F00000)
#define EBC_NAND_CYCLES_TRR_Msk                 ((uint32_t)0x0F000000)

/*-- EBC_CONTROL: EBC Control Register -----------------------------*/
/* Bit field positions: */
#define EBC_CONTROL_ROM_Pos                     0
#define EBC_CONTROL_RAM_Pos                     1
#define EBC_CONTROL_NAND_Pos                    2
#define EBC_CONTROL_CPOL_Pos                    3
#define EBC_CONTROL_BUSY_Pos                    7
#define EBC_CONTROL_WAIT_STATE_Pos              12

/* Bit field masks: */
#define EBC_CONTROL_ROM                         ((uint32_t)0x00000001)
#define EBC_CONTROL_RAM                         ((uint32_t)0x00000002)
#define EBC_CONTROL_NAND                        ((uint32_t)0x00000004)
#define EBC_CONTROL_CPOL                        ((uint32_t)0x00000008)
#define EBC_CONTROL_BUSY                        ((uint32_t)0x00000080)
#define EBC_CONTROL_WAIT_STATE_Msk              ((uint32_t)0x0000F000)

/** @} */ /* End of group MDR32Fx_Periph_EBC_Defines */

/** @} */ /* End of group MDR32Fx_Periph_EBC */

/** @} */ /* End of group __MDR32Fx_Peripheral_Units */


/** @defgroup __Peripheral_Memory_Map Peripheral Memory Map
  * @{
  */

#define MDR_CAN1_BASE       (0x40000000)
#define MDR_CAN2_BASE       (0x40008000)
#define MDR_USB_BASE        (0x40010000)
#define MDR_EEPROM_BASE     (0x40018000)
#define MDR_RST_CLK_BASE    (0x40020000)
#define MDR_DMA_BASE        (0x40028000)
#define MDR_UART1_BASE      (0x40030000)
#define MDR_UART2_BASE      (0x40038000)
#define MDR_SSP1_BASE       (0x40040000)
#define MDR_I2C_BASE        (0x40050000)
#define MDR_POWER_BASE      (0x40058000)
#define MDR_WWDG_BASE       (0x40060000)
#define MDR_IWDG_BASE       (0x40068000)
#define MDR_TIMER1_BASE     (0x40070000)
#define MDR_TIMER2_BASE     (0x40078000)
#define MDR_TIMER3_BASE     (0x40080000)
#define MDR_ADC_BASE        (0x40088000)
#define MDR_DAC_BASE        (0x40090000)
#define MDR_COMP_BASE       (0x40098000)
#define MDR_SSP2_BASE       (0x400A0000)
#define MDR_PORTA_BASE      (0x400A8000)
#define MDR_PORTB_BASE      (0x400B0000)
#define MDR_PORTC_BASE      (0x400B8000)
#define MDR_PORTD_BASE      (0x400C0000)
#define MDR_PORTE_BASE      (0x400C8000)
#define MDR_BKP_BASE        (0x400D8000)
#define MDR_PORTF_BASE      (0x400E8000)
#define MDR_EBC_BASE        (0x400F0000)

/** @} */ /* End of group __Peripheral_Memory_Map */

/** @defgroup __Peripheral_declaration Peripheral declaration
  * @{
  */

#define MDR_CAN1            ((MDR_CAN_TypeDef     *) MDR_CAN1_BASE)
#define MDR_CAN2            ((MDR_CAN_TypeDef     *) MDR_CAN2_BASE)
#define MDR_USB             ((MDR_USB_TypeDef     *) MDR_USB_BASE)
#define MDR_EEPROM          ((MDR_EEPROM_TypeDef  *) MDR_EEPROM_BASE)
#define MDR_RST_CLK         ((MDR_RST_CLK_TypeDef *) MDR_RST_CLK_BASE)
#define MDR_DMA             ((MDR_DMA_TypeDef     *) MDR_DMA_BASE)
#define MDR_UART1           ((MDR_UART_TypeDef    *) MDR_UART1_BASE)
#define MDR_UART2           ((MDR_UART_TypeDef    *) MDR_UART2_BASE)
#define MDR_SSP1            ((MDR_SSP_TypeDef     *) MDR_SSP1_BASE)
#define MDR_I2C             ((MDR_I2C_TypeDef     *) MDR_I2C_BASE)
#define MDR_POWER           ((MDR_POWER_TypeDef   *) MDR_POWER_BASE)
#define MDR_WWDG            ((MDR_WWDG_TypeDef    *) MDR_WWDG_BASE)
#define MDR_IWDG            ((MDR_IWDG_TypeDef    *) MDR_IWDG_BASE)
#define MDR_TIMER1          ((MDR_TIMER_TypeDef   *) MDR_TIMER1_BASE)
#define MDR_TIMER2          ((MDR_TIMER_TypeDef   *) MDR_TIMER2_BASE)
#define MDR_TIMER3          ((MDR_TIMER_TypeDef   *) MDR_TIMER3_BASE)
#define MDR_ADC             ((MDR_ADC_TypeDef     *) MDR_ADC_BASE)
#define MDR_DAC             ((MDR_DAC_TypeDef     *) MDR_DAC_BASE)
#define MDR_COMP            ((MDR_COMP_TypeDef    *) MDR_COMP_BASE)
#define MDR_SSP2            ((MDR_SSP_TypeDef     *) MDR_SSP2_BASE)
#define MDR_PORTA           ((MDR_PORT_TypeDef    *) MDR_PORTA_BASE)
#define MDR_PORTB           ((MDR_PORT_TypeDef    *) MDR_PORTB_BASE)
#define MDR_PORTC           ((MDR_PORT_TypeDef    *) MDR_PORTC_BASE)
#define MDR_PORTD           ((MDR_PORT_TypeDef    *) MDR_PORTD_BASE)
#define MDR_PORTE           ((MDR_PORT_TypeDef    *) MDR_PORTE_BASE)
#define MDR_BKP             ((MDR_BKP_TypeDef     *) MDR_BKP_BASE)
#define MDR_PORTF           ((MDR_PORT_TypeDef    *) MDR_PORTF_BASE)
#define MDR_EBC             ((MDR_EBC_TypeDef     *) MDR_EBC_BASE)

/** @} */ /* End of group __Peripheral_declaration */

#ifdef __cplusplus
}
#endif

/** @} */ /* End of group __MDR32Fx */

/** @} */ /* End of group __CMSIS */

#endif /* __MDR32FX_H */

/******************* (C) COPYRIGHT 2010 Phyton *********************************
*
* END OF FILE MDR32Fx.h */
