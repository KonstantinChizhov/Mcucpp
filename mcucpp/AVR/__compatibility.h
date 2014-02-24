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

// Compiler compatibility header
#pragma once

#if defined(__ICCAVR__) // IAR for AVR
	#define ENABLE_BIT_DEFINITIONS
	#include <ioavr.h>
	#include <intrinsics.h>
	
static inline uint8_t ReadLowFuse()
{
	return _SPM_GET_FUSEBITS();
}

#elif defined(__AVR__) // avr-gcc
	#include <avr/io.h>
	
	#include <avr/interrupt.h>
	#include <avr/builtins.h>
	#include <avr/eeprom.h>

	#if defined(SPMCSR) || defined(SPMCR)
		#include <avr/boot.h>
		static inline uint8_t ReadLowFuse()
		{
			return boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS);
		}
	#else
		
		static inline uint8_t ReadLowFuse()
		{
			return LFUSE_DEFAULT;
		}
	#endif
#else
	#error "Unknown compiler"
#endif