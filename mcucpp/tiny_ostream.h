
#pragma once
#include "enum.h"
#include "template_utils.h"
#include <string.h>
#include <stdlib.h>
#include <tiny_ios.h>

namespace IO
{

	template<class T, class CharT>
	CharT * IntToString(T value, CharT *bufferEnd, unsigned radix)
	{
		CharT *ptr = bufferEnd;
		do
		{
            *--ptr = CharTrates<CharT>::DigitToLit(value % radix);
            value /= radix;
		}
		while (value != 0);
		return ptr;
	}

	template<class OutputPolicy,
            class CharT = char,
            class IOS = basic_ios<CharT>
            >
	class FormatWriter :public OutputPolicy, public IOS
	{
	private:
		const CharT *_formatSrting;
		typedef CharTrates<CharT> Trates;
	public:
		typedef FormatWriter Self;
	private:

		template<class T>
		struct ConvertBufferSize
		{
			static const int value = sizeof(T) * 3;
		};

		unsigned Base()
		{
			if(IOS::flags() & IOS::hex) return 16;
			if(IOS::flags() & IOS::oct) return 8;
			return 10;
		}

        void FieldFillPost(int lastOutputLength)
        {
            if(IOS::flags() & IOS::right)
                return;
            int fillcount = IOS::width(0) - lastOutputLength;
            for(int i=0; i<fillcount; i++)
                OutputPolicy::put(IOS::fill());
        }

        void FieldFillPre(int lastOutputLength)
        {
            if(IOS::flags() & IOS::left)
                return;
            int fillcount = IOS::width(0) - lastOutputLength;
            for(int i=0; i<fillcount; i++)
                OutputPolicy::put(IOS::fill());
        }

		template<class T>
		void PutInteger(T value)
		{
			const int bufferSize = ConvertBufferSize<int>::value;
			CharT buffer[bufferSize];
			bool sign = false;

			if(Util::IsSigned<T>::value)
			{
				if(value < 0 && IOS::flags() & IOS::dec)
				{
					value = -value;
					sign = true;
				}
			}

			typedef typename Util::Unsigned<T>::Result UT;
			UT uvalue = static_cast<UT>(value);
			CharT * str = IntToString(uvalue, buffer + bufferSize, Base());

			if(sign)
				*--str = Trates::Minus();

			if((IOS::flags() & (IOS::hex | IOS::showbase)) == (IOS::hex | IOS::showbase))
			{
				*--str = 'x';
				*--str = '0';
			}
            int outputSize = buffer + bufferSize - str;
            FieldFillPre(outputSize);
			OutputPolicy::write(str, buffer + bufferSize - str);
			FieldFillPost(outputSize);
		}

		void PutBool(bool value)
		{
			if(IOS::flags() & IOS::boolalpha)
			{
				if(value)
				{
					puts(Trates::True());
				}
				else
				{
					puts(Trates::False());
				}
			}
			else
			{
			    FieldFillPre(1);
				if(value)
					OutputPolicy::put(Trates::DigitToLit(1));
				else
					OutputPolicy::put(Trates::DigitToLit(0));
                FieldFillPost(1);
			}
		}

		void ProcessFormat()
		{
			if(_formatSrting)
			{
				const CharT *ptr = _formatSrting;
				while(*ptr != '%' && *ptr != '\0')
				{
					ptr++;
				}
				int outputSize = ptr - _formatSrting;
				OutputPolicy::write(_formatSrting, outputSize);

				if(*ptr == '%') ptr++;
				if(*ptr == '\0')
					_formatSrting = 0;
				else
					_formatSrting = ptr;
			}
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
