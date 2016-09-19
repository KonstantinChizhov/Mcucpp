#pragma once

#include "ioreg.h"
#include "stm32f0xx.h"
#include "clock.h"
#include "iopins.h"
#include "pinlist.h"
#include "enum.h"

namespace Mcucpp
{
  using namespace IO;

  namespace SpiPrivate
  {
    class SpiBase
    {
      static const uint16_t SPI_Mode_Select = 0xF7FF;
     public:

      enum ClockDivider
      {
        Div2 = 0,
        Div4 = SPI_CR1_BR_0,
        Div8 = SPI_CR1_BR_1,
        Div16 = SPI_CR1_BR_0 | SPI_CR1_BR_1,
        Div32 = SPI_CR1_BR_2,
        Div64 = SPI_CR1_BR_2 | SPI_CR1_BR_0,
        Div128 = SPI_CR1_BR_2 | SPI_CR1_BR_1,
        Div256 = SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0,
        Fastest = Div2,
        Fast = Div8,
        Medium = Div32,
        Slow = Div128,
        Slowest = Div256,
      };

      enum ModeFlags
      {
        SoftSlaveControl = SPI_CR1_SSM,
        HardSlaveControl = 0,
        Master = SPI_CR1_MSTR | SPI_CR1_SSI,
        Slave = 0,
        DataSize16 = SPI_CR2_DS,
        DataSize8 = (SPI_CR2_DS_0 | SPI_CR2_DS_1 | SPI_CR2_DS_2),
        ClockPolarityLow = 0,
        ClockPolarityHigh = SPI_CR1_CPOL,
        ClockPhase1Edge = 0,
        ClockPhase2Edge = SPI_CR1_CPHA,
        LsbFirst = SPI_CR1_LSBFIRST,
        MsbFirst = 0
      };

      enum RemapPins
      {
        eNoRemap,
        eRemap1
      };
      enum MaskPin
      {
        eMOSIPin = 0x01,
        eMISOPin = 0x02,
        eSCKPin = 0x04,
        eNSSPin = 0x08,
        eDefaultPins = eMOSIPin | eMISOPin | eSCKPin,
        eMasterPins = eMOSIPin | eSCKPin,
        eSlavePins = eMOSIPin | eMISOPin | eSCKPin | eNSSPin
      };

      enum InterrupFlags
      {
        NoInterrupt = 0,
        TxEmptyInt = SPI_CR2_TXEIE,
        TxCompleteInt = SPI_CR2_TXEIE,
        RxNotEmptyInt = SPI_CR2_RXNEIE,
        ErrorInt = SPI_CR2_ERRIE,

        AllInterrupts = TxEmptyInt | RxNotEmptyInt | ErrorInt
      };

      enum Error
      {
        NoError = 0,
        OverrunError = SPI_SR_OVR,
        CRCError = SPI_SR_CRCERR,
        FramingError = SPI_SR_FRE ,
        ParityError = SPI_SR_MODF
      };
    };

    DECLARE_ENUM_OPERATIONS( SpiBase::MaskPin );

    inline SpiBase::ModeFlags operator|( SpiBase::ModeFlags left, SpiBase::ModeFlags right )
    {
      return static_cast< SpiBase::ModeFlags >( static_cast< int >( left ) | static_cast< int >( right ) );
    }

    template< class Cr1, class Cr2, class Sr, class Dr, class Crcpr, class RxCrcr, class TxCrcr,
              class I2SCfgr, class I2Spr, IRQn_Type IQRNumber, class ClockCtrl,
              class MOSI_Pins, class MISO_Pins, class SCK_Pins, class NSS_Pins, class AltFunction >
    class Spi : public SpiBase
    {
     public:

      static void Enable( )
      {
        ClockCtrl::Enable( );
      }

      static void Disable( )
      {
        ClockCtrl::Disable( );
      }

      static void Init( ClockDivider divider, ModeFlags mode = Master | SoftSlaveControl )
      {
        Enable( );
        Cr1::Set( ( unsigned )divider | SPI_CR1_SPE | mode );
        Cr2::Or( SPI_CR2_SSOE );
        I2SCfgr::And( SPI_Mode_Select );
      }

      static void Write( uint8_t outValue )
      {
        while ((Sr::Get( ) & SPI_SR_TXE) == 0);
        Dr::Set( outValue );
      }

      static uint8_t Read( )
      {
        uint16_t timeout = 5000;
        while ((Sr::Get( ) & SPI_SR_RXNE) == 0 && timeout--);
        if (timeout != 0)
          return Dr::Get( );
        return 0xff;
      }

      static uint8_t ReadWrite( uint8_t outValue )
      {
        Write( outValue );
        return Read( );
      }

      static void EnableSoftSSControl( )
      {
        Cr1::Or( SPI_CR1_SSM );
      }

      static void SetSS( )
      {
        Cr1::Or( SPI_CR1_SSI );
      }

      static void ClearSS( )
      {
        Cr1::And( ~SPI_CR1_SSI );
      }


      template< bool IsNvicConfig >
      static void EnableInterrupt( InterruptFlags interrupt = UpdateInt, uint32_t priority = 3 )
      {
        Regs( )->DIER |= interrupt;

        if (IsNvicConfig == true && interruptFlags != 0)
        {
          NVIC_SetPriority( IQRNumber, priority );
          NVIC_EnableIRQ( IQRNumber );
        }
      }

      static void DisableInterrupt( InterruptFlags interrupt = UpdateInt )
      {
        Regs( )->DIER &= ~interrupt;
      }

      static InterruptFlags IsInterrupt( )
      {
        return ( InterruptFlags )(Regs( )->SR & TIM_SR_UIF);
      }

      static void ClearInterruptFlag( InterruptFlags interrupt = UpdateInt )
      {
        Regs( )->SR &= ~( uint32_t )interrupt;
      }
      static InterruptFlags GetInterruptFlag( InterruptFlags interrupt = UpdateInt )
      {
        return static_cast< InterruptFlags >( Regs( )->SR & interrupt & Regs( )->DIER );
      }

      template< RemapPins pinsRemap, MaskPin maskPins = eDefaultPins >
      static void SelectPins( )
      {
        if (maskPins & eMOSIPin)
        {
          STATIC_ASSERT( pinsRemap <= MOSI_Pins::Length );
          typedef typename MOSI_Pins:: template Pin< pinsRemap > Pin1;

          Pin1:: AltFuncNumber( AltFunction::PinAltFunc[ pinsRemap ] );
          Pin1:: template SetConfiguration< Pin1::Port::AltFunc >( );
        }
        if (maskPins & eMISOPin)
        {
          STATIC_ASSERT( pinsRemap <= MISO_Pins::Length );
          typedef typename MISO_Pins:: template Pin< pinsRemap > Pin2;

          Pin2:: AltFuncNumber( AltFunction::PinAltFunc[ pinsRemap ] );
          Pin2:: template SetConfiguration< Pin2::Port::AltFunc >( );
        }
        if (maskPins & eSCKPin)
        {
          STATIC_ASSERT( pinsRemap <= SCK_Pins::Length );
          typedef typename SCK_Pins:: template Pin< pinsRemap > Pin3;

          Pin3:: AltFuncNumber( AltFunction::PinAltFunc[ pinsRemap ] );
          Pin3:: template SetConfiguration< Pin3::Port::AltFunc >( );
        }
        if (maskPins & eNSSPin)
        {
          STATIC_ASSERT( pinsRemap <= NSS_Pins::Length );
          typedef typename NSS_Pins:: template Pin< pinsRemap > Pin4;

          Pin4:: AltFuncNumber( AltFunction::PinAltFunc[ pinsRemap ] );
          Pin4:: template SetConfiguration< Pin4::Port::AltFunc >( );
        }
      }
      template< RemapPins pinsRemap, uint16_t maskPins = (eMOSIPin | eMISOPin | eSCKPin) >
      static void DisablePins( )
      {
        if (maskPins & eMOSIPin)
        {
          STATIC_ASSERT( pinsRemap <= MOSI_Pins::Length );
          typedef typename MOSI_Pins:: template Pin< pinsRemap > Pin1;

          Pin1:: template SetConfiguration< Pin1::Port::In >( );
        }

        if (maskPins & eMISOPin)
        {
          STATIC_ASSERT( pinsRemap <= MISO_Pins::Length );
          typedef typename MISO_Pins:: template Pin< pinsRemap > Pin2;

          Pin2:: template SetConfiguration< Pin2::Port::In >( );
        }
        if (maskPins & eSCKPin)
        {
          STATIC_ASSERT( pinsRemap <= SCK_Pins::Length );
          typedef typename SCK_Pins:: template Pin< pinsRemap > Pin3;

          Pin3:: template SetConfiguration< Pin3::Port::In >( );
        }
        if (maskPins & eNSSPin)
        {
          STATIC_ASSERT( pinsRemap <= NSS_Pins::Length );
          typedef typename NSS_Pins:: template Pin< pinsRemap > Pin4;

          Pin4:: template SetConfiguration< Pin4::Port::In >( );
        }
      }

      class Crc
      {
        static void Enable( uint16_t polynom = 0x0007 )
        {
          Crcpr::Set( polynom );
          Cr1::Or( SPI_CR1_CRCEN );
        }

        static void Transmit( )
        {
          Cr1::Or( SPI_CR1_CRCNEXT );
        }

        static uint16_t RxCrc( )
        {
          return RxCrcr::Get( );
        }

        static uint16_t TxCrc( )
        {
          return TxCrcr::Get( );
        }
      };
    };

    typedef IO::PinList< IO::Pa7, IO::Pb5 > Spi1_MOSI_Pins;
    typedef IO::PinList< IO::Pa6, IO::Pb4 > Spi1_MISO_Pins;
    typedef IO::PinList< IO::Pa5, IO::Pb3 > Spi1_SCK_Pins;
    typedef IO::PinList< IO::Pa4, IO::Pa15 > Spi1_NSS_Pins;


    struct Spi1AlterFuncPin {
      enum
      {Numer = 1};
      static const uint8_t             PinAltFunc[];
    };
    const uint8_t Spi1AlterFuncPin:: PinAltFunc[] = { 0, 0 };


    typedef IO::PinList< IO::Pb15 > Spi2_MOSI_Pins;
    typedef IO::PinList< IO::Pb14 > Spi2_MISO_Pins;
    typedef IO::PinList< IO::Pb13, IO::Pb10 > Spi2_SCK_Pins;
    typedef IO::PinList< IO::Pb13, IO::Pb9 > Spi2_NSS_Pins;

    struct Spi2AlterFuncPin {
      enum
      {Numer = 2};
      static const uint8_t             PinAltFunc[];
    };
    const uint8_t Spi2AlterFuncPin:: PinAltFunc[] = { 0, 5 };


  }



#define DECLARE_SPI(CR1, CR2, SR, DR, CRCPR, RXCRCR, TXCRCR, I2SCFGR, I2SPR, IRQ, className) \
   namespace SpiPrivate{\
                IO_REG_WRAPPER(CR1, className ## Cr1, uint32_t);\
                IO_REG_WRAPPER(CR2, className ## Cr2, uint32_t);\
                IO_REG_WRAPPER(SR, className ## Sr, uint32_t);\
                IO_REG_WRAPPER(DR, className ## Dr, uint32_t);\
                IO_REG_WRAPPER(CRCPR, className ## Crcpr, uint32_t);\
                IO_REG_WRAPPER(RXCRCR, className ## RxCrcr, uint32_t);\
                IO_REG_WRAPPER(TXCRCR, className ## TxCrcr, uint32_t);\
                IO_REG_WRAPPER(I2SCFGR, className ## I2SCfgr, uint32_t);\
                IO_REG_WRAPPER(I2SPR, className ## I2Spr, uint32_t);\
        }\
          typedef SpiPrivate::Spi<\
                        SpiPrivate::className ## Cr1, \
                        SpiPrivate::className ## Cr2, \
                        SpiPrivate::className ## Sr, \
                        SpiPrivate::className ## Dr, \
                        SpiPrivate::className ## Crcpr, \
                        SpiPrivate::className ## RxCrcr, \
                        SpiPrivate::className ## TxCrcr, \
                        SpiPrivate::className ## I2SCfgr, \
                        SpiPrivate::className ## I2Spr, \
                        IRQ,\
                        Clock:: className ## Clock,\
                        SpiPrivate::className ## _MOSI_Pins, \
                        SpiPrivate::className ## _MISO_Pins, \
                        SpiPrivate::className ## _SCK_Pins, \
                        SpiPrivate::className ## _NSS_Pins, \
                        SpiPrivate::className ## AlterFuncPin\
                        > className;


  DECLARE_SPI( SPI1->CR1, SPI1->CR2, SPI1->SR, SPI1->DR, SPI1->CRCPR, SPI1->RXCRCR, SPI1->TXCRCR, SPI1->I2SCFGR, SPI1->I2SPR, SPI1_IRQn, Spi1)

  DECLARE_SPI(SPI2->CR1, SPI2->CR2, SPI2->SR, SPI2->DR, SPI2->CRCPR, SPI2->RXCRCR, SPI2->TXCRCR, SPI2->I2SCFGR, SPI2->I2SPR, SPI2_IRQn, Spi2)



}
