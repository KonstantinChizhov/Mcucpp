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
