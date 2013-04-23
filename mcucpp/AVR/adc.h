#ifndef ADC_HPP
#define ADC_HPP

#include <__compatibility.h>
#include <stdint.h>
#include <pinlist.h>

namespace Mcucpp
{
	namespace Private
	{
		using namespace IO;
#if defined(__ATmega16__) || defined(__AVR_ATmega16__) // atmega16
		typedef IO::PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7> AdcPins;
		const uint16_t InternalReference = 0x028f; // 2.56V
#elif defined(__ATmega8__) || defined(__AVR_ATmega8__) || defined(__ATmega8A__) // atmega8
		typedef IO::PinList<Pc0, Pc1, Pc2, Pc3, Pc4, Pc5, NullPin, NullPin> AdcPins;
		const uint16_t InternalReference = 0x028f;// 2.56V
#elif defined(__ATmega168__) || defined(__AVR_ATmega168__) // atmega168
		typedef IO::PinList<Pc0, Pc1, Pc2, Pc3, Pc4, Pc5> AdcPins;
		const uint16_t InternalReference = 0x011A; // 1.1V
#else
#error ADC support is not implemented for this device yet.
#endif
	}
	
	class Adc
	{
		enum{ADC_FREE_RUN=5};
	public:
		typedef uint16_t DataType;
		// ACD internal reference voltage in fixed point Q8 format
		static const uint16_t InternalReference = Private::InternalReference;

		static const unsigned AdcTimeoutCycles = 128 * 25 / 3;
		
		enum Vref
		{
			External = 0 << REFS1 | 0 << REFS0,
			VCC      = 0 << REFS1 | 1 << REFS0,
			Internal = 1 << REFS1 | 1 << REFS0
		};

		enum AdcDivider
		{
			Div2	= 0 << ADPS2 | 0 << ADPS1 | 1 << ADPS0, 
			Div4	= 0 << ADPS2 | 1 << ADPS1 | 0 << ADPS0, 
			Div8	= 0 << ADPS2 | 1 << ADPS1 | 1 << ADPS0, 
			Div16	= 1 << ADPS2 | 0 << ADPS1 | 0 << ADPS0, 
			Div32	= 1 << ADPS2 | 0 << ADPS1 | 1 << ADPS0, 
			Div64	= 1 << ADPS2 | 1 << ADPS1 | 0 << ADPS0, 
			Div128	= 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0
		};

		enum 
		{
			DividerMask = 1 << ADPS2 | 1 << ADPS1 | 1 << ADPS0,
			MuxMask =  1 << MUX3 | 1 << MUX2 | 1 << MUX1 | 1 << MUX0,
			VrefMask = 1 << REFS1 | 1 << REFS0
		};
		
		enum ClockSource
		{
			MainClock = 0,
			AdcClock = 0
		};
		
		static void SetClockSource(ClockSource)
		{
			// Nothing to be done
		}

		static void SetClockDivider(AdcDivider divider)
		{
			ADCSRA = (ADCSRA & DividerMask) | divider;
		}

		static void SetVref(Vref ref)
		{
			ADMUX = (ADMUX & ~VrefMask) | ref;
		}

		static void SetChannel(uint8_t channel)
		{
			ADMUX = (ADMUX & ~MuxMask) | (channel & MuxMask);
		}
		 
		static void Init(uint8_t channel = 0, AdcDivider divider = Div32, Vref ref = VCC)
		{
			ADMUX = (ADMUX & ~(MuxMask | VrefMask)) | (channel & MuxMask) | ref;
			ADCSRA = (ADCSRA & DividerMask) | divider | 1 << ADEN;
		}
		
		static void StartContinuousConversions()
		{
			ADCSRA |= _BV(ADC_FREE_RUN) | _BV(ADSC) | _BV(ADEN);
		}

		static void EnableInterrupt()
		{
			ADCSRA |= _BV(ADIE) | _BV(ADEN);
		}

		static void StartSingleConversion()
		{
			ADCSRA |= 1 << ADSC | 1 << ADEN;
		}

		static uint16_t Read()
		{
			unsigned timeout = AdcTimeoutCycles;
			while (!ResultReady() && --timeout)
				;
			if(timeout == 0)
				return 0xffff;
			return ADC;
		}

		static bool ResultReady()
		{
			return (ADCSRA & (1 << ADSC)) == 0;
		}

		static uint16_t SingleConversion()
		{
			StartSingleConversion();
			return Read();
		}

		static void Stop()
		{
			ADMUX=0;
			ADCSRA=0;
		}
		
		static void Enable()
		{
			ADCSRA |= 1 << ADEN;
		}
		
		static void Disable()
		{
			ADCSRA &= ~(1 << ADEN);
		}
	};
	
	typedef Adc Adc1;
}

#endif
