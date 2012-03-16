#pragma once

namespace Clock
{
	class SysClock
	{
	public:
		static unsigned long FCore()
		{
			return CpuFreq;
		}
		
		static unsigned long FPeriph()
		{
			return CpuFreq;
		}
		
		static const unsigned long CpuFreq = 10000000;
	};
	
}
