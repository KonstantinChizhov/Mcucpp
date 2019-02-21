
//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2018
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

#include <mcu_header.h>
#include <clock.h>

namespace Mcucpp
{
	class HwCrc
	{
	public:

		template<class CrcParams>
		static void Init()
		{
			Clock::CrcClock::Enable();

			CRC->INIT = CrcParams::Init;
			CRC->POL = CrcParams::Poly;
			uint32_t refin = 0, polySize = 0;

			if(CrcParams::Width == 32)
			{
				polySize = 0;
				refin = CRC_CR_REV_IN_0 | CRC_CR_REV_IN_1;
			}
			if(CrcParams::Width == 16)
			{
				polySize = CRC_CR_POLYSIZE_0;
				refin = CRC_CR_REV_IN_1;
			}
			if(CrcParams::Width == 8)
			{
				polySize = CRC_CR_POLYSIZE_1;
				refin = CRC_CR_REV_IN_0;
			}
			if(CrcParams::Width == 7)
			{
				polySize = CRC_CR_POLYSIZE_0 | CRC_CR_POLYSIZE_1;
				refin = CRC_CR_REV_IN_0;
			}

			refin = CRC_CR_REV_IN_1;

			CRC->CR = 0
					//(CrcParams::RefOut ? CRC_CR_REV_OUT : 0)
					//| (CrcParams::RefIn ? refin : 0)
					 // CRC_CR_REV_OUT
					// | CRC_CR_REV_IN_0
					// | CRC_CR_REV_IN_1
					| polySize
					| CRC_CR_RESET;
		}

		static void Reset()
		{
			CRC->CR |= CRC_CR_RESET;
		}

		static void Update32(uint32_t value)
		{
			CRC->DR = value;
		}

		static void Update16(uint16_t value)
		{
			*((volatile uint16_t*)&CRC->DR) = value;
		}

		static void Update8(uint8_t value)
		{
			*((volatile uint8_t*)&CRC->DR) = value;
		}

		static uint32_t Get()
		{
			return CRC->DR;
		}
	};
}
