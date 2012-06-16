#include <stdlib.h>

namespace Mcucpp
{

	template<class OutputPolicy, class char_type, class IOS>
	unsigned basic_ostream<OutputPolicy, char_type, IOS>::Base()
	{
		if(IOS::flags() & IOS::hex) return 16;
		if(IOS::flags() & IOS::oct) return 8;
		return 10;
	}

	template<class OutputPolicy, class char_type, class IOS>
	void basic_ostream<OutputPolicy, char_type, IOS>::FieldFill(streamsize_t lastOutputLength, typename IOS::fmtflags mask)
	{
		if(IOS::flags() & mask)
		{
			streamsize_t width = IOS::width(0);
			if(width < lastOutputLength)
				return;
			streamsize_t fillcount = width - lastOutputLength;
			char_type c = IOS::fill(' ');
			for(streamsize_t i=0; i<fillcount; i++)
				put(c);
		}
	}

	template<class OutputPolicy, class char_type, class IOS>
	template<class T>
	void basic_ostream<OutputPolicy, char_type, IOS>::PutInteger(T value)
	{
		const int bufferSize = ConvertBufferSize<T>::value;
		char_type buffer[bufferSize];
		const int maxPrefixSize = 3;
		char_type prefix[maxPrefixSize];
		char_type *prefixPtr = prefix + maxPrefixSize;

		if((IOS::flags() & (IOS::hex | IOS::oct)) == 0)
		{
			if(Util::IsSigned<T>::value)
			{
				if(value < 0)
				{
					value = -value;
					*--prefixPtr = trates::Minus();
				} else if(IOS::flags() & IOS::showpos)
					*--prefixPtr = trates::Plus();
			}
			else
			{
				if(IOS::flags() & IOS::showpos)
					*--prefixPtr = trates::Plus();
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
		char_type * str = IntToString(uvalue, buffer + bufferSize, Base());

		int outputSize = buffer + bufferSize - str + prefix + maxPrefixSize - prefixPtr;

		FieldFill(outputSize, IOS::right);
		write(prefixPtr, prefix + maxPrefixSize);
		FieldFill(outputSize, IOS::internal);
		write(str, buffer + bufferSize);
		FieldFill(outputSize, IOS::left);
	}

	template<class OutputPolicy, class char_type, class IOS>
	void basic_ostream<OutputPolicy, char_type, IOS>::PutBool(bool value)
	{
		if(IOS::flags() & IOS::boolalpha)
		{
			if(value)
			{
				puts(trates::True());
			}
			else
			{
				puts(trates::False());
			}
		}
		else
		{
			FieldFill(1, IOS::right);
			if(value)
				put(trates::DigitToLit(1));
			else
				put(trates::DigitToLit(0));
			FieldFill(1, IOS::left);
		}
	}
}
