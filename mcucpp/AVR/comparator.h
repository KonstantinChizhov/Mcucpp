#pragma once

#include <avr/io.h>

class Comparator
{
protected:
	enum{AdmuxMask = (1<<MUX0)|(1<<MUX1)|(1<<MUX2)|(1<<MUX3)};
	static volatile uint8_t& Control()
	{
		return ACSR;
	}
	static volatile uint8_t& Multiplexer()
	{
		return ADMUX;
	}
	static void MuxEnable()
	{
		ADCSR &= ~(1<<ADEN);	//turn off ADC
		ACSR |= (1<<ACME);		//comparator multiplexer enable
	}

	static void MuxDisable()
	{
		ACSR &= ~(1<<ACME);
	}

public:
	enum ComparatorChannel {AIN1=255, ADC0=0, ADC1=1, ADC2=2, ADC3=3, ADC4=4, ADC5=6, ADC6=6, ADC7=7, ADC8=8, ADC9=1, ADC10=10};
	enum InterruptMode{Togle=0, Falling=2, Rising=3};

	static void SetChannel(ComparatorChannel ch)
	{
		if(ch == AIN1)
		{
			MuxDisable();
		}
		else
		{
			MuxEnable();
			Multiplexer() = (Multiplexer() & AdmuxMask) | ch;
		}
	}

	static void EnableInterrupt(InterruptMode mode)
	{
		Control() |= (1 << ACIE) | (mode << ACIS0);
	}

	static void DisableInterrupt()
	{
		Control() &= ~(1 << ACIE);
	}
	
	static uint8_t IsSet()
	{
		return Control() & (1<<ACO);
	}
};
