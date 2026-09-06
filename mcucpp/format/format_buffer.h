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
#include <array>

namespace Mcucpp
{

    template <class char_type = char>
    struct format_buffer
    {
        char_type *_buffer;
        size_t _size;
        size_t _pos;

        format_buffer(char_type *buffer, size_t size)
            : _buffer(buffer), _size(size)
        {
            reset();
        }

        format_buffer(const format_buffer &) = delete;
        format_buffer(format_buffer &&) = delete;
        format_buffer &operator=(const format_buffer &) = delete;
        format_buffer &operator=(format_buffer &&) = delete;

        void reset()
        {
            _pos = 0;
            std::fill_n(_buffer, _size, 0);
        }

        void write(const char_type *begin, const char_type *end)
        {
            size_t avail = available();
            size_t value_size = end - begin;
            if (avail < value_size)
            {
                end = begin + avail;
                value_size = avail;
            }
            std::copy(begin, end, _buffer + _pos);
            _pos += value_size;
        }

        void fill_n(char_type fill_ch, size_t count)
        {
            size_t avail = available();
            if (avail < count)
            {
                count = avail;
            }
            std::fill_n(_buffer + _pos, count, fill_ch);
            _pos += count;
        }

        size_t available() const noexcept { return _size - _pos; }
        char_type *data() noexcept { return _buffer; }
        size_t size() const noexcept { return _size; }

        std::basic_string_view<char_type> str() const noexcept { return {_buffer, _pos}; }
    };

    template <size_t Size, class char_type = char>
    class fixed_format_buffer
    {
        std::array<char_type, Size> _buffer;
        size_t _pos = 0;

    public:
        fixed_format_buffer()
        {
            reset();
        }

        fixed_format_buffer(const fixed_format_buffer &) = delete;
        fixed_format_buffer(fixed_format_buffer &&) = delete;
        fixed_format_buffer &operator=(const fixed_format_buffer &) = delete;
        fixed_format_buffer &operator=(fixed_format_buffer &&) = delete;

        void reset()
        {
            _pos = 0;
            std::fill(_buffer.begin(), _buffer.end(), 0);
        }

        void write(const char_type *begin, const char_type *end)
        {
            size_t avail = available();
            size_t value_size = end - begin;
            if (avail < value_size)
            {
                end = begin + avail;
                value_size = avail;
            }
            std::copy(begin, end, _buffer.begin() + _pos);
            _pos += value_size;
        }

        void fill_n(char_type fill_ch, size_t count)
        {
            size_t avail = available();
            if (avail < count)
            {
                count = avail;
            }
            std::fill_n(_buffer.begin() + _pos, count, fill_ch);
            _pos += count;
        }

        size_t available() const noexcept { return _buffer.size() - _pos; }
        char_type *data() noexcept { return _buffer.data(); }
        size_t size() const noexcept { return _buffer.size(); }

        std::basic_string_view<char_type> str() const noexcept { return {_buffer.begin(), _pos}; }
    };
}