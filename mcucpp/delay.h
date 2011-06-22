#pragma once

// platform specific delay loop implementation
#include <platform_dalay.h>
#include <clock.h>

namespace Util
{
	template<unsigned long ns, unsigned long CpuFreq>
	void delay_ns()
	{
		const unsigned long delayLoops32 = CpuFreq / (1.0e9 * PlatformCyslesPerDelayLoop32) * ns;
		const unsigned long delayLoops16 = CpuFreq / (1.0e9 * PlatformCyslesPerDelayLoop16) * ns;
		const unsigned long delayLoops8 = CpuFreq / (1.0e9 * PlatformCyslesPerDelayLoop8) * ns;

		if(delayLoops16 > 0xfffful)
			PlatformDelayCycle32(delayLoops32);
		else if(delayLoops8 > 0xfful)
			PlatformDelayCycle16((uint16_t)delayLoops16);
		else PlatformDelayCycle8((uint8_t)delayLoops8);
	}

	template<unsigned long us, unsigned long CpuFreq >
	void delay_us()
	{
		const unsigned long delayLoops32 = CpuFreq / (1.0e6 * PlatformCyslesPerDelayLoop32) * us;
		const unsigned long delayLoops16 = CpuFreq / (1.0e6 * PlatformCyslesPerDelayLoop16) * us;
		const unsigned long delayLoops8 = CpuFreq / (1.0e6 * PlatformCyslesPerDelayLoop8) * us;

		if(delayLoops16 > 0xfffful)
			PlatformDelayCycle32(delayLoops32);
		else if(delayLoops8 > 0xfful)
			PlatformDelayCycle16((uint16_t)delayLoops16);
		else PlatformDelayCycle8((uint8_t)delayLoops8);
	}

	template<unsigned long ms, unsigned long CpuFreq>
	void delay_ms()
	{
		delay_us<ms * 1000, CpuFreq>();
	}
}
