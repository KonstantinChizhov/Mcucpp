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

#ifdef __AVR__
enum
{	
	PlatformCyslesPerDelayLoop32 = 6,	
	PlatformCyslesPerDelayLoop16 = 4,	
	PlatformCyslesPerDelayLoop8 = 3
};

inline void PlatformDelayCycle16(uint16_t delayLoops);


inline void PlatformDelayCycle32(uint32_t delayLoops)
{		
	__asm volatile (
		"1: subi %A0, 1" "\n\t"
		"sbci %B0, 0"  "\n\t"
		"sbci %C0, 0"  "\n\t"
		"sbci %D0, 0"  "\n\t"
		"brne 1b"
		: "+d" (delayLoops)
	);
}

inline void PlatformDelayCycle16(uint16_t delayLoops)
{	
	__asm volatile (
		"1: sbiw %0,1" "\n\t"
		"brne 1b"
		: "=w" (delayLoops)
		: "0" (delayLoops)
	);
}

inline void PlatformDelayCycle8(uint8_t delayLoops)
{
	__asm volatile (
		"1: dec %0" "\n\t"
		"brne 1b"
		: "=r" (delayLoops)
		: "0" (delayLoops)	
	);
}
#elif defined(__ICCAVR__)

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
