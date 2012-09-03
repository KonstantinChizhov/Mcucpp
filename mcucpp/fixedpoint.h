//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2012
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

#include <select_size.h>
#include <static_assert.h>
#include <template_utils.h>
#include <stddef.h>

namespace Mcucpp
{
	template<class T, size_t IntegerBits>
	class FixedPoint
	{
		BOOST_STATIC_ASSERT(IntegerBits <= sizeof(T) * 8);
	public:
		typedef T DataT;
		typedef typename Util::Unsigned<T>::Result UnsignedT;
		enum{IsSigned = T(-1) > 0};
		static const T FractionalBits = sizeof(T) * 8 - IntegerBits;
		static const T FractionalPartMask = (T(1) << FractionalBits) - 1;
		static const T IntegerPartMask = ((T(1) << IntegerBits) - 1) << FractionalBits;
		static const T RawOne = T(1) << FractionalBits;

		inline FixedPoint();
		explicit inline FixedPoint(int);
		explicit inline FixedPoint(float);
		explicit inline FixedPoint(double);
		explicit inline FixedPoint(unsigned);
		inline FixedPoint(int, unsigned);
		inline FixedPoint operator+(int)const;
		inline FixedPoint operator+(const FixedPoint&)const;
		inline FixedPoint& operator+=(int);
		inline FixedPoint& operator+=(const FixedPoint&);
		inline FixedPoint operator-(int)const;
		inline FixedPoint operator-(const FixedPoint&)const;
		inline FixedPoint& operator-=(int);
		inline FixedPoint& operator-=(const FixedPoint&);
		inline FixedPoint operator*(int)const;
		inline FixedPoint operator*(const FixedPoint&)const;
		inline FixedPoint& operator*=(int);
		inline FixedPoint& operator*=(const FixedPoint&);
		inline FixedPoint operator/(int)const;
		inline FixedPoint operator/(const FixedPoint&)const;
		inline FixedPoint& operator/=(int);
		inline FixedPoint& operator/=(const FixedPoint&);
		inline int ToInt()const;
		inline long ToLong()const;
		inline long long ToLongLong()const;
		inline float ToFloat()const;
		inline double ToDouble()const;
		inline DataT IntegerPart()const;
		inline UnsignedT FractionalPart()const;
	private:
		DataT _data;
	};
} //namespace Mcucpp

#include <impl/fixedpointimpl.h>
