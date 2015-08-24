/*****************************************************************************
*
* Author       : Хоменко С.П.
* Created      : 09.11.2014
* $Rev:: 17   $:  Revision of last commit
* $Date:: 2014-12-20 16:56:18#$:  Date of last commit
* Editor Tabs  : 2
*
* NOTE: 
******************************************************************************/

#ifndef _leds_array_H
#define _leds_array_H

/*******************************************************************************
*  Секция include: здесь подключаются заголовочные файлы используемых модулей
******************************************************************************/
//#include <bitset>
#include <stdint.h>
#include "pinlist.h"
#include "iopins.h"
#include "timers.h"
//#include "select_size.h"


/*******************************************************************************
*  Секция определения макросов
******************************************************************************/
/*******************************************************************************
*  Секция определения типов
******************************************************************************/
namespace Mcucpp
{
  using namespace Mcucpp::IO;

  /**============================================================================
* @brief Класс матрицы светодиодов  
*   
*  @param  PinRow - список пинов управлением рядами
*  @param  PinColumn - список пинов управлением колонками
*  @param  Timer - таймер
*  @param  CanalTimer - канал таймера Timer для организации ШИМ
*/
  template< class PinRow, class PinColumn, class Timer, uint32_t  CanalTimer >
  class  LedsArray
  {
   public:
    typedef typename SelectSize< PinRow::Length >::Result tDataRow;
    typedef typename SelectSize< PinColumn::Length >::Result tDataColunm;

    typedef Timer  TimerHandler;

   private:
    static const  uint8_t  LedsAll       = PinRow::Length * PinColumn::Length;

    //! текущее состояние светодиодов
    uint32_t _currentState;

    //! частота отображения кадров в Гц
    uint8_t  _frequency;
    //! величина PWM в %
    uint8_t  _percentPWM;
    //! номер отображаемого ряда
    uint8_t  _currentRow;
    //! признак инициализации матрицы
    bool     _isInit;

   public:


    /**============================================================================
    * @brief    Инициализация матрицы светодиодов
    * 
    * @param uint8_t Frequency - частота отображения кадров в Гц
    * @param uint8_t percent - величина PWM в %
    */
    void Init( uint8_t Frequency = 80, uint8_t percent = 100 );

    void Disable( )
    {
      _isInit = false;
      Timer::Disable( );

      PinColumn::SetConfiguration( PinColumn::In );
      PinRow::SetConfiguration( PinRow::In );
    }
    /**============================================================================
    * @brief  Включает все светодиоды
    */
    void Set( ) {_currentState = 0xFFFFFFFF;}

    /**============================================================================
    * @brief  Включает светодиоды
    */
    void Set( uint32_t data  ) {_currentState = data;}

    /**============================================================================
    * @brief  Включение светодиода
    * 
    * @param uint8_t led - номер светодиода
    * @param bool state - состояние true- включен
    */
    void  Set( uint8_t  led, bool state )
    {
      if (led >= LedsAll)
        return;

      if (state == true)
        _currentState |= (1 << led);
      else
        _currentState &= ~(1 << led);
    }
    template< bool state >
    void  Set( uint8_t  led )
    {
      if (led >= LedsAll)
        return;

      if (state == true)
        _currentState |= (1 << led);
      else
        _currentState &= ~(1 << led);
    }

    uint32_t Get( ) {return  _currentState;}

    void Toggle( uint8_t  led ) {_currentState ^= (1 << led);}
    //void Toggle( const uint8_t  led ) {_currentState ^= (1 << led);}
    template< uint8_t led >
    void Toggle( ) {_currentState ^= (1 << led);}
    /**============================================================================
    * @brief  Выключает все светодиоды 
    */
    void Clear( ) {_currentState = 0;}

    static uint8_t   Size( )  {return LedsAll; }
    void SetPWM( uint8_t percent );
    void SetFrequency( uint8_t frequency );

    bool Handler( typename TimerHandler::InterruptFlags flags )
    {
      static uint8_t rowCurrent = PinRow::Length;
      tDataColunm    colunm     = 0;

      // выключения строчки
      // может по сравнению (для ШИМ) и по переполнению
      PinRow:: template Set < (1 << PinRow::Length) - 1 > ();
      PinColumn:: template Clear < (1 << PinRow::Length) - 1  > ();


      if ((flags & TimerHandler::UpdateInt) != 0)
      {
        if (++rowCurrent >= PinRow::Length)
          rowCurrent = 0;

        colunm = (_currentState >> (rowCurrent * PinColumn::Length)) & ((1 << PinColumn::Length) - 1);

        PinColumn::Set( colunm );
        // включение строки
        PinRow::Clear( (1 << rowCurrent) );
      }

      TimerHandler::ClearInterruptFlag( Timer::AllInt );
      if (rowCurrent == 0)
        return true;

      return false;
    }
  };
/*******************************************************************************
*  Секция определения глобальных переменных
******************************************************************************/

/*******************************************************************************
*  Секция прототипов глобальных функций
******************************************************************************/

//==========================================================================
  template< class PinRow, class PinColumn, class Timer, uint32_t  CanalTimer  >
  void  LedsArray< PinRow, PinColumn, Timer, CanalTimer > ::Init( uint8_t frequency, uint8_t percent )
  {
    _frequency = frequency;
    _percentPWM = percent;
    _currentRow = 0;

    if (_isInit == true)
      return;

    _isInit = true;
    PinColumn::SetConfiguration( PinColumn::Out );

    PinRow::SetConfiguration( PinRow::Out );
    PinRow::SetDriverType( PinRow::OpenDrain );
    PinRow:: template Set < (1 << PinRow::Length) - 1 > ();


    // найстройка таймера
    _percentPWM = percent;
    Timer:: template Compare< CanalTimer >:: SetUs( (10000u * _percentPWM) / _frequency / PinRow::Length );

    _frequency = frequency;
    Timer::Start( 1000000u / _frequency / PinRow::Length );

    Timer::EnableInterrupt( );
    if (_percentPWM < 100)
      Timer::template Compare< CanalTimer >:: template EnableInterrupt< true >( );
  }

//==========================================================================
  template< class PinRow, class PinColumn, class Timer, uint32_t  CanalTimer >
  void  LedsArray< PinRow, PinColumn, Timer, CanalTimer > ::SetPWM( uint8_t percent )
  {
    _percentPWM = percent;
    Timer::template Compare< CanalTimer >:: SetUs( (10000u * _percentPWM) / _frequency / PinRow::Length );

    if (_percentPWM < 100)
      Timer::template Compare< CanalTimer >:: template EnableInterrupt< false >( );
    else
      Timer::template Compare< CanalTimer >::DisableInterrupt( );
  }
//==========================================================================
  template< class PinRow, class PinColumn, class Timer, uint32_t  CanalTimer  >
  void  LedsArray< PinRow, PinColumn, Timer, CanalTimer > ::SetFrequency( uint8_t frequency )
  {
    _frequency = frequency;
    // установим период сработки таймера в мкс
    Timer::SetPeriod( 1000000u / _frequency / PinRow::Length );
  }
}





#endif

/*******************************************************************************
*  ENF OF FILE
******************************************************************************/


