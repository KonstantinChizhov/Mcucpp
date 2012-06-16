#pragma once
#include <tiny_ostream.h>

namespace Mcucpp
{

	struct SetwT { int width; };

	inline SetwT setw(int width)
	{
		SetwT f = { width };
		return f;
	}

	template<class OutputPolicy, class CharT, class IOS>
	basic_ostream<OutputPolicy, CharT, IOS>&  operator<<
			( basic_ostream<OutputPolicy, CharT, IOS>& os, SetwT f)
	{
		os.width(f.width);
		return os;
	}
}
