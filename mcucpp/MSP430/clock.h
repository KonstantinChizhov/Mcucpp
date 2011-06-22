#pragma once

#ifndef F_CPU
#error F_CPU must be defined to proper cpu frequency
#endif
namespace Clock
{
	class SysClock
	{
	public:
		static unsigned long FCore()
		{
			return F_CPU;
		}
		
		static unsigned long FPeriph()
		{
			return F_CPU;
		}
		
		static const unsigned long CpuFreq = F_CPU;
	};
	
}