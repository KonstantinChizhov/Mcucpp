#ifndef MIK32_MEMORY_MAP_H_INCLUDED
#define MIK32_MEMORY_MAP_H_INCLUDED

#define EEPROM_BASE_ADDRESS                 0x01000000
#define EEPROM_SIZE                         (8*1024)

#define RAM_BASE_ADDRESS                    0x02000000
#define RAM_SIZE                            (16*1024)

#define SPIFI_BASE_ADDRESS                  0x80000000
#define SPIFI_SIZE                          (2*1024*1024*1024)

#define BOOT_BASE_ADDRESS                   0x00000000
#define BOOT_SIZE                           (16*1024)

#define APB_PERIPH_STEP                     0x00000400

#define DMA_CONFIG_BASE_ADDRESS             0x000040000

#define APB_DOM_0_BASE_ADDRESS              0x000050000
#define PM_BASE_ADDRESS                     0x000050000
#define EPIC_BASE_ADDRESS                   0x000050400
#define TIMER32_0_BASE_ADDRESS              0x000050800
#define PAD_CONFIG_BASE_ADDRESS             0x000050c00
#define WDT_BUS_BASE_ADDRESS                0x000051000
#define OTP_BASE_ADDRESS                    0x000051400
#define PVD_CONTROL_BASE_ADDRESS            0x000051800

#define WU_BASE_ADDRESS                     0x00060000
#define RTC_BASE_ADDRESS                    0x00060400
#define BOOT_MANAGER_BASE_ADDRESS           0x00060800

#define SPIFI_CONFIG_BASE_ADDRESS           0x00070000
#define EEPROM_REGS_BASE_ADDRESS            0x00070400
#define CRYPTO_BASE_ADDRESS                 0x00080000
#define CRC_BASE_ADDRESS                    0x00080400

#define APB_DOM_3_BASE_ADDRESS              0x00081000

#define WDT_BASE_ADDRESS                    0x00081000
#define UART_0_BASE_ADDRESS                 0x00081400
#define UART_1_BASE_ADDRESS                 0x00081800
#define TIMER16_0_BASE_ADDRESS              0x00081C00
#define TIMER16_1_BASE_ADDRESS              0x00082000
#define TIMER16_2_BASE_ADDRESS              0x00082400
#define TIMER32_1_BASE_ADDRESS              0x00082800
#define TIMER32_2_BASE_ADDRESS              0x00082C00
#define SPI_0_BASE_ADDRESS                  0x00083000
#define SPI_1_BASE_ADDRESS                  0x00083400
#define I2C_0_BASE_ADDRESS                  0x00083800
#define I2C_1_BASE_ADDRESS                  0x00083C00
#define GPIO_0_BASE_ADDRESS                 0x00084000
#define GPIO_1_BASE_ADDRESS                 0x00084400
#define GPIO_2_BASE_ADDRESS                 0x00084800
#define GPIO_IRQ_BASE_ADDRESS               0x00084C00
#define ANALOG_REG_BASE_ADDRESS             0x00085000


#define SCR1_TIMER_BASE_ADDRESS             0x00490000


#define DMA_CONFIG      (( DMA_CONFIG_TypeDef       *)  DMA_CONFIG_BASE_ADDRESS     )

#define PM              (( PM_TypeDef               *)  PM_BASE_ADDRESS             ) 
#define EPIC            (( EPIC_TypeDef             *)  EPIC_BASE_ADDRESS           )     
#define TIMER32_0       (( TIMER32_TypeDef          *)  TIMER32_0_BASE_ADDRESS      ) 
#define PVD_CONTROL     (( PVD_CONTROL_TypeDef      *)  PVD_CONTROL_BASE_ADDRESS    ) 
#define PAD_CONFIG      (( PAD_CONFIG_TypeDef       *)  PAD_CONFIG_BASE_ADDRESS     ) 
#define WDT_BUS         (( WDT_BUS_TypeDef          *)  WDT_BUS_BASE_ADDRESS        ) 
#define OTP             (( OTP_TypeDef              *)  OTP_BASE_ADDRESS            )

#define WU              (( WU_TypeDef               *)  WU_BASE_ADDRESS             ) 
#define RTC             (( RTC_TypeDef              *)  RTC_BASE_ADDRESS            )
#define BOOT_MANAGER    (( BOOT_MANAGER_TypeDef     *)  BOOT_MANAGER_BASE_ADDRESS   )

#define SPIFI_CONFIG    (( SPIFI_CONFIG_TypeDef     *)  SPIFI_CONFIG_BASE_ADDRESS   )
#define EEPROM_REGS     (( EEPROM_REGS_TypeDef      *)  EEPROM_REGS_BASE_ADDRESS    )
#define CRYPTO          (( CRYPTO_TypeDef           *)  CRYPTO_BASE_ADDRESS         )
#define CRC             (( CRC_TypeDef              *)  CRC_BASE_ADDRESS            )

#define WDT             (( WDT_TypeDef              *)  WDT_BASE_ADDRESS            )
#define UART_0          (( UART_TypeDef             *)  UART_0_BASE_ADDRESS         )
#define UART_1          (( UART_TypeDef             *)  UART_1_BASE_ADDRESS         )
#define TIMER16_0       (( TIMER16_TypeDef          *)  TIMER16_0_BASE_ADDRESS      )
#define TIMER16_1       (( TIMER16_TypeDef          *)  TIMER16_1_BASE_ADDRESS      )
#define TIMER16_2       (( TIMER16_TypeDef          *)  TIMER16_2_BASE_ADDRESS      )
#define TIMER32_1       (( TIMER32_TypeDef          *)  TIMER32_1_BASE_ADDRESS      )
#define TIMER32_2       (( TIMER32_TypeDef          *)  TIMER32_2_BASE_ADDRESS      )
#define SPI_0           (( SPI_TypeDef              *)  SPI_0_BASE_ADDRESS          )
#define SPI_1           (( SPI_TypeDef              *)  SPI_1_BASE_ADDRESS          )
#define I2C_0           (( I2C_TypeDef              *)  I2C_0_BASE_ADDRESS          )
#define I2C_1           (( I2C_TypeDef              *)  I2C_1_BASE_ADDRESS          )
#define GPIO_0          (( GPIO_TypeDef             *)  GPIO_0_BASE_ADDRESS         )
#define GPIO_1          (( GPIO_TypeDef             *)  GPIO_1_BASE_ADDRESS         )
#define GPIO_2          (( GPIO_TypeDef             *)  GPIO_2_BASE_ADDRESS         )
#define GPIO_IRQ        (( GPIO_IRQ_TypeDef         *)  GPIO_IRQ_BASE_ADDRESS       )
#define ANALOG_REG      (( ANALOG_REG_TypeDef       *)  ANALOG_REG_BASE_ADDRESS     )

#define SCR1_TIMER      (( SCR1_TIMER_TypeDef       *)  SCR1_TIMER_BASE_ADDRESS     )



// Clock gating masks to be used with PM module 
//AHB BUS
#define PM_CLOCK_AHB_CPU_S              0
#define PM_CLOCK_AHB_CPU_M              (1 << PM_CLOCK_AHB_CPU_S)
#define PM_CLOCK_AHB_EEPROM_S           1
#define PM_CLOCK_AHB_EEPROM_M           (1 << PM_CLOCK_AHB_EEPROM_S)
#define PM_CLOCK_AHB_RAM_S              2
#define PM_CLOCK_AHB_RAM_M              (1 << PM_CLOCK_AHB_RAM_S)
#define PM_CLOCK_AHB_SPIFI_S            3
#define PM_CLOCK_AHB_SPIFI_M            (1 << PM_CLOCK_AHB_SPIFI_S)
#define PM_CLOCK_AHB_TCB_S              4
#define PM_CLOCK_AHB_TCB_M              (1 << PM_CLOCK_AHB_TCB_S)
#define PM_CLOCK_AHB_DMA_S              5
#define PM_CLOCK_AHB_DMA_M              (1 << PM_CLOCK_AHB_DMA_S)
#define PM_CLOCK_AHB_CRYPTO_S           6
#define PM_CLOCK_AHB_CRYPTO_M           (1 << PM_CLOCK_AHB_CRYPTO_S)
#define PM_CLOCK_AHB_CRC32_S            7
#define PM_CLOCK_AHB_CRC32_M            (1 << PM_CLOCK_AHB_CRC32_S)


//APB M BUS
#define PM_CLOCK_APB_M_PM_S               0
#define PM_CLOCK_APB_M_PM_M               (1 << PM_CLOCK_APB_M_PM_S)
#define PM_CLOCK_APB_M_EPIC_S             1
#define PM_CLOCK_APB_M_EPIC_M             (1 << PM_CLOCK_APB_M_EPIC_S)
#define PM_CLOCK_APB_M_TIMER32_0_S        2
#define PM_CLOCK_APB_M_TIMER32_0_M        (1 << PM_CLOCK_APB_M_TIMER32_0_S)
#define PM_CLOCK_APB_M_PAD_CONFIG_S       3
#define PM_CLOCK_APB_M_PAD_CONFIG_M       (1 << PM_CLOCK_APB_M_PAD_CONFIG_S)
#define PM_CLOCK_APB_M_WDT_BUS_S          4
#define PM_CLOCK_APB_M_WDT_BUS_M          (1 << PM_CLOCK_APB_M_WDT_BUS_S)
#define PM_CLOCK_APB_M_OTP_CONTROLLER_S   5
#define PM_CLOCK_APB_M_OTP_CONTROLLER_M   (1 << PM_CLOCK_APB_M_OTP_CONTROLLER_S)
#define PM_CLOCK_APB_M_PVD_CONTROL_S      6
#define PM_CLOCK_APB_M_PVD_CONTROL_M      (1 << PM_CLOCK_APB_M_PVD_CONTROL_S)
#define PM_CLOCK_APB_M_WU_S               7
#define PM_CLOCK_APB_M_WU_M               (1 << PM_CLOCK_APB_M_WU_S)
#define PM_CLOCK_APB_M_RTC_S              8
#define PM_CLOCK_APB_M_RTC_M              (1 << PM_CLOCK_APB_M_RTC_S)

//APB P BUS
#define PM_CLOCK_APB_P_WDT_S          0
#define PM_CLOCK_APB_P_WDT_M          (1 << PM_CLOCK_APB_P_WDT_S)
#define PM_CLOCK_APB_P_UART_0_S       1
#define PM_CLOCK_APB_P_UART_0_M       (1 << PM_CLOCK_APB_P_UART_0_S)
#define PM_CLOCK_APB_P_UART_1_S       2
#define PM_CLOCK_APB_P_UART_1_M       (1 << PM_CLOCK_APB_P_UART_1_S)
#define PM_CLOCK_APB_P_TIMER16_0_S    3
#define PM_CLOCK_APB_P_TIMER16_0_M    (1 << PM_CLOCK_APB_P_TIMER16_0_S)
#define PM_CLOCK_APB_P_TIMER16_1_S    4
#define PM_CLOCK_APB_P_TIMER16_1_M    (1 << PM_CLOCK_APB_P_TIMER16_1_S)
#define PM_CLOCK_APB_P_TIMER16_2_S    5
#define PM_CLOCK_APB_P_TIMER16_2_M    (1 << PM_CLOCK_APB_P_TIMER16_2_S)
#define PM_CLOCK_APB_P_TIMER32_1_S    6
#define PM_CLOCK_APB_P_TIMER32_1_M    (1 << PM_CLOCK_APB_P_TIMER32_1_S)
#define PM_CLOCK_APB_P_TIMER32_2_S    7
#define PM_CLOCK_APB_P_TIMER32_2_M    (1 << PM_CLOCK_APB_P_TIMER32_2_S)
#define PM_CLOCK_APB_P_SPI_0_S        8
#define PM_CLOCK_APB_P_SPI_0_M        (1 << PM_CLOCK_APB_P_SPI_0_S)
#define PM_CLOCK_APB_P_SPI_1_S        9
#define PM_CLOCK_APB_P_SPI_1_M        (1 << PM_CLOCK_APB_P_SPI_1_S)
#define PM_CLOCK_APB_P_I2C_0_S        10
#define PM_CLOCK_APB_P_I2C_0_M        (1 << PM_CLOCK_APB_P_I2C_0_S)
#define PM_CLOCK_APB_P_I2C_1_S        11
#define PM_CLOCK_APB_P_I2C_1_M        (1 << PM_CLOCK_APB_P_I2C_1_S)
#define PM_CLOCK_APB_P_GPIO_0_S       12
#define PM_CLOCK_APB_P_GPIO_0_M       (1 << PM_CLOCK_APB_P_GPIO_0_S)
#define PM_CLOCK_APB_P_GPIO_1_S       13
#define PM_CLOCK_APB_P_GPIO_1_M       (1 << PM_CLOCK_APB_P_GPIO_1_S)
#define PM_CLOCK_APB_P_GPIO_2_S       14
#define PM_CLOCK_APB_P_GPIO_2_M       (1 << PM_CLOCK_APB_P_GPIO_2_S)
#define PM_CLOCK_APB_P_ANALOG_REGS_S  15
#define PM_CLOCK_APB_P_ANALOG_REGS_M  (1 << PM_CLOCK_APB_P_ANALOG_REGS_S)
#define PM_CLOCK_APB_P_GPIO_IRQ_S     16
#define PM_CLOCK_APB_P_GPIO_IRQ_M     (1 << PM_CLOCK_APB_P_GPIO_IRQ_S)

// Timer connection to PM multiplexor controling timer inputs
//
#define PM_TIMER32_0_INDEX      0
#define PM_TIMER32_1_INDEX      1
#define PM_TIMER32_2_INDEX      2
#define PM_TIMER16_0_INDEX      3
#define PM_TIMER16_1_INDEX      4
#define PM_TIMER16_2_INDEX      5


// Interrupt lines to be used with EPIC module
//
#define EPIC_TIMER32_0_INDEX        0
#define EPIC_UART_0_INDEX           1
#define EPIC_UART_1_INDEX           2
#define EPIC_SPI_0_INDEX            3
#define EPIC_SPI_1_INDEX            4
#define EPIC_GPIO_IRQ_INDEX         5
#define EPIC_I2C_0_INDEX            6
#define EPIC_I2C_1_INDEX            7
#define EPIC_WDT_INDEX              8
#define EPIC_TIMER16_0_INDEX        9
#define EPIC_TIMER16_1_INDEX        10
#define EPIC_TIMER16_2_INDEX        11
#define EPIC_TIMER32_1_INDEX        12
#define EPIC_TIMER32_2_INDEX        13
#define EPIC_SPIFI_INDEX            14
#define EPIC_RTC_INDEX              15
#define EPIC_EEPROM_INDEX           16
#define EPIC_WDT_DOM3_INDEX         17
#define EPIC_WDT_SPIFI_INDEX        18
#define EPIC_WDT_EEPROM_INDEX       19
#define EPIC_DMA_INDEX              20
#define EPIC_FREQ_MON_INDEX         21
#define EPIC_PVD_AVCC_UNDER         22
#define EPIC_PVD_AVCC_OVER          23
#define EPIC_PVD_VCC_UNDER          24
#define EPIC_PVD_VCC_OVER           25
#define EPIC_BATTERY_NON_GOOD       26
#define EPIC_BOR_INDEX              27
#define EPIC_TSENS_INDEX            28
#define EPIC_ADC_INDEX              29
#define EPIC_DAC0_INDEX             30
#define EPIC_DAC1_INDEX             31


// DMA request lines
//
#define DMA_UART_0_INDEX            0
#define DMA_UART_1_INDEX            1
#define DMA_CRYPTO_INDEX            2
#define DMA_SPI_0_INDEX             3
#define DMA_SPI_1_INDEX             4
#define DMA_I2C_0_INDEX             5
#define DMA_I2C_1_INDEX             6
#define DMA_SPIFI_INDEX             7
#define DMA_TIMER32_1_INDEX         8
#define DMA_TIMER32_2_INDEX         9
#define DMA_DAC0_INDEX              10
#define DMA_DAC1_INDEX              11
#define DMA_TIMER32_0_INDEX         12


#endif // 32_MEMORY_MAP_H_INCLUDED
