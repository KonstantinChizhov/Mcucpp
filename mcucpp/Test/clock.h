//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2016
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
	namespace Clock
	{
		class SysClock
		{
		public:
			enum ClockSource
			{
				Internal = 0,
				External = 1
			};
			
			enum ErrorCode
			{
				Success = 0,
				ClockSourceFailed = 1,
				InvalidClockSource = 2,
				ClockSelectFailed = 3
			};
			
		public:
			static uint32_t MaxFreq() {return 2000000000u;}
			static inline ErrorCode SelectClockSource(ClockSource /*clockSource*/){return Success;}
			static inline uint32_t SetClockFreq(uint32_t freq){CpuFreq = freq; return CpuFreq;}
			static inline uint32_t ClockFreq(){return CpuFreq;}
			static inline uint32_t SrcClockFreq(){return CpuFreq;}
			
			static uint32_t CpuFreq;
		};
	}
}