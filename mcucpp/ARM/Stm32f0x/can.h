/*****************************************************************************
*
* Author       : Хоменко С.П.
* Created      : 04.12.2014
* $Rev:: 101  $:  Revision of last commit
* $Date:: 2015-03-24 13:22:09#$:  Date of last commit
* Target MCU   : STM32F042C8 
* Compiler     : Keil 5.05   
* Editor Tabs  : 2
*
* NOTE: 
******************************************************************************/



#pragma once

#include "ioreg.h"
#include "stm32f0xx.h"
#include "clock.h"
#include "iopins.h"
#include "pinlist.h"
#include "enum.h"
#include "debug.h"

#include "Typelist.h"


namespace Mcucpp
{
  using namespace IO;

  class  CanBase
  {
   public:

    /** \enum tMode
    * @brief Режимы CAN
    */
    enum eComand
    {
      NoComand,
      InitialReq =      CAN_MCR_INRQ,                 /*!< 0x0001 Initialization Request */
      SleepReq =        CAN_MCR_SLEEP,                /*!< 0x0002 Sleep Mode Request */
      TransmitPriorty = CAN_MCR_TXFP,                 /*!< 0x0004 Transmit FIFO Priority */
      ReceiveLocked =   CAN_MCR_RFLM,                 /*!< 0x0008 Receive FIFO Locked Mode */
      NoRetrans =       CAN_MCR_NART,                 /*!< 0x0010 No Automatic Retransmission */
      AutoWakeup =      CAN_MCR_AWUM,                 /*!< 0x0020 Automatic Wakeup Mode */
      AutomaticBusOff =  CAN_MCR_ABOM,                /*!< 0x0040 Automatic Bus-Off Management */
      TimeTriggered  =   CAN_MCR_TTCM,                /*!< 0x0080 Time Triggered Communication Mode */
      Reset  =    CAN_MCR_RESET,                      /*!< 0x8000 bxCAN software master reset */
      Debug  =    0x00010000,                         /*!<  Debug freeze*/
      ReceiveLocDebug  =    Debug | ReceiveLocked
    };
    enum eMode
    {
      ModeNormal,                                     /*!< Normal mode   */
      ModeLoopback = CAN_BTR_LBKM,                    /*!< Loopback mode */
      ModeSilent  =   CAN_BTR_SILM,                   /*!< Silent mode   */
      ModeSilentLoopback = CAN_BTR_LBKM | CAN_BTR_SILM /*!< Loopback combined with silent mode */
    };

    enum eStatusHrw
    {
      Initial =    CAN_MSR_INAK  ,                    /*!< 0x0001 Initialization Acknowledge */
      Sleep =    CAN_MSR_SLAK  ,                      /*!< 0x0002 Sleep Acknowledge */
      Error =    CAN_MSR_ERRI  ,                      /*!< 0x0004 Error Interrupt */
      Wakeup =    CAN_MSR_WKUI  ,                     /*!< 0x0008 Wakeup Interrupt */
      SleepAckInt =    CAN_MSR_SLAKI ,                /*!< 0x0010 Sleep Acknowledge Interrupt */
      Transmit =    CAN_MSR_TXM   ,                   /*!< 0x0100 Transmit Mode */
      Receive =    CAN_MSR_RXM   ,                    /*!< 0x0200 Receive Mode */
      LastRxPin =    CAN_MSR_SAMP  ,                  /*!< 0x0400 Last Sample Point */
      ActualRxPin =    CAN_MSR_RX                     /*!< 0x0800 CAN Rx Signal */
    };


    enum eInterruptFlags
    {
      TxEmptyInt  = CAN_IER_TMEIE ,                   /*!< 0x00001 Transmit Mailbox Empty Interrupt*/
      Rx0_NotEmptyInt =  CAN_IER_FMPIE0,              /*!< 0x00002 FIFO Message Pending Interrupt E*/
      Rx0_FullInt = CAN_IER_FFIE0 ,                   /*!< 0x00004 FIFO Full Interrupt Enable */
      Rx0_OverInt = CAN_IER_FOVIE0,                   /*!< 0x00008 FIFO Overrun Interrupt Enable */
      Rx1_NotEmptyInt = CAN_IER_FMPIE1,               /*!< 0x00010 FIFO Message Pending Interrupt E*/
      Rx1_FullInt = CAN_IER_FFIE1 ,                   /*!< 0x00020 FIFO Full Interrupt Enable */
      Rx1_OverInt =  CAN_IER_FOVIE1,                  /*!< 0x00040 FIFO Overrun Interrupt Enable */
      ErrorWarningInt  =  CAN_IER_EWGIE ,             /*!< 0x00100 Error Warning Interrupt Enable */
      ErrorPassiveInt  =  CAN_IER_EPVIE ,             /*!< 0x00200 Error Passive Interrupt Enable */
      BusOffInt   =  CAN_IER_BOFIE ,                  /*!< 0x00400 Bus-Off Interrupt Enable */
      LastErrorCodeInt = CAN_IER_LECIE ,              /*!< 0x00800 Last Error Code Interrupt Enable*/
      ErrorInt  =    CAN_IER_ERRIE ,                  /*!< 0x08000 Error Interrupt Enable */
      WakeupInt  =   CAN_IER_WKUIE ,                  /*!< 0x10000 Wakeup Interrupt Enable */
      SleepInt     =  CAN_IER_SLKIE,                  /*!< 0x20000 Sleep Interrupt Enable */
      DefaultInt     =  TxEmptyInt | Rx0_NotEmptyInt | Rx1_NotEmptyInt,

      AllInt =  0xFFFF
    };
    enum eRemapPins
    {
      eNoRemap,
      eRemap1,
      eRemap2,
      eRemap3
    };
    enum eFilterMode
    {
      Mask,                                           /*Two 32-bit registers  in Identifier Mask mode.*/
      List                                            /*Two 32-bit registers  in Identifier List mode.*/
    };

    enum eFilterScaleConfig
    {
      Dual_16_bit,                                    /*0: Dual 16-bit scale configuration   */
      Single_32_bit                                   /*1: Single 32-bit scale configuration */
    };
    enum eFilterFifoAssignment
    {
      Fifo0,
      Fifo1
    };
    enum eTypeID
    {
      Standart,
      Extended = 0x04
    };

    struct Filter {
      eFilterMode           Mode;
      eFilterScaleConfig    Scale;
      eFilterFifoAssignment Fifo;
      eTypeID               TypeId;
      uint32_t              Mask;
      uint32_t              IdMes;
    };
  };
  DECLARE_ENUM_OPERATIONS( CanBase::eComand )
  DECLARE_ENUM_OPERATIONS( CanBase::eInterruptFlags )
  DECLARE_ENUM_OPERATIONS( CanBase::eMode )




  namespace Privat
  {
    template< class Reg,  class ClockCtrl, class PinListRxTx, class RemapField >
    class  Can : public CanBase
    {


     public:

      static bool Init( eMode mode = ModeNormal, eComand comand = NoComand,
                        eRemapPins pins = eNoRemap, uint32_t  baud = 250000u )
      {
        uint32_t  i = 0;

        SelectPins( pins );
        Enable( );

        Reg( )->MCR |= InitialReq;

        while ((Reg( )->MSR & Initial) == 0)
        {
          delay_us< 1, F_CPU >( );
          if (i++ > 0xffff)
            return false;
        }

        Reg( )->MCR = comand | InitialReq;

        SetBaud( baud );


        Reg( )->MCR &= ~InitialReq;

        i = 0;
        while ((Reg( )->MSR & Initial) != 0)
        {
          delay_us< 1, F_CPU >( );
          if (i++ > 0xffff)
            return false;
        }
        return true;
      }

      template< eMode mode, eComand comand, eRemapPins pins, uint32_t  baud >
      static bool Init(  )
      {
        uint32_t  i = 0;

        SelectPins< pins >(  );
        Enable( );

        Reg( )->MCR |= InitialReq;

        while ((Reg( )->MSR & Initial) == 0)
        {
          delay_us< 1, F_CPU >( );
          if (i++ > 0xffff)
            return false;
        }

        Reg( )->MCR = comand | InitialReq;

        SetBaud< baud, 13, 2, 1  >(  );

        Reg( )->MCR &= ~InitialReq;

        i = 0;
        while ((Reg( )->MSR & Initial) != 0)
        {
          delay_us< 1, F_CPU >( );
          if (i++ > 0xffff)
            return false;
        }
        return true;
      }
      static void Enable( )                 {ClockCtrl::Enable( ); }
      static void Disable( )                {ClockCtrl::Disable( ); }

      static void SetFilter( uint8_t  number, Filter *filterConf )
      {
        uint32_t  maskBit = (1 << number);

        /* Initialisation mode for the filter */
        Reg( )->FMR |= ( uint32_t )CAN_FMR_FINIT;

        /* Filter Deactivation */
        Reg( )->FA1R &= ~maskBit;

        if (filterConf->Scale == Single_32_bit)
        {
          Reg( )->FS1R |= maskBit;

          Reg( )->sFilterRegister[ number ].FR1 = (filterConf->Mask << 3) | filterConf->TypeId;
          Reg( )->sFilterRegister[ number ].FR2 = (filterConf->IdMes << 3) | filterConf->TypeId;
        }
        else
        {
          Reg( )->FS1R &= ~maskBit;
          /*
         =======================================
           *To-Do
           для  16 битного режима
         =======================================
         */
        }

        if (filterConf->Mask == Mask)
          Reg( )->FM1R &= ~maskBit;
        else
          Reg( )->FM1R |= maskBit;

        if (filterConf->Fifo == Fifo0)
          Reg( )->FFA1R &= ~maskBit;
        else
          Reg( )->FFA1R |= maskBit;

        /* Filter activation */
        Reg( )->FA1R |= maskBit;
        /* Active mode for the filter */
        Reg( )->FMR &= ~( uint32_t )CAN_FMR_FINIT;
      }
      static void SetFilter( uint8_t  number, const Filter *filterConf )
      {
        uint32_t  maskBit = (1 << number);

        /* Initialisation mode for the filter */
        Reg( )->FMR |= ( uint32_t )CAN_FMR_FINIT;

        /* Filter Deactivation */
        Reg( )->FA1R &= ~maskBit;

        if (filterConf->Scale == Single_32_bit)
        {
          Reg( )->FS1R |= maskBit;

          Reg( )->sFilterRegister[ number ].FR1 = (filterConf->Mask << 3) | filterConf->TypeId;
          Reg( )->sFilterRegister[ number ].FR2 = (filterConf->IdMes << 3) | filterConf->TypeId;
        }
        else
        {
          Reg( )->FS1R &= ~maskBit;
          /*
         =======================================
           *To-Do
           для  16 битного режима
         =======================================
         */
        }

        if (filterConf->Mask == Mask)
          Reg( )->FM1R &= ~maskBit;
        else
          Reg( )->FM1R |= maskBit;

        if (filterConf->Fifo == Fifo0)
          Reg( )->FFA1R &= ~maskBit;
        else
          Reg( )->FFA1R |= maskBit;

        /* Filter activation */
        Reg( )->FA1R |= maskBit;
        /* Active mode for the filter */
        Reg( )->FMR &= ~( uint32_t )CAN_FMR_FINIT;
      }

      template< bool NVIC_Init >
      static void EnableInterrupt( eInterruptFlags interrupt = DefaultInt, uint32_t priority = 3 )
      {
        Reg( )->IER |= interrupt;

        if (NVIC_Init == true)
        {
          NVIC_SetPriority( CEC_CAN_IRQn, priority );
          NVIC_EnableIRQ( CEC_CAN_IRQn );
        }
      }

      static void DisableInterrupt( eInterruptFlags interrupt = DefaultInt )
      {
        Reg( )->IER &= ~interrupt;
      }
      static eStatusHrw GetStatus(  )
      {
        return (static_cast< eStatusHrw >( Reg( )->MSR ));
      }

      static void SetBaud( unsigned baud, uint8_t  seg1 = 13, uint8_t  seg2 = 2, uint8_t  sync = 1 )
      {
        Reg( )->BTR = ((ClockCtrl::GetClock( ) / baud / (seg1 + seg2 + sync)) - 1) |
            ((seg1 - 1) << 16) | ((seg2 - 1) << 20) | ((sync - 1) << 24);
      }

      template< unsigned baud, uint8_t  seg1, uint8_t  seg2, uint8_t  sync >
      static void SetBaud( )
      {
        Reg( )->BTR = ((ClockCtrl::GetClock( ) / baud / (seg1 + seg2 + sync)) - 1) |
            ((seg1 - 1) << 16) | ((seg2 - 1) << 20) | ((sync - 1) << 24);
      }

      static uint32_t GetBaud( )
      {
        return 250000u;
      }

      template< eRemapPins  pinsRemap >
      static void SelectPins( )
      {
        STATIC_ASSERT( pinsRemap <= PinListRxTx::Length / 2 );

        typedef typename PinListRxTx::template Slice< pinsRemap *2, 2 > PinsCan;

        PinsCan:: AltFuncNumber(  RemapField::PinAltFunc[ pinsRemap ] );
        PinsCan:: template SetConfiguration< PinsCan::AltFunc >(  );
        PinsCan::SetSpeed( PinsCan::Fastest );

      }
      static void SelectPins( eRemapPins  pinsRemap )
      {
        MCUCPP_ASSERT( pinsRemap <= PinListRxTx::Length / 2 );

        typedef typename PinListRxTx::ValueType Type;

        Type maskTxRx( 0x03 << ( pinsRemap * 2 ) );

        PinListRxTx::AltFuncNumber( maskTxRx, RemapField::PinAltFunc[ pinsRemap ] );
        PinListRxTx::SetConfiguration( maskTxRx, PinListRxTx::AltFunc );
        PinListRxTx:: SetSpeed( maskTxRx,  PinListRxTx::Fastest );
      }

    };


    typedef IO::PinList< IO::Pa11/*Rx*/, IO::Pa12/*Tx*/, IO::Pb8, /*Rx*/ IO::Pb9 /*Tx*/ >  Can042;

    template< uint8_t CanNum >
    struct  CanAlterFuncPin {
      enum
      {Numer = CanNum};
      static const uint8_t                       PinAltFunc[];
    };

    template<  uint8_t Timer >
    const uint8_t CanAlterFuncPin<  Timer > :: PinAltFunc[] = { 4, 4 };

    IO_STRUCT_WRAPPER( CAN, CanRegs, CAN_TypeDef );

  }





  //STM32F042
  typedef Privat::Can< Privat::CanRegs, Clock::CanClock, Privat::Can042, Privat::CanAlterFuncPin< 0 >  > CanStm32f042;


}
