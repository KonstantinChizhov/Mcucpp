
#include <iopins.h>
namespace Mcucpp
{
	class Mco
	{
	public:
		enum Source
		{
			HsiClock = 0,
			LseOsc = RCC_CFGR_MCO1_0,
			HseOsc = RCC_CFGR_MCO1_1,
			PllClock = RCC_CFGR_MCO1,
		};
		
		enum Div
		{
			Div1 = 0,
			Div2 = RCC_CFGR_MCO1PRE_2,
			Div3 = RCC_CFGR_MCO1PRE_2 | RCC_CFGR_MCO1PRE_0,
			Div4 = RCC_CFGR_MCO1PRE_2 | RCC_CFGR_MCO1PRE_1,
			Div5 = RCC_CFGR_MCO1PRE
		};
		
		static void Enable(Source src, Div div = Div1)
		{
			RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_MCO1 | RCC_CFGR_MCO1PRE)) | src | div;
			IO::Pa8::AltFuncNumber(0);
			IO::Pa8::SetConfiguration(IO::Pa8::Port::AltFunc);
		}
		
		static void Disable()
		{
			IO::Pa8::SetConfiguration(IO::Pa8::Port::In);
		}
	};
	
	class Mco2
	{
	public:
		enum Source
		{
			SysClock = 0,
			PllI2sClock = RCC_CFGR_MCO2_0,
			HseOsc = RCC_CFGR_MCO2_1,
			PllClock = RCC_CFGR_MCO2,
		};
		
		enum Div
		{
			Div1 = 0,
			Div2 = RCC_CFGR_MCO2PRE_2,
			Div3 = RCC_CFGR_MCO2PRE_2 | RCC_CFGR_MCO2PRE_0,
			Div4 = RCC_CFGR_MCO2PRE_2 | RCC_CFGR_MCO2PRE_1,
			Div5 = RCC_CFGR_MCO2PRE
		};
		
		static void Enable(Source src, Div div = Div1)
		{
			RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_MCO2 | RCC_CFGR_MCO2PRE)) | src | div;
			IO::Pc9::AltFuncNumber(0);
			IO::Pc9::SetConfiguration(IO::Pc9::Port::AltFunc);
		}
		
		static void Disable()
		{
			IO::Pc9::SetConfiguration(IO::Pc9::Port::In);
		}
	};
}