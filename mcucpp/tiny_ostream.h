
#pragma once
#include "enum.h"
#include "template_utils.h"
#include <string.h>
#include <stdlib.h>
#include <tiny_ios.h>

namespace IO
{
	template<class OutputPolicy,
            class CharT = char,
            class IOS = basic_ios<CharT>
            >
	class basic_ostream :public OutputPolicy, public IOS
	{
		typedef CharTrates<CharT> Trates;
		typedef basic_ostream Self;
	private:

		inline unsigned Base();
		inline void FieldFill(streamsize_t lastOutputLength, typename IOS::fmtflags mask);
		template<class T>
		inline void PutInteger(T value);
		inline void PutBool(bool value);

	public:
		using OutputPolicy::put;

		basic_ostream()
		{}

		Self& operator<< (bool value)
		{
			PutBool(value);
			return *this;
		}

		Self& operator<< (int value)
		{
			PutInteger(value);
			return *this;
		}

		Self& operator<< (long value)
		{
			PutInteger(value);
			return *this;
		}

		Self& operator<< (unsigned long value)
		{
			PutInteger(value);
			return *this;
		}

		Self& operator<< (unsigned value)
		{
			PutInteger(value);
			return *this;
		}

		Self& operator<< (const CharT* value)
		{
			puts(value);
			return *this;
		}

		Self& operator<< (CharT value)
		{
			put(value);
			return *this;
		}

		Self&
		operator<<(Self& (*__pf)(Self&))
		{
			return __pf(*this);
		}

		Self&
		operator<<(IOS& (*__pf) (IOS&))
		{
			__pf(*this);
			return *this;
		}

		void puts(const CharT *str)
		{
		    const size_t outputSize = Trates::SrtLen(str);
            FieldFill(outputSize, IOS::right);
			write(str, outputSize);
			FieldFill(outputSize, IOS::left);
		}
		
		template<class CharPtr>
		void puts(CharPtr str)
		{
			CharPtr strEnd = str;
			while(*strEnd) ++strEnd;
			int outputSize = strEnd - str;
            FieldFill(outputSize, IOS::right);
			 
			while(CharT c = *str)
			{
				put(c);
				++str;
			}
			FieldFill(outputSize, IOS::left);
		}

		template<class CharPtr>
		void write(CharPtr str, size_t size)
		{
			CharPtr end = str + size;
			write(str, end);
		}

		template<class CharPtr>
		void write(CharPtr begin, CharPtr end)
		{
			while(begin != end)
			{
				put(*begin);
				++begin;
			}
		}
	};

	template<class OutputPolicy, class CharT, class IOS>
	basic_ostream<OutputPolicy, CharT, IOS>& endl ( basic_ostream<OutputPolicy, CharT, IOS>& os)
	{
		os.put('\n');
		return os;
	}

	template<class OutputPolicy, class CharT, class IOS>
	basic_ostream<OutputPolicy, CharT, IOS>& ends ( basic_ostream<OutputPolicy, CharT, IOS>& os)
	{
		os.put('\0');
		return os;
	}
}
#include <impl/tiny_ostream.h>
