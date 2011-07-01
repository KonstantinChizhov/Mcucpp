
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

		Self& operator% (bool value)
		{
			PutBool(value);
			ProcessFormat();
			return *this;
		}

		Self& operator% (int value)
		{
			PutInteger(value);
			ProcessFormat();
			return *this;
		}

		Self& operator% (long value)
		{
			PutInteger(value);
			ProcessFormat();
			return *this;
		}

		Self& operator% (unsigned long value)
		{
			PutInteger(value);
			ProcessFormat();
			return *this;
		}

		Self& operator% (unsigned value)
		{
			PutInteger(value);
			ProcessFormat();
			return *this;
		}

		Self& operator% (const CharT* value)
		{
			puts(value);
			ProcessFormat();
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

	template<class OutputPolicy, class CharT, class IOS>
	FormatWriter<OutputPolicy, CharT, IOS>& dec ( FormatWriter<OutputPolicy, CharT, IOS>& os)
	{
		os.setf(IOS::dec, IOS::basefield);
		return os;
	}

	template<class OutputPolicy, class CharT, class IOS>
	FormatWriter<OutputPolicy, CharT, IOS>& hex ( FormatWriter<OutputPolicy, CharT, IOS>& os)
	{
		os.setf(IOS::hex, IOS::basefield);
		return os;
	}

	template<class OutputPolicy, class CharT, class IOS>
	FormatWriter<OutputPolicy, CharT, IOS>& oct ( FormatWriter<OutputPolicy, CharT, IOS>& os)
	{
		os.setf(IOS::oct, IOS::basefield);
		return os;
	}

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
#include <impl/tiny_ostream.tcc>
