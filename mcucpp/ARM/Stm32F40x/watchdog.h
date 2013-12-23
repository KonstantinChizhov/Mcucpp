
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

namespace Mcucpp
{
	class Watchdog
	{
		static const uint16_t ReloadKey = 0xAAAA;
		static const uint16_t EnableKey = 0xCCCC;
		static const uint16_t AccessKey = 0x5555;
	public:
		static const unsigned ClockFreq = 32000;
		
		static void Start(unsigned periodMSec)
		{
			const unsigned MaxReload = (1 << 12) - 1;
			unsigned reload  = (periodMSec * ClockFreq) / 1000;
			unsigned prescaller = 0;
			unsigned divider = 4;
			while(reload / divider > MaxReload)
			{
				divider*=2;
				prescaller++;
			}
			reload /= divider;
			if(divider > 256)
			{
				prescaller = 7;
				reload = MaxReload;
			}
			IWDG->KR = AccessKey;
			IWDG->PR = prescaller;
			IWDG->RLR = reload;
			IWDG->KR = ReloadKey;
			IWDG->KR = EnableKey;
		}
		
		static void Disable()
		{
			
		}
		
		static void Reset()
		{
			IWDG->KR = ReloadKey;
		}
		
		static bool CauseReset()
		{
			return RCC->CSR & RCC_CSR_IWDGRSTF;
		}
	};
}