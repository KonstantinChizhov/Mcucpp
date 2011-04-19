#ifndef ADC_HPP
#define ADC_HPP
#include <avr/io.h>
#include <avr/interrupt.h>


class Adc
{
public:
	enum{ADC_FREE_RUN=5};
	
	enum Vref
	{
		External	= 0 << REFS1 | 0 << REFS0,
		VCC			= 0 << REFS1 | 1 << REFS0,
		Int2_56		= 1 << REFS1 | 1 << REFS0
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
		MuxMask = 1 << MUX4 | 1 << MUX3 | 1 << MUX2 | 1 << MUX1 | 1 << MUX0,
		VrefMask = 1 << REFS1 | 1 << REFS0
	};

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
	
	static void StartFreeRun()
	{
		ADCSRA |= _BV(ADC_FREE_RUN) | _BV(ADSC) | _BV(ADEN);
	}

	static void StartFreeRunInterrupt()
	{
		ADCSRA |= _BV(ADC_FREE_RUN) | _BV(ADIE) | _BV(ADSC) | _BV(ADEN);
	}

	static int16_t ReadSingle()
	{
		ADCSRA |= 1 << ADSC | 1 << ADEN;
		while (ADCSRA & (1 << ADSC));
		return ADC;
	}

	static void Stop()
	{
		ADMUX=0;
		ADCSRA=0;
	}

protected:

};


#endif
