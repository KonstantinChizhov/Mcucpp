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

#pragma once
#include "enum.h"
#include "template_utils.h"
#include <tiny_ios.h>

namespace Mcucpp
{
	template<class OutputPolicy,
			class char_type = char,
			class IOS = basic_ios<char_type>
			>
	class basic_ostream :public OutputPolicy, public IOS
	{
		typedef basic_ostream Self;
	private:

		inline unsigned Base();
		inline void FieldFill(streamsize_t lastOutputLength, typename IOS::fmtflags mask);
		template<class T>
		inline void PutInteger(T value);
		inline void PutFloat(float value);
		inline void PutBool(bool value);

	public:
        typedef typename IOS::trates trates;
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

		Self& operator<< (float value)
		{
			PutFloat(value);
			return *this;
		}

		Self& operator<< (const char_type* value)
		{
			puts(value);
			return *this;
		}

		Self& operator<< (char_type value)
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
		operator<<(ios_base& (*__pf) (ios_base&))
		{
			__pf(*this);
			return *this;
		}

		void puts(const char_type *str)
		{
			const size_t outputSize = trates::StrLen(str);
			FieldFill(outputSize, IOS::right);
			write(str, outputSize);
			FieldFill(outputSize, IOS::left);
		}

		template<class CharPtr>
		void puts(CharPtr str)
		{
			CharPtr strEnd = str;
			while(*strEnd) ++strEnd;
			streamsize_t outputSize = strEnd - str;
			FieldFill(outputSize, IOS::right);

			while(char_type c = *str)
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

	template<class OutputPolicy, class char_type, class IOS>
	basic_ostream<OutputPolicy, char_type, IOS>& endl ( basic_ostream<OutputPolicy, char_type, IOS>& os)
	{
		os.put('\n');
		return os;
	}

	template<class OutputPolicy, class char_type, class IOS>
	basic_ostream<OutputPolicy, char_type, IOS>& ends ( basic_ostream<OutputPolicy, char_type, IOS>& os)
	{
		os.put('\0');
		return os;
	}
}
#include <impl/tiny_ostream.h>
