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

#include <stdint.h>

namespace Mcucpp
{
	static inline uint_fast8_t FirstZeroBitPosition(uint8_t value)
	{
		if(value == 0xff)
			return 0xff;
		uint_fast8_t bitPos = 0;
		if((value & 0xf) == 0xf)
		{
			bitPos+=4;
			value>>=4;
		}
		if((value & 0x3) == 0x3)
		{
			bitPos+=2;
			value>>=2;
		}
		if((value & 0x1) == 0x1)
		{
			bitPos+=1;
		}
		return bitPos;
	}

	static inline uint_fast8_t FirstZeroBitPosition(uint16_t value)
	{
		if(value == 0xffff)
			return 0xff;
		uint_fast8_t bitPos = 0;
		if((value & 0xff) == 0xff)
		{
			bitPos+=8;
			value>>=8;
		}
		if((value & 0xf) == 0xf)
		{
			bitPos+=4;
			value>>=4;
		}
		if((value & 0x3) == 0x3)
		{
			bitPos+=2;
			value>>=2;
		}
		if((value & 0x1) == 0x1)
		{
			bitPos+=1;
		}
		return bitPos;
	}

	static inline uint_fast8_t FirstZeroBitPosition(uint32_t value)
	{
		if(value == 0xffffffff)
			return 0xff;
		uint_fast8_t bitPos = 0;
		if((value & 0xfffful) == 0xfffful)
		{
			bitPos+=16;
			value>>=16;
		}
		if((value & 0xff) == 0xff)
		{
			bitPos+=8;
			value>>=8;
		}
		if((value & 0xf) == 0xf)
		{
			bitPos+=4;
			value>>=4;
		}
		if((value & 0x3) == 0x3)
		{
			bitPos+=2;
			value>>=2;
		}
		if((value & 0x1) == 0x1)
		{
			bitPos+=1;
		}
		return bitPos;
	}

	static inline int_fast8_t FirstZeroBitPosition(uint64_t value)
	{
		if(!~value)
			return 0xff;
		int_fast8_t bitPos = 0;
		if((value & 0xffffffffull) == 0xffffffffull)
		{
			bitPos+=32;
			value>>=32;
		}
		if((value & 0xfffful) == 0xfffful)
		{
			bitPos+=16;
			value>>=16;
		}
		if((value & 0xff) == 0xff)
		{
			bitPos+=8;
			value>>=8;
		}
		if((value & 0xf) == 0xf)
		{
			bitPos+=4;
			value>>=4;
		}
		if((value & 0x3) == 0x3)
		{
			bitPos+=2;
			value>>=2;
		}
		if((value & 0x1) == 0x1)
		{
			bitPos+=1;
		}
		return  bitPos;
	}


	static inline int CountLeadingZeros(uint16_t x)
	{
	#if defined (__GNUC__)
		if (x == 0)
		{
			return 16;
		}
		return __builtin_clz(x) - ((sizeof(unsigned) - sizeof(x)) * 8);
	#else
		if (x == 0)
		{
			return 16;
		}
		int n  = 0;
		if ((x & 0xff00) == 0)
		{
			n += 8;
			x <<= 8;
		}
		if ((x & 0xf000) == 0)
		{
			n += 4;
			x <<= 4;
		}
		if ((x & 0xC000) == 0)
		{
			n += 2;
			x <<= 2;
		}
		if ((x & 0x8000) == 0)
			n += 1;
		return n;
	#endif
	}

	static inline int CountLeadingZeros(uint32_t x)
	{
	#if defined (__GNUC__)
		if (x == 0)
		{
			return 32;
		}
		return __builtin_clz(x);
	#else
		if (x == 0)
		{
			return 32;
		}
		int n  = 0;
		if ((x & 0xffff0000) == 0)
		{
			n += 16;
			x <<= 16;
		}
		if ((x & 0xff000000) == 0)
		{
			n += 8;
			x <<= 8;
		}
		if ((x & 0xf0000000) == 0)
		{
			n += 4;
			x <<= 4;
		}
		if ((x & 0xC0000000) == 0)
		{
			n += 2;
			x <<= 2;
		}
		if ((x & 0x80000000) == 0)
			n += 1;
		return n;
	#endif
	}

	static inline int CountLeadingZeros(uint64_t x)
	{
	#if defined (__GNUC__)
		if(x == 0)
		{
			return 64;
		}
		return __builtin_clzl(x);
	#else
		if (x == 0)
		{
			return 64;
		}
		int n  = 0;
		if ((x & 0xffffffff00000000ull) == 0)
		{
			n += 32;
			x <<= 32;
		}
		if ((x & 0xffff000000000000ull) == 0)
		{
			n += 16;
			x <<= 16;
		}
		if ((x & 0xff00000000000000ull) == 0)
		{
			n += 8;
			x <<= 8;
		}
		if ((x & 0xf000000000000000ull) == 0)
		{
			n += 4;
			x <<= 4;
		}
		if ((x & 0xC000000000000000ull) == 0)
		{
			n += 2;
			x <<= 2;
		}
		if ((x & 0x8000000000000000ull) == 0)
			n += 1;
		return n;
	#endif
	}

	static inline int CountTrailingZeros(uint32_t x)
	{
	#if defined (__GNUC__)
		if(x == 0)
		{
			return 32;
		}
		return __builtin_ctz(x);
	#else
		if (x == 0)
		{
			return 32;
		}
		int n = 1;
		if ((x & 0x0000FFFF) == 0) {n = n +16; x = x >>16;}
		if ((x & 0x000000FF) == 0) {n = n + 8; x = x >> 8;}
		if ((x & 0x0000000F) == 0) {n = n + 4; x = x >> 4;}
		if ((x & 0x00000003) == 0) {n = n + 2; x = x >> 2;}
		return n - (x & 1);
	#endif
	}

	static inline int CountTrailingZeros(uint16_t x)
	{
	#if defined (__GNUC__)
		if(x == 0)
		{
			return 16;
		}
		return __builtin_ctz(x);
	#else
				if (x == 0)
		{
			return 16;
		}
		int n = 1;
		if ((x & 0x00FF) == 0) {n = n + 8; x = x >> 8;}
		if ((x & 0x000F) == 0) {n = n + 4; x = x >> 4;}
		if ((x & 0x0003) == 0) {n = n + 2; x = x >> 2;}
		return n - (x & 1);
	#endif
	}

	static inline int CountTrailingZeros(uint64_t x)
	{
	#if defined (__GNUC__)
		if(x == 0)
		{
			return 64;
		}
		return __builtin_ctzl(x);
	#else
				if (x == 0)
		{
			return 64;
		}
		int n = 1;
		if ((x & 0xFFFFFFFFul) == 0) {n = n +32; x = x >>32;}
		if ((x & 0x0000FFFFul) == 0) {n = n +16; x = x >>16;}
		if ((x & 0x000000FFul) == 0) {n = n + 8; x = x >> 8;}
		if ((x & 0x0000000Ful) == 0) {n = n + 4; x = x >> 4;}
		if ((x & 0x00000003ul) == 0) {n = n + 2; x = x >> 2;}
		return n - (x & 1);
	#endif
	}
}
