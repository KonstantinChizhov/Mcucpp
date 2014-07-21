//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2012
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

#ifndef __KS0108__
#define __KS0108__

#include <static_assert.h>
#include <delay.h>
#include <iopins.h>
#include <pinlist.h>
#include <template_utils.h>

namespace Mcucpp
{
	using namespace Clock;
	using namespace Util;

	class KS0108Base
	{
	protected:
		enum ControlBusBits
		{
			Cs1Bit = 1,
			Cs2Bit = 2,
			RwBit = 8,
			DiBit = 16
		};

		enum Commands
		{
			DisplayOnCmd = 0x3F,
			DisplayOffCmd = 0x3E,
			SetAddressCmd = 0x40,
			SetPageCmd = 0xB8,
			StartLineCmd = 0xC0
		};

		enum Operations
		{
			OpAnd = 1,
			OpXor = 2,
			OpCopy = 4,
			OpInvert = 8
		};
	public:
		enum OutputMode
		{
			WriteMode = OpCopy,
			XorMode = OpXor,
			AndMode = OpAnd,
			AndNotMode = OpAnd | OpInvert,
			InvertMode = OpInvert | OpCopy
		};

		typedef int_fast16_t Coord;
		typedef uint_fast8_t Color;
		static const Color DefaultColor = 1;
		static const Color DefaultBackground = 0;

		static Coord Width() {return 128;}
		static Coord Height() {return 64;}

	};


	template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
			class KS0108 :public KS0108Base
	{
		STATIC_ASSERT(DataBus::Length == 8);
		typedef IO::PinList<Cs1, Cs2, Reset, Rw, Di, E> ControlBus;
		typedef IO::PinList<Cs1, Cs2, IO::NullPin, Rw, Di> Controls;
		using KS0108Base::Commands;
		using KS0108Base::ControlBusBits;
		static const unsigned PulseDelay = 7; // microsec
	public:
		using KS0108Base::OutputMode;
		using KS0108Base::Coord;
		using KS0108Base::Color;
		static void Init();
		static void Fill(Color color);
		static void PutPixel(Coord x, Coord y, Color color);
		template<class BitmapT>
		static void DrawBitmap(const BitmapT &bitmap, Coord x, Coord y, Color foreground = DefaultColor, Color background = DefaultBackground);
		static void Flush();
		inline static void SetOutputMode(OutputMode mode)
		{
			_mode = mode;
		}
	protected:
		static void Write(uint8_t data);
		static void SetPage(uint_fast8_t page, uint_fast8_t x);
		static void WritePage(uint8_t page, uint_fast8_t mask, uint_fast8_t x);
		static void PulseE();
		static OutputMode _mode;
	};

	template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
			KS0108Base::OutputMode KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::_mode = KS0108Base::WriteMode;

	template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
			void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::PulseE()
	{
		E::Set();
		delay_us<PulseDelay, SysClock::CpuFreq>();
		E::Clear();
	}

	template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
			void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::PutPixel(Coord x, Coord y, Color color)
	{
		if((unsigned)x >= (unsigned)Width()) return;
		if((unsigned)y >= (unsigned)Height()) return;
		uint8_t _x = (uint8_t)x;
		
		if(_x > 63)
		{
			Cs2::Set();
			Cs1::Clear();
			_x -= 64;
		}
		else
		{
			Cs2::Clear();
			Cs1::Set();
		}
		Di::Clear();
		uint8_t page = y >> 3;
		Write(SetPageCmd | page);
		Write(SetAddressCmd | _x);

		Di::Set(); Rw::Set();
		DataBus::template SetConfiguration<0xff, DataBus::In>();
		PulseE();
		Color mask = 1 << (y & 0x07);
		E::Set();
		delay_us<PulseDelay, SysClock::CpuFreq>();
		Color data = DataBus::PinRead();
		E::Clear();
		Rw::Clear();
		DataBus::template SetConfiguration<0xff, DataBus::Out>();
		Di::Clear();

		if(_mode & OpInvert)
			color = !color;

		if(_mode & (OpAnd | OpCopy))
			data &= ~mask;

		if(_mode & OpXor)
			data ^= mask;
			
		if(color && (_mode & OpCopy))
			data |= mask;

		Write(SetAddressCmd | _x);
		Di::Set();
		Write(data);
		Di::Clear();
	}


	template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
			void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::Init()
	{
		ControlBus::template SetConfiguration<0xff, ControlBus::Out>();
		DataBus::template SetConfiguration<0xff, DataBus::Out>();
		Reset::Clear();
		delay_ms<10, SysClock::CpuFreq>();
		Reset::Set();
		delay_ms<10, SysClock::CpuFreq>();
		Controls::template Write<Cs1Bit | Cs2Bit>();
		Write(DisplayOnCmd);
		Write(SetAddressCmd);
		Write(SetPageCmd);
		Write(StartLineCmd);
		Fill(0);
		Flush();
	}

	template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
			void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::Fill(Color color)
	{
		if(color) color = 0xff;
		if(_mode & OpInvert)
			color = ~color;
		for(uint8_t i = 0; i < 8; i++)
		{
			Controls::template Write<Cs1Bit | Cs2Bit>();
			Write(SetPageCmd + i);
			Write(SetAddressCmd + 0);
			Controls::template Write<Cs1Bit | Cs2Bit | DiBit>();
			DataBus::Write(color);
			for(uint8_t j = 0; j < 64; j++)
			{
				PulseE();
			}
		}
	}

	template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
			void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::Write(uint8_t cmd)
	{
		DataBus::Write(cmd);
		PulseE();
	}

	template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
			void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::SetPage(uint_fast8_t page, uint_fast8_t x)
	{
		Controls::template Write<Cs1Bit | Cs2Bit>();
		Write(SetPageCmd | page);
		uint_fast8_t cs2addr = 0;
		if(x > 63)
			cs2addr = x - 64;
		else
		{
			Cs2::Clear();
			Write(SetAddressCmd | x);
		}
		Cs1::Clear();
		Cs2::Set();
		Write(SetAddressCmd | cs2addr);
	}

	template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
			void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::WritePage(uint8_t data, uint_fast8_t mask, uint_fast8_t cx)
	{
		if(cx  > 63)
		{
			Cs2::Set();
			Cs1::Clear();
			cx -= 64;
		}
		else
		{
			Cs2::Clear();
			Cs1::Set();
		}

		Di::Set();
		if(mask || (_mode & (OpXor | OpAnd)) )
		{
			DataBus::template SetConfiguration<0xff, DataBus::In>();
			Rw::Set();
			PulseE();

			if(_mode & OpInvert)
				data ^= ~mask;

			E::Set();
			delay_us<PulseDelay, SysClock::CpuFreq>();
			uint8_t dest = DataBus::PinRead();
			if(_mode & OpCopy)
				data = (dest & mask) | data;
			else
			{
				if(_mode & OpXor)
					data ^= dest;
			
				if(_mode & OpAnd)
					data = (data | mask) & dest;
			}

			E::Clear();
			DataBus::template SetConfiguration<0xff, DataBus::Out>();

			Rw::Clear();
			Di::Clear();
			Write(SetAddressCmd + cx);
			Di::Set();
		}
		else if(_mode & OpInvert)
			data ^= 0xff;

		Write(data);
	}

	inline uint8_t PageMask(uint8_t pageStart, uint8_t pageEnd)
	{
		uint8_t mask = (1 << (pageStart & 7)) - 1;
		mask |= 0xFE << ((pageEnd - 1) & 7);
		return mask;
	}


	template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
			template<class BitmapT>
			void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::
				DrawBitmap(const BitmapT &bitmap, Coord x, Coord y, Color foreground, Color /*background*/)
	{
		if(y >= Height() || x >= Width())
			return;
		if(x < -(int)bitmap.Width() || y < -(int)bitmap.Height())
			return;

		OutputMode oldMode = _mode;
		if(!foreground)
			_mode = (OutputMode)(_mode ^ OpInvert);

		uint_fast8_t pageStart = (uint_fast8_t)max<Coord>(0, y);
		uint_fast8_t pageEnd = ((pageStart + 8) & 0xf8);
		uint_fast8_t maxY = (uint_fast8_t)min<Coord>(y + bitmap.Height(), Height());
		uint_fast8_t maxX = (uint_fast8_t)min<Coord>(bitmap.Width() + x, Width());
		uint_fast8_t minX = (uint_fast8_t)max<Coord>(0, x);
		if(pageEnd > maxY)
				pageEnd = maxY;

		while(pageEnd <= maxY && pageStart < maxY)
		{
			SetPage(pageStart >> 3 , minX);
			uint_fast8_t pagePixels = pageEnd - pageStart;
			uint_fast8_t mask = PageMask(pageStart, pageEnd);
			uint_fast8_t dataShift = 7 - ((pageEnd - 1) & 7);
			
			for(uint_fast8_t j = minX; j < maxX; j++)
			{
				uint8_t data = 0;
				Coord dx = j - x;
				Coord dy = pageStart - y;
				for(uint_fast8_t i = 0; i < pagePixels; i++)
				{
					data >>= 1;
					if(bitmap(dx , i + dy))
						data |= 0x80;
				}
				data >>= dataShift;
				WritePage(data, mask, j);
			}
			pageStart = pageEnd;
			pageEnd += 8;
			if(pageEnd > maxY)
				pageEnd = maxY;
		}
		_mode = oldMode;
	}

	template<class Cs1, class Cs2, class Reset, class Rw, class Di, class E,  class DataBus>
			void KS0108<Cs1, Cs2, Reset, Rw, Di, E, DataBus>::Flush()
	{

	}
} // namespace Mcucpp
#endif
