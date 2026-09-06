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

#include <format/int_to_str.h>

namespace Mcucpp
{

    static inline uint32_t mulhsu_r(uint32_t u, uint32_t v)
    {
        uint64_t prod = (uint64_t)u * v;
        prod += u >> 2; // additional rounding
        return prod >> 32;
    }

    static inline uint64_t mulhsu_r(uint64_t u, uint64_t v)
    {
#ifdef __SIZEOF_INT128__
        __uint128_t prod = (__uint128_t)u * v;
        prod += u >> 1; // additional rounding
        return prod >> 64;
#else
        uint64_t u0, v0, w0, t, w1;
        uint32_t u1, v1, w2;
        u0 = (uint32_t)u;
        u1 = (uint32_t)(u >> 32);
        v0 = (uint32_t)v;
        v1 = (uint32_t)(v >> 32);
        w0 = (uint64_t)u0 * v0;
        t = (uint64_t)u1 * v0 + (w0 >> 32);
        t += (u >> 33); // additional rounding
        w1 = (uint32_t)t;
        w2 = t >> 32;
        w1 = u0 * v1 + w1;
        return (uint64_t)u1 * v1 + w2 + (w1 >> 32);
#endif
    }

    // Implementation details https://konstantinchizhov.github.io/2025/08/08/utoa.html

    std::string_view utoa(uint32_t value, char *ptr)
    {
        constexpr uint32_t max_pow10 = 100'000'000ul;
        constexpr uint32_t extract_bit_pos = 27;
        constexpr uint32_t magic_constant = ((1ull << (32 + extract_bit_pos)) / max_pow10) - (1ull << 32);
        constexpr uint32_t overflow_limit = (1ull << 32) * max_pow10 / (1ull << extract_bit_pos);
        constexpr uint32_t extract_mask = ((1ull << 32) - 1) >> (32 - extract_bit_pos);

        char *start = ptr;

        if (value == 0)
        {
            ptr[0] = '0';
            return std::string_view{start, 1};
        }

        char *end = ptr + 9;
        // convert int to fraction

        uint32_t t = value + mulhsu_r(value, magic_constant) + 1;

        uint8_t digit = uint8_t(t >> extract_bit_pos);

        bool overflow = value >= overflow_limit;
        // handle overfloaw
        if (overflow || digit > 9)
        {
            ptr[0] = overflow ? '3' : '0';
            ptr[1] = (overflow ? '2' : '0') + digit;
            while (ptr[1] > '9')
            {
                ptr[1] -= 10;
                ptr[0]++;
            }
            end++;
            ptr += 2;
        }
        else
        {
            // skip leading zeros
            while ((t & 0xfffE'0000) == 0)
            {
                t *= 10000;
                end -= 4;
            }

            while ((t & ~extract_mask) == 0)
            {
                t *= 10;
                end--;
            }

            digit = uint8_t(t >> extract_bit_pos);
            *ptr++ = digit + '0';
        }

        // digit extraction
        while (ptr < end)
        {
            t = (t & extract_mask) * 10;
            digit = uint8_t(t >> extract_bit_pos);
            *ptr++ = digit + '0';
        }

        return std::string_view{start, static_cast<size_t>(ptr - start)};
    }

    // Implementation details https://konstantinchizhov.github.io/2025/08/08/utoa.html

    std::string_view utoa(uint64_t value, char *ptr)
    {
        //constexpr uint64_t max_pow10 = 1'000'000'000'000'000'000ull;
        constexpr int extract_bit_pos = 60;
        constexpr uint64_t magic_constant = 0x2725dd1d243aba0eull; // (((__uint128_t(1ull) << (64 + extract_bit_pos)) / max_pow10) - (__uint128_t(1ull) << 64));
        constexpr uint64_t overflow_limit = 0xde0b6b3a76400000ull; //(__uint128_t(1ull) << 64) * max_pow10 / (1ull << extract_bit_pos);
        constexpr uint64_t extract_mask = 0x0fffffffffffffff;      // ((__uint128_t(1ull) << 64) - 1) >> (64 - extract_bit_pos);

        char *start = ptr;

        if (value == 0)
        {
            ptr[0] = '0';
            return std::string_view{start, 1};
        }

        // convert int to fraction
        uint64_t t = value + mulhsu_r(value, magic_constant) + 1;

        uint8_t digit = uint8_t(t >> extract_bit_pos);
        // handle overfloaw
        bool overflow = value >= overflow_limit;
        char *end = ptr + 19;

        if (overflow || digit > 9)
        {
            ptr[0] = overflow ? '1' : '0';
            ptr[1] = (overflow ? '6' : '0') + digit;
            while (ptr[1] > '9')
            {
                ptr[1] -= 10;
                ptr[0]++;
            }
            end++;
            ptr += 2;
        }
        else
        {
            // skip leading zeros
            while ((t & 0xfffc'0000'0000'0000) == 0)
            {
                t *= 10000;
                end -= 4;
            }

            while ((t & ~extract_mask) == 0)
            {
                t *= 10;
                end--;
            }
            digit = uint8_t(t >> extract_bit_pos);
            *ptr++ = digit + '0';
        }

        // digit extraction
        while (ptr < end)
        {
            t &= extract_mask;
            t = (t << 1) + (t << 3);
            digit = uint8_t(t >> extract_bit_pos);
            *ptr++ = digit + '0';
        }

        return std::string_view{start, static_cast<size_t>(ptr - start)};
    }

}