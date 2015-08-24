/*****************************************************************************
*
* Author       : Хоменко С.П.
* Created      : 20.10.2014
* $Rev:: 17   $:  Revision of last commit
* $Date:: 2014-12-20 16:56:18#$:  Date of last commit
* Editor Tabs  : 2
*
* NOTE: 
******************************************************************************/

#ifndef __KEYBOARD_H_INCL__
#define __KEYBOARD_H_INCL__

/*******************************************************************************
*  Секция include: здесь подключаются заголовочные файлы используемых модулей
******************************************************************************/
#include "pinlist.h"
#include "select_size.h"

using namespace Mcucpp;
using namespace Mcucpp::IO;
/*******************************************************************************
*  Секция определения типов
******************************************************************************/

class KeyboardBase
{
 public:

  /** \enum State
  * @brief Состояние кнопок
  */
  enum State
  {
    eReleased,                                        /*< Кнопка отжата */
    ePressing,                                        /*< Кнопка нажата */
    eKeeping,                                         /*< Кнопка удержана */
    eBadState,                                        /*< Кнопка залипла */
    eMaxState
  };
  /** \enum Event
  * @brief События по кнопкам
  */
  enum tEvent
  {
    eNoEvent,
    eReleasedEv,
    ePressingEv,
    eKeepingEv,
    eBadStateEv,
    eMaxEvent
  };
#pragma anon_unions
  union tEventKeys {
    uint8_t Byte;
    struct {
      tEvent Event:3;
      uint8_t Key:5;
    };
  };
};


/**============================================================================
* @brief Класс клавиатуры построеной на матрице кнопок.
*   
*   @param Row -  список типов пинов управления рядами
*   @param Column - список типов пинов управления солонками
*   @param Period - период вызова метода Processing() в мс
*  
*/
template< class Row, class Column, class T = uint32_t, uint8_t Period = 10 >
class Keyboard : public KeyboardBase
{
  enum
  {ePeriod = Period };

  typedef typename SelectSize< Column::Length >::Result tDataRead;

  static const uint16_t timePressing          = 100 / Period;  /* 160ms*/
  static const uint16_t timeKeeping           = 1200 / Period;  /* 1.2sec*/
  static const uint16_t timeBadState          = 8000 / Period; /* 8sec*/


  const static uint8_t  keysAll               = Row::Length * Column::Length;

  const static uint8_t  keysPerElement        = sizeof(T) * 8;
  const static uint8_t  element               =
                   (keysAll % keysPerElement) == 0 ? keysAll / keysPerElement : keysAll / keysPerElement + 1;
  const static uint8_t  mask                  = (1 << Column::Length) - 1;


  //! счетчик периодов нажатия
  uint16_t              _cntState[keysAll];

  //! текущее состояние кнопок
  T                     _currentState[element];

  //! true - все текущее данные считаны
  bool                  _isDataCurrentReading;
  //! true матрица активированна
  bool                  _isEnable;

  uint16_t              _cntError;

 public:
  // Constructor
  Keyboard( )
  {
  }

  void Init( );
  void Disable( );

  /**============================================================================
  * @brief  Сохраняет текущее состояние кнопок  
  *   
  *  @note Должна вызываться с периодом Period/Row::Length
  * 
  */
  bool GetCurrentState( );

  /**============================================================================
  * @brief  Обработка текущего состояния  
  *   
  *  @note Должна вызываться раз в Period, по установке флага _isDataCurrentReading, 
  *  не из прерывания.
  * 
  * @return Event - события по кнопкам
  */
  uint8_t Processing( );

  /**============================================================================
  * @brief  Возвращает состояние кнопки 
  * 
  * @param numberKey - номер кнопки
  * 
  * @return State - состояние кнопки @see State
  */
  State GetState( size_t numberKey ) const;

  uint8_t Size( ) const
  {
    return keysAll;
  }
};
//==============================================================================
template< class Row, class Column, class T, uint8_t Period >
void Keyboard< Row, Column, T, Period >::Init( )
{
  Column:: template SetConfiguration < (1 << Column::Length) - 1,Column::In > ();

  Row:: template Write< 0 >( );
  Row:: template SetConfiguration < (1 << Row::Length) - 1,Row::In > ();
  Row:: template SetConfiguration< 1, Row::Out >( );

  _isEnable = true;

}

//==============================================================================
template< class Row, class Column, class T, uint8_t Period >
void Keyboard< Row, Column, T, Period >::Disable( )
{
  Row:: template Write< 0 >( );
  Row:: template SetConfiguration < (1 << Row::Length) - 1,Row::In > ();

  _isEnable = false;
}



//==============================================================================
template< class Row, class Column, class T, uint8_t Period >
bool Keyboard< Row, Column, T, Period >::GetCurrentState( )
{
  static size_t cntCurrentData = 0;
  uint8_t       i;
  tDataRead     dataRead       = ~Column::PinRead( );

  if( _isEnable == false )
  {
    cntCurrentData = 0;
    return false;
  }


  if( _isDataCurrentReading == true )
  {
    // предыдущее считаное состояние не обработано
    _cntError++;
    // сброс текущих данных
    for( i = 0; i < (sizeof(_currentState) / sizeof(T)); i++ )
      _currentState[ i ] = 0;
  }
  const uint8_t elem  = (Column::Length * cntCurrentData) / keysPerElement;
  const uint8_t shift = (Column::Length * cntCurrentData) % keysPerElement;

  dataRead &= ((1 << Column::Length) - 1);

  _currentState[ elem ] |= static_cast< T >( dataRead << shift );

  if( (keysPerElement % Column::Length) != 0 )
    _currentState[ elem + 1 ] |= static_cast< T >( dataRead >> (keysPerElement - shift) );

  if( ++cntCurrentData >= Row::Length )
  {
    cntCurrentData = 0;
    // перейдем к обработке результатов
    _isDataCurrentReading = true;
  }
  Row:: template Write< 0 >( );
  Row:: template SetConfiguration < (1 << Row::Length) - 1,Row::In > ();
  // подготовка для опроса следующего ряда
  Row:: SetConfiguration( 1 << cntCurrentData, Row::Out );

  return _isDataCurrentReading;
}
//==============================================================================
template< class Row, class Column, class T, uint8_t Period >
uint8_t Keyboard< Row, Column, T, Period >::Processing( )
{
  size_t     k;

  State      oldStateKey;
  State      newStateKey;
  tEventKeys event;

        event.Byte       = 0;

  if( _isDataCurrentReading == false )
    return event.Byte;

  _isDataCurrentReading = false;

  for( k = 0; k < keysAll; k++ )
  {
    oldStateKey = GetState( k );

    if( _currentState[ k / keysPerElement ] & (1 << (k % keysPerElement)) )
    {
      _cntState[ k ]++;

      newStateKey = GetState( k );

      if( newStateKey == KeyboardBase::eBadState )
        _cntState[ k ] = timeBadState + 1;
    }
    else
    {
      newStateKey = KeyboardBase::eReleased;
      _cntState[ k ] = 0;
    }

    if( oldStateKey != newStateKey )
    {
      // события по кнопке
      event.Key = k;
      event.Event = static_cast< tEvent >( newStateKey + 1 );
      break;
    }
  }

  // сброс текущих данных, для подготовки принятия новых даных
  for( k = 0; k < (sizeof(_currentState) / sizeof(T)); k++ )
    _currentState[ k ] = 0;

  return event.Byte;
}
//===============================================================================
template< class Row, class Column, class T, uint8_t Period >
Keyboard< Row, Column, T, Period >::State Keyboard< Row, Column, T, Period >::GetState( size_t numberKey ) const
{
  State stateKey = KeyboardBase::eReleased;

  if( _cntState[ numberKey ] > timeBadState )
    stateKey = KeyboardBase::eBadState;
  else if( _cntState[ numberKey ] > timeKeeping )
    stateKey = KeyboardBase::eKeeping;
  else if( _cntState[ numberKey ] > timePressing )
    stateKey = KeyboardBase::ePressing;

  return stateKey;
}




/*******************************************************************************
*  Секция методов
******************************************************************************/
//// Constructor implementation
//inline Keyboard::Keyboard( )
//{
//}
//
//// Destructor implementation
//inline Keyboard::~Keyboard( )
//{
//}


#endif                       // Закрывающий #endif к блокировке повторного включения
/*******************************************************************************
*  ENF OF FILE
******************************************************************************/






