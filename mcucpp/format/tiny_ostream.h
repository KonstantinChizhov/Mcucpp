//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2025
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
#include <format/tiny_ios.h>
#include <format/ftoa_engine.h>
#include <format/int_to_str.h>
#include <format/format_parser.h>

#include <algorithm>
#include <cctype>
#include <string_view>

namespace Mcucpp
{

	enum class FormatResult
	{
		Success,
		BufferFull,
		InvalidFormat,
		UnmatchedBrase,
	};

	template <class OutputPolicy,
			  class char_type = char>
	class basic_ostream : public OutputPolicy, public basic_ios<char_type>
	{
		using IOS = basic_ios<char_type>;

	private:
		inline unsigned int_base();
		inline void field_fill(streamsize_t lastOutputLength, typename IOS::fmtflags mask);
		template <class T>
		inline void put_int(T value);
		inline void put_float(float value);
		inline void put_bool(bool value);

	public:
		using IOS::flags;
		using OutputPolicy::write;
		using trates = std::char_traits<char_type>;

		template <typename... Args>
		basic_ostream(const Args &...args)
			: OutputPolicy{args...}
		{
		}

		basic_ostream &operator<<(bool value)
		{
			put_bool(value);
			return *this;
		}

		basic_ostream &operator<<(int value)
		{
			put_int(value);
			return *this;
		}

		basic_ostream &operator<<(long value)
		{
			put_int(value);
			return *this;
		}

		basic_ostream &operator<<(long long value)
		{
			put_int(value);
			return *this;
		}

		basic_ostream &operator<<(unsigned long long value)
		{
			put_int(value);
			return *this;
		}

		template <class T>
		basic_ostream &operator<<(const T *value)
		{
			IOS::setf(ios_base::hex | ios_base::showbase);
			put_int((size_t)value);
			return *this;
		}

		basic_ostream &operator<<(unsigned long value)
		{
			put_int(value);
			return *this;
		}

		basic_ostream &operator<<(unsigned value)
		{
			put_int(value);
			return *this;
		}

		basic_ostream &operator<<(float value)
		{
			put_float(value);
			return *this;
		}

		basic_ostream &operator<<(std::basic_string_view<char_type> value)
		{
			puts(value);
			return *this;
		}

		basic_ostream &operator<<(const char_type *value)
		{
			puts({value, trates::length(value)});
			return *this;
		}

		basic_ostream &operator<<(char_type value)
		{
			puts({&value, 1});
			return *this;
		}

		basic_ostream &operator<<(basic_ostream &(*__pf)(basic_ostream &))
		{
			return __pf(*this);
		}

		basic_ostream &operator<<(ios_base &(*__pf)(ios_base &))
		{
			__pf(*this);
			return *this;
		}

		void puts(std::basic_string_view<char_type> value)
		{
			field_fill(value.size(), IOS::right);
			write(value.begin(), value.end());
			field_fill(value.size(), IOS::left);
			reset_fill_and_width();
		}

		void write(std::basic_string_view<char_type> value)
		{
			write(value.begin(), value.end());
		}

		template <typename... Args>
		FormatResult format(const char_type *fmt, Args &&...args) noexcept;

	private:
		void reset_fill_and_width()
		{
			IOS::fill(' ');
			IOS::width(0);
		}

		template <class T>
		void format_arg(T val, format_parser<char_type> &parser)
		{
			if ((parser.flags & ios_base::adjustfield) == 0)
			{
				parser.flags |= std::is_arithmetic<T>::value ? ios_base::right : ios_base::left;
			}
			parser.flags ^= ios_base::boolalpha;

			IOS::width(parser.width);
			IOS::precision(parser.presc);
			IOS::flags(parser.flags);
			IOS::fill(parser.fill);

			*this << val;
		}
	};

	template <class OutputPolicy, class char_type>
	void basic_ostream<OutputPolicy, char_type>::field_fill(streamsize_t lastOutputLength, typename IOS::fmtflags mask)
	{
		bool center_fill = (mask != ios_base::internal) && (IOS::flags() & ios_base::center);

		if ((IOS::flags() & mask) || center_fill)
		{
			streamsize_t width = IOS::width();
			if (width < lastOutputLength)
				return;
			streamsize_t fillcount = width - lastOutputLength;
			if (center_fill)
			{
				streamsize_t half = fillcount / 2;
				if (mask == ios_base::left)
					fillcount -= half;
				else
					fillcount = half;
			}
			char_type c = IOS::fill();
			OutputPolicy::fill_n(c, fillcount);
		}
	}

	template <class OutputPolicy, class char_type>
	template <class T>
	void basic_ostream<OutputPolicy, char_type>::put_int(T value)
	{
		const int buffer_size = sizeof(T) * 8;
		char_type buffer[buffer_size];
		char_type *prefix_ptr = buffer;

		using unsigned_type = std::make_unsigned_t<T>;
		unsigned_type unsigned_value;
		auto flags = IOS::flags();

		std::basic_string_view<char_type> digits;

		if ((flags & IOS::hex) != 0)
		{
			unsigned_value = (unsigned_type)value;
			if (flags & IOS::showbase)
			{
				*prefix_ptr++ = '0';
				*prefix_ptr++ = (flags & IOS::uppercase) ? 'X' : 'x';
			}
			digits = utoa_hex(unsigned_value, prefix_ptr, flags & IOS::uppercase);
		}
		else if (flags & IOS::oct)
		{
			unsigned_value = (unsigned_type)value;
			if (flags & IOS::showbase)
			{
				*prefix_ptr++ = '0';
			}
			digits = utoa_oct(unsigned_value, prefix_ptr);
		}
		else if (flags & IOS::bin)
		{
			unsigned_value = (unsigned_type)value;
			if (flags & IOS::showbase)
			{
				*prefix_ptr++ = '0';
				*prefix_ptr++ = (flags & IOS::uppercase) ? 'B' : 'b';
			}
			digits = utoa_bin(unsigned_value, prefix_ptr);
		}
		else
		{
			if constexpr (std::is_signed_v<T>)
			{
				if (value < 0)
				{
					*prefix_ptr++ = '-';
					unsigned_value = -value;
				}
				else
				{
					if (flags & IOS::showpos)
						*prefix_ptr++ = '+';
					if (flags & IOS::posspace)
						*prefix_ptr++ = ' ';
					unsigned_value = value;
				}
			}
			else
			{
				if (flags & IOS::showpos)
					*prefix_ptr++ = '+';
				unsigned_value = value;
			}
			digits = std::string_view{utoa(unsigned_value, prefix_ptr)};
		}

		auto output_size = (prefix_ptr - buffer) + digits.size();

		field_fill(output_size, IOS::right);
		write(buffer, prefix_ptr);
		field_fill(output_size, IOS::internal);
		write(digits);
		field_fill(output_size, IOS::left);
		reset_fill_and_width();
	}

	template <class OutputPolicy, class char_type>
	void basic_ostream<OutputPolicy, char_type>::put_bool(bool value)
	{
		if (IOS::flags() & IOS::boolalpha)
		{
			if (value)
				puts("true");
			else
				puts("false");
		}
		else
		{
			if (value)
				puts("1");
			else
				puts("0");
		}
	}

template <class OutputPolicy, class char_type>
 	void basic_ostream<OutputPolicy, char_type>::put_float(float value)
 	{
 		char_type buffer[FtoaBufferSize];

 		streamsize_t precision = IOS::precision();
 		constexpr auto float_related_flags = ios_base::fixed | ios_base::scientific | ios_base::showpos | ios_base::showpoint | ios_base::uppercase | ios_base::posspace;
 		std::string_view result = ftoa_engine(value, std::begin(buffer), std::end(buffer), precision, static_cast<ftoa_flags>(IOS::flags() & float_related_flags));

 		auto output_size = result.size();

 		field_fill(output_size, IOS::right);
 		write(result);
 		field_fill(output_size, IOS::left);
 		reset_fill_and_width();
 	}

	template <class OutputPolicy, class char_type>
	template <typename... Args>
	FormatResult basic_ostream<OutputPolicy, char_type>::format(const char_type *fmt, Args &&...args) noexcept
	{
		const char_type *end = fmt + trates::length(fmt);
		int arg_n = 0;

		while (fmt < end)
		{
			auto raw_fragment = format_parser<char_type>::get_raw_fragment({fmt, end});
			write(raw_fragment);

			auto spec = format_parser<char_type>::get_format_spec({raw_fragment.end(), end});

			if (spec.size() == 0) // escaped sequence
			{
				fmt = spec.end();
				continue;
			}

			spec = spec.substr(1, spec.size() - 2); // trim {}

			format_parser<char_type> parser;

			if (!parser.parse_spec(spec.begin(), spec.end()))
				return FormatResult::InvalidFormat;

			int arg_pos = parser.arg_id >= 0 ? parser.arg_id : arg_n;

			int i = 0;
			((i++ == arg_pos ? format_arg(args, parser) : void()), ...);

			arg_n++;
			fmt = spec.end() + 1;
		}

		return FormatResult::Success;
	}

	template <class OutputPolicy, class char_type>
	basic_ostream<OutputPolicy, char_type> &endl(basic_ostream<OutputPolicy, char_type> &os)
	{
		os.puts("\r\n");
		return os;
	}

	template <class OutputPolicy, class char_type, class IOS>
	basic_ostream<OutputPolicy, char_type> &ends(basic_ostream<OutputPolicy, char_type> &os)
	{
		os.puts("\0");
		return os;
	}

	struct SetwT
	{
		int width;
	};

	inline SetwT setw(int width)
	{
		SetwT f = {width};
		return f;
	}

	template <class OutputPolicy, class char_type>
	basic_ostream<OutputPolicy, char_type> &operator<<(basic_ostream<OutputPolicy, char_type> &os, SetwT f)
	{
		os.width(f.width);
		return os;
	}
}
