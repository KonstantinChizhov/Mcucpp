
#pragma once
#include "enum.h"
#include "template_utils.h"
#include <string.h>
#include <stdlib.h>
#include <tiny_ios.h>

namespace Mcucpp
{

	template<class InputPolicy,
				class CharT = char,
				class IOS=basic_ios<CharT>
				>
	class basic_istream :public InputPolicy, public IOS
	{
		typedef CharTrates<CharT> Trates;
		typedef basic_istream Self;
		CharT SkipWs()
		{
			CharT c;
			for(c = get(); isspace(c) && !eof(); c = get());
			return c;
		}

		template<class T>
		Self& ReadInteger(T &value)
		{
            CharT cvtBuffer[Impl::ConvertBufferSize<int>::value];
			CharT *ptr = cvtBuffer;
			*this >> cvtBuffer;

			if((cvtBuffer[0] == '0' && (cvtBuffer[1] == 'x' || cvtBuffer[1] == 'X')) || (IOS::flags() & IOS::hex))
			{
			    ptr+=2;
                value = Impl::StringToIntHex<T, CharT*>(ptr);
			}
			else if(cvtBuffer[0] == '0' || (IOS::flags() & IOS::oct))
			{
                ptr++;
                value = Impl::StringToIntOct<T, CharT*>(ptr);
			}
			else
			{
                if(cvtBuffer[0] == '-')
                    ptr++;
                value = Impl::StringToIntDec<T, CharT*>(ptr);
                if(cvtBuffer[0] == '-')
                    value=-value;
			}
			return *this;
		}
	public:
		using IOS::eof;

		basic_istream()
		{

		}

		CharT get()
		{
			return InputPolicy::get(*this);
		}

		Self& operator>> (bool &value)
		{
			return *this;
		}

		Self& operator>> (int &value)
		{
			return ReadInteger<int>(value);
		}

		Self& operator>> (long &value)
		{
			return ReadInteger<long>(value);
		}

		Self& operator>> (unsigned long &value)
		{
			return ReadInteger<unsigned long>(value);
		}

		Self& operator>> (unsigned value)
		{
			return ReadInteger<unsigned>(value);
		}

		Self& operator>> (CharT* value)
		{
			for(CharT c = SkipWs(); !isspace(c) && !eof(); c = get())
			{
				*value = c;
				value++;
			}
			*value = 0;
			return *this;
		}

		Self& operator>> (CharT &value)
		{
			get(value);
			return *this;
		}

		Self&
		operator>>(Self& (*__pf)(Self&))
		{
			return __pf(*this);
		}

		Self&
		operator>>(IOS& (*__pf) (IOS&))
		{
			__pf(*this);
			return *this;
		}

		void getline(const CharT *str)
		{

		}

		void read(CharT str, size_t size)
		{
			CharT end = str + size;
			read(str, end);
		}

		void read(CharT begin, CharT end)
		{
			while(begin != end && 1)
			{
				*begin = get(*begin);
				++begin;
			}
		}
	};
}
