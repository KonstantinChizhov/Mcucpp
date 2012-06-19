#pragma once
#include "static_if.h"
#include <stdint.h>
namespace Mcucpp
{
	template<unsigned sizeBits>
	struct SelectSize
	{
		static const bool LessOrEq8 = sizeBits <= 8;
		static const bool LessOrEq16 = sizeBits <= 16;
		static const bool LessOrEq32 = sizeBits <= 32;

		typedef typename StaticIf<
				LessOrEq8,
				uint_fast8_t,
				typename StaticIf<LessOrEq16, uint_fast16_t, 
				typename StaticIf<LessOrEq32, uint_fast32_t, uint_fast64_t>::Result
				>::Result>
				::Result Result;
	};


	template<unsigned size>
	struct SelectSizeForLength
	{
		static const bool LessOrEq8 = size <= 0xff;
		static const bool LessOrEq16 = size <= 0xffff;

		typedef typename StaticIf<
				LessOrEq8,
				uint_fast8_t,
				typename StaticIf<LessOrEq16, uint_fast16_t, uint_fast32_t>::Result>
				::Result Result;
	};
}