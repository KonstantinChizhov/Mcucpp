
#pragma once
#include "enum.h"
#include "template_utils.h"
#include <string.h>
#include <stdlib.h>
#include <tiny_ios.h>

namespace Mcucpp
{

	template<class InputPolicy,
				class CharT = char
				>
	class basic_istream :public InputPolicy
	{
		typedef CharTrates<CharT> Trates;
		typedef basic_istream Self;
		typedef  basic_ios<CharT> IOS;
		CharT SkipWs()
		{
			CharT c;
			for(c = get(); isspace(c) && !eof(); c = get());
			return c;
		}
	public:
		basic_istream()
		{

		}

		using InputPolicy::eof;
		using InputPolicy::get;

		Self& operator>> (bool &value)
		{
			return *this;
		}

		Self& operator>> (int &value)
		{
			CharT cvtBuffer[Impl::ConvertBufferSize<int>::value];
			CharT *ptr = cvtBuffer;
			*this >> cvtBuffer;
			value = Impl::StringToIntDec<int, CharT*>(ptr);
			return *this;
		}

		Self& operator>> (long &value)
		{
			return *this;
		}

		Self& operator>> (unsigned long &value)
		{
			return *this;
		}

		Self& operator>> (unsigned value)
		{
			return *this;
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
