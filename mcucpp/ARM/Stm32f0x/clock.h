#pragma once


#include "stm32f0xx.h"
#include "ioreg.h"
#include "delay.h"
#include "TypeManip.h"
#include "template_utils.h"

#ifndef HSE_VALUE
  #warning HSE_VALUE не определена. HSE_VALUE по умолчанию 8 MHZ.
#define HSE_VALUE 8000000u
#endif

#if defined(STM32F030x6) || defined(STM32F030x8) || defined(STM32F031x6) || \
        defined(STM32F038xx) || defined(STM32F051x8) || defined(STM32F058xx)
  #define VariantClock1
#elif defined(STM32F042) ||defined(STM32F048xx) || defined(STM32F071xB) || \
        defined(STM32F072xB) || defined(STM32F078xx)
  #define VariantClock2
#else
  #error "Please select first the target STM32F0xx device used in your application "
#endif

namespace Mcucpp
{
  namespace Clock
  {
#define FCPU_MAX      ((uint32_t) 48000000u)
#define FPeriph_MAX  ((uint32_t) 48000000u )

    IO_REG_WRAPPER( RCC->CR, RegClockControl, uint32_t);
    IO_REG_WRAPPER( RCC->CR2, RegClockControl2, uint32_t);
    IO_REG_WRAPPER( RCC->CFGR, RegClockConfig, uint32_t);
    IO_REG_WRAPPER( RCC->CFGR2, RegClockConfig2, uint32_t);
    IO_REG_WRAPPER( RCC->CFGR3, RegClockConfig3, uint32_t);
    IO_REG_WRAPPER( RCC->BDCR, RegClockStatus, uint32_t);
    IO_REG_WRAPPER( RCC->APB2ENR, PeriphClockEnable2, uint32_t);
    IO_REG_WRAPPER( RCC->APB1ENR, PeriphClockEnable1, uint32_t);
    IO_REG_WRAPPER( RCC->AHBENR, AhbClockEnableReg, uint32_t);
    IO_REG_WRAPPER( FLASH->ACR, RegFlashAcess, uint32_t);

    IO_STRUCT_WRAPPER( RCC, RccReg, uint32_t );

    template< uint32_t FcpuMax, uint32_t Fapb1Max >
    class SysClockBase
    {
     public:
      enum ClockValue
      {
        HSI_VALUE = 8000000u,
        HSI14_VALUE = 14000000u,
        LSI_VALUE = 40000u,
#if defined( VariantClock2)
        HSI48_VALUE = 48000000u
#endif
      };
      enum ClockSource
      {
        eHSI_SOURSE = RCC_CFGR_SW_HSI,                /** Внутрений генератор */
        eHSE_SOURSE = RCC_CFGR_SW_HSE,                /** Внешний кварц */
        ePLL_SOURSE = RCC_CFGR_SW_PLL,                /** PLL */
#if defined( VariantClock2)
        eHSI_SOURSE_48MHz = RCC_CFGR_SW_HSI48 ,       /** Внутрений генератор 48МГц*/
#endif
        eMaskSourseClock = RCC_CFGR_SW
      };

      enum PLL_Source
      {
        Internal_8MHzDIV2 = RCC_CFGR_PLLSRC_HSI_DIV2,
        External = RCC_CFGR_PLLSRC_HSE_PREDIV,
#ifdef VariantClock2
        Internal_8MHz = RCC_CFGR_PLLSRC_HSI_PREDIV,
        Internal_48MHz = RCC_CFGR_PLLSRC_HSI48_PREDIV
#endif
      };
      enum eUSB_Source
      {
        eUSB_Int_48MHz = 0,
        eUSB_Pll_Source = RCC_CFGR3_USBSW
      };
      enum Pll_value
      {
        eMaxMultp = 16,                               /* максимальный умножитель*/
        eMaxDiv = 16                                  /* максимальный делитель*/
      };
      enum Timing
      {
        HSE_TIMEOUT_VALUE = (( uint32_t )5000),       /*!< Time out for HSE start up, in ms */
        HSI_TIMEOUT_VALUE = (( uint32_t )100),        /* 100 ms */
        LSI_TIMEOUT_VALUE = (( uint32_t )100),        /* 100 ms */
        LSE_TIMEOUT_VALUE = (( uint32_t )5000),       /* 5 s    */
        HSI14_TIMEOUT_VALUE = (( uint32_t )100),      /* 100 ms */
        HSI48_TIMEOUT_VALUE = (( uint32_t )100),      /* 100 ms */
        PLL_TIMEOUT_VALUE = (( uint32_t )100),        /* 100 ms */
        CLOCKSWITCH_TIMEOUT_VALUE = (( uint32_t )5000) /* 5 s    */
      };

      enum ClockStatus
      {
        eClockOk,                                     /**  */
        eBadHSE,                                      /** Внешний кварц неисправен */
        eBadPll,                                      /** Неисправность PLL */
        eBadNewClock                                  /** Неверное значение новой частоты */
      };

      /**============================================================================
       * @brief   Установка частоты ядра и перефериии
       * 
       * @param SourceClock - источник частоты @see ClockSourse
       *                   
       * @param newClock - частота ядра в герцах
       * 
       * @return ClockStatus - результат установки частоты контроллера
       *    @see ClockStatus
       */
      template< ClockSource SourceClock, uint32_t newClock >
      static ClockStatus Set( )
      {
        uint32_t    i              = 0;
        uint32_t    clockGenerator = HSI_VALUE;
        ClockSource sourseClock    = eHSI_SOURSE;

        if( SourceClock == eHSE_SOURSE )
        {
          RegClockControl::Or( RCC_CR_HSEON );

          while( RegClockControl:: template BitIsClear< Util::BitNumber< RCC_CR_HSERDY >::value >( ) )
          {
            Mcucpp::template delay_us< 1, HSI_VALUE >( );
            if( i++ > (HSE_TIMEOUT_VALUE * 1000) )
              return eBadHSE;
          }
          clockGenerator = HSE_VALUE;
          sourseClock = eHSE_SOURSE;

        }
        else if( SourceClock == eHSI_SOURSE_48MHz )
        {
          RegClockControl2::Or( RCC_CR2_HSI48ON );

          while( RegClockControl2:: template BitIsClear< Util::BitNumber< RCC_CR2_HSI48RDY >::value >( ) )
          {
            Mcucpp::template delay_us< 1, HSI_VALUE >( );
            if( i++ > (HSI48_TIMEOUT_VALUE * 1000) )
              return eBadHSE;
          }

          clockGenerator = HSI48_VALUE;
          sourseClock = eHSI_SOURSE_48MHz;
        }

        if( newClock > clockGenerator )
        {
          // ИСПОЛЬЗОВАНИЕ PLL
          PLL_Source pllSourse = Internal_8MHz;
          i = 1;

#ifdef VariantClock2
          switch( SourceClock )
          {
          case eHSE_SOURSE:
            pllSourse = External;
            break;

          case eHSI_SOURSE_48MHz:
            pllSourse = Internal_48MHz;
            break;

          default :
            return eBadNewClock;
          }
          do
          {
            if( i > eMaxDiv )
              return eBadNewClock;

            if( (clockGenerator % i) != 0 )
              continue;

            if( (newClock % (clockGenerator / i)) == 0 )
            {
              clockGenerator = clockGenerator;
              break;
            }
          }
          while( i++ );
#endif
          // установка пределителя
          RegClockConfig2::Set( i - 1 );

          // установка умножителя
          RegClockConfig::AndOr( ~RCC_CFGR_PLLMUL, (newClock / (clockGenerator / i) - 2) << 18 );

          // установка источника PLL
          RegClockConfig::AndOr( ~RCC_CFGR_PLLSRC, pllSourse );

          //включение PLL
          RegClockControl::Or( RCC_CR_PLLON );

          // ожидание готовночти
          i = 0;
          while( RegClockControl::BitIsClear< Util::BitNumber< RCC_CR_PLLRDY >::value >( ) )
          {
            Mcucpp::template delay_us< 1, HSI_VALUE >( );
            if( i++ > (PLL_TIMEOUT_VALUE * 1000) )
              return eBadHSE;
          }


          sourseClock = ePLL_SOURSE;
        }

        RegClockConfig::Or( sourseClock );

        while( ((RegClockConfig::Get( ) >> 2) & 0x00000003) != sourseClock )
        {
          Mcucpp::template delay_us< 1, HSI_VALUE >( );
          if( i++ > (CLOCKSWITCH_TIMEOUT_VALUE * 1000) )
            return eBadHSE;
        }

        RegFlashAcess::And( ~FLASH_ACR_LATENCY );

        if( newClock <= 24000000u )
          RegFlashAcess::And( ~FLASH_ACR_LATENCY );
        else if( newClock <= 48000000u )
          RegFlashAcess::Or( FLASH_ACR_LATENCY );
        else
          return eBadNewClock;

        //CpuFreq = newClock;
        CpuPeriph = newClock;

        return eClockOk;
      }

      static bool HSI14MHz_Enable( )
      {
        uint32_t timeOut = HSI14_TIMEOUT_VALUE * 1000;

        RegClockControl2:: Or( RCC_CR2_HSI14ON );

        while( RegClockControl2:: template BitIsClear< Util::BitNumber< RCC_CR2_HSI14RDY >::value >( ) &&
               timeOut-- )
        {
          Mcucpp::template delay_us< 1, HSI_VALUE >( );
        }

        return( timeOut != 0 );
      }
      static void HSI14MHz_Disable( )
      {
        RegClockControl2:: And( ~RCC_CR2_HSI14ON );
      }


      static const unsigned long CpuFreq    = F_CPU;
      static unsigned long       CpuPeriph;
      static unsigned long       CpuPeriph1;

      static unsigned long FCore( ) {return CpuFreq;}

      static unsigned long FPeriph1( ) {return CpuPeriph;}
      static unsigned long FPeriph2( ) {return CpuPeriph1;}

    };

    template< uint32_t FcpuMax, uint32_t Fapb1Max >
    unsigned long SysClockBase< FcpuMax, Fapb1Max >       ::CpuPeriph1;

                template< uint32_t FcpuMax, uint32_t Fapb1Max >
    unsigned long SysClockBase< FcpuMax, Fapb1Max >       ::CpuPeriph;

                template< uint32_t FcpuMax, uint32_t Fapb1Max >
    const unsigned long SysClockBase< FcpuMax, Fapb1Max > ::CpuFreq;



    typedef SysClockBase< 48000000u, 48000000u > SysClock;


    template< class Reg, unsigned Mask >
    class ClockControl
    {
     public:

      enum DomainClock
      {
        APB1_Domain,                                  /*< Тактирование переферии от APB1*/
        APB2_Domain,                                  /*< Тактирование переферии от APB2*/
        AHB_Domain                                    /*< Тактирование переферии от AHB*/
      };

      /**============================================================================
      * @brief  Включает тактирование перефериии 
      */
      static void Enable( )
      {
        Reg::Or( Mask );
      }
      /**============================================================================
      * @brief  Включает тактирование перефериии USB
      */
      static SysClock::ClockStatus Enable( SysClock::eUSB_Source usbSource )
      {
        uint32_t i = 0;

        if( (usbSource == SysClock::eUSB_Int_48MHz) &&
            (RegClockControl2:: template BitIsClear< Util::BitNumber< RCC_CR2_HSI48RDY >::value >( ) == true) )
        {
          //включение генератора 48МГц
          RegClockControl2::Or( RCC_CR2_HSI48ON );

          // ожидание готовности
          i = 0;
          while( RegClockControl2:: template BitIsClear< Util::BitNumber< RCC_CR2_HSI48RDY >::value >( ) )
          {
            Mcucpp::template delay_us< 1, F_CPU >( );
            if( i++ > (SysClock::HSI48_TIMEOUT_VALUE * 1000) )
              return SysClock::eBadHSE;
          }
        }

        RegClockConfig3::Or( usbSource );

        Reg::Or( Mask );

        return SysClock::eClockOk;
      }

      /**============================================================================
       * @brief  Выключает тактирование перефериии 
       */
      static void Disable( )
      {
        Reg::And( ~Mask );
      }
      /**============================================================================
       * @brief Возвращает входную частоту  переферии
       * 
       * @return unsigned long - частота в герцах
       */
      static unsigned long GetClock( )
      {
        if( Loki::IsSameType< Reg, PeriphClockEnable1 >::value )
          return Clock::SysClock::FPeriph1( );
        else if( Loki::IsSameType< Reg, PeriphClockEnable2 >::value )
          return SysClock::FPeriph2( );
        else
          return SysClock::FCore( );
      }
      /**============================================================================
      * @brief  Определяет домен тактирования 
      * 
      * @return @see DomainClock   
      */
      static DomainClock GetDomain( )
      {
        if( Loki::IsSameType< Reg, PeriphClockEnable1 >::value )
          return APB1_Domain;
        else if( Loki::IsSameType< Reg, PeriphClockEnable2 >::value )
          return APB2_Domain;
        else
          return AHB_Domain;
      }

    };


    typedef ClockControl< AhbClockEnableReg, RCC_AHBENR_DMAEN > Dma1Clock;
    typedef ClockControl< AhbClockEnableReg, RCC_AHBENR_SRAMEN > SramClock;
    typedef ClockControl< AhbClockEnableReg, RCC_AHBENR_FLITFEN > FlitfClock;
    typedef ClockControl< AhbClockEnableReg, RCC_AHBENR_CRCEN > CrcfClock;
    typedef ClockControl< AhbClockEnableReg, RCC_AHBENR_GPIOAEN > PortaClock;
    typedef ClockControl< AhbClockEnableReg, RCC_AHBENR_GPIOBEN > PortbClock;
    typedef ClockControl< AhbClockEnableReg, RCC_AHBENR_GPIOCEN > PortcClock;
    typedef ClockControl< AhbClockEnableReg, RCC_AHBENR_GPIOFEN > PortfClock;
#if defined(STM32F071xB) || defined(STM32F072xB) || defined(STM32F078xx)
    typedef ClockControl< AhbClockEnableReg, RCC_AHBENR_GPIODEN > PortdClock;
    typedef ClockControl< AhbClockEnableReg, RCC_AHBENR_GPIOEEN > PorteClock;
#endif
    typedef ClockControl< AhbClockEnableReg, RCC_AHBENR_TSCEN > TscClock;



    typedef ClockControl< PeriphClockEnable2, RCC_APB2ENR_SYSCFGCOMPEN > AfioClock;
    typedef ClockControl< PeriphClockEnable2, RCC_APB2ENR_ADCEN > Adc1Clock;
    typedef ClockControl< PeriphClockEnable2, RCC_APB2ENR_TIM1EN > Tim1Clock;
    typedef ClockControl< PeriphClockEnable2, RCC_APB2ENR_SPI1EN > Spi1Clock;
    typedef ClockControl< PeriphClockEnable2, RCC_APB2ENR_USART1EN > Usart1Clock;
#if defined( STM32F072xB)
    typedef ClockControl< PeriphClockEnable2, RCC_APB2ENR_TIM15EN > Tim15Clock;
#endif
    typedef ClockControl< PeriphClockEnable2, RCC_APB2ENR_TIM16EN > Tim16Clock;
    typedef ClockControl< PeriphClockEnable2, RCC_APB2ENR_TIM17EN > Tim17Clock;
    typedef ClockControl< PeriphClockEnable2, RCC_APB2ENR_DBGMCUEN > DbgmClock;


    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_TIM2EN > Tim2Clock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_TIM3EN > Tim3Clock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_TIM14EN > Tim14Clock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_WWDGEN > WatchDogClock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_SPI2EN > Spi2Clock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_USART2EN > Usart2Clock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_USART3EN > Usart3Clock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_USART4EN > Usart4Clock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_I2C1EN > I2c1Clock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_I2C2EN > I2c2Clock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_USBEN > UsbClock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_CANEN > CanClock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_CRSEN > CrsClock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_PWREN > PwrClock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_DACEN > DacClock;
    typedef ClockControl< PeriphClockEnable1, RCC_APB1ENR_CECEN > CecClock;

  }
}
