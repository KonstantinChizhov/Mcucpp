#include <stdlib.h>

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
		inline int isxdigit(CharT c)
		{
			return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
		}

		template<class T, class StrT>
		T StringToIntDec(StrT &str)
		{
			T result = 0;
			while(isdigit(*str))
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
			while(isoctdigit(*str))
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
			while(1)
			{
				if(isdigit(*str))
					delta = '0';
				else if((*str >= 'A' && *str <= 'F'))
					delta = 'A';
				else if((*str >= 'a' && *str <= 'f'))
					delta = 'a';
					else break;

				result = result * 16 + (*str - delta);
				str++;
			}
		}
	}

	template<class OutputPolicy, class CharT, class IOS>
	unsigned basic_ostream<OutputPolicy, CharT, IOS>::Base()
	{
		if(IOS::flags() & IOS::hex) return 16;
		if(IOS::flags() & IOS::oct) return 8;
		return 10;
	}

	template<class OutputPolicy, class CharT, class IOS>
	void basic_ostream<OutputPolicy, CharT, IOS>::FieldFill(streamsize_t lastOutputLength, typename IOS::fmtflags mask)
	{
		if(IOS::flags() & mask)
		{
			streamsize_t width = IOS::width(0);
			if(width < lastOutputLength)
				return;
			streamsize_t fillcount = width - lastOutputLength;
			CharT c = IOS::fill(' ');
			for(streamsize_t i=0; i<fillcount; i++)
				put(c);
		}
	}

	template<class OutputPolicy, class CharT, class IOS>
	template<class T>
	void basic_ostream<OutputPolicy, CharT, IOS>::PutInteger(T value)
	{
		const int bufferSize = Impl::ConvertBufferSize<T>::value;
		CharT buffer[bufferSize];
		const int maxPrefixSize = 3;
		CharT prefix[maxPrefixSize];
		CharT *prefixPtr = prefix + maxPrefixSize;

		if((IOS::flags() & (IOS::hex | IOS::oct)) == 0)
		{
			if(Util::IsSigned<T>::value)
			{
				if(value < 0)
				{
					value = -value;
					*--prefixPtr = Trates::Minus();
				} else if(IOS::flags() & IOS::showpos)
					*--prefixPtr = Trates::Plus();
			}
			else
			{
				if(IOS::flags() & IOS::showpos)
					*--prefixPtr = Trates::Plus();
			}
		}
		else
		if(IOS::flags() & IOS::showbase)
		{
			if(IOS::flags() & IOS::hex)
				*--prefixPtr = 'x';
			*--prefixPtr = '0';
		}

		typedef typename Util::Unsigned<T>::Result UT;
		UT uvalue = static_cast<UT>(value);
		CharT * str = Impl::IntToString(uvalue, buffer + bufferSize, Base());

		int outputSize = buffer + bufferSize - str + prefix + maxPrefixSize - prefixPtr;

		FieldFill(outputSize, IOS::right);
		write(prefixPtr, prefix + maxPrefixSize);
		FieldFill(outputSize, IOS::internal);
		write(str, buffer + bufferSize);
		FieldFill(outputSize, IOS::left);
	}

	template<class OutputPolicy, class CharT, class IOS>
	void basic_ostream<OutputPolicy, CharT, IOS>::PutBool(bool value)
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
			FieldFill(1, IOS::right);
			if(value)
				put(Trates::DigitToLit(1));
			else
				put(Trates::DigitToLit(0));
			FieldFill(1, IOS::left);
		}
	}
}
