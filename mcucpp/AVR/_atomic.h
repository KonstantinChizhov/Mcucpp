#pragma once

#ifndef MCUCPP_ATOMIC_H
#error Do not include '<Arch>/_atomic.h' files directly, use 'atomic.h' instead.
#endif

namespace Mcucpp
{
	class DisableInterrupts
	{
	public:
		DisableInterrupts()
		{
			_sreg = SREG;
			asm("cli");
		}
		~DisableInterrupts()
		{
			SREG = _sreg;
		}
		operator bool()
		{return false;}
	private:
		uint8_t _sreg;
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
	}\
	template<class T, class T2>\
	static T OP_NAME ## AndFetch(volatile T * ptr, T2 value)\
	{\
		ATOMIC \
		{\
			*ptr = *ptr OPERATION value;\
			return *ptr;\
		}\
	}
	
	class Atomic 
	{
		Atomic();
	public:
		DECLARE_OP(+, Add)
		DECLARE_OP(-, Sub)
		DECLARE_OP(|, Or)
		DECLARE_OP(&, And)
		DECLARE_OP(^, Xor)
		
		template<class T>
		static T Fetch(volatile T * ptr)
		{
			ATOMIC
			{
				return *ptr;
			}
		}

		template<class T, class T2>
		static bool CompareExchange(T * ptr, T2 oldValue, T2 newValue)
		{
			ATOMIC 
			{
				if(*ptr != oldValue)
					return false;
				*ptr = newValue;
			}
			return true;
		}
	};
#undef DECLARE_OP
}
