//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2020
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

#include <static_assert.h>
#include <iopins.h>
#include <pinlist.h>
#include <template_utils.h>

namespace Mcucpp
{
using namespace Clock;
using namespace Util;

class Ssd1306Base
{
protected:
	static constexpr uint8_t MultipleData = 0b01000000;
	static constexpr uint8_t SingleData = 0b11000000;
	static constexpr uint8_t Command = 0b10000000;
	static constexpr uint8_t I2cAddr = 60;
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

	static Coord Width() { return 128; }
	static Coord Height() { return 64; }
};

template <class I2c>
class Ssd1306 : public Ssd1306Base
{

public:
	using Ssd1306Base::Color;
	using Ssd1306Base::Coord;
	using Ssd1306Base::OutputMode;

	void Init();
	void Fill(Color color);
	void PutPixel(Coord x, Coord y, Color color);
	template <class BitmapT>
	void DrawBitmap(const BitmapT &bitmap, Coord x, Coord y, Color foreground = DefaultColor, Color background = DefaultBackground);
	void Flush();
	inline void SetOutputMode(OutputMode mode)
	{
		_mode = mode;
	}
	bool IsConnected() { return _connected; }

protected:
	void InitInternal();
	void WritePage(uint8_t data, uint_fast8_t mask, uint_fast8_t page, uint_fast8_t cx);
	uint8_t _buffer[128 * 64 / 8];
	OutputMode _mode = Ssd1306Base::WriteMode;
	bool _connected = false;
};

template <class I2c>
void Ssd1306<I2c>::PutPixel(Coord x, Coord y, Color color)
{
	if ((unsigned)x >= (unsigned)Width())
		return;
	if ((unsigned)y >= (unsigned)Height())
		return;

	uint8_t page = y >> 3;

	Color mask = 1 << (y & 0x07);
	int index = page * 64 + x;
	Color data = _buffer[index];

	if (_mode & OpInvert)
		color = !color;

	if (_mode & (OpAnd | OpCopy))
		data &= ~mask;

	if (_mode & OpXor)
		data ^= mask;

	if (color && (_mode & OpCopy))
		data |= mask;

	_buffer[index] = data;
}
template <class I2c>
void Ssd1306<I2c>::InitInternal()
{
constexpr uint8_t init[] =
		{
			0xA8,
			0x3F,
			0xD3, // Set Display Offset
			0x00, // Set Lower Column Start Address for Page Addressing Mode
			0x40, // Set Display Start Line
			0xA1, // Set Segment Re-map
			0xC8, // Set COM Output Scan Direction
			0xDA, // Set COM Pins Hardware Configuration
			0x12, // Set Higher Column Start Address for Page Addressing Mode
			0x81,
			0x7F,
			0xA4, // Entire Display on
			0xA6, // normal/ivverse mode A6/A7
			0xD5,
			0x80,
			0x8D,
			0x14,
			0xAF, // Display on
		};

	for (auto cc : init)
	{
		I2c1::WriteU8(I2cAddr, Command, cc);
	}
	_connected = I2c::GetError() == I2cError::NoError;
}

template <class I2c>
void Ssd1306<I2c>::Init()
{
	InitInternal();
	if(!_connected)
	{
		return;
	}
	Fill(0);
	Flush();
}

template <class I2c>
void Ssd1306<I2c>::Fill(Color color)
{
	if (color)
		color = 0xff;
	if (_mode & OpInvert)
		color = ~color;
	for (size_t i = 0; i < sizeof(_buffer); i++)
	{
		_buffer[i] = color;
	}
}

template <class I2c>
void Ssd1306<I2c>::WritePage(uint8_t data, uint_fast8_t mask, uint_fast8_t page, uint_fast8_t cx)
{
	if (mask || (_mode & (OpXor | OpAnd)))
	{
		if (_mode & OpInvert)
			data ^= ~mask;

		uint8_t dest = _buffer[page * 128 + cx];
		if (_mode & OpCopy)
			data = (dest & mask) | data;
		else
		{
			if (_mode & OpXor)
				data ^= dest;

			if (_mode & OpAnd)
				data = (data | mask) & dest;
		}
	}
	else if (_mode & OpInvert)
		data ^= 0xff;

	_buffer[page * 128 + cx] = data;
}

inline uint8_t PageMask(uint8_t pageStart, uint8_t pageEnd)
{
	uint8_t mask = (1 << (pageStart & 7)) - 1;
	mask |= 0xFE << ((pageEnd - 1) & 7);
	return mask;
}

template <class I2c>
template <class BitmapT>
void Ssd1306<I2c>::
	DrawBitmap(const BitmapT &bitmap, Coord x, Coord y, Color foreground, Color /*background*/)
{
	if (y >= Height() || x >= Width())
		return;
	if (x < -(int)bitmap.Width() || y < -(int)bitmap.Height())
		return;

	OutputMode oldMode = _mode;
	if (!foreground)
		_mode = (OutputMode)(_mode ^ OpInvert);

	uint_fast8_t pageStart = (uint_fast8_t)max<Coord>(0, y);
	uint_fast8_t pageEnd = ((pageStart + 8) & 0xf8);
	uint_fast8_t maxY = (uint_fast8_t)min<Coord>(y + bitmap.Height(), Height());
	uint_fast8_t maxX = (uint_fast8_t)min<Coord>(bitmap.Width() + x, Width());
	uint_fast8_t minX = (uint_fast8_t)max<Coord>(0, x);
	if (pageEnd > maxY)
		pageEnd = maxY;

	while (pageEnd <= maxY && pageStart < maxY)
	{
		uint_fast8_t page = pageStart >> 3;
		uint_fast8_t pagePixels = pageEnd - pageStart;
		uint_fast8_t mask = PageMask(pageStart, pageEnd);
		uint_fast8_t dataShift = 7 - ((pageEnd - 1) & 7);

		for (uint_fast8_t j = minX; j < maxX; j++)
		{
			uint8_t data = 0;
			Coord dx = j - x;
			Coord dy = pageStart - y;
			for (uint_fast8_t i = 0; i < pagePixels; i++)
			{
				data >>= 1;
				if (bitmap(dx, i + dy))
					data |= 0x80;
			}
			data >>= dataShift;
			WritePage(data, mask, page, j);
		}
		pageStart = pageEnd;
		pageEnd += 8;
		if (pageEnd > maxY)
			pageEnd = maxY;
	}
	_mode = oldMode;
}

template <class I2c>
void Ssd1306<I2c>::Flush()
{
	constexpr uint8_t comm[] = {
		0x20, 0,
		0, 0x22,
		0,
		0x07,
		0x21,
		0, 127};

	if(!_connected)
	{
		InitInternal();
	}

	I2cError err = I2c::Write(I2cAddr, 0, comm, sizeof(comm));
	if(err != I2cError::NoError)
	{
		_connected = false;
		return;
	}

	for (int page = 0; page < 8; page++)
	{
		err = I2c::Write(I2cAddr, MultipleData, &_buffer[page * 128], 128);
		if(err != I2cError::NoError)
		{
			break;
		}
	}
}
} // namespace Mcucpp
