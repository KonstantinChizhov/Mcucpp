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

#include <stdint.h>
#include <string_view>
#include <type_traits>

namespace Mcucpp
{
    /// @brief Converts unsigned integer to string in base 10. Does not write terminating 0.
    /// @param value - integer to convert
    /// @param ptr - buffer begin ptr, must be at least 10 chars
    /// @return string_view of the converted string

    std::string_view utoa(uint32_t value, char *ptr);

    /// @brief Converts unsigned integer to string in base 10. Does not write terminating 0.
    /// @param value - integer to convert
    /// @param ptr - buffer begin ptr, must be at least 20 chars
    /// @return string_view of the converted string
    std::string_view utoa(uint64_t value, char *ptr);

    inline std::string_view utoa(unsigned value, char *ptr) { return utoa((uint32_t)value, ptr); }
    inline std::string_view utoa(unsigned short value, char *ptr) { return utoa((uint32_t)value, ptr); }
    inline std::string_view utoa(unsigned char value, char *ptr) { return utoa((uint32_t)value, ptr); }

    template <class T>
    std::string_view utoa_oct(T value, char *ptr)
    {
        char *start = ptr;
        do
        {
            uint_fast8_t c = value & 0x07;
            value >>= 3;
            *ptr++ = c + '0';
        } while (value);

        char *end = ptr - 1;
        char *result = ptr;
        ptr = start;

        while (ptr < end)
        {
            char tmp = *ptr;
            *ptr = *end;
            *end = tmp;
            ++ptr;
            --end;
        }

        return std::string_view{start, static_cast<size_t>(result - start)};
    }

    template <class T, class char_type>
    std::basic_string_view<char_type> utoa_hex(T value, char_type *ptr, bool uppercase = false)
    {
        constexpr int bits_for_digit = 4;
        constexpr int max_digits = sizeof(T) * 8 / bits_for_digit;
        constexpr int extract_bit_pos = sizeof(T) * 8 - bits_for_digit;

        char_type *start = ptr;
        int i = 0;
        uint8_t digit;
        // skip leading zeros
        do
        {
            digit = (value >> extract_bit_pos) & 0x0f;
            value <<= bits_for_digit;
        } while (digit == 0 && i++ < max_digits);

        char_type letter_base = uppercase ? 'A' : 'a';
        // digit extraction
        do
        {
            *ptr++ = (digit < 10 ? '0' : letter_base - 10) + digit;
            digit = (value >> extract_bit_pos) & 0x0f;
            value <<= bits_for_digit;
        } while (++i < max_digits);

        return std::basic_string_view<char_type>{start, static_cast<size_t>(ptr - start)};
    }

    template <class T, class char_type>
    std::basic_string_view<char_type> utoa_bin(T value, char_type *ptr)
    {
        constexpr int bits_for_digit = 1;
        constexpr int max_digits = sizeof(T) * 8 / bits_for_digit;
        constexpr int extract_bit_pos = sizeof(T) * 8 - bits_for_digit;

        char_type *start = ptr;
        int i = 0;
        uint8_t digit;
        // skip leading zeros
        do
        {
            digit = (value >> extract_bit_pos) & 0x01;
            value <<= bits_for_digit;
        } while (digit == 0 && i++ < max_digits);

        // digit extraction
        do
        {
            *ptr++ = digit + '0';
            digit = (value >> extract_bit_pos) & 0x01;
            value <<= bits_for_digit;
        } while (++i < max_digits);

        return std::basic_string_view<char_type>{start, static_cast<size_t>(ptr - start)};
    }

    template <class T, class char_type>
    std::basic_string_view<char_type> itoa(T value, char_type *ptr, uint_fast8_t radix = 10)
    {
        char_type *start = ptr;
        using unsigned_type = std::make_unsigned_t<T>;
        unsigned_type unsigned_value;

        if constexpr (std::is_signed_v<T>)
        {
            if (value < 0)
            {
                *ptr++ = '-';
                unsigned_value = -value;
            }
            else
            {
                unsigned_value = value;
            }
        }
        else
        {
            unsigned_value = value;
        }

        if (radix == 10)
            return std::basic_string_view<char_type>{start, static_cast<size_t>(utoa(unsigned_value, ptr).size() + (ptr - start))};
        if (radix == 16)
            return std::basic_string_view<char_type>{start, static_cast<size_t>(utoa_hex(unsigned_value, ptr).size() + (ptr - start))};
        if (radix == 8)
            return std::basic_string_view<char_type>{start, static_cast<size_t>(utoa_oct(unsigned_value, ptr).size() + (ptr - start))};
        return std::basic_string_view<char_type>{};
    }

}