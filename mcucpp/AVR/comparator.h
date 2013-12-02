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
