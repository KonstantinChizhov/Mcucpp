#pragma once
#include "core_cm0.h"

namespace Mcucpp
{
  class DisableInterrupts
  {
   public:
    DisableInterrupts( )
      : _sreg( __get_PRIMASK( ) )
    {
      __disable_irq( );
    }
    ~DisableInterrupts( )
    {
      __set_PRIMASK( _sreg );
    }
    operator bool( )
    {return false;}
   private:
    uint32_t _sreg;
  };

#define ATOMIC if(Mcucpp::DisableInterrupts di = Mcucpp::DisableInterrupts()){}else

  

#define DECLARE_OP(OPERATION, OP_NAME) \
	template<class T, class T2>\
	static T FetchAnd ## OP_NAME (volatile T * ptr, T2 value)\
	{\
		ATOMIC \
		{\
			T tmp = *ptr;\
			*ptr = tmp OPERATION value;\
			return tmp;\
		}\
		return T(0); \
	}\
	template<class T, class T2>\
	static T OP_NAME ## AndFetch(volatile T * ptr, T2 value)\
	{\
		ATOMIC \
		{\
			*ptr = *ptr OPERATION value;\
			return *ptr;\
		}\
		return T(0); \
	}


  class Atomic
  {
    Atomic( );
   public:
    DECLARE_OP( +, Add)
      DECLARE_OP(-, Sub)
      DECLARE_OP(|, Or)
      DECLARE_OP(&, And)
      DECLARE_OP(^, Xor)

      template<class T>
      static T Fetch(volatile T *ptr)
    {
        ATOMIC
      {
          return *ptr;
      }
        return T(0);                                  // shutup compiler
    }

      static uint8_t Fetch( volatile uint8_t *ptr )
    {
      return *ptr;
    }

    static int8_t Fetch( volatile int8_t *ptr )
    {
      return *ptr;
    }

    static char Fetch( volatile char *ptr )
    {
      return *ptr;
    }

    template< class T, class T2 >
    static bool CompareExchange( T *ptr, T2 oldValue, T2 newValue )
    {
      ATOMIC
      {
        if (*ptr != oldValue)
          return false;
        *ptr = newValue;
      }
      return true;
    }
  };
#undef DECLARE_OP
}
