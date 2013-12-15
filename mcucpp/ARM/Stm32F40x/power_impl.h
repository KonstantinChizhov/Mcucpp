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
	namespace Private
	{
		IO_BITFIELD_WRAPPER(PWR->CR, PwrPls, uint32_t, 5, 3);
	}
	
	bool Power::Detect()
	{
		return (PWR->CSR & PWR_CSR_PVDO) == 0;
	}
	
	unsigned Power::GetVdd()
	{
		uint8_t voltageTable[] = {20, 21, 23, 25, 26, 27, 28, 29};
		PWR->CR |= PWR_CR_PVDE;
		uint32_t tmp = Private::PwrPls::Get();
		for(int i = 0; i < 8; i++)
		{
			Private::PwrPls::Set((uint32_t)i);
			if(!Detect())
			{
				Private::PwrPls::Set(tmp);
				return voltageTable[i];
			}
		}
		return 30;
	}
}
