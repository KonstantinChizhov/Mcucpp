
#pragma once

#include <clock.h>
#include <iopins.h>
#include <pinlist.h>

namespace Mcucpp
{
	namespace Private
	{
		typedef IO::PinList
		<
			IO::Pd0,
			IO::Pd1,
			IO::Pd2,
			IO::Pd3,
			IO::Pd4,
			IO::Pd5,
			IO::Pd6,
			IO::Pd7,
			IO::Pd8,
			IO::Pd9,
			IO::Pd10,
			IO::Pd11,
			IO::Pd12,
			IO::Pd13,
			IO::Pd14,
			IO::Pd15
		> AdcPins;
	}
	
	class Adc1
	{
		static const unsigned ChannelSelectClearMask = ADC1_CFG_REG_CHS_Msk | ADC1_CFG_SEL_VREF | ADC1_CFG_SEL_TS;
		// Maximum ADC convertion time MaxCockDivider * AdcConvertionCycles / cpu cycle per wait loop
		static const unsigned AdcTimeoutCycles = 2048 * 28 / 4;
	public:
		enum Vref
		{
			External = ADC1_CFG_M_REF,
			VCC      = 0,
			Internal = ADC2_CFG_ADC1_OP
		};

		enum AdcDivider
		{
			Div1	= 0 << ADC1_CFG_REG_DIVCLK_Pos,
			Div2	= 1 << ADC1_CFG_REG_DIVCLK_Pos,
			Div4	= 2 << ADC1_CFG_REG_DIVCLK_Pos,
			Div8	= 3 << ADC1_CFG_REG_DIVCLK_Pos,
			Div16	= 4 << ADC1_CFG_REG_DIVCLK_Pos,
			Div32	= 5 << ADC1_CFG_REG_DIVCLK_Pos,
			Div64	= 6 << ADC1_CFG_REG_DIVCLK_Pos,
			Div128	= 7 << ADC1_CFG_REG_DIVCLK_Pos,
			Div256  = 8 << ADC1_CFG_REG_DIVCLK_Pos,
			Div512  = 9 << ADC1_CFG_REG_DIVCLK_Pos,
			Div1024 = 10 << ADC1_CFG_REG_DIVCLK_Pos,
			Div2048 = 11 << ADC1_CFG_REG_DIVCLK_Pos
		};

		enum ClockSource
		{
			MainClock = 0,
			AdcClock = ADC2_CFG_REG_CLKS
		};
		
		static const unsigned TempSensorChannel = 31;
		static const unsigned ReferenceChannel = 30;
		
		static void SetClockSource(ClockSource clockSrc)
		{
			
			MDR_ADC->ADC2_CFG = (MDR_ADC->ADC2_CFG & ~ADC2_CFG_REG_CLKS) | (clockSrc & ADC2_CFG_REG_CLKS);
		}
		
		static void SetClockDivider(AdcDivider divider)
		{
			MDR_ADC->ADC1_CFG = (MDR_ADC->ADC1_CFG & ~ADC1_CFG_M_REF) | divider;
		}

		static void SetVref(Vref ref)
		{
			MDR_ADC->ADC1_CFG = (MDR_ADC->ADC1_CFG & ~ADC1_CFG_M_REF) | ref | ADC1_CFG_REG_ADON;
			MDR_ADC->ADC2_CFG = (MDR_ADC->ADC2_CFG & ~ADC2_CFG_ADC1_OP) | (ref & ADC2_CFG_ADC1_OP);
		}

		static void SetChannel(unsigned channel)
		{
			if(channel < Private::AdcPins::Length)
			{
				Private::AdcPins::SetConfiguration(1u << channel, Private::AdcPins::AnalogIn);
			}
			
			unsigned vrefAndTempSensFlags = 0;
			if(channel == 30)
				vrefAndTempSensFlags = ADC1_CFG_SEL_VREF | ADC1_CFG_TS_EN;
			if(channel == 31)
				vrefAndTempSensFlags = ADC1_CFG_SEL_TS | ADC1_CFG_TS_EN;
			
			MDR_ADC->ADC1_CFG = vrefAndTempSensFlags | ADC1_CFG_REG_ADON | (MDR_ADC->ADC1_CFG & ~ChannelSelectClearMask) | ((channel << ADC1_CFG_REG_CHS_Pos) & ADC1_CFG_REG_CHS_Msk);
		}
		 
		static void Init(uint8_t channel = 0, AdcDivider divider = Div32, Vref ref = VCC)
		{
			Enable();
			SetChannel(channel);
			SetClockDivider(divider);
			SetVref(ref);
		}
		
		static void StartContinuousConversions()
		{
			MDR_ADC->ADC1_CFG |= ADC1_CFG_REG_SAMPLE | ADC1_CFG_REG_GO;
		}
		
		static void StartSingleConversion()
		{
			MDR_ADC->ADC1_CFG |= ADC1_CFG_REG_GO;
		}

		static void EnableInterrupt()
		{
			MDR_ADC->ADC1_STATUS |= ADC_STATUS_ECOIF_IE;
		}
		
		static uint16_t Read()
		{
			return MDR_ADC->ADC1_RESULT & ADC_RESULT_Msk;
		}
		
		static bool ResultReady()
		{
			return (MDR_ADC->ADC1_STATUS & ADC_STATUS_FLG_REG_EOCIF);
		}

		static uint16_t SingleConversion()
		{
			uint32_t timeout = AdcTimeoutCycles;
			StartSingleConversion();
			while (!ResultReady() && --timeout)
				;
			if(timeout == 0)
				return 0xffff;
			return MDR_ADC->ADC1_RESULT & ADC_RESULT_Msk;
		}

		static void Stop()
		{
			MDR_ADC->ADC1_CFG = 0;
		}
		
		static void Enable()
		{
			Clock::AdcClock::Enable();
		}
		
		static void Disable()
		{
			Clock::AdcClock::Disable();
		}
	};
	
	class Adc2
	{
		static const unsigned ChannelSelectClearMask = ADC2_CFG_REG_CHS_Msk;
		// Maximum ADC convertion time MaxCockDivider * AdcConvertionCycles / cpu cycle per wait loop
		static const unsigned AdcTimeoutCycles = 2048 * 28 / 4; 
	public:
		enum Vref
		{
			External = ADC2_CFG_M_REF,
			VCC      = 0,
			Internal = ADC2_CFG_ADC2_OP
		};

		enum AdcDivider
		{
			Div1	= 0 << ADC2_CFG_REG_DIVCLK_Pos,
			Div2	= 1 << ADC2_CFG_REG_DIVCLK_Pos,
			Div4	= 2 << ADC2_CFG_REG_DIVCLK_Pos,
			Div8	= 3 << ADC2_CFG_REG_DIVCLK_Pos,
			Div16	= 4 << ADC2_CFG_REG_DIVCLK_Pos,
			Div32	= 5 << ADC2_CFG_REG_DIVCLK_Pos,
			Div64	= 6 << ADC2_CFG_REG_DIVCLK_Pos,
			Div128	= 7 << ADC2_CFG_REG_DIVCLK_Pos,
			Div256  = 8 << ADC2_CFG_REG_DIVCLK_Pos,
			Div512  = 9 << ADC2_CFG_REG_DIVCLK_Pos,
			Div1024 = 10 << ADC2_CFG_REG_DIVCLK_Pos,
			Div2048 = 11 << ADC2_CFG_REG_DIVCLK_Pos
		};

		enum ClockSource
		{
			MainClock = 0,
			AdcClock = ADC2_CFG_REG_CLKS
		};
				
		static void SetClockSource(ClockSource clockSrc)
		{
			MDR_ADC->ADC2_CFG = (MDR_ADC->ADC2_CFG & ~ADC2_CFG_REG_CLKS) | (clockSrc & ADC2_CFG_REG_CLKS);
		}
		
		static void SetClockDivider(AdcDivider divider)
		{
			MDR_ADC->ADC2_CFG = (MDR_ADC->ADC2_CFG & ~ADC2_CFG_M_REF) | divider;
		}

		static void SetVref(Vref ref)
		{
			MDR_ADC->ADC2_CFG = (MDR_ADC->ADC2_CFG & ~ADC2_CFG_M_REF) | ref | ADC2_CFG_REG_ADON;
			MDR_ADC->ADC2_CFG = (MDR_ADC->ADC2_CFG & ~ADC2_CFG_ADC2_OP) | (ref & ADC2_CFG_ADC2_OP);
		}

		static void SetChannel(unsigned channel)
		{	
			if(channel < Private::AdcPins::Length)
			{
				Private::AdcPins::SetConfiguration(1u << channel, Private::AdcPins::AnalogIn);
			}
			
			MDR_ADC->ADC2_CFG = ADC2_CFG_REG_ADON | (MDR_ADC->ADC2_CFG & ~ChannelSelectClearMask) | ((channel << ADC2_CFG_REG_CHS_Pos) & ADC2_CFG_REG_CHS_Msk);
		}
		 
		static void Init(uint8_t channel = 0, AdcDivider divider = Div32, Vref ref = VCC)
		{
			Enable();
			SetChannel(channel);
			SetClockDivider(divider);
			SetVref(ref);
		}
		
		static void StartContinuousConversions()
		{
			MDR_ADC->ADC2_CFG |= ADC2_CFG_REG_SAMPLE | ADC2_CFG_REG_GO;
		}
		
		static void StartSingleConversion()
		{
			MDR_ADC->ADC2_CFG |= ADC2_CFG_REG_GO;
		}

		static void EnableInterrupt()
		{
			MDR_ADC->ADC2_STATUS |= ADC_STATUS_ECOIF_IE;
		}
		
		static uint16_t Read()
		{
			return MDR_ADC->ADC2_RESULT & ADC_RESULT_Msk;
		}
		
		static bool ResultReady()
		{
			return (MDR_ADC->ADC2_STATUS & ADC_STATUS_FLG_REG_EOCIF);
		}

		static uint16_t SingleConversion()
		{
			uint32_t timeout = AdcTimeoutCycles;
			StartSingleConversion();
			while (!ResultReady() && --timeout)
				;
			if(timeout == 0)
				return 0xffff;
			return MDR_ADC->ADC2_RESULT & ADC_RESULT_Msk;
		}

		static void Stop()
		{
			MDR_ADC->ADC2_CFG = 0;
		}
		
		static void Enable()
		{
			Clock::AdcClock::Enable();
		}
		
		static void Disable()
		{
			Clock::AdcClock::Disable();
		}
	};
}