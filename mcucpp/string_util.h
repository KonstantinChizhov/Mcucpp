
namespace Mcucpp
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
			*--ptr = char_trates<CharT>::DigitToLit(rem);
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
