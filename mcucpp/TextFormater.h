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



#pragma once
#include <stdlib.h>
#include <util.h>

template<class DATA_SOURCE, int Base = 10, uint8_t fieldSize=8>
class TextFormater :public DATA_SOURCE
{
	typedef TextFormater<DATA_SOURCE, Base, fieldSize> SelfType;
public:

	SelfType& operator<< (int value)
	{
		char buffer[7];
		itoa(value, buffer, Base);
		Puts(buffer);
		return *this;
	}

	SelfType& operator<< (long value)
	{
		char buffer[11];
		ltoa(value, buffer, Base);
		Puts(buffer);
		return *this;
	}

	SelfType& operator<< (unsigned long value)
	{
		char buffer[11];
		ultoa(value, buffer, Base);
		Puts(buffer);
		return *this;
	}

	SelfType& operator<< (unsigned value)
	{
		char buffer[7];
		utoa(value, buffer, Base);
		Puts(buffer);
		return *this;
	}

	SelfType& operator<< (double value)
	{
		const uint8_t presision=2;
		char buffer[fieldSize+1];
		if(value < Pow<10, fieldSize - presision - 1>::value - 0.1)
		{
			dtostrf(value, fieldSize - presision - 1, presision, buffer);
		}
		else
		{
			dtostre(value, buffer, fieldSize - 6, 0);
		}
		Puts(buffer);
		return *this;
	}

	SelfType& operator<< (const char *value)
	{
		Puts(value);
		return *this;
	}

	void Write(const void *data, uint16_t size)
	{
		for(uint16_t i=0; i<size; ++i)
		{
			DATA_SOURCE::Putch(((uint8_t*)data)[i]);
		}
	}

	void Puts(const char *str)
	{
		while(*str)
		{
			DATA_SOURCE::Putch(*str);
			str++;
		}
	}

	void Read(void *data, uint16_t size)
	{
		for(uint16_t i=0; i<size; ++i)
		{
			DATA_SOURCE::Getch(((uint8_t*)data)[i]);
		}
	}

	void Gets(char *buffer, uint8_t len)
	{
		uint8_t c, pos=0;
		do
		{
			DATA_SOURCE::Getch(c);
			if(c=='\r')
			{
				DATA_SOURCE::Getch(c);
				break;
			}
			buffer[pos++]=c;
			if(pos>=len)break;
		}
		while(c!='\n' && c!=0);
		buffer[pos]=0;
	}

	SelfType& operator>> (unsigned &value)
	{
		char buffer[7];
		Gets(buffer, sizeof(buffer));
		value = atoi(buffer);
		return *this;
	}

	SelfType& operator>> (unsigned long &value)
	{
		char buffer[11];
		Gets(buffer, sizeof(buffer));
		value = atol(buffer);
		return *this;
	}

	template<class T>
	void Write(T value)
	{
		Write(&value, sizeof(T));
	}

	template<class T>
	T Read()
	{
		T value;
		Read(&value, sizeof(T));
		return value;
	}
};
