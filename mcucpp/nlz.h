
#pragma once

#include <stdint.h>
/////////////////////////////////////////////////////////////////////////////// 
/// Number of leading zeros
///////////////////////////////////////////////////////////////////////////////

static inline int nlz(uint16_t x)
{
	if (x == 0) 
		return 16;
	int n  = 0;
	if ((x & 0xff00) == 0)
	{
		n += 8; 
		x <<= 8;
	}
	if ((x & 0xf000) == 0)
	{
		n += 4; 
		x <<= 4;
	}
	if ((x & 0xC000) == 0)
	{
		n += 2; 
		x <<= 2;
	}
	if ((x & 0x8000) == 0)
		n += 1;
	return n;
}

static inline int nlz(uint32_t x)
{
	if (x == 0) 
		return 32;
	int n  = 0;
	if ((x & 0xffff0000) == 0) 
	{
		n += 16; 
		x <<= 16;
	}
	if ((x & 0xff000000) == 0)
	{
		n += 8; 
		x <<= 8;
	}
	if ((x & 0xf0000000) == 0)
	{
		n += 4; 
		x <<= 4;
	}
	if ((x & 0xC0000000) == 0)
	{
		n += 2; 
		x <<= 2;
	}
	if ((x & 0x80000000) == 0)
		n += 1;
	return n;
}

static inline int nlz(uint64_t x)
{
	if (x == 0) 
		return 64;
	int n  = 0;
	if ((x & 0xffffffff00000000ull) == 0) 
	{
		n += 32; 
		x <<= 32;
	}
	if ((x & 0xffff000000000000ull) == 0) 
	{
		n += 16; 
		x <<= 16;
	}
	if ((x & 0xff00000000000000ull) == 0)
	{
		n += 8; 
		x <<= 8;
	}
	if ((x & 0xf000000000000000ull) == 0)
	{
		n += 4; 
		x <<= 4;
	}
	if ((x & 0xC000000000000000ull) == 0)
	{
		n += 2; 
		x <<= 2;
	}
	if ((x & 0x8000000000000000ull) == 0)
		n += 1;
	return n;
}