
#pragma once
#include <clock.h>
#include <ioreg.h>
#include <static_assert.h>
#include <iopins.h>
#include <pinlist.h>
#include <debug.h>
#include <stddef.h>
//#include <dma.h>

namespace Mcucpp
{
  class UsartBase
  {
   public:
    enum UsartMode
    {
      DataBits8 = 0,
      DataBits9 = USART_CR1_M,

      NoneParity = 0,
      EvenParity = USART_CR1_PCE,
      OddParity  = USART_CR1_PS | USART_CR1_PCE,

      NoClock = 0,

      Disabled = 0,
      RxEnable = USART_CR1_RE,
      TxEnable = USART_CR1_TE,
      RxTxEnable  = USART_CR1_RE | USART_CR1_TE,
      Default = RxTxEnable,

      OneStopBit         = 0,
      HalfStopBit        = USART_CR2_STOP_0 << 16,
      TwoStopBits        = USART_CR2_STOP_1 << 16,
      OneAndHalfStopBits = (USART_CR2_STOP_0 | USART_CR2_STOP_1) << 16
    };

    enum InterrupFlags
    {
      NoInterrupt = 0,

      ParityErrorInt = USART_CR1_PEIE,
      TxEmptyInt     = USART_CR1_TXEIE,
      TxCompleteInt  = USART_CR1_TCIE,
      RxNotEmptyInt  = USART_CR1_RXNEIE,
      IdleInt        = USART_CR1_IDLEIE,

      LineBreakInt   = USART_CR2_LBDIE,

      ErrorInt       = USART_ISR_FE | USART_ISR_NE | USART_ISR_ORE,
      CtsInt         = USART_ISR_CTS,
      AllInterrupts  = ParityErrorInt | TxEmptyInt | TxCompleteInt | RxNotEmptyInt | IdleInt | LineBreakInt | ErrorInt | CtsInt
    };

    enum Error
    {
      NoError = 0,
      OverrunError = USART_ISR_ORE,
      NoiseError = USART_ISR_NE,
      FramingError = USART_ISR_FE,
      ParityError = USART_ISR_PE
    };

    enum RemapPins
    {
      eNoRemap,
      eRemap1,
      eRemap2
    };

   protected:

    static const unsigned ErrorMask = USART_ISR_ORE | USART_ISR_NE | USART_ISR_FE | USART_ISR_PE;

    static const unsigned InterruptMask = ParityErrorInt | TxEmptyInt |
        TxCompleteInt | RxNotEmptyInt | IdleInt | LineBreakInt |
        ErrorInt | CtsInt;

    static const unsigned CR1ModeMask   =
        USART_CR1_M |
          USART_CR1_PCE |
          USART_CR1_PS |
          USART_CR1_RE |
          USART_CR1_TE;

    static const unsigned CR2ModeMask   = USART_CR2_STOP_0 | USART_CR2_STOP_1;

    enum
    {
      CR1ModeShift = 0,
      CR2ModeShift = 16
    };
  };

  inline UsartBase::UsartMode operator|( UsartBase::UsartMode left, UsartBase::UsartMode right )
  {return static_cast< UsartBase::UsartMode >( static_cast< unsigned >( left ) | static_cast< unsigned >( right ) );	}

  inline UsartBase::InterrupFlags operator|( UsartBase::InterrupFlags left, UsartBase::InterrupFlags right )
  {return static_cast< UsartBase::InterrupFlags >( static_cast< unsigned >( left ) | static_cast< unsigned >( right ) );	}

  inline UsartBase::Error operator|( UsartBase::Error left, UsartBase::Error right )
  {return static_cast< UsartBase::Error >( static_cast< unsigned >( left ) | static_cast< unsigned >( right ) );	}


  namespace Private
  {
    template< class Regs, IRQn_Type IQRNumber, class ClockCtrl, class TxPins, class RxPins, class AltFunction >
    class Usart : public UsartBase
    {
     public:

      template< unsigned long baud, UsartMode usartMode, RemapPins  pinsNum >
      static inline void Init( )
      {
        ClockCtrl::Enable( );
        SetBaud< baud >(  );

        Regs( )->CR2 = ((usartMode >> CR2ModeShift) & CR2ModeMask);
        Regs( )->CR1 = (((usartMode >> CR1ModeShift) & CR1ModeMask) | USART_CR1_UE);

        SelectTxRxPins< pinsNum >( );
      }

      static void Init( unsigned baud, UsartMode usartMode = Default )
      {
        ClockCtrl::Enable( );
        SetBaud( baud );
        Regs( )->CR2 = ((usartMode >> CR2ModeShift) & CR2ModeMask);
        Regs( )->CR1 = (((usartMode >> CR1ModeShift) & CR1ModeMask) | USART_CR1_UE);
      }

      static void SetBaud( unsigned baud )
      {
        unsigned brr = ClockCtrl::GetClock( ) / baud;
        Regs( )->BRR = brr;
      }

      template< unsigned baud >
      static void SetBaud( )
      {
        unsigned brr = ClockCtrl::GetClock( ) / baud;
        Regs( )->BRR = brr;
      }

      static void Write( uint8_t c )
      {
        while (!TxReady( ))
        ;
        Regs( )->TDR = c;
      }

      static uint8_t Read( )
      {
        while (!RxReady( ))
        ;
        return Regs( )->RDR;
      }

      static uint8_t ReadData( ) {return Regs( )->RDR;}

      static void WriteData( uint8_t c ) {Regs( )->TDR = c;}

      static uint8_t  WriteAndRead( uint8_t c )
      {
        while (!TxReady( ));
        Regs( )->TDR = c;
        while (!TxReady( ));
        return Regs( )->RDR;
      }

      static bool TxReady( )
      {
        bool dmaActive = (Regs( )->CR3 & USART_CR3_DMAT)/*&& DmaChannel::Enabled( )*/;
        return false /*(!dmaActive || DmaChannel::TrasferComplete( )) && (Regs( )->SR & USART_ISR_TXE)*/;
      }

      static bool RxReady( )
      {
        return Regs( )->SR & USART_ISR_RXNE;
      }

      template< bool IsNvicConfig >
      static void EnableInterrupt( InterrupFlags interruptFlags, uint32_t   priority = 3)
      {
        uint32_t cr1Mask = 0;
        uint32_t cr2Mask = 0;
        uint32_t cr3Mask = 0;

        if (interruptFlags & ParityErrorInt)
        cr1Mask |= USART_CR1_PEIE;

        STATIC_ASSERT(
                      USART_CR1_TXEIE  == USART_ISR_TXE &&
                      USART_CR1_TCIE   == USART_ISR_TC &&
                      USART_CR1_RXNEIE == USART_ISR_RXNE &&
                      USART_CR1_IDLEIE == USART_ISR_IDLE
                     );

        cr1Mask |= interruptFlags &(USART_CR1_TXEIE | USART_CR1_TCIE | USART_CR1_RXNEIE | USART_CR1_IDLEIE);

        if (interruptFlags & LineBreakInt)
        cr2Mask |= USART_CR2_LBDIE;

        if (interruptFlags & ErrorInt)
        cr3Mask |= USART_CR3_EIE;

        if (interruptFlags & CtsInt)
        cr3Mask |= USART_CR3_CTSIE;

        Regs()->CR1 |= cr1Mask;
        Regs()->CR2 |= cr2Mask;
        Regs()->CR3 |= cr3Mask;

        if (IsNvicConfig == true  && interruptFlags != 0) 
        {
          NVIC_SetPriority(IQRNumber, priority);
          NVIC_EnableIRQ(IQRNumber);
        }
      }
      static void EnableTxInterrupt(  )
      {
         Regs( )->CR1 |= ( USART_ISR_TC | USART_ISR_TXE);

      }
      static void DisableTxInterrupt( )
      {
        Regs( )->CR1 &= ~( USART_ISR_TC | USART_ISR_TXE);
      }
      static void DisableInterrupt( InterrupFlags interruptFlags )
      {
        uint32_t cr1Mask = 0;
        uint32_t cr2Mask = 0;
        uint32_t cr3Mask = 0;

        if (interruptFlags & ParityErrorInt)
          cr1Mask |= USART_CR1_PEIE;

        STATIC_ASSERT(
                       USART_CR1_TXEIE  == USART_ISR_TXE &&
                       USART_CR1_TCIE   == USART_ISR_TC &&
                       USART_CR1_RXNEIE == USART_ISR_RXNE &&
                       USART_CR1_IDLEIE == USART_ISR_IDLE
                      );

        cr1Mask |= interruptFlags & (USART_CR1_TXEIE | USART_CR1_TCIE | USART_CR1_RXNEIE | USART_CR1_IDLEIE);

        if (interruptFlags & LineBreakInt)
          cr2Mask |= USART_CR2_LBDIE;

        if (interruptFlags & ErrorInt)
          cr3Mask |= USART_CR3_EIE;

        if (interruptFlags & CtsInt)
          cr3Mask |= USART_CR3_CTSIE;

        Regs( )->CR1 &= ~cr1Mask;
        Regs( )->CR2 &= ~cr2Mask;
        Regs( )->CR3 &= ~cr3Mask;
      }

      static InterrupFlags InterruptSource( )
      {
        return static_cast< InterrupFlags >( Regs( )->ISR & InterruptMask );
      }

      static Error GetError( )
      {
        return static_cast< Error >( Regs( )->SR & ErrorMask );
      }

      static void ClearInterruptFlag( InterrupFlags interruptFlags )
      {
        Regs( )->ICR &= ~interruptFlags;
      }

      template< RemapPins  pinsRemap >
      static void SelectTxRxPins( )
      {
        STATIC_ASSERT( pinsRemap <= TxPins::Length );

        typedef typename TxPins:: template Pin< pinsRemap > TxPin;
        typedef typename RxPins:: template Pin< pinsRemap > RxPin;


        TxPin:: AltFuncNumber(  AltFunction::PinAltFunc[ pinsRemap ] );
        TxPin:: template SetConfiguration< TxPin::Port::AltFunc >(  );

        RxPin:: AltFuncNumber(  AltFunction::PinAltFunc[ pinsRemap ] );
        RxPin:: template SetConfiguration< RxPin::Port::AltFunc >(  );
      }

      static void SelectTxRxPins( RemapPins  pinsRemap )
      {
        MCUCPP_ASSERT( pinsRemap <= TxPins::Length );

        typedef typename TxPins::ValueType Type;
        Type maskTx( 1 << pinsRemap );
        TxPins::AltFuncNumber( maskTx, AltFunction::PinAltFunc[ pinsRemap ] );
        TxPins::SetConfiguration( maskTx, TxPins::AltFunc );

        Type maskRx( 1 << pinsRemap );
        TxPins::AltFuncNumber( maskTx, AltFunction::PinAltFunc[ pinsRemap ] );
        RxPins::SetConfiguration( maskRx, RxPins::AltFunc );
      }

      static void DeselectTxRxPins( )
      {
        typedef typename TxPins::ValueType Type;

        TxPins::SetConfiguration( ((1 << TxPins::Length) - 1), TxPins::In );
        RxPins::SetConfiguration( ((1 << RxPins::Length) - 1), RxPins::In );
      }

      static void Write( const void *data, size_t size, bool async = false )
      {
        if (async && size > 1)
        {
          while (!TxReady( ))
          ;
//          DmaChannel::ClearTrasferComplete( );
          Regs( )->CR3 |= USART_CR3_DMAT;
          Regs( )->ISR &= ~USART_ISR_TC;
//          DmaChannel::Init( DmaChannel::Mem2Periph | DmaChannel::MemIncriment, data, &Regs( )->DR, size );
        }
        else
        {
          uint8_t *ptr = ( uint8_t * )data;
          while (size--)
          {
            Write( *ptr );
            ptr++;
          }
        }
      }
    };

    typedef IO::PinList< IO::Pa9,  IO::Pb6 > Usart1TxPins;
    typedef IO::PinList< IO::Pa10, IO::Pb7 > Usart1RxPins;

    struct  Usart1AlterFuncPin {
      enum
      {Numer = 1};
      static const uint8_t               PinAltFunc[];
    };
    const uint8_t Usart1AlterFuncPin:: PinAltFunc[] = { 1, 0 };




    typedef IO::PinList< IO::Pa2, IO::Pa14 > Usart2TxPins;
    typedef IO::PinList< IO::Pa3, IO::Pa15 > Usart2RxPins;

    struct  Usart2AlterFuncPin {
      enum
      {Numer = 2};
      static const uint8_t               PinAltFunc[];
    };
    const uint8_t Usart2AlterFuncPin:: PinAltFunc[] = { 1, 1 };


  }

#define DECLARE_USART(REGS, IRQ, CLOCK, className) \
	namespace Private \
	{\
		IO_STRUCT_WRAPPER(REGS, className ## _REGS, USART_TypeDef);\
	}\
	typedef Private::Usart<\
		Private::className ## _REGS, \
		IRQ,\
		CLOCK,\
		Private::className ## TxPins, \
		Private::className ## RxPins, \
		Private::className ## AlterFuncPin\
		> className

  DECLARE_USART( USART1, USART1_IRQn, Clock::Usart1Clock, Usart1 );

  DECLARE_USART( USART2, USART2_IRQn, Clock::Usart2Clock, Usart2 );


}
