#ifndef LED_HPP
#define LED_HPP

//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2010
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


#include "static_assert.h"
#include <stdlib.h>

class LedMapTable
{
public:
	static uint8_t Map(uint8_t bcd)
	{
		return pgm_read_byte(PSTR("\x3f\x06\x5b\x4f\x66\x6d\x7d\x7\x7f\x6f\x77\x7c\x39\x5e\x79\x71")+bcd);
	}

	static uint8_t Minus(){return 0x40;}
	static uint8_t Empty(){return 0x0;}
	 
};

template<class Segments, class Commons, class MapTable=LedMapTable>
class LedDisplay
{
public:
	static void WriteDec(int16_t value)
	{
		int16_t i= 0; 
		uint8_t minus=0;
	    if (value < 0)  
		{
	        value = -value;
			minus = 1;
		}
		div_t res;
	    do {
			res = div(value, 10);
	        _value[NumDidgits - 1 - i++] = MapTable::Map(res.rem);
			if(i>=NumDidgits)return;
	    } while ((value = res.quot) > 0);
	    if (minus)
	        _value[NumDidgits - 1 - i++] = MapTable::Minus();
		while(i < NumDidgits)
			_value[NumDidgits - 1 - i++] = MapTable::Empty();
	}

	static void Write(int16_t value)
	{
		itoa(value, (char*)_value, 10);
		uint8_t i=NumDidgits;
		do
		{
			_value[i] = MapTable::Map(_value[i] - '0');
		}while(i--);
	}

	/*void WriteDec(int32_t value)
	{
		int16_t i= 0; 
		uint8_t minus=0;
	    if (value < 0)  
		{
	        value = -value;
			minus = 1;
		}
		div_t res;
	    do {
			res = div(value, 10);
	        _value[NumDidgits - 1 - i++] = _table.Map(res.rem);
			if(i>=NumDidgits)return;
	    } while ((value = res.quot) > 0);
	    if (minus)
	        _value[i++] = _table.Minus();
		while(i++ < NumDidgits)
			_value[NumDidgits - 1 - i] = _table.Empty();
	}*/

	static void WriteHex(uint16_t value)
	{
		for(uint8_t i = 0; i < NumDidgits; i++)
		{
			uint8_t rem = value & 0x0f;
			_value[NumDidgits-i-1] = MapTable::Map(rem);
			value >>= 4;
			if(value==0 && rem==0)
			{
				_value[NumDidgits-i-1] = 0;
			}	
		}
	}

	static void WriteHex(uint32_t value)
	{
		for(uint8_t i = 0; i < NumDidgits; i++)
		{
			uint8_t rem = value & 0x0f;
			_value[NumDidgits-i-1] = MapTable::Map(rem);
			value >>= 4;
			if(value==0 && rem==0)
			{
				_value[NumDidgits-i-1] = 0;
			}	
		}
	}

	static void Update()
	{
		Commons::SetConfiguration(Commons::Out);
		Segments::SetConfiguration(Segments::Out);

		Commons::Write(1 << position);
		Segments::Write(_value[position++]);
		if(position >= NumDidgits)
			position = 0;
	}
	
protected:
	static uint8_t position;
	enum{NumDidgits = (Commons::Length)};
	static uint8_t _value[Commons::Length];
};

template<class Segments, class Commons, class MapTable>
uint8_t LedDisplay<Segments, Commons, MapTable>::position;

template<class Segments, class Commons, class MapTable>
uint8_t LedDisplay<Segments, Commons, MapTable>::_value[Commons::Length];

#endif
