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

#include <stdint.h>
#include <bit>
#include <algorithm>
#include <format/ftoa_engine.h>

/// @brief returns upper half of 32*32=>64 bit product with rounding up
/// @param u first operand
/// @param v second operand
/// @return upper half of 32*32=>64 bit product
static inline uint32_t mul32hu(uint32_t u, uint32_t v)
{
	uint64_t prod = (uint64_t)u * v;
	prod += u >> 1; // additional rounding
	return prod >> 32;
}

namespace Mcucpp
{
	/// @brief table for converting floating point mantissa to exp 0 fraction for digit extraction.
	/// Contais 1/8 of all required values values, so exponent correction is required
	static const uint32_t ftoaTable[] =
		{
			0xF0BDC21A,
			0x3DA137D5,
			0x9DC5ADA8,
			0x2863C1F5,
			0x6765C793,
			0x1A784379,
			0x43C33C19,
			0xAD78EBC5,
			0x2C68AF0B,
			0x71AFD498,
			0x1D1A94A2,
			0x4A817C80,
			0xBEBC2000,
			0x30D40000,
			0x7D000000,
			0x20000000,
			0x51EB851E,
			0xD1B71758,
			0x35AFE535,
			0x89705F41,
			0x232F3302,
			0x5A126E1A,
			0xE69594BE,
			0x3B07929F,
			0x971DA050,
			0x26AF8533,
			0x63090312,
			0xFD87B5F2,
			0x40E75996,
			0xA6274BBD,
			0x2A890926,
			0x6CE3EE76};

	static constexpr uint32_t extract_mask = 0x0fff'ffff;
	static constexpr int valuable_float_digits = 6;

	static inline char *ret_zero(char *&ptr, char *end, int presc)
	{
		*ptr++ = '0';
		if (presc > 0 && ptr < end)
		{
			*ptr++ = '.';
			for (int i = 0; i < presc && ptr < end; i++)
				*ptr++ = '0';
		}
		return ptr;
	}

	static inline char *ret_nan_or_inf(uint32_t fraction, char *&ptr, char *end, ftoa_flags flags)
	{
		char shift = HasAnyFlag(flags, ftoa_flags::uppercase) ? 'A' - 'a' : 0;
		if (fraction & 0x007fffff)
		{
			*ptr++ = 'n' + shift;
			if (ptr >= end)
				return ptr;
			*ptr++ = 'a' + shift;
			if (ptr >= end)
				return ptr;
			*ptr++ = 'n' + shift;
		}
		else
		{
			*ptr++ = 'i' + shift;
			if (ptr >= end)
				return ptr;
			*ptr++ = 'n' + shift;
			if (ptr >= end)
				return ptr;
			*ptr++ = 'f' + shift;
		}
		return ptr;
	}

	static inline uint32_t convert_to_pow0_fraction(uint32_t fraction, uint8_t exponent)
	{
		fraction <<= 8;
		uint32_t t = mul32hu(fraction, ftoaTable[exponent / 8]) + 1;
		// Handle missing values in ftoaTable. Intermediate results are fitting to 32 bits, so no prescision lost happening.
		uint_fast8_t shift = 7 - (exponent & 7);
		t >>= shift;
		return t;
	}

	static inline char *write_leading_zeros(char *ptr, char *end, int exp10, int &presc)
	{
		*ptr++ = '0';
		if (presc > 0 && ptr < end)
			*ptr++ = '.';
		int leading_zeros = std::min(-1 - exp10, presc);
		for (int i = 0; i < leading_zeros && ptr < end; i++)
			*ptr++ = '0';
		presc -= leading_zeros;
		return ptr;
	}

	static inline char *write_exponent(ftoa_flags flags, int exp10, char *ptr, char *end)
	{
		if (HasAnyFlag(flags, ftoa_flags::scientific) && exp10 != 0 && ptr < end)
		{
			*ptr++ = HasAnyFlag(flags, ftoa_flags::uppercase) ? 'E' : 'e';
			if (ptr >= end)
				return ptr;
			*ptr++ = exp10 > 0 ? '+' : '-';
			if (ptr >= end)
				return ptr;
			exp10 = std::abs(exp10);
			// exponent can be only 2 digits from -38 to 38
			char low = exp10 % 10 + '0';
			exp10 /= 10;
			if (ptr >= end)
				return ptr;
			*ptr++ = exp10 + '0';
			if (ptr >= end)
				return ptr;
			*ptr++ = low;
		}
		return ptr;
	}

	static inline char *round_up(uint32_t t, char *end, char *begin)
	{
		end--; // points to the last digit
		// roundup last digit if required

		uint8_t digit1 = uint8_t(t >> 28); // first digit after the last one in the visible output
		t = (t & extract_mask) * 10;
		uint8_t digit2 = uint8_t(t >> 28); // second digit after the last one in the visible output

		// in some cases digit1 has to be 5 but it can not be represented exactly, so it is like x.xxx49..., where x.xxx is visible output and 4 and 9 are digit1 and digit2 respectively.
		if (digit2 >= 9)
		{
			digit1 += 1;
		}
		if (digit1 >= 5)
		{
			*end += 1;
		}

		while (end > begin)
		{
			if (*end > '9')
			{
				*end -= 10;
				if (end[-1] == '.') // skip the dot
					end--;
				end[-1] += 1;
			}
			end--;
		}
		return end;
	}

	static inline char *write_digits(uint32_t &t, char *ptr, char *end, int count)
	{
		for (int i = 0; i < count && ptr < end; i++)
		{
			uint8_t digit = (uint8_t)(t >> 28);
			*ptr++ = digit + '0';
			t = (t & extract_mask) * 10;
		}
		return ptr;
	}

	static inline char *write_integer_part(uint32_t &t, char *ptr, char *end, int &exp10, ftoa_flags flags)
	{
		// extract digits. integer part. At least 1 digit
		int digits = exp10 + 1;
		if (HasAnyFlag(flags, ftoa_flags::scientific))
			digits = 1;

		uint8_t digit = (uint8_t)(t >> 28);
		if (digit >= 10)
		{
			digit -= 10;
			*ptr++ = '1';
			exp10++;
			if (HasAnyFlag(flags, ftoa_flags::scientific))
			{
				digits = 0;
			}
		}
		if (!HasAnyFlag(flags, ftoa_flags::scientific))
		{
			exp10++;
		}

		return write_digits(t, ptr, end, digits);
	}

	static char *roundup_correction(char *ptr, ftoa_flags flags, int &exp10, char *end)
	{
		// correction after round up, if rounding produces one extra decimal digit like 999.9 -> 1000
		if (*ptr > '9')
		{
			*ptr -= 10;
			char *move_begin = ptr;
			if (HasAnyFlag(flags, ftoa_flags::scientific))
			{
				move_begin += 2;
				exp10++;
			}
			else
			{
				end++;
			}
			std::copy_backward(move_begin, end, end + 1);
			*ptr = '1';
		}
		return end;
	}

	static char *trim_trailing_zeros(char *begin, char *end, ftoa_flags flags, int presc)
	{
		if (presc == 0 || HasAnyFlag(flags, ftoa_flags::fixed | ftoa_flags::scientific))
			return end;

		while (end > begin && end[-1] == '0')
		{
			*--end = 0;
		}
		if (end[-1] == '.')
		{
			*--end = 0;
		}
		return end;
	}

	static uint32_t skip_leading_zeros(uint32_t t, int &exp10)
	{
		while (((t & ~extract_mask) == 0))
		{
			t *= 10;
			exp10--;
		}
		return t;
	}

/// @brief Converts float value to string representation. Supports normal, fixed and scientific formats.
 /// @param value - float ot conver
 /// @param buffer - buffer to hold the result.
 /// @param buffer_end - end of conversion buffer. If result dies not fit, it is truncated.
 /// @param presc number of digits after decimal point
 /// @param flags - conversion options
 /// @return std::string_view pointing to the converted value in the @param buffer
 std::string_view ftoa_engine(float value, char *buffer, char *buffer_end, int presc, ftoa_flags flags)
 {
	// uint32_t uvalue = std::bit_cast<uint32_t>(value);
 	uint32_t uvalue = *((uint32_t*)&value);
 	uint8_t exponent = uint8_t(uvalue >> 23);
 	uint32_t fraction = (uvalue & 0x00ffffff) | 0x00800000;

 	char *start = buffer;
 	char *ptr = buffer;

		presc = std::min(10, presc);

		if (uvalue & 0x80000000)
		{
			*ptr++ = '-';
			buffer++;
		}
		else if (HasAnyFlag(flags, ftoa_flags::showpos))
		{
			*ptr++ = '+';
			buffer++;
		}
		else if (HasAnyFlag(flags, ftoa_flags::posspace))
		{
			*ptr++ = ' ';
			buffer++;
		}
		if (ptr >= buffer_end)
			return std::string_view{start, (size_t)(ptr - start)};

		if (exponent == 0) // subnormals treat as zero
			return std::string_view{start, (size_t)(ret_zero(ptr, buffer_end, presc) - start)};

		if (exponent == 0xff)
			return std::string_view{start, (size_t)(ret_nan_or_inf(fraction, ptr, buffer_end, flags) - start)};

		// approximation for log10(2^exponent). May be up by 1, has to be corrected when counting leading zeros.
		// exp10 is in range -38..+38
		int exp10 = ((((exponent >> 3)) * 77 + 63) >> 5) - 38;

		uint32_t t = convert_to_pow0_fraction(fraction, exponent);

		// skip leading zeros and correct exp
		t = skip_leading_zeros(t, exp10);

		if (presc < 0)
		{
			if (HasAnyFlag(flags, ftoa_flags::scientific))
				presc = valuable_float_digits;
			else if (exp10 < -4)
			{
				flags |= ftoa_flags::scientific;
				presc = 0;
			}
			else if (exp10 > valuable_float_digits)
			{
				flags |= ftoa_flags::scientific;
				presc = valuable_float_digits;
			}
		}

		if (exp10 < 0 && !HasAnyFlag(flags, ftoa_flags::scientific))
		{
			ptr = write_leading_zeros(ptr, buffer_end, exp10, presc);
		}
		else
		{
			ptr = write_integer_part(t, ptr, buffer_end, exp10, flags);

			if (presc < 0)
				presc = std::max(valuable_float_digits - exp10 + 1, 0);

			if (presc > 0)
				*ptr++ = '.';
		}

		// Extract digits. Fractional part.
		ptr = write_digits(t, ptr, buffer_end, presc);

		char *end = ptr;

		ptr = round_up(t, ptr, buffer);

		end = roundup_correction(ptr, flags, exp10, end);

		end = trim_trailing_zeros(buffer, end, flags, presc);

		end = write_exponent(flags, exp10, end, buffer_end);
		return std::string_view{start, (size_t)(end - start)};
 	}
}
