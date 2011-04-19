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


#ifndef HD44780_HPP
#define HD44780_HPP

#include "static_assert.h"

class LcdBase
{
	protected:
	LcdBase(){}
	static void Delay()
	{
		_delay_us(200); 
	}
};



template<class BUS, uint8_t LINE_WIDTH=8, uint8_t LINES=2>
class Lcd: public LcdBase
{
	enum{RsBit = 0, RwBit = 1, EBit = 2, BusBits = 3};
	typedef typename BUS::template Pin<RsBit> RS;
	typedef typename BUS::template Pin<RwBit> RW;
	typedef typename BUS::template Pin<EBit> E;
	typedef typename BUS::template Slice<BusBits, 4> DATA_BUS;

public:
	static uint8_t LineWidth()
	{
		return LINE_WIDTH;
	}

	static uint8_t Lines()
	{
		return LINES;
	}

	static void Init()
	{
		BUS::template SetConfiguration<BUS::Out, 0xff>();
		IO::PinList<RS, RW>::template Clear<0x03>();
		DATA_BUS::template Write< 0x03<<BusBits >(); 
		Strobe();
		Strobe();
		Strobe();
		_delay_ms(60);
		DATA_BUS::template Write<0x02<<BusBits>(); // set 4 bit mode 
		Strobe();
		Write(0x28); // 4 bit mode, 1/16 duty, 5x8 font 
	
		Write(0x08); // display off 
		Write(0x0E); // display on, blink curson on 
		Write(0x06); // entry mode 
	}

	Lcd()
	{
		Init();
	}

	static void Clear(void)
	{
		RS::Clear();
		Write(0x01); 
		Write(0x02);
	}

	static void Goto(uint8_t pos)
	{
		RS::Clear();
		Write(0x80+pos);
	}

	static void Goto(uint8_t x, uint8_t y)
	{
		RS::Clear();
		if(y == 1)
			x += 0x40;
		Write(0x80+x);
	}

	static void Home(void)
	{
		RS::Clear();
		Write(0x02);
	}

	static void Puts(const char *s, uint8_t len)
	{
		RS::Set(); // write characters 
		while(len-- && *s){
			Write(*s++);
		} 
	}

	static void Putch(char c)
	{
		RS::Set();
		Write(c);
	}

	static bool Busy()
	{
		RS::Clear();
		return Read() & 0x80;
	}
	
protected:
	static void Strobe()//__attribute__ ((noinline))
	{
		E::Set();
		Delay();
		E::Clear();
		Delay();
	}

	static void Write(uint8_t c)//__attribute__ ((noinline))
	{
		RW::Clear();
		DATA_BUS::SetConfiguration(DATA_BUS::Out);
		DATA_BUS::Write(c>>(4-BusBits));
		Strobe();
		DATA_BUS::Write(c<<BusBits); 
		Strobe();
	}

	static uint8_t Read() //__attribute__ ((noinline))
	{
		DATA_BUS::SetConfiguration(DATA_BUS::In);
		RW::Set();
		E::Set();
		//Delay();
		uint8_t res = DATA_BUS::Read() << 4;
		E::Clear();
		Delay();
		E::Set();
		res |= DATA_BUS::Read();
		E::Clear();
		RW::Clear();
		return res;
	}
};


#endif
