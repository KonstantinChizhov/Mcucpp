#pragma once
#include <stdint.h>
#include <csr.h>
#include <arch.h>

// max 85 sec
inline void DelayLoop(uint32_t delayLoops)
{
	uint32_t start = rdcycle();
	while (rdcycle() - start < delayLoops)
		;
}
constexpr uint32_t PlatformCyslesPerDelayLoop32 = 1;
constexpr uint32_t PlatformCyslesPerDelayLoop16 = 1;
constexpr uint32_t PlatformCyslesPerDelayLoop8 = 1;

inline void PlatformDelayCycle32(uint32_t delayLoops) 
{
	DelayLoop(delayLoops);
}

inline void PlatformDelayCycle16(uint32_t delayLoops) 
{
	DelayLoop(delayLoops);
}

inline void PlatformDelayCycle8(uint32_t delayLoops) 
{
	DelayLoop(delayLoops);
}

