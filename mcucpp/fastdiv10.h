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

namespace Mcucpp
{
	namespace Private
	{
		template<class T, int Shift, bool Skip=(sizeof(T)*8 <= Shift)>
		struct ShiftIf
		{
			static T ShiftAdd(T value)
			{
				return value;
			}
		};
		
		template<class T, int Shift>
		struct ShiftIf<T, Shift, false>
		{
			static T ShiftAdd(T value)
			{
				return value + (value>>Shift);
			}
		};
	}
	
	template<class T>
	struct divmod10_t
	{
		T quot;
		uint8_t rem;
	};

	template<class T>
	inline static divmod10_t<T> divmodu10(T n)
	{
		divmod10_t<T> res;
		res.quot = n >> 1;
		res.quot += res.quot >> 1;
		res.quot += res.quot >> 4;
		//res.quot += res.quot >> 8;
		//res.quot += res.quot >> 16;

		res.quot = Private::ShiftIf<T, 8>::ShiftAdd(res.quot);
		res.quot = Private::ShiftIf<T, 16>::ShiftAdd(res.quot);
		res.quot = Private::ShiftIf<T, 32>::ShiftAdd(res.quot);
		
		T qq = res.quot;
		res.quot >>= 3;
		res.rem = uint8_t(n - ((res.quot << 1) + (qq & ~((T)7))));
		if(res.rem > 9)
		{
			res.rem -= 10;
			res.quot++;
		}
		return res;
	}

}