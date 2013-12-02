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

#include <string_util.h>
#include <char_trates.h>
#include <mul32hu.h>
#include <flashptr.h>

namespace Mcucpp
{	
	static const uint32_t ftoaTable[] FLASH_STORAGE =
	{
		0xF0BDC21A,
		0x3DA137D5,
		0x9DC5ADA8,
		0x2863C1F5,
		0x6765C793,
		0x1A784379,
		0x43C33C19,
		0xAD78EBC5,
		0x2C68AF0B,
		0x71AFD498,
		0x1D1A94A2,
		0x4A817C80,
		0xBEBC2000,
		0x30D40000,
		0x7D000000,
		0x20000000,
		0x51EB851E,
		0xD1B71758,
		0x35AFE535,
		0x89705F41,
		0x232F3302,
		0x5A126E1A,
		0xE69594BE,
		0x3B07929F,
		0x971DA050,
		0x26AF8533,
		0x63090312,
		0xFD87B5F2,
		0x40E75996,
		0xA6274BBD,
		0x2A890926,
		0x6CE3EE76
	};

	template<class CharT>
	int ftoaEngine(float value, CharT *buffer, int presc)
	{
		union
		{
			float fvalue;
			uint32_t uvalue;
		};
		fvalue = value;
		uint16_t uvalue_hi16 = (uint16_t)(uvalue >> 16);
		uint8_t exponent = (uint8_t(uvalue_hi16 >> 7));
		uint32_t fraction = (uvalue & 0x00ffffff) | 0x00800000;
		char *ptr = buffer;

		if(uvalue & 0x80000000)
			*ptr++ = '-';
		else
			*ptr++ = '+';

		if(exponent == 0) // don't care about a subnormals
		{
			ptr[0] = '0';
			ptr[1] = 0;
			return 0xff;
		}

		if(exponent == 0xff)
		{
			if(fraction & 0x007fffff)
			{
				ptr[0] = 'n';
				ptr[1] = 'a';
				ptr[2] = 'n';
				ptr[3] = 0;
			}
			else
			{
				ptr[0] = 'i';
				ptr[1] = 'n';
				ptr[2] = 'f';
				ptr[3] = 0;
			}
			return 0xff;
		}

		*ptr++ = '0';

		int exp10 = ((((exponent>>3))*77+63)>>5) - 38;
		fraction <<= 8; 
		uint32_t t = mul32hu(fraction, MakeFlashPtr(ftoaTable)[exponent / 8] ) + 1;
		uint_fast8_t shift = 7 - (exponent & 7);
		t >>= shift;

		uint8_t digit = t >> 24;
		digit >>= 4;
		while(digit == 0)
		{
			t &= 0x0fffffff;
			t <<= 1;
			t += t << 2;
			digit = (uint8_t)(t >> 24);
			digit >>= 4;
			exp10--;
		}

		for(uint_fast8_t i = presc+1; i > 0; i--)
		{
			digit = (uint8_t)(t >> 24);
			digit >>= 4;
			*ptr++ = digit + '0';
			t &= 0x0fffffff;
			t <<= 1;
			t += t << 2;
		}
		// roundup
		if(buffer[presc+2] >= '5')
			buffer[presc+1]++;
		ptr[-1] = 0;
		ptr-=2;
		for(uint_fast8_t i = presc + 1; i > 1; i--)
		{
			if(buffer[i] > '9')
			{
				buffer[i]-=10;
				buffer[i-1]++;
			}
		}
		while(ptr[0] == '0')
		{
			*ptr-- = 0;
		}
		return exp10;
	}

}
