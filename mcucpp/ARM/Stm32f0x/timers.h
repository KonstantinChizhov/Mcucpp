#pragma once

#include "ioreg.h"
#include "stm32f0xx.h"
#include "clock.h"
#include "iopins.h"
#include "pinlist.h"
#include "enum.h"

#include "Typelist.h"


namespace Mcucpp
{
  using namespace IO;

  namespace Timers
  {
    namespace Private
    {
      class TimersBase
      {
       public:
        typedef uint16_t DataT;
        static const uint16_t     MaxValue = 0xffff;

            typedef uint16_t ClockDivider;
        static const ClockDivider Div1     = 1;
        static const ClockDivider Div2     = 2;
        static const ClockDivider Div4     = 4;
        static const ClockDivider Div8     = 8;
        static const ClockDivider Div16    = 16;
        static const ClockDivider Div32    = 32;
        static const ClockDivider Div64    = 64;
        static const ClockDivider Div128   = 128;
        static const ClockDivider Div256   = 256;
        static const ClockDivider Div1024  = 1024;
        static const ClockDivider Div2048  = 2048;
        static const ClockDivider Div4096  = 4096;
        static const ClockDivider Div8192  = 8192;
        static const ClockDivider Div16384 = 16384;
        static const ClockDivider Div32768 = 32768;

        /** \enum tMode
        * @brief ?????? ??????
        */
        enum tMode
        {
          eEnable = TIM_CR1_CEN,                      /**<  ?????????? */
          eUpdateDisable = TIM_CR1_UDIS,              /**<   */
          eUpdateRequestSrc = TIM_CR1_URS,            /**<   */
          eOnePulse = TIM_CR1_OPM,                    /**<   */
          eDirection = TIM_CR1_DIR                    /**<   */
        };



        enum InterruptFlags
        {
          OverflowInt = TIM_SR_UIF,
          UpdateInt = TIM_SR_UIF,
          Canal1_Int = TIM_SR_CC1IF,
          Canal2_Int = TIM_SR_CC2IF,
          Canal3_Int = TIM_SR_CC3IF,
          Canal4_Int = TIM_SR_CC4IF,

          AllInt =  0xFFFF
        };
        enum eRemapPins
        {
          eNoRemap,
          eRemap1,
          eRemap2,
          eRemap3
        };
        enum Polarity
        {
          /*< 0: non-inverted: capture is done on a rising edgeof IC1.
            When used as external trigger, IC1  is non-inverted.*/
          RisingEdge,
          /*< 1: inverted: capture is done on a falling edge of IC1. When used
           *as external trigger, IC1 is    inverted.*/
          FallingEdge
        };
        enum PolarityCompare
        {
          ActiveHigh,
          ActiveLow
        };

        enum UsedInput
        {
          /*< 00: CC1 channel is configured as output */
          Output,
          /*< 01: CC1 channel is configured as input, IC1 is mapped on TI1/TI3 */
          Input,
          /*< 10: CC1 channel is configured as input, IC1 is mapped on TI2/TI4 */
          InputMapped,
          /*< 11: CC1 channel is configured as input, IC1 ismapped on TRC. This
           *mode is working only if an internal trigger input is selected through
           TS bit (TIMx_SMCR register)*/
          InputInternalTrigger
        };
        enum CompareMode
        {
          /*These bits define the behavior of the output reference signal OC1REF from which OC1 and
          OC1N are derived.OC1REF is active high whereas OC1 and OC1N active level depends
          on CC1P and CC1NP bits.*/
          /* 000: Frozen - The comparison between the output compare register
           * TIMx_CCR1 and the counter TIMx_CNT has no effect on the outputs.
           (this mode is used to generate a timing  base).*/
          Frozen,
          /* 001: Set channel 1 to active level on match. OC1REF signal is forced
           * high when the counter TIMx_CNT matches the capture/compare register
            (TIMx_CCRX).*/
          SetActiveLevel,
          /* 010: Set channel 1 to inactive level on match. OC1REF signal is forced
           * low when the counter TIMx_CNT matches the capture/compare register
           (TIMx_CCRX).*/
          SetInactiveLevel,
          /*011: Toggle - OC1REF toggles when TIMx_CNT=TIMx_CCRX.*/
          Toggle,
          /* 100: Force inactive level - OC1REF is forced low.*/
          ForceInactiveLevel,
          /* 101: Force active level - OC1REF is forced high.  */
          ForceActiveLevel,
          /* 110: PWM mode 1 - In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCRX
           else inactive. In downcounting, channel 1 is inactive (OC1REF=‘0) as long as
           TIMx_CNT>TIMx_CCR1 else active (OC1REF=1).*/
          PWM1,
          /* 111: PWM mode 2 - In upcounting, channel 1 is inactive as long as TIMx_CNT<TIMx_CCRX
          else active. In downcounting, channel 1 is active as long as TIMx_CNT>TIMx_CCR1 else
          inactive. */
          PWM2
        };

        enum Filter
        {
          NoFilter,                                   /*< 0000: No filter, sampling is done at fDTS*/
          Fck_int_2evants,                            /*< 0001: fSAMPLING=fCK_INT, N=2     */
          Fck_int_4evants,                            /*< 0010: fSAMPLING=fCK_INT, N=4     */
          Fck_int_8evants,                            /*< 0011: fSAMPLING=fCK_INT, N=8     */
          Fdts_2_6evants,                             /*< 0100: fSAMPLING=fDTS/2, N=6      */
          Fdts_2_8evants,                             /*< 0101: fSAMPLING=fDTS/2, N=8      */
          Fdts_4_6evants,                             /*< 0110: fSAMPLING=fDTS/4, N=6      */
          Fdts_4_8evants,                             /*< 0111: fSAMPLING=fDTS/4, N=8      */
          Fdts_8_6evants,                             /*< 1000: fSAMPLING=fDTS/8, N=6      */
          Fdts_8_8evants,                             /*< 1001: fSAMPLING=fDTS/8, N=8      */
          Fdts_16_5evants,                            /*< 1010: fSAMPLING=fDTS/16, N=5     */
          Fdts_16_6evants,                            /*< 1011: fSAMPLING=fDTS/16, N=6     */
          Fdts_16_8evants,                            /*< 1100: fSAMPLING=fDTS/16, N=8     */
          Fdts_32_5evants,                            /*< 1101: fSAMPLING=fDTS/32, N=5     */
          Fdts_32_6evants,                            /*< 1110: fSAMPLING=fDTS/32, N=6     */
          Fdts_32_8evants                             /*< 1111: fSAMPLING=fDTS/32, N=8     */
        };
      };

      DECLARE_ENUM_OPERATIONS( TimersBase::InterruptFlags );
      DECLARE_ENUM_OPERATIONS( TimersBase::tMode );


      template< class Regs, class ClockEnReg, class PinList, class RemapField, IRQn_Type IQRNumber, int CanalsCaptureCompare  >
      class BaseTimer : public TimersBase
      {
       public:

        static void Enable( )                 {ClockEnReg::Enable( ); }
        static void Disable( )                {ClockEnReg::Disable( ); }
        static void Set( DataT val )           {Regs( )->CNT = val; }
        static void Update( )                  {Regs( )->EGR  |= 1; }
        static void SetDivider( ClockDivider divider ) {Regs( )->PSC = divider - 1; }
        static DataT Get( )                   {return Regs( )->CNT; }
        static DataT GetPeriod( )     {return Regs( )->ARR; }

        static void Stop( )
        {
          Regs( )->CR1 = 0;
          Regs( )->CR2 = 0;
        }

        static void Start( uint32_t us, tMode modeTimer = eEnable )
        {
          Enable( );
          SetPeriod(  us  );

          Regs( )->CR1 = modeTimer;
        }

        static void EnableInterrupt( InterruptFlags interrupt = UpdateInt, uint32_t priority = 3 )
        {
          Regs( )->DIER |= interrupt;

          NVIC_SetPriority( IQRNumber, priority );
          NVIC_EnableIRQ( IQRNumber );
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
        static InterruptFlags GetInterruptFlag(  InterruptFlags interrupt = UpdateInt )
        {
          return static_cast< InterruptFlags>(Regs( )->SR & interrupt & Regs( )->DIER);
        }

        /**============================================================================
        * @brief  ????????? ??????? ??????????
        *
        * @param uint32_t us ?????? ? ???
        */
        static void SetPeriod( uint32_t us  )
        {
          uint32_t tickPer = (ClockEnReg::GetClock( ) / 1000000u) * us;
          uint32_t delit;

//        if (ClockEnReg::GetDomain( ) == ClockEnReg::APB1_Domain)
//          tickPer *= 2;

            delit   = (tickPer % (MaxValue + 1)) == 0 ?
            tickPer / (MaxValue + 1) : (tickPer / (MaxValue + 1)) + 1;

          Regs( )->ARR = (tickPer / delit) - 1;
          Regs( )->PSC = delit - 1;
        }

        template< uint32_t FreqIn, uint32_t us >
        static void SetPeriod(  )
        {
          const uint32_t tickPer = (FreqIn / 1000000u) * us;

          const uint32_t delit   = (tickPer % (MaxValue + 1)) == 0 ?
            tickPer / (MaxValue + 1) : (tickPer / (MaxValue + 1)) + 1;

          Regs( )->ARR = (tickPer / delit) - 1;
          Regs( )->PSC     = delit - 1;
        }

        static void SetAutoReload( DataT value )
        {
          Regs( )->ARR = value;
          Regs( )->EGR |= TIM_EGR_UG;
        }

        template< int Number > class Capture;
        template< int Number > class Compare;

      };


      /**============================================================================
      * @brief  ??????????? ??????? ???????
      */
      template< class Regs, class ClockEnReg,   class PinList, class RemapField, IRQn_Type IQRNumber, int CanalsCaptureCompare   >
      template< int Number >
      class BaseTimer< Regs, ClockEnReg, PinList, RemapField,  IQRNumber,  CanalsCaptureCompare > :: Capture
      {
        STATIC_ASSERT( Number< CanalsCaptureCompare );

        IO_REG_WRAPPER( Regs( )->CCMR1, MODE1, uint16_t);
        IO_REG_WRAPPER( Regs( )->CCMR2, MODE2, uint16_t);
        IO_REG_WRAPPER( Regs( )->CCR1,Data1, uint16_t);
        IO_REG_WRAPPER( Regs( )->CCR2,Data2, uint16_t);
        IO_REG_WRAPPER( Regs( )->CCR3,Data3, uint16_t);
        IO_REG_WRAPPER( Regs( )->CCR4,Data4, uint16_t);

        typedef typename StaticIf< (Number< 2), MODE1, MODE2 >::Result  ModeReg;

        IO_BITFIELD_WRAPPER( Regs( )->CCMR1, ModeCanal1, uint16_t, 0, 8);
        IO_BITFIELD_WRAPPER( Regs( )->CCMR1, ModeCanal2, uint16_t, 8, 8);

        IO_BITFIELD_WRAPPER( Regs( )->CCMR2, ModeCanal3, uint16_t, 0, 8);
        IO_BITFIELD_WRAPPER( Regs( )->CCMR2, ModeCanal4, uint16_t, 8, 8);

        typedef LOKI_TYPELIST_4( ModeCanal1, ModeCanal2, ModeCanal3, ModeCanal4 ) ModeList;

        typedef typename TypeAt< ModeList, Number >::Result ModeBitField;

        typedef LOKI_TYPELIST_4( Data1, Data2, Data3, Data4 ) DataList;

        static const unsigned Length         = PinList::Length;
        static const unsigned LengthPerCanal = PinList::Length / CanalsCaptureCompare;

        typedef typename PinList::template Slice< Number* LengthPerCanal, LengthPerCanal > PinsCanal;

        typedef typename TypeAt< DataList, Number >::Result CanalData;


       public:
        typedef uint16_t DataT;
        static const uint16_t       MaxValue     = 0xffff;
        static const uint8_t        NumberCanala = Number;


        static void Init( UsedInput input        = Input, Polarity polar = RisingEdge,
                          Filter filter = NoFilter, eRemapPins  pinsRemap = eNoRemap )
        {
          SetPolarity( polar );
          SelectPins( pinsRemap );

          ModeBitField::Set( input | (filter << 4) );
        }

        template< UsedInput input, Polarity polar, Filter filter, eRemapPins  pinsRemap  >
        static void Init( )
        {
          SetPolarity( polar );
          SelectPins< pinsRemap >(  );

          ModeBitField::Set( input | (filter << 4) );
        }

        static void Enable(   ) {Regs( )->CCER |= (1 << (NumberCanala * 4));}

        static void Disable(   ) {Regs( )->CCER &= ~(1 << (NumberCanala * 4)); }
        /**============================================================================
        * @brief  ?????? ?????? ???????
        */
        static DataT Get(   ) {return CanalData::Get( );}

        static uint32_t  GetUs(   )
        {
          uint32_t  tickPerUs = ClockEnReg::GetClock( ) / 1000000u;

//        if (ClockEnReg::GetDomain( ) == ClockEnReg::APB1_Domain)
//          tickPerUs *= 2;

          return (CanalData::Get( ) * (Regs( )->PSC + 1)) / tickPerUs;
        }

        /**============================================================================
        * @brief  ????? ???? ??????? ??? ???????, ????? ??? ????
        *
        * @param Polarity polar &see Polarity
        */
        static void SetPolarity( Polarity polar = RisingEdge  )
        {
          if (polar == RisingEdge)
            Regs( )->CCER &= ~(1 << ((NumberCanala * 4) + 1));
          //BitBand::Clear < NumberCanala * 4 + 1 > (Regs( )->CCER);
          else
            Regs( )->CCER |= (1 << ((NumberCanala * 4) + 1));
        }


        /**============================================================================
        * @brief  ???????????????? ???????? ????
        *
        * @param RemapPins pinsRemap ????? ???? ??? ?????????????????
        */
        template< eRemapPins  pinsRemap >
        static void SelectPins( )
        {
          STATIC_ASSERT( pinsRemap <= PinsCanal::Length );

          typedef typename PinsCanal:: template Pin< pinsRemap > PinInput;

          PinInput:: AltFuncNumber(  RemapField::PinAltFunc[ NumberCanala * LengthPerCanal + pinsRemap ] );
          PinInput:: template SetConfiguration< PinInput::Port::AltFunc >(  );
        }
        static void SelectPins( eRemapPins  pinsRemap )
        {
          MCUCPP_ASSERT( pinsRemap <= PinsCanal::Length );

          typedef typename PinsCanal::ValueType Type;

          Type maskTx( 1 << pinsRemap );

          PinsCanal::SetConfiguration( maskTx, PinsCanal::AltFunc );
          PinsCanal::AltFuncNumber( maskTx, RemapField::PinAltFunc[ NumberCanala * LengthPerCanal + pinsRemap ] );
        }

        static void EnableInterrupt( uint32_t priority = 5 )
        {
          Regs( )->SR &= ~(1 << (NumberCanala + 1));
          Regs( )->DIER |= (1 << (NumberCanala + 1));
//        BitBand::Clear <  NumberCanala  + 1 > (Regs( )->SR);
//        BitBand::Set <  NumberCanala  + 1 > (Regs( )->DIER);

          NVIC_SetPriority( IQRNumber, priority );
          NVIC_EnableIRQ( IQRNumber );
        }

        static void DisableInterrupt( ) {Regs( )->DIER &= ~(1 << (NumberCanala + 1)); /*BitBand::Clear <  NumberCanala  + 1 > (Regs( )->DIER);*/}
      };

      /**============================================================================
      * @brief  ??????????? ??????? ??????????
      */
      template< class Regs, class ClockEnReg,   class PinList, class RemapField, IRQn_Type IQRNumber, int CanalsCaptureCompare   >
      template< int Number >
      class BaseTimer< Regs, ClockEnReg, PinList, RemapField,  IQRNumber,  CanalsCaptureCompare > :: Compare
      {
        STATIC_ASSERT( Number< CanalsCaptureCompare );

        IO_REG_WRAPPER( Regs( )->CCMR1, MODE1, uint16_t);
        IO_REG_WRAPPER( Regs( )->CCMR2, MODE2, uint16_t);
        IO_REG_WRAPPER( Regs( )->CCR1,Compare1, uint16_t);
        IO_REG_WRAPPER( Regs( )->CCR2,Compare2, uint16_t);
        IO_REG_WRAPPER( Regs( )->CCR3,Compare3, uint16_t);
        IO_REG_WRAPPER( Regs( )->CCR4,Compare4, uint16_t);

        typedef typename StaticIf< (Number< 2), MODE1, MODE2 >::Result  ModeReg;

        IO_BITFIELD_WRAPPER( Regs( )->CCMR1, ModeCanal1, uint16_t, 0, 8);
        IO_BITFIELD_WRAPPER( Regs( )->CCMR1, ModeCanal2, uint16_t, 8, 8);

        IO_BITFIELD_WRAPPER( Regs( )->CCMR2, ModeCanal3, uint16_t, 0, 8);
        IO_BITFIELD_WRAPPER( Regs( )->CCMR2, ModeCanal4, uint16_t, 8, 8);

        typedef LOKI_TYPELIST_4( ModeCanal1, ModeCanal2, ModeCanal3, ModeCanal4 ) ModeList;

        typedef typename TypeAt< ModeList, Number >::Result ModeBitField;

        typedef LOKI_TYPELIST_4( Compare1, Compare2, Compare3, Compare4 ) CompareList;

        static const unsigned Length         = PinList::Length;
        static const unsigned LengthPerCanal = PinList::Length / CanalsCaptureCompare;

        typedef typename PinList::template Slice< Number* LengthPerCanal, LengthPerCanal > PinsCanal;

        typedef typename TypeAt< CompareList, Number >::Result CompareValve;


       public:
        typedef uint16_t DataT;
        static const uint16_t         MaxValue     = 0xffff;
        static const uint8_t          NumberCanala = Number;


        static void Init( CompareMode mode         = Frozen, PolarityCompare polar = ActiveHigh )
        {
          SetPolarity( polar );

          ModeBitField::Set( TimersBase::Output | (mode << 4) );
        }
        static void Enable(   ) {Regs( )->CCER |= (1 << (NumberCanala * 4));}

        static void Disable(   ) {Regs( )->CCER &= ~(1 << (NumberCanala * 4)); }

//      static void Enable(   ) {BitBand::Set< NumberCanala * 4 >( Regs( )->CCER  );}
//      static void Disable(   ) {BitBand::Clear< NumberCanala * 4 >( Regs( )->CCER  );}

        static void  SetUs( uint32_t  us )
        {
          uint32_t  tickPer = (ClockEnReg::GetClock( ) / 1000000u) * us;

//        if (ClockEnReg::GetDomain( ) == ClockEnReg::APB1_Domain)
//          tickPer *= 2;

          CompareValve::Set( (tickPer / (Regs( )->PSC + 1)) - 1 );
        }
        template< uint32_t FreqIn, uint32_t us >
        static void SetUs(  )
        {
          const uint32_t tickPer = (FreqIn / 1000000u) * us;

          CompareValve::Set( (tickPer / (Regs( )->PSC + 1)) - 1 );
        }

        /**============================================================================
        * @brief  ????? ???? ??????? ??? ???????, ????? ??? ????
        *
        * @param PolarityCompare polar @see PolarityCompare
        */
        static void SetPolarity( PolarityCompare polar = ActiveHigh  )
        {
          if (polar == ActiveHigh)
            Regs( )->CCER &= ~(1 << ((NumberCanala * 4) + 1));
//          BitBand::Clear < NumberCanala * 4 + 1 > (Regs( )->CCER);
          else
            Regs( )->CCER |= (1 << ((NumberCanala * 4) + 1));
//          BitBand::Set < NumberCanala * 4 + 1 > (Regs( )->CCER);
        }



        /**============================================================================
        * @brief  ???????????????? ???????? ????
        *
        * @param RemapPins pinsRemap ????? ???? ??? ?????????????????
        */
        template< eRemapPins  pinsRemap >
        static void SelectPins( )
        {
          STATIC_ASSERT( pinsRemap <= PinsCanal::Length );

          typedef typename PinsCanal:: template Pin< pinsRemap > PinInput;

          PinInput:: AltFuncNumber(  RemapField::PinAltFunc[ NumberCanala * LengthPerCanal + pinsRemap ] );
          PinInput:: template SetConfiguration< PinInput::Port::AltFunc >(  );
        }
        static void SelectPins( eRemapPins  pinsRemap )
        {
          MCUCPP_ASSERT( pinsRemap <= PinsCanal::Length );

          typedef typename PinsCanal::ValueType Type;

          Type maskTx( 1 << pinsRemap );
          PinsCanal::SetConfiguration( maskTx, PinsCanal::AltFunc );
          PinsCanal::AltFuncNumber( maskTx, RemapField::PinAltFunc[ NumberCanala * LengthPerCanal + pinsRemap ] );
        }

        template< bool IsNvicConfig >
        static void EnableInterrupt( uint32_t priority = 3 )
        {
          Regs( )->SR &= ~(1 << (NumberCanala + 1));
          Regs( )->DIER |= (1 << (NumberCanala + 1));

          if (IsNvicConfig == true)
          {
            NVIC_SetPriority( IQRNumber, priority );
            NVIC_EnableIRQ( IQRNumber );
          }
        }

        static void DisableInterrupt( ) {Regs( )->DIER &= ~(1 << (NumberCanala + 1)); /*BitBand::Clear <  NumberCanala  + 1 > (Regs( )->DIER);*/}
      };


      typedef IO::PinList< IO::Pa0, IO::Pa15,         //канал1
                           IO::Pa1, IO::Pb3,          //канал2
                           IO::Pa2, IO::Pb10,         //канал3
                           IO::Pa3, IO::Pb11          //канал4
                           > Timer2_Pins;

      template< uint8_t Timer > struct  Timer2AlterFuncPin {
        enum
        {Numer = Timer};
        static const uint8_t   PinAltFunc[];
      };

      template<  uint8_t Timer >
      const uint8_t Timer2AlterFuncPin<  Timer > :: PinAltFunc[] = { 2, 2, 2, 2, 2, 2, 2, 2 };


      typedef IO::PinList< IO::Pa6, IO::Pb4,          //канал1
                           IO::Pa7, IO::Pb5,          //канал2
                           IO::Pb0, IO::Pb10,         //канал3
                           IO::Pb1, IO::Pb11          //канал4
                           > Timer3_Pins;

      template< uint8_t Timer > struct  Timer3AlterFuncPin {
        enum
        {Numer = Timer};
        static const uint8_t                         PinAltFunc[];
      };
      template<  uint8_t Timer >
      const uint8_t Timer3AlterFuncPin<  Timer >:: PinAltFunc[] = { 1, 1, 1, 1, 1, 1, 1, 1 };


      typedef IO::PinList< IO::Pb1, IO::Pa4, IO::Pa7  //канал1
                           > Timer14_Pins;

      template< uint8_t Timer > struct  Timer14AlterFuncPin {
        enum
        {Numer = Timer};
        static const uint8_t                         PinAltFunc[];
      };
      template<  uint8_t Timer >
      const uint8_t Timer14AlterFuncPin<  Timer >::          PinAltFunc[] = { 0, 4, 4 };


      IO_STRUCT_WRAPPER( TIM1, Tim1Regs, TIM_TypeDef );
      IO_STRUCT_WRAPPER( TIM2, Tim2Regs, TIM_TypeDef );
      IO_STRUCT_WRAPPER( TIM3, Tim3Regs, TIM_TypeDef );
//      IO_STRUCT_WRAPPER( TIM4, Tim4Regs, TIM_TypeDef );
//      IO_STRUCT_WRAPPER( TIM6, Tim6Regs, TIM_TypeDef );
//      IO_STRUCT_WRAPPER( TIM7, Tim7Regs, TIM_TypeDef );
//      IO_STRUCT_WRAPPER( TIM8, Tim8Regs, TIM_TypeDef );
//      IO_STRUCT_WRAPPER( TIM9, Tim9Regs, TIM_TypeDef );
//      IO_STRUCT_WRAPPER( TIM10, Tim10Regs, TIM_TypeDef );
//      IO_STRUCT_WRAPPER( TIM11, Tim11Regs, TIM_TypeDef );
//      IO_STRUCT_WRAPPER( TIM12, Tim12Regs, TIM_TypeDef );
//      IO_STRUCT_WRAPPER( TIM13, Tim13Regs, TIM_TypeDef );
      IO_STRUCT_WRAPPER( TIM14, Tim14Regs, TIM_TypeDef );
//      IO_STRUCT_WRAPPER( TIM15, Tim15Regs, TIM_TypeDef );
      IO_STRUCT_WRAPPER( TIM16, Tim16Regs, TIM_TypeDef );
      IO_STRUCT_WRAPPER( TIM17, Tim17Regs, TIM_TypeDef );
    }


    class Timer2 : public Private::BaseTimer< Private::Tim2Regs, Clock::Tim2Clock, Private::Timer2_Pins, Private::Timer2AlterFuncPin< 2 >,  TIM2_IRQn, 4 >
    {
    };

    class Timer3 : public Private::BaseTimer< Private::Tim3Regs, Clock::Tim3Clock, Private::Timer3_Pins, Private::Timer3AlterFuncPin< 3 >, TIM3_IRQn, 4 >
    {
    };
    class Timer14 : public Private::BaseTimer< Private::Tim14Regs, Clock::Tim14Clock, Private::Timer14_Pins, Private::Timer14AlterFuncPin< 14 >, TIM14_IRQn, 1 >
    {
    };


#if defined (STM32F10X_HD) || defined  (STM32F10X_CL)
    class Timer7 : public Private::BaseTimer< Private::Tim6Regs, Clock::Tim6Clock, NullType, NullType, TIM7_IRQn, 0 >
    {
    };
    class Timer6 : public Private::BaseTimer< Private::Tim6Regs, Clock::Tim6Clock, NullType, NullType, TIM6_DAC_IRQn, 0 >
    {
    };
#endif


  }
}
#undef DECLARE_STM32_BASIC_TIMER


