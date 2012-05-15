
#include "./resources.h"

#include "./font5x8.h"
#include "./font11x16.h"
#include "./bitmapdata.h"

FontInfo smallFontAscii = 
{
	' ', '~', font5x8Ascii, 0
};
FontInfo smallFontRus = 
{
	192, 255, font5x8Rus, 0
};

FontInfo smallFontData[] = {smallFontAscii, smallFontRus};

FontInfo bigFontAscii = 
{
	' ', '~', font11x16, font11x16Descr
};

FontInfo bigFontData[]={bigFontAscii};


RastrFont<FLASH_PTR(uint8_t), FLASH_PTR(CharInfo) > smallFont(smallFontData, 2, 5, 8, '?');

RastrFont<FLASH_PTR(uint8_t), FLASH_PTR(CharInfo) > bigFont(bigFontData, 1, 11, 16, '?');

RowOrderedBitmap<FLASH_PTR(uint8_t)> bitmap(bitmapData, 64, 64);

