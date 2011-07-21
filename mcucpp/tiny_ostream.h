
#pragma once
#include "enum.h"
#include "template_utils.h"
#include <string.h>
#include <stdlib.h>
#include <tiny_ios.h>

namespace IO
{
	template<class OutputPolicy,
            class CharT = char,
            class IOS = basic_ios<CharT>
            >
	class FormatWriter :public OutputPolicy, public IOS
	{
	    static const bool ScanFloatPrecision = false;
		const CharT *_formatSrting;
		typedef CharTrates<CharT> Trates;
		typedef FormatWriter Self;
	private:

		inline unsigned Base();
		inline void FieldFill(int lastOutputLength);
        inline void FieldFillPost(int lastOutputLength);
        inline void FieldFillPre(int lastOutputLength);
		template<class T>
		inline void PutInteger(T value);
		inline void PutBool(bool value);
		inline void ProcessFormat();
		void inline ClearFmt()
		{
            IOS::setf(IOS::right | IOS::dec, IOS::unitbuf | IOS::showpos |
											IOS::boolalpha | IOS::adjustfield |
											IOS::basefield | IOS::floatfield |
											IOS::skipws | IOS::showbase |
											IOS::showpoint | IOS::uppercase);
		}
	public:

		FormatWriter()
		{
			_formatSrting = 0;
		}

		Self& Format(const CharT * format)
		{
			_formatSrting = format;
			ProcessFormat();
			return *this;
		}

		template<class T>
		Self& operator% (T value)
		{
			if(_formatSrting)
			{
				*this << value;
				ClearFmt();
				ProcessFormat();
			}
			return *this;
		}

		Self& operator<< (bool value)
		{
			PutBool(value);
			return *this;
		}

		Self& operator<< (int value)
		{
			PutInteger(value);
			return *this;
		}

		Self& operator<< (long value)
		{
			PutInteger(value);
			return *this;
		}

		Self& operator<< (unsigned long value)
		{
			PutInteger(value);
			return *this;
		}

		Self& operator<< (unsigned value)
		{
			PutInteger(value);
			return *this;
		}

		Self& operator<< (const CharT* value)
		{
			puts(value);
			return *this;
		}

		Self&
		operator<<(Self& (*__pf)(Self&))
		{
			return __pf(*this);
		}

		Self&
		operator<<(IOS& (*__pf) (IOS&))
		{
			__pf(*this);
			return *this;
		}

		Self&
		operator%(Self& (*__pf)(Self&))
		{
			return __pf(*this);
		}

		Self&
		operator%(IOS& (*__pf) (IOS&))
		{
			__pf(*this);
			return *this;
		}

		void puts(const CharT *str)
		{
		    const int outputSize = Trates::SrtLen(str);
            FieldFillPre(outputSize);
			OutputPolicy::write(str, outputSize);
			FieldFillPost(outputSize);
		}

	};


	template<class OutputPolicy, class CharT, class IOS>
	FormatWriter<OutputPolicy, CharT, IOS>& endl ( FormatWriter<OutputPolicy, CharT, IOS>& os)
	{
		os.put('\n');
		return os;
	}

	template<class OutputPolicy, class CharT, class IOS>
	FormatWriter<OutputPolicy, CharT, IOS>& ends ( FormatWriter<OutputPolicy, CharT, IOS>& os)
	{
		os.put('\0');
		return os;
	}


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

    template<class OutputPolicy, class CharT, class IOS>
    FormatWriter<OutputPolicy, CharT, IOS>&  operator%
            ( FormatWriter<OutputPolicy, CharT, IOS>& os, SetwT f)
    {
        os.width(f.width);
        return os;
    }

}
#include <impl/tiny_ostream.h>
