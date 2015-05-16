//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2012
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

// platform specific delay loop implementation
#include <platform_dalay.h>

namespace Mcucpp
{
	template<unsigned long ns, unsigned long CpuFreq>
	void delay_ns()
	{
                const unsigned long delayLoops32 = (unsigned long)(CpuFreq / (1.0e9 * PlatformCyslesPerDelayLoop32) * ns);
                const unsigned long delayLoops16 = (unsigned long)(CpuFreq / (1.0e9 * PlatformCyslesPerDelayLoop16) * ns);
                const unsigned long delayLoops8 = (unsigned long)(CpuFreq / (1.0e9 * PlatformCyslesPerDelayLoop8) * ns);

		if(delayLoops16 > 0xfffful)
			PlatformDelayCycle32(delayLoops32);
		else if(delayLoops8 > 0xfful)
			PlatformDelayCycle16((uint16_t)delayLoops16);
		else
		{
			if(delayLoops8 == 0) return;
			PlatformDelayCycle8((uint8_t)delayLoops8);
		}
	}

	template<unsigned long us, unsigned long CpuFreq >
	void delay_us()
	{
                const unsigned long delayLoops32 = (unsigned long)(CpuFreq / (1.0e6 * PlatformCyslesPerDelayLoop32) * us);
                const unsigned long delayLoops16 = (unsigned long)(CpuFreq / (1.0e6 * PlatformCyslesPerDelayLoop16) * us);
                const unsigned long delayLoops8 = (unsigned long)(CpuFreq / (1.0e6 * PlatformCyslesPerDelayLoop8) * us);

		if(delayLoops16 > 0xfffful)
			PlatformDelayCycle32(delayLoops32);
		else if(delayLoops8 > 0xfful)
			PlatformDelayCycle16((uint16_t)delayLoops16);
		else
		{
			if(delayLoops8 == 0) return;
			PlatformDelayCycle8((uint8_t)delayLoops8);
		}
	}

	template<unsigned long ms, unsigned long CpuFreq>
	void delay_ms()
	{
		delay_us<ms * 1000, CpuFreq>();
	}
}
