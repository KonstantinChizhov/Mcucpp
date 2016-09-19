#pragma once

#include "ioreg.h"
#include "stm32f0xx.h"
#include "clock.h"

#include "enum.h"

namespace Mcucpp
{
  IO_STRUCT_WRAPPER( PWR, PowerRegs, PWR_TypeDef );
  IO_STRUCT_WRAPPER( SYSCFG, SysConfRegs, SYSCFG_TypeDef );

  class  Power
  {

   public:
    enum ePldThreshold
    {
      Lvl0 = PWR_CR_PLS_LEV0,
      Lvl1 = PWR_CR_PLS_LEV1,
      Lvl2 = PWR_CR_PLS_LEV2,
      Lvl3 = PWR_CR_PLS_LEV3,
      Lvl4 = PWR_CR_PLS_LEV4,
      Lvl5 = PWR_CR_PLS_LEV5,
      Lvl6 = PWR_CR_PLS_LEV6,
      Lvl7 = PWR_CR_PLS_LEV7
    };

    static void SetPvdThreshold( ePldThreshold lvl )
    {
      Clock::PwrClock::Enable( );
      PowerRegs( )-> CR  = ( PWR_CR_PVDE | lvl);
    }

  };
}
