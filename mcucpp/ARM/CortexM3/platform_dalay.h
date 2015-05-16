#pragma once
#include <stdint.h>

#if defined(__GNUC__)	// gcc compiler
inline void DelayLoop(uint32_t delayLoops)
{
	__asm__ __volatile__
	(
		"1: \n"
		" CBZ %[delayLoops], 2f \n"
		" SUB %[delayLoops], %[delayLoops], #1 \n"
		" B 1b \n"
		"2: \n"
		: [delayLoops] "+l"(delayLoops)
	);
}

enum
{	
	PlatformCyslesPerDelayLoop32 = 4,
	PlatformCyslesPerDelayLoop16 = 4,
	PlatformCyslesPerDelayLoop8 = 4
};

#elif defined(__ICCARM__)	// IAR compiler
#include <intrinsics.h>

inline void DelayLoop(uint32_t delayLoops)
{
	do
	{
	// TODO: rewrite in assembly
	 // __no_operation();
	}while(delayLoops--);
}	
enum
{	
	PlatformCyslesPerDelayLoop32 = 4,
	PlatformCyslesPerDelayLoop16 = 4,
	PlatformCyslesPerDelayLoop8 = 4
};

#elif defined ( __CC_ARM   ) // ARM (RealView) compiler
inline void DelayLoop(uint32_t delayLoops)
{
	__asm{
		start:
		SUBS     delayLoops,delayLoops,#1	// 1
		BCS      start						// 1+2
	}
}
enum
{	
	PlatformCyslesPerDelayLoop32 = 4,
	PlatformCyslesPerDelayLoop16 = 4,
	PlatformCyslesPerDelayLoop8 = 4
};

#else
#error Not supported compiler
#endif


inline void PlatformDelayCycle16(uint16_t delayLoops) 
{
	DelayLoop(delayLoops);
}

inline void PlatformDelayCycle8(uint8_t delayLoops)
{
	DelayLoop(delayLoops);
}

inline void PlatformDelayCycle32(uint32_t delayLoops)
{
	DelayLoop(delayLoops);
}
