#pragma once
#include <stdint.h>
#include <core_cm4.h>

namespace
{
	class DwtInitializer
	{
	public:
		DwtInitializer()
		{
			if (!(CoreDebug->DEMCR & CoreDebug_DEMCR_TRCENA_Msk)) 
			{
				CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
				DWT->CYCCNT = 0;
				DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
			}
		}
	};
	
	DwtInitializer dwtInitializer;
}

inline void DelayLoop(uint32_t delayLoops)
{
	uint32_t start = DWT->CYCCNT;
	while(DWT->CYCCNT - start < delayLoops);
}

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

enum
{	
	PlatformCyslesPerDelayLoop32 = 1,
	PlatformCyslesPerDelayLoop16 = 1,
	PlatformCyslesPerDelayLoop8 = 1
};


