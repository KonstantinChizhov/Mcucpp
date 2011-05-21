#pragma once

namespace Atomic
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

#define ATOMIC if(Atomic::DisableInterrupts di = Atomic::DisableInterrupts()){}else

#define DECLARE_OP(OPERATION, OP_NAME) \
	template<class T, class T2>\
	T FetchAnd ## OP_NAME (volatile T * ptr, T2 value)\
	{\
		ATOMIC \
		{\
			T tmp = *ptr;\
			*ptr = tmp OPERATION value;\
			return tmp;\
		}\
	}\
	template<class T, class T2>\
	T OP_NAME ## AndFetch(volatile T * ptr, T2 value)\
	{\
		ATOMIC \
		{\
			T tmp = *ptr OPERATION value;\
			*ptr = tmp;\
			return tmp;\
		}\
	}


	DECLARE_OP(+, Add)
	DECLARE_OP(-, Sub)
	DECLARE_OP(|, Or)
	DECLARE_OP(&, And)
	DECLARE_OP(^, Xor)


	template<class T, class T2>
	bool CompareExchange(T * ptr, T2 oldValue, T2 newValue)
	{
		ATOMIC 
		{
			if(*ptr != oldValue)
				return false;
			*ptr = newValue;
		}
		return true;
	}

}
