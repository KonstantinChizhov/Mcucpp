

#ifndef MCUCPP_ATOMIC_H
#define MCUCPP_ATOMIC_H

#include <_atomic.h>

namespace Mcucpp
{
	#define DECLARE_OP(OPERATION, OP_NAME) \
	template<class T, class T2>\
	static inline T FetchAnd ## OP_NAME (T * ptr, T2 value)\
	{\
		T tmp = *ptr;\
		*ptr = tmp OPERATION value;\
		return tmp;\
	}\
	template<class T, class T2>\
	static inline T OP_NAME ## AndFetch(T * ptr, T2 value)\
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
		static inline T Fetch(T * ptr)
		{
			return *ptr;
		}

		template<class T, class T2>
		static inline bool CompareExchange(T * ptr, T2 oldValue, T2 newValue)
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
