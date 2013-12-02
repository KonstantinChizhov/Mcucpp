//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or 
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************

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
		static inline T Fetch(const T * const ptr)
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
