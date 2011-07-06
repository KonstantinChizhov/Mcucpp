
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
		unsigned StringToIntDec(StrT str, T &result)
		{
			result = 0;
			unsigned count = 0;
			while(isdigit(*str))
			{
				result = result * 10 + (*str - '0');
				str++;
				count++;
			}
			return count;
		}

		template<class T, class StrT>
		unsigned StringToIntOct(StrT str, T &result)
		{
			result = 0;
			unsigned count = 0;
			while(isoctdigit(*str))
			{
				result = result * 8 + (*str - '0');
				str++;
				count++;
			}
			return count;
		}

		template<class T, class StrT>
		unsigned StringToIntHex(StrT str, T &result)
		{
			result = 0;
			int delta;
			unsigned count = 0;
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
			return count;
		}
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
		if(IOS::flags() & IOS::left)
            FieldFill(lastOutputLength);
	}

	template<class OutputPolicy, class CharT, class IOS>
	void FormatWriter<OutputPolicy, CharT, IOS>::FieldFillPre(int lastOutputLength)
	{
		if(!(IOS::flags() & IOS::left))
            FieldFill(lastOutputLength);
	}

	template<class OutputPolicy, class CharT, class IOS>
	void FormatWriter<OutputPolicy, CharT, IOS>::FieldFill(int lastOutputLength)
	{
		int fillcount = IOS::width(0) - lastOutputLength;
		CharT c = IOS::fill(' ');
		for(int i=0; i<fillcount; i++)
			OutputPolicy::put(c);
	}

	template<class OutputPolicy, class CharT, class IOS>
	template<class T>
	void FormatWriter<OutputPolicy, CharT, IOS>::PutInteger(T value)
	{
		const int bufferSize = Impl::ConvertBufferSize<int>::value;
		CharT buffer[bufferSize];
		CharT sign = 0;

		if(Base() == 10)
		{
			if(Util::IsSigned<T>::value)
			{
				if(value < 0)
				{
					value = -value;
					sign = Trates::Minus();
				} else if(IOS::flags() & IOS::showpos)
					sign = Trates::Plus();
			}
			else
			{
				if(IOS::flags() & IOS::showpos)
					sign = Trates::Plus();
			}
		}
		

		typedef typename Util::Unsigned<T>::Result UT;
		UT uvalue = static_cast<UT>(value);
		CharT * str = Impl::IntToString(uvalue, buffer + bufferSize, Base());

		if(sign)
			*--str = sign;

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
			while(true)
			{
			    if(*_formatSrting == '%')
			    {
			         _formatSrting++;
                    if(*_formatSrting != '%')
                    {
						bool flags=true;
						do{
							switch(*_formatSrting)
							{
								case '+': 
									IOS::setf(IOS::showpos);
									break;
								case '#': 
									IOS::setf(IOS::showbase | IOS::boolalpha );
									break;
								case 'x': 
									IOS::setf(IOS::hex, IOS::basefield);
									break;
								case '0':
									IOS::fill('0');
									IOS::setf(IOS::right, IOS::adjustfield);
									break;
								case '-':
									IOS::fill(' ');
									IOS::setf(IOS::left, IOS::adjustfield);
									break;
								//case ' ': case '*':
								//	break;
								default:
								flags = false;
							}
							if(flags) _formatSrting++;
						}while(flags);
						uint8_t width;
						_formatSrting += Impl::StringToIntDec<uint8_t>(_formatSrting, width);
						IOS::width(width);
						if(*_formatSrting == '.')
						{
							_formatSrting++;
							uint8_t presc;
							_formatSrting += Impl::StringToIntDec<uint8_t>(_formatSrting, presc);
							IOS::precision(presc);
						}

                        break;
                    }
                }
				
                if(*_formatSrting == '\0')
                {
                    _formatSrting = 0;
                    break;
                }
				OutputPolicy::put(*_formatSrting);
				_formatSrting++;
			}
		}
	}
}
