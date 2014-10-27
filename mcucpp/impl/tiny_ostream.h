//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or 
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************

#include <string_util.h>
#include <impl/ftoa_engine.h>
#include <template_utils.h>

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
		char_type buffer[bufferSize+1];
		const int maxPrefixSize = 3;
		char_type prefix[maxPrefixSize];
		char_type *prefixPtr = prefix + maxPrefixSize;

		if((IOS::flags() & (IOS::hex | IOS::oct)) == 0)
		{
			if(Util::IsSigned<T>::value)
			{
				if(Util::IsNegative(value))
				{
					value = Util::abs(value);
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
		char_type * str = Utoa(uvalue, buffer + bufferSize, Base());

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
		const int bufferSize = 10;
		char_type buffer[bufferSize+1];

		streamsize_t precision = IOS::precision();
		if(precision > bufferSize - 1)
			precision =  bufferSize - 1;
		if(precision <= 2)
			precision = 2;

		int exp10 = ftoaEngine(value, buffer, precision);
		if(exp10 == 0xff)
		{
			char_type *ptr = buffer;
			if(buffer[0] == char_trates<char_type>::Plus() && (IOS::flags() & IOS::showpos) == 0)
				ptr++;
			puts(ptr);
			return;
		}

		char_type *str_begin = &buffer[2];
		if(buffer[1] != char_trates<char_type>::DigitToLit(0))
		{
			exp10++;
			str_begin--;
		}

		uint_fast8_t digits = char_trates<char_type>::StrLen(str_begin);

		uint_fast8_t intDigits=0, leadingZeros = 0;
		if((streamsize_t)Util::abs(exp10) >= precision)
		{
			intDigits = 1;
		}else if(exp10 >= 0)
		{
			intDigits = exp10+1;
			exp10 = 0;
		}else
		{
			intDigits = 0;
			leadingZeros = -exp10 - 1;
			exp10 = 0;
		}
		uint_fast8_t fractDigits = digits > intDigits ? digits - intDigits : 0;

		if(buffer[0] == char_trates<char_type>::Minus() || (IOS::flags() & IOS::showpos))
			put(buffer[0]);

		if(intDigits)
		{
			uint_fast8_t count = intDigits > digits ? digits : intDigits;
			while(count--)
				put(*str_begin++);
			int_fast8_t tralingZeros = intDigits - digits;
			while(tralingZeros-- > 0)
				put(char_trates<char_type>::DigitToLit(0));
		}
		else
			put(char_trates<char_type>::DigitToLit(0));

		if(fractDigits)
		{
			put(char_trates<char_type>::DecimalDot());
			while(leadingZeros--)
				put(char_trates<char_type>::DigitToLit(0));
			while(fractDigits--)
				put(*str_begin++);
		}
		if(exp10 != 0)
		{
			put(char_trates<char_type>::Exp());
			uint_fast8_t upow10;
			if(exp10 < 0)
			{
				put(char_trates<char_type>::Minus());
				upow10 = -exp10;
			}
			else
			{
				put(char_trates<char_type>::Plus());
				upow10 = exp10;
			}
			char *powPtr = UtoaFastDiv(upow10, buffer + bufferSize);
			while(powPtr < buffer + bufferSize)
			{
				put(*powPtr++);
			}
		}
	}
}
