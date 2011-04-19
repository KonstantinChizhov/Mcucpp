
#ifndef UTIL_HPP
#define UTIL_HPP
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

union Int32
{
	uint32_t Dword;
	uint16_t Words[2];
	uint8_t Bytes[4];
};

union Int16
{
	uint16_t Word;
	uint8_t Bytes[2];
};

//integer part of decimal logarithm
template <unsigned long Arg>
struct Log10
{
        enum { value = Log10<Arg/10>::value + 1};
};

template <>
struct Log10<1>
{
        enum { value = 0 };
};

template <>
struct Log10<0>
{
        enum { value = -1 };
};


template<unsigned num, unsigned pow> 
struct Pow 
{
	enum { value = num * Pow<num, pow-1>::value };
};

template<unsigned num> 
struct Pow<num, 0> 
{
	enum { value = 1 };
};

// returns true if T is signed integral type
template<class T>
bool TestSigned()
{
	return T(-1) < 0;
}

template<class T>
T MaxValue()
{
	if(TestSigned<T>())
		return Pow<2, sizeof(T)*8-1>::value - 1;
	else
		return Pow<2, sizeof(T)*8>::value - 1;
}

//utility functions
template<class T>
T min(T v1, T v2)
{
	return v1 > v2 ? v2 : v1;
}

template<class T>
T max(T v1, T v2)
{
	return v1 < v2 ? v2 : v1;
}

//rounding to power of 2
inline uint16_t flp2 (uint16_t x)
{
	x |= (x >> 1);
	x |= (x >> 2);
	x |= (x >> 4);
	x |= (x >> 8);
//	x |= (x >> 16);
	return x - (x >> 1);
}

template<class T>
inline uint8_t ilog2 (T x)
{
    int8_t res=-1;
    do
    {
        res++;
    }while(x >>= 1);
    return res;
}

template<class T>
inline uint8_t ilog10 (T x)
{
    T p = 1;
    int8_t i;
    for (i = -1; i <= 2.408*sizeof(x); i++)
    {
        if (x < p)
            return i;
        p *= 10;
    }
    return i;
}

inline uint16_t div5(uint16_t num, uint16_t &rem)
{
    uint16_t q = num*0x6667ul >> 16;
    q >>= 1;
    rem = num - q*5;
    return q;
}

inline uint16_t div10(uint16_t num, uint16_t &rem)
{
    uint16_t q = num*0x6667ul >> 16;
    q >>= 2;
    rem = num - q*10;
    return q;
}

inline uint32_t div10(uint32_t num, uint32_t &rem)
{
    uint32_t q = num*0x66666667ull >> 32;
    q >>= 2;
    rem = num - q*10;
    return q;
}

inline uint8_t CountOfOnes(uint8_t val)
{
	val = (val & 0x55) + ((val >> 1) & 0x55);
	val = (val & 0x33) + ((val >> 2) & 0x33);
	val = (val & 0x0F) + ((val >> 4) & 0x0F);
	return val;
}

template<class DATA_SOURCE>
class WaitAdapter :public DATA_SOURCE
{
public:
	static void Write(uint8_t c)
	{
		while(!DATA_SOURCE::Putch(c));
	}

	static uint8_t Read()
	{
		//uint16_t timeout = 1000;
		uint8_t c=0;
		while(!DATA_SOURCE::Getch(c));
		return c;
	}
};

#endif
 
