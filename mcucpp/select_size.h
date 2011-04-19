#pragma once
#include <static_if.h>

	template<unsigned sizeBits>
	struct SelectSize
	{
		static const bool LessOrEq8 = sizeBits <= 8;
		static const bool LessOrEq16 = sizeBits <= 16;

		typedef typename StaticIf<
				LessOrEq8,
				uint8_t,
				typename StaticIf<LessOrEq16, uint16_t, uint32_t>::Result>
				::Result Result;
	};