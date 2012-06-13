

namespace Mcucpp
{
	namespace Impl
	{
		template<class T, class CharT>
		CharT * IntToString(T value, CharT *bufferEnd, unsigned radix)
		{
			CharT *ptr = bufferEnd;
			do
			{
				T q = value / radix;
				T rem = value - q*radix;
				value = q;
				*--ptr = CharTrates<CharT>::DigitToLit(rem);
			}
			while (value != 0);
			return ptr;
		}

		template<class T>
		struct ConvertBufferSize
		{
			static const int value = sizeof(T) * 3 + 1;
		};

		template<class CharT>
		inline bool isdigit(CharT c)	{ return c >= '0' && c <= '9';}

		template<class CharT>
		inline bool isoctdigit(CharT c)	{ return c >= '0' && c <= '7';}

		template<class CharT>
		inline bool isxdigit(CharT c)
		{
			return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
		}

		template<class CharT>
		inline bool isspace(CharT c)
		{
			return c == ' ' || c == '\n' || c == '\r' || c == '\t' || c == '\0';
		}

		template<class T, class StrT>
		T StringToIntDec(StrT &str)
		{
			T result = 0;
			uint_fast8_t count = ConvertBufferSize<T>::value;
			while(isdigit(*str) && --count)
			{
				result = result * 10 + (*str - '0');
				str++;
			}
			return result;
		}

		template<class T, class StrT>
		T StringToIntOct(StrT &str)
		{
			T result = 0;
			uint_fast8_t count = ConvertBufferSize<T>::value;
			while(isoctdigit(*str) && --count)
			{
				result = result * 8 + (*str - '0');
				str++;
			}
			return result;
		}

		template<class T, class StrT>
		T StringToIntHex(StrT &str)
		{
			T result = 0;
			int delta;
			uint_fast8_t count = ConvertBufferSize<T>::value;
			while(--count)
			{
				if(isdigit(*str))
					delta = '0';
				else if((*str >= 'A' && *str <= 'F'))
					delta = 'A' - 10;
				else if((*str >= 'a' && *str <= 'f'))
					delta = 'a' - 10;
					else break;

				result = result * 16 + (*str - delta);
				str++;
			}
			return result;
		}
	}

    ios_base::fmtflags ios_base::flags ( ) const
    {return _flags;}

    ios_base::fmtflags ios_base::flags ( ios_base::fmtflags fmtfl )
    {
        fmtflags tmp = _flags;
        _flags = fmtfl;
        return tmp;
    }

    ios_base::fmtflags ios_base::setf ( ios_base::fmtflags fmtfl )
    {
        fmtflags tmp = _flags;
        _flags |= fmtfl;
        return tmp;
    }

    ios_base::fmtflags ios_base::setf ( ios_base::fmtflags fmtfl, ios_base::fmtflags mask )
    {
        fmtflags tmp = _flags;
        _flags = (_flags & ~mask) | fmtfl;
        return tmp;
    }

    ios_base::fmtflags ios_base::unsetf ( ios_base::fmtflags mask )
    {
        fmtflags tmp = _flags;
        _flags &= ~mask;
        return tmp;
    }

    streamsize_t ios_base::width ( ) const
    {
        return _width;
    }

    streamsize_t ios_base::width ( streamsize_t width )
    {
        streamsize_t tmp = _width;
        _width = width;
        return tmp;
    }

	streamsize_t ios_base::precision ( ) const
	{
		return _prec;
	}
	streamsize_t ios_base::precision ( streamsize_t prec )
	{
		streamsize_t tmp = _prec;
        _prec = prec;
        return tmp;
	}

	template<class CharT>
	bool basic_ios<CharT>::good () const
    {
        return _state == goodbit;
    }

    template<class CharT>
    bool basic_ios<CharT>::fail () const
    {
        return _state & failbit;
    }

    template<class CharT>
    bool basic_ios<CharT>::bad () const
    {
        return _state & badbit;
    }

    template<class CharT>
    bool basic_ios<CharT>::eof ( ) const
    {
        return _state & eofbit;
    }

    template<class CharT>
    typename basic_ios<CharT>::iostate basic_ios<CharT>::rdstate ( ) const
    {
        return _state;
    }

    template<class CharT>
    void basic_ios<CharT>::setstate (typename basic_ios<CharT>::iostate state )
    {
        _state |= state;
    }

    template<class CharT>
    void basic_ios<CharT>::clear (typename basic_ios<CharT>::iostate state )
    {
        _state = state;
    }

    template<class CharT>
    CharT basic_ios<CharT>::fill ( ) const
    {
        return _fillch;
    }

    template<class CharT>
    CharT basic_ios<CharT>::fill ( CharT fillch )
    {
        uint8_t tmp = _fillch;
        _fillch = fillch;
        return tmp;
    }
}
