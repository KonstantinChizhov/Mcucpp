//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or 
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************

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
#elif defined(__AVR_ATmega1284P__) || defined(__AVR_ATmega1284__) // atmega1284
		typedef IO::PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7> AdcPins;
		const uint16_t InternalReference = 0x011A; // 1.1V
#else
#error ADC support is not implemented for this device yet.
#endif
	}
	
	class Adc
	{
		enum{ADC_FREE_RUN=5};
	public:
		typedef uint16_t DataT;
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
		
		static DataT ResolutionBits()
		{
			return 10;
		}
		
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

		static DataT Read()
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

		static DataT SingleConversion()
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
