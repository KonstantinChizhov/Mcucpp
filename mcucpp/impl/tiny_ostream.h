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

	template<class OutputPolicy, class char_type, class IOS>
	void basic_ostream<OutputPolicy, char_type, IOS>::PutFloat(float value)
	{
		const int bufferSize = ConvertBufferSize<uint32_t>::value;
		char_type intBuffer[bufferSize];
		char_type fracBuffer[bufferSize];
		uint32_t uvalue = *reinterpret_cast<uint32_t*>(&value);
		uint8_t sign = uvalue & 0x80000000 ? 1 : 0;
		int8_t exponent = int8_t(uint8_t(uvalue >> 23) - 127);
		uint32_t fraction = (uvalue & 0x00ffffff) | 0x00800000;

		if((uvalue & 0x7fffffff) == 0)
		{
			put(trates::DigitToLit(0));
			return;
		}

		uint32_t intPart = 0;
		uint32_t fracPart= 0;
		if(exponent >= 23)
			intPart = fraction << (exponent - 23);
		else if(exponent >= 0)
		{
			intPart = fraction >> (23 - exponent);
			fracPart = (fraction << (exponent + 1)) & 0xffffff;
		}else
			fracPart = fraction >> -(exponent + 1);

		uint8_t maxFract = ios_base::precision();
		if((ios_base::flags() & ios_base::floatfield) == 0)
		{
			uint8_t intDigits = DecimalDigits(intPart);
			if(intDigits > maxFract)
                maxFract = 0;
            else
                maxFract -= intDigits;
		}
		if(maxFract >= bufferSize-1)
            maxFract = bufferSize-2;

        fracPart *= 10;
        uint8_t fractDigit = char_type(fracPart >> 24);

		char_type *fractPartPtr = &fracBuffer[0];
		if(fracPart && maxFract)
		{
			*fractPartPtr++ = trates::DecimalDot();

			while(maxFract--)
			{
				*fractPartPtr++ = char_type(fracPart >> 24) + '0';
				fracPart &= 0xffffff;
				fracPart *= 10;
			}

			//char_type lastDigit = char_type(fracPart >> 24) + '0';
			while(*--fractPartPtr == '0')
			{
				//lastDigit = *fractPartPtr;
			}
			if(trates::DecimalDot() == *fractPartPtr)
                --fractPartPtr;

			*++fractPartPtr = 0;
		}

		uint8_t fractPartSize = fractPartPtr - &fracBuffer[0];
        if(fractPartSize == 0 && fractDigit >= 5) // round up
            intPart++;
		char_type *intPartPtr = IntToString(intPart, intBuffer + bufferSize, 10);
		uint8_t intPartSize = intBuffer + bufferSize - intPartPtr;
		if(sign)
		{
			*--intPartPtr = trates::Minus();
			intPartSize++;
		}

		FieldFill(fractPartSize + intPartSize, IOS::right);
		write(intPartPtr, intPartSize);
		write(fracBuffer, fractPartSize);
		FieldFill(fractPartSize + intPartSize, IOS::left);
	}
}
