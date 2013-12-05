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
#include <stddef.h>
#include "ioreg.h"
#include "stm32f4xx.h"

namespace Mcucpp
{
	class Flash
	{
		static void ConfigureFreq(uint32_t clockFreq)
		{
			// use PVD to get it
			const uint32_t div = 24000000; // for 2.4 - 2.7 V voltage range
			uint32_t ws = (clockFreq-1) / div;
			if(ws > 7)
				ws = 7;
			FLASH->ACR = FLASH_ACR_PRFTEN | 
						FLASH_ACR_ICEN |
						FLASH_ACR_DCEN |
						ws;
		}
		
		static uint32_t TotalSize();
		static uint32_t PageCount();
		static uint32_t PageAddress(unsigned page);
		static bool ErasePage(unsigned page);
		static bool WritePage(unsigned page, void *data, size_t length);
		static bool MassErase();
		static bool WriteProtect(bool enable);
		static bool ReadoutProtect(bool enable);
		static bool WriteProtectEnabled();
		static bool ReadoutProtectEnabled();
	};
}