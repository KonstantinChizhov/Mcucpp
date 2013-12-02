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

#ifdef __MSP430__
enum
{
	PlatformCyslesPerDelayLoop32 = 4,
	PlatformCyslesPerDelayLoop16 = 3,
	PlatformCyslesPerDelayLoop8 = 3
};

inline void PlatformDelayCycle32(uint32_t delayLoops)
{
	uint16_t hi = delayLoops >> 16;
	uint16_t lo = delayLoops;
	__asm__ __volatile__
	(
		"1: \n"
		"add.w	#0xffff,	%[delayLoopsLow] \n"
		"addc.w	#0xffff,	%[delayLoopsHi] \n"
		"jc	1b \n"
        :   [delayLoopsLow] "+r"(lo),
            [delayLoopsHi] "+r"(hi)
	);
}

inline void PlatformDelayCycle16(uint16_t delayLoops)
{
	__asm__ __volatile__
	(
		"1: \n"
		" dec %[delayLoops] \n"
		" jne 1b \n"
        : [delayLoops] "+r"(delayLoops));
}

inline void PlatformDelayCycle8(uint8_t delayLoops)
{
	PlatformDelayCycle16(delayLoops);
}
#elif defined(__ICCMSP__)

#include <intrinsics.h>
enum
{
	PlatformCyslesPerDelayLoop32 = 1,
	PlatformCyslesPerDelayLoop16 = 1,
	PlatformCyslesPerDelayLoop8 = 1
};

inline void PlatformDelayCycle32(uint32_t delayLoops)
{
	__delay_cycles(delayLoops);
}

inline void PlatformDelayCycle16(uint16_t delayLoops)
{
	__delay_cycles(delayLoops);
}

inline void PlatformDelayCycle8(uint8_t delayLoops)
{
	__delay_cycles(delayLoops);
}
#endif
