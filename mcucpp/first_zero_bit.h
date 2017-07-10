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

#include <stdint.h>

namespace Mcucpp
{
	static inline uint_fast8_t FirstZeroBitPosition(uint8_t value)
	{
		if(!~value)
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
		if(!~value)
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
		if(!~value)
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
	
	// static inline int_fast8_t FirstZeroBitPosition(uint64_t value)
	// {
		// if(!~value)
			// return 0xff;
		// int_fast8_t bitPos = 0;
		// if((value & 0xffffffffull) == 0xffffffffulll)
		// {
			// bitPos+=32;
			// value>>=32;
		// }
		// if((value & 0xfffful) == 0xfffful)
		// {
			// bitPos+=16;
			// value>>=16;
		// }
		// if((value & 0xff) == 0xff)
		// {
			// bitPos+=8;
			// value>>=8;
		// }
		// if((value & 0xf) == 0xf)
		// {
			// bitPos+=4;
			// value>>=4;
		// }
		// if((value & 0x3) == 0x3)
		// {
			// bitPos+=2;
			// value>>=2;
		// }
		// if((value & 0x1) == 0x1)
		// {
			// bitPos+=1;
		// }
		// return  bitPos;
	// }
}