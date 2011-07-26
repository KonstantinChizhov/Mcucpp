#pragma once
#include <tiny_ostream.h>

namespace IO
{

#define IO_DECLARE_STREAM_MANIPULATOR(NAME, FLAG, MASK) \
template<class OutputPolicy, class CharT, class IOS> \
FormatWriter<OutputPolicy, CharT, IOS>& NAME ( FormatWriter<OutputPolicy, CharT, IOS>& os)\
{\
	os.setf(FLAG, MASK);\
	return os;\
}

#define IO_DECLARE_STREAM_UNSET_MANIPULATOR(NAME, FLAG) \
template<class OutputPolicy, class CharT, class IOS> \
FormatWriter<OutputPolicy, CharT, IOS>& NAME ( FormatWriter<OutputPolicy, CharT, IOS>& os)\
{\
	os.unsetf(FLAG);\
	return os;\
}

IO_DECLARE_STREAM_MANIPULATOR(showbase, IOS::showbase, IOS::showbase)
IO_DECLARE_STREAM_MANIPULATOR(boolalpha, IOS::boolalpha, IOS::boolalpha)
IO_DECLARE_STREAM_MANIPULATOR(showpos, IOS::showpos, IOS::showpos)
IO_DECLARE_STREAM_MANIPULATOR(oct, IOS::oct, IOS::basefield)
IO_DECLARE_STREAM_MANIPULATOR(dec, IOS::dec, IOS::basefield)
IO_DECLARE_STREAM_MANIPULATOR(hex, IOS::hex, IOS::basefield)
IO_DECLARE_STREAM_MANIPULATOR(uppercase, IOS::uppercase, IOS::uppercase)
IO_DECLARE_STREAM_MANIPULATOR(unitbuf, IOS::unitbuf, IOS::unitbuf)
IO_DECLARE_STREAM_MANIPULATOR(scientific, IOS::scientific, IOS::floatfield)
IO_DECLARE_STREAM_MANIPULATOR(fixed, IOS::fixed, IOS::floatfield)

IO_DECLARE_STREAM_MANIPULATOR(left, IOS::left, IOS::adjustfield)
IO_DECLARE_STREAM_MANIPULATOR(right, IOS::right, IOS::adjustfield)
IO_DECLARE_STREAM_MANIPULATOR(internal, IOS::internal, IOS::adjustfield)


IO_DECLARE_STREAM_UNSET_MANIPULATOR(noshowbase, IOS::showbase)
IO_DECLARE_STREAM_UNSET_MANIPULATOR(noboolalpha, IOS::boolalpha)
IO_DECLARE_STREAM_UNSET_MANIPULATOR(noshowpos, IOS::showpos)
IO_DECLARE_STREAM_UNSET_MANIPULATOR(nouppercase, IOS::uppercase)
IO_DECLARE_STREAM_UNSET_MANIPULATOR(nounitbuf, IOS::unitbuf)

    struct SetwT { int width; };

    inline SetwT setw(int width)
    {
        SetwT f = { width };
        return f;
    }

	template<class OutputPolicy, class CharT, class IOS>
    FormatWriter<OutputPolicy, CharT, IOS>&  operator<<
            ( FormatWriter<OutputPolicy, CharT, IOS>& os, SetwT f)
    {
        os.width(f.width);
        return os;
    }
}
