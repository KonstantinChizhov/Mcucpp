
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


#ifndef SEVEN_SEGMENTS_H
#define SEVEN_SEGMENTS_H

#include <static_assert.h>
#include <flashptr.h>
#include <template_utils.h>
#include <static_if.h>
#include <stddef.h>

namespace Mcucpp
{
	class CharMap7Segment
	{
	public:
		static uint8_t Map(uint8_t bcd)
		{
			static const FLASH_STORAGE uint8_t table[] = "\x3f\x06\x5b\x4f\x66\x6d\x7d\x7\x7f\x6f\x77\x7c\x39\x5e\x79\x71";
			FLASH_PTR(uint8_t) tablePtr(table);
			return *(tablePtr + size_t(bcd));
		}

		static uint8_t Minus(){return 0x40;}
		static uint8_t Empty(){return 0x0;}
		static uint8_t Exp(){return 0x23;}
	};

	class Base7Segment
	{
		Base7Segment();
	public:
		enum FeaturesT
		{
			Seg7None = 0,
			Seg7BigNum = 1,
			Seg7Align = 2,
			Seg7All = 3
		};

		enum FlagsT
		{
			DecFlag = 0,
			HexFlag = 1,
			LeftFlag = 2,
			RightFlag = 0
		};
	};


	template<	class Segments,
				class Commons,
				Base7Segment::FeaturesT Features = Base7Segment::Seg7All,
				class CharMap = CharMap7Segment>
	class Display7Segment: public Base7Segment
	{
		BOOST_STATIC_ASSERT(Segments::Length == 7 || Segments::Length == 8);
		BOOST_STATIC_ASSERT(Commons::Length >= 2);

		typedef typename StaticIf<Commons::Length <= 2, uint8_t,
					typename StaticIf<Commons::Length <= 4, uint16_t,
						typename StaticIf<Commons::Length <= 8, uint32_t, uint64_t>::Result
					>::Result
				>::Result MaxDisplayNumberT;
	public:
		enum{ Digits = Commons::Length};
		enum{ MaxDisplayNumber = Util::Pow<10, Digits>::value-1};
		enum{ MaxDisplayNumberWithSign = Util::Pow<10, Digits-1>::value-1};
		enum{ MaxDisplayNumberHex = Util::Pow<16, Digits>::value-1};

		static void Update();
		static void LeftAlign(){_data.flags |= LeftFlag;}
		static void RightAlign(){_data.flags &= ~LeftFlag;}
		static void Hex(){_data.flags |= HexFlag;}
		static void Dec(){_data.flags &= ~DecFlag;}

		template<class T>
		static void Write(T value)
		{
			bool minusSign = false;
			if(Util::IsSigned<T>::value && !(_data.flags & HexFlag) )
			{
				if(value < 0)
				{
					value = -value;
					minusSign = true;
				}
			}

			typedef typename Util::Unsigned<T>::Result UT;
			UT uvalue = static_cast<UT>(value);
			uint8_t exp = 0;
			uint8_t radix = _data.flags & HexFlag ? 16 : 10;
			uint8_t *ptr = &_data.value[0] + Digits;

			MaxDisplayNumberT maxNum = MaxDisplayNumber;
			if(minusSign)
				maxNum = MaxDisplayNumberWithSign;
			if(_data.flags & HexFlag)
				maxNum = MaxDisplayNumberHex;
			do
			{
				T q = uvalue / radix;
				if(uvalue >= maxNum)
				{
					exp ++;
				}
				else
				{
				    uint8_t rem = uint8_t(uvalue - q*radix);
					*--ptr = CharMap::Map(rem);
				}
				uvalue = q;
			}
			while (uvalue != 0);

			if(minusSign)
				*--ptr = CharMap::Minus();

			if((Features & Base7Segment::Seg7BigNum) &&
                Digits > 2 &&
                exp)
			{
				ptr = &_data.value[0] + Digits;
				if(exp >= radix - 1)
					exp++;
				exp++;
				do
				{
					uint8_t eq = exp / radix;
					uint8_t rem = uint8_t(exp - eq*radix);
					*--ptr = CharMap::Map(rem);
					exp = eq;
				}
				while (exp != 0);
				*--ptr = CharMap::Exp();
			}
			else
			{
				uint8_t shift = uint8_t(ptr - &_data.value[0]);
				uint8_t outputSize = Digits - shift;
				if((Features & Base7Segment::Seg7Align) && _data.flags & LeftFlag)
				{
					for(uint8_t i=0; i < outputSize; i++)
					{
						_data.value[i] = _data.value[i + shift];
					}
					ptr = &_data.value[0] + Digits;
				}
				for(uint8_t i=0; i < shift; i++)
					*--ptr = CharMap::Empty();
			}
		}

	private:
		struct PrivateData
		{
			uint8_t flags;
			uint8_t position;
			uint8_t value[Digits];
		};

		static PrivateData _data;
	};

	template<class Segments, class Commons, Base7Segment::FeaturesT Features, class CharMap>
	typename Display7Segment<Segments, Commons, Features, CharMap>::PrivateData
		Display7Segment<Segments, Commons, Features, CharMap>::_data =
	{
		Display7Segment<Segments, Commons, Features, CharMap>::DecFlag,
		0,
		{0}
	};

	template<class Segments, class Commons, Base7Segment::FeaturesT Features, class CharMap>
	void Display7Segment<Segments, Commons, Features, CharMap>::Update()
	{
		Commons::template SetConfiguration<Commons::Out>();
		Segments::template SetConfiguration<Segments::Out>();
		Commons::Write(typename Commons::DataType(1) << _data.position);
		Segments::Write(_data.value[_data.position++]);
		if(_data.position >= Digits)
			_data.position = 0;
	}
}

#endif
