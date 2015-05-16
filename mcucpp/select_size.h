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

#pragma once
#include <static_if.h>
#include <stdint.h>

namespace Mcucpp
{
	template<unsigned sizeBits>
	struct SelectSize
	{
		static const bool LessOrEq8 = sizeBits <= 8;
		static const bool LessOrEq16 = sizeBits <= 16;
		static const bool LessOrEq32 = sizeBits <= 32;

		typedef typename StaticIf<
				LessOrEq8,
				uint_fast8_t,
				typename StaticIf<LessOrEq16, uint_fast16_t, 
				typename StaticIf<LessOrEq32, uint_fast32_t, uint_fast64_t>::Result
				>::Result>
				::Result Result;
	};


	template<unsigned size>
	struct SelectSizeForLength
	{
		static const bool LessOrEq8 = size <= 0xff;
		static const bool LessOrEq16 = size <= 0xffff;

		typedef typename StaticIf<
				LessOrEq8,
				uint_fast8_t,
				typename StaticIf<LessOrEq16, uint_fast16_t, uint_fast32_t>::Result>
				::Result Result;
	};
}
