
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
#include <stm32f4xx.h>
#include <clock.h>

namespace Mcucpp
{
	
	class HwRandom
	{
		static const unsigned Timeout = 40 * 4;
	public:
		static inline void Init();
		static inline uint32_t Next();
		static inline uint32_t Next(uint32_t maxVal);
		static inline bool IsOk();
	};
	
	void HwRandom::Init()
	{
		Clock::RngClock::Enable();
		if((RNG->CR & RNG_CR_RNGEN) == 0)
		{
			RNG->CR |= RNG_CR_RNGEN;
		}
	}
	
	HwRandom::HwRandom()
	{
		Init();
	}
	
	uint32_t HwRandom::Next()
	{
		Init();
		unsigned timeout = Timeout;
		while((RNG->SR & RNG_SR_DRDY) == 0 && --timeout){}
		return timeout != 0 ? RNG->DR : 0;
	}
	
	uint32_t HwRandom::Next(uint32_t maxVal)
	{
		return Next() % maxVal;
	}
	
	bool HwRandom::IsOk()
	{
		return (RNG->SR & (RNG_SR_CECS | RNG_SR_SECS)) == 0;
	}
}
