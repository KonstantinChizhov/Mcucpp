
#pragma once
#include <delegate.h>

namespace Mcucpp
{
	template<int number>
	class Adc
	{
	public:
		typedef uint16_t DataT;
		
		enum Vref
		{
			External = 0,
			VCC = 0
		};
		
		typedef unsigned AdcDivider;
		// Clock dividers
		static const AdcDivider Div2   = 2;
		static const AdcDivider Div4   = 4;
		static const AdcDivider Div6   = 6;
		static const AdcDivider Div8   = 8;
		
		enum SampleTime
		{
			SampleTime_1_5 = 0,
			SampleTime_7_5 = 1,
			SampleTime_13_5 = 2,
			SampleTime_28_5 = 3,
			SampleTime_41_5 = 4,
			SampleTime_55_5 = 5,
			SampleTime_71_5 = 6,
			SampleTime_239_5 = 7,
		};
		
		static DataT ResolutionBits()
		{
			return 12;
		}
		
		
		static void SetClockDivider(AdcDivider)
		{
		}

		static void SetVref(Vref)
		{
		}

		static void SetChannel(uint8_t channel)
		{
			
		}
		 
		static void Init(uint8_t channel, AdcDivider divider = Div2, Vref ref = VCC)
		{
			SetVref(ref);
			SetClockDivider(divider);
		}
		
		static void StartContinuousConversions()
		{
			
		}
		
		static void StartSingleConversion()
		{
			
		}

		static void EnableInterrupt()
		{
			
		}
		
		static uint16_t Read()
		{
			return DataSource();
		}
		
		static bool ResultReady()
		{
			return true;
		}

		static uint16_t SingleConversion()
		{
			return Read();
		}
		
		static void Stop()
		{
		}
		
		static void Enable()
		{
		}
		
		static void Disable()
		{
		}
		
		static bool ReadSequence(DataT *data, uint8_t *channels, uint8_t channelsCount)
		{
			return false;
		}
		
		static bool SequenceCompleted()
		{
			return false;
		}
		
		static void SetSampleTime(SampleTime sampleTime, uint8_t channel = 1)
		{
		}
		
		static Delegate<uint16_t> DataSource;
	};
	
	template<int number>
	Delegate<uint16_t> Adc<number>::DataSource;
	
	typedef Adc<1> Adc1;
	typedef Adc<2> Adc2;
}
