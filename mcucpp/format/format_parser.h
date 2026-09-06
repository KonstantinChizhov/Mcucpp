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
#include <algorithm>
#include <format/tiny_ios.h>

namespace Mcucpp
{

    template <class char_type = char>
    struct format_parser
    {
        ios_base::fmtflags flags = ios_base::none;
        int width = 0;
        int presc = -1;
        int arg_id = -1;
        char_type fill = ' ';

        using iterator = const char_type *;

        constexpr iterator parse_int(iterator begin, iterator end, int &out_value) const noexcept
        {
            int value = -1;
            while (begin < end && *begin >= '0' && *begin <= '9')
            {
                if (value < 0)
                    value = *begin - '0';
                else
                    value = value * 10 + *begin - '0';
                begin++;
            }
            if (value >= 0)
                out_value = value;
            return begin;
        }

        constexpr ios_base::fmtflags get_align(char_type c) const noexcept
        {
            switch (c)
            {
            case '<':
                return ios_base::left;
            case '>':
                return ios_base::right;
            case '^':
                return ios_base::center;
            default:
                return ios_base::none;
            }
        }

        constexpr iterator parse_align(iterator begin, iterator end) noexcept
        {
            if (end - begin >= 2)
                if (auto align = get_align(begin[1]))
                {
                    fill = *begin;
                    flags |= align;
                    return begin + 2;
                }

            if (auto align = get_align(begin[0]))
            {
                fill = ' ';
                flags |= align;
                return begin + 1;
            }
            return begin;
        }

        constexpr iterator parse_sign(iterator begin, iterator) noexcept
        {
            switch (*begin)
            {
            case '+':
                flags |= ios_base::showpos;
                [[fallthrough]];
            case '-':
                begin++;
                break;
            case ' ':
                flags |= ios_base::posspace;
                begin++;
                break;
            }
            return begin;
        }

        constexpr bool parse_spec(iterator begin, iterator end) noexcept
        {
            begin = parse_int(begin, end, arg_id);

            if (end - begin < 2)
                return true;

            if (*begin != ':') // has to be ':' or end
                return false;

            begin++;

            begin = parse_align(begin, end);
           
            if (begin >= end)
                return true;

            begin = parse_sign(begin, end);
            if (begin >= end)
                return true;

            if (*begin == '#')
            {
                flags |= ios_base::showbase;
                begin++;
            }

            if (begin >= end)
                return true;

            if (*begin == '0') // ignore
            {
                begin++;
            }
            if (begin >= end)
                return true;

            begin = parse_int(begin, end, width);

            if (begin >= end)
                return true;

            if (*begin == '.')
            {
                begin++;

                if (begin >= end)
                    return false;

                begin = parse_int(begin, end, presc);

                if (begin >= end)
                    return true;
            }

            parse_type(begin);

            return true;
        }

        constexpr void parse_type(const char_type *begin)
        {
            switch (*begin)
            {
            case 'X':
                flags |= ios_base::uppercase;
                [[fallthrough]];
            case 'x':
                flags |= ios_base::hex;
                break;
            case 'o':
                flags |= ios_base::oct;
                break;
            case 'd':
                flags |= ios_base::dec;
                break;
            case 'B':
                flags |= ios_base::uppercase;
                [[fallthrough]];
            case 'b':
                flags |= ios_base::bin | ios_base::boolalpha;
                break;
            case 'E':
                flags |= ios_base::uppercase;
                [[fallthrough]];
            case 'e':
                flags |= ios_base::scientific;
                break;
            case 'f':
                flags |= ios_base::fixed;
                break;
            }
        }

        static constexpr bool starts_with_escape_sequence(std::basic_string_view<char_type> fmt) noexcept
        {
            return (fmt[0] == '}' && fmt[1] == '}') || (fmt[0] == '{' && fmt[1] == '{');
        }

        static constexpr std::basic_string_view<char_type> get_raw_fragment(std::basic_string_view<char_type> fmt) noexcept
        {
            const char_type *frag_start = fmt.begin();
            const char_type *search_start = fmt.begin();

            if (fmt.size() >= 2 && starts_with_escape_sequence(fmt)) // escape sequence
            {
                frag_start++;
                search_start += 2;
            }

            const char_type *frag_end = std::find_if(search_start, fmt.end(), [](char_type ch)
                                                     { return ch == '{' || ch == '}'; });

            return {frag_start, frag_end};
        }

        static constexpr std::basic_string_view<char_type> get_format_spec(std::basic_string_view<char_type> fmt) noexcept
        {
            const char_type *spec_start = fmt.begin();

            if (fmt.size() < 2)
                return {fmt.end(), fmt.end()};

            if (starts_with_escape_sequence(fmt))
                return {spec_start, spec_start};

            const char_type *spec_end = std::find_if(spec_start + 1, fmt.end(), [](char_type ch)
                                                     { return ch == '{' || ch == '}'; });

            if (spec_end == fmt.end() || *spec_end == '{') // brases mismatch
                return {fmt.end(), fmt.end()};

            return {spec_start, spec_end + 1};
        }
    };
}