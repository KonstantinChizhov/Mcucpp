
namespace IO
{
	namespace Impl
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

		template<class T>
		struct ConvertBufferSize
		{
			static const int value = sizeof(T) * 3;
		};
	}

	template<class OutputPolicy, class CharT, class IOS>
	unsigned FormatWriter<OutputPolicy, CharT, IOS>::Base()
	{
		if(IOS::flags() & IOS::hex) return 16;
		if(IOS::flags() & IOS::oct) return 8;
		return 10;
	}

	template<class OutputPolicy, class CharT, class IOS>
	void FormatWriter<OutputPolicy, CharT, IOS>::FieldFillPost(int lastOutputLength)
	{
		if(IOS::flags() & IOS::right)
			return;
		int fillcount = IOS::width(0) - lastOutputLength;
		for(int i=0; i<fillcount; i++)
			OutputPolicy::put(IOS::fill());
	}

	template<class OutputPolicy, class CharT, class IOS>
	void FormatWriter<OutputPolicy, CharT, IOS>::FieldFillPre(int lastOutputLength)
	{
		if(IOS::flags() & IOS::left)
			return;
		int fillcount = IOS::width(0) - lastOutputLength;
		for(int i=0; i<fillcount; i++)
			OutputPolicy::put(IOS::fill());
	}

	template<class OutputPolicy, class CharT, class IOS>
	template<class T>
	void FormatWriter<OutputPolicy, CharT, IOS>::PutInteger(T value)
	{
		const int bufferSize = Impl::ConvertBufferSize<int>::value;
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
		CharT * str = Impl::IntToString(uvalue, buffer + bufferSize, Base());

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

	template<class OutputPolicy, class CharT, class IOS>
	void FormatWriter<OutputPolicy, CharT, IOS>::PutBool(bool value)
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

	template<class OutputPolicy, class CharT, class IOS>
	void FormatWriter<OutputPolicy, CharT, IOS>::ProcessFormat()
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
}
