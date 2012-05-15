#pragma once

#include <stdint.h>
#include <graphics/font.h>
#include <graphics/bitmap.h>
#include <flashptr.h>

using namespace Mcucpp;
using namespace Graphics;

extern RastrFont<FLASH_PTR(uint8_t), FLASH_PTR(CharInfo) > smallFont;
extern RastrFont<FLASH_PTR(uint8_t), FLASH_PTR(CharInfo) > bigFont;

extern RowOrderedBitmap<FLASH_PTR(uint8_t)> bitmap;

