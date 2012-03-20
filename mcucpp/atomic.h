

#ifndef MCUCPP_ATOMIC_H
#define MCUCPP_ATOMIC_H

#include <_atomic.h>

namespace Mcucpp
{
	#define DECLARE_OP(OPERATION, OP_NAME) \
	template<class T, class T2>\
	static T FetchAnd ## OP_NAME (volatile T * ptr, T2 value)\
	{\
		T tmp = *ptr;\
		*ptr = tmp OPERATION value;\
		return tmp;\
	}\
	template<class T, class T2>\
	static T OP_NAME ## AndFetch(volatile T * ptr, T2 value)\
	{\
			*ptr = *ptr OPERATION value;\
			return *ptr;\
	}

	class VoidAtomic
	{
		VoidAtomic();
	public:
		DECLARE_OP(+, Add)
		DECLARE_OP(-, Sub)
		DECLARE_OP(|, Or)
		DECLARE_OP(&, And)
		DECLARE_OP(^, Xor)

		template<class T>
		static T Fetch(T * ptr)
		{
			return *ptr;
		}

		template<class T, class T2>
		static bool CompareExchange(T * ptr, T2 oldValue, T2 newValue)
		{
			if(*ptr != oldValue)
				return false;
			*ptr = newValue;
			return true;
		}
	};
#undef DECLARE_OP
}

#endif
