#pragma once
#include "stdafx.h"

#include <pinlist.h>
#include <iopins.h>
#include <stdexcept>
#include <drivers/KS0108.h>

using namespace Mcucpp;
using namespace Mcucpp::IO;
using namespace Mcucpp::IO::Test;

class Glcd :public KS0108Base
{
	static const int width = 128;
	static const int height = 64;
public:

	Glcd(void)
	{
		LcdDataPort::callback = this;
		LcdControlPort::callback = this;
	}

	~Glcd(void)
	{
	}

	void Draw(HDC dc)
	{
		RECT rect;
		GetClipBox(dc, &rect);
		
		BITMAPINFO info;
		memset(&info, 0, sizeof(info));
		info.bmiHeader.biSize        = sizeof(BITMAPINFOHEADER);
		info.bmiHeader.biBitCount	 = 32;
		info.bmiHeader.biWidth       = width;
		info.bmiHeader.biHeight      = -height; 
		info.bmiHeader.biPlanes      = 1;
		info.bmiHeader.biCompression = BI_RGB;
		info.bmiHeader.biSizeImage   = width*height*sizeof(_buffer[0]);

		StretchDIBits(dc, 
					rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
					 0, 0, width, height,
					_buffer, &info, DIB_RGB_COLORS, SRCCOPY);
	}

	template<class Port>
    inline void PortChanged();

	typedef TestPort<uint8_t, 222, Glcd> LcdDataPort;
	typedef TestPort<uint8_t, 333, Glcd> LcdControlPort;

	typedef InvertedPin<LcdControlPort, 0> Cs1;
	typedef InvertedPin<LcdControlPort, 1> Cs2;
	typedef TPin<LcdControlPort, 2> Rst;
	typedef TPin<LcdControlPort, 3> Rw;
	typedef TPin<LcdControlPort, 4> Di;
	typedef TPin<LcdControlPort, 5> E;

private:
	void SetPixel(unsigned x, unsigned y, unsigned color)
	{
		_buffer[y*width + x] = color ? 0 : 0x00ffffff;
	}

	unsigned GetPixel(unsigned x, unsigned y)
	{
		return _buffer[y*width + x] == 0;
	}

	struct LcdCtrl
    {
        uint8_t page;
        uint8_t addr;
    } c1, c2;
	unsigned _buffer[width*height];
};

template<>
inline void Glcd::PortChanged<Glcd::LcdDataPort>()
{
}

template<>
inline void Glcd::PortChanged<Glcd::LcdControlPort>()
{
    if(!(LcdControlPort::OutReg & (1 << Rst::Number)))
        return;
    // E passes from 1 to 0
    if(LcdControlPort::OutReg & (1 << E::Number))
        return;

    if(!(LcdControlPort::PrevOutReg & (1 << E::Number)))
        return;

    if(LcdControlPort::OutReg & (1 << Rw::Number))
    {
        if(LcdDataPort::DirReg != 0)
            throw std::runtime_error("Reading port set to output");
		if(LcdControlPort::OutReg & (1 << Di::Number) == 0)// status read
		{
			LcdDataPort::InReg = 0;
			return;
		}
		
        if((LcdControlPort::OutReg & (1 << Cs1::Number)) == 0)
        {
            uint8_t data = 0;
            for(int i=0; i < 8; i++)
            {
                if( GetPixel(c1.addr, i + c1.page*8))
                    data |= (1 << i);
            }
            LcdDataPort::InReg = data;
            c1.addr++;
            c1.addr &= 63;
        }
        if((LcdControlPort::OutReg & (1 << Cs2::Number)) == 0)
        {
            uint8_t data = 0;
            for(int i=0; i < 8; i++)
            {
                if( GetPixel(c2.addr + 64, i + c2.page*8))
                    data |= (1 << i);
            }
            LcdDataPort::InReg = data;
            c2.addr++;
            c2.addr &= 63;
        }
    }
    else
    {
        if(LcdDataPort::DirReg != 0xff)
            throw std::runtime_error("Writing port set to input");
        if(LcdControlPort::OutReg & (1 << Di::Number))// data/instruction
        {
            if((LcdControlPort::OutReg & (1 << Cs1::Number)) == 0)
            {
                for(int i=0; i < 8; i++)
                {
                    SetPixel(c1.addr, i + c1.page*8, (LcdDataPort::OutReg & (1 << i)));
                }
                c1.addr++;
                c1.addr &= 63;
            }
            if((LcdControlPort::OutReg & (1 << Cs2::Number)) == 0)
            {
                for(int i=0; i < 8; i++)
                {
                    SetPixel(c2.addr + 64, i + c2.page*8, (LcdDataPort::OutReg & (1 << i)));
                }
                c2.addr++;
                c2.addr &= 63;
            }
        }
        else
        {
            if((LcdDataPort::OutReg & 0xf8) ==  SetPageCmd)
            {
                if((LcdControlPort::OutReg & (1 << Cs1::Number)) == 0)
                    c1.page = LcdDataPort::OutReg & 7;
                if((LcdControlPort::OutReg & (1 << Cs2::Number)) == 0)
                    c2.page = LcdDataPort::OutReg & 7;
            }
            else
                if((LcdDataPort::OutReg & 0xc0) ==  SetAddressCmd)
                {
                if((LcdControlPort::OutReg & (1 << Cs1::Number)) == 0)
                    c1.addr = LcdDataPort::OutReg & 63;
                if((LcdControlPort::OutReg & (1 << Cs2::Number)) == 0)
                    c2.addr = LcdDataPort::OutReg & 63;
            }
        }
    }
}