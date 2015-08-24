

#pragma once
#include <../CortexM0/_debug.h>
#include "stm32f0xx.h"

namespace Mcucpp
{
  IO_STRUCT_WRAPPER( DBGMCU, DebugRegs, DBGMCU_TypeDef );

  class  DebugPeref
  {


   public:
    enum eDebPerefAPB1
    {
      NoPerf1   = 0 ,
      TIM2_STOP = DBGMCU_APB1_FZ_DBG_TIM2_STOP         , /*0x00000001 TIM2 counter stopped when core is halted */
      TIM3_STOP = DBGMCU_APB1_FZ_DBG_TIM3_STOP        , /*0x00000002 TIM3 counter stopped when core is halted */
      TIM14_STOP = DBGMCU_APB1_FZ_DBG_TIM14_STOP       , /*0x00000100 TIM14 counter stopped when core is halted */
      RTC_STOP  = DBGMCU_APB1_FZ_DBG_RTC_STOP         , /*0x00000400 RTC Calendar frozen when core is halted */
      WWDG_STOP = DBGMCU_APB1_FZ_DBG_WWDG_STOP        , /*0x00000800 Debug Window Watchdog stopped when Core is halted */
      IWDG_STOP = DBGMCU_APB1_FZ_DBG_IWDG_STOP        , /*0x00001000 Debug Independent Watchdog stopped when Core is halted */
      I2C1_SMBUS = DBGMCU_APB1_FZ_DBG_I2C1_SMBUS_TIMEOUT , /*0x00200000 I2C1 SMBUS timeout mode stopped when Core is halted */
      CAN_STOP =   DBGMCU_APB1_FZ_DBG_CAN_STOP        /*0x02000000 CAN debug stopped when Core is halted */
    };

    enum eDebPerefAPB2
    {
      NoPerf2   = 0 ,
      TIM1_STOP = DBGMCU_APB2_FZ_DBG_TIM1_STOP  ,     /*0x00000800 TIM1 counter stopped when core is halted */
      TIM16_STOP = DBGMCU_APB2_FZ_DBG_TIM16_STOP ,    /*0x00020000 TIM16 counter stopped when core is halted */
      TIM17_STOP = DBGMCU_APB2_FZ_DBG_TIM17_STOP      /*0x00040000 TIM17 counter stopped when core is halted */
    };



    static void Stop( const eDebPerefAPB1 perf1, const eDebPerefAPB2 perf2 )
    {
      Clock::DbgmClock::Enable( );
      if (perf1 != 0)
        DebugRegs( )->APB1FZ |= perf1;

      if (perf2 != 0)
        DebugRegs( )->APB2FZ |= perf2;
    }

    static     void Stop( const eDebPerefAPB1 perf1 )
    {
      Clock::DbgmClock::Enable( );
      if (perf1 != 0)
        DebugRegs( )->APB1FZ |= perf1;

    }

    template< eDebPerefAPB1 perf1, eDebPerefAPB2 perf2 >
    static void Stop( )
    {
      Clock::DbgmClock::Enable( );
      if (perf1 != 0)
        DebugRegs( )->APB1FZ |= perf1;

      if (perf2 != 0)
        DebugRegs( )->APB2FZ |= perf2;
    }
    template< eDebPerefAPB1 perf1 >
    static void Stop( )
    {
      Clock::DbgmClock::Enable( );
      if (perf1 != 0)
        DebugRegs( )->APB1FZ |= perf1;

    }
  };


  DECLARE_ENUM_OPERATIONS( DebugPeref::eDebPerefAPB1 )
  DECLARE_ENUM_OPERATIONS( DebugPeref::eDebPerefAPB2 )


}

