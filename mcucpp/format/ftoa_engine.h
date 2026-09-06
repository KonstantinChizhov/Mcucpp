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
#include <enum.h>

namespace Mcucpp
{
    // values matches ios_base::fmtflags
    enum class ftoa_flags
    {
        none = 0,
        fixed = 1 << 2,
        scientific = 1 << 8,
        showpoint = 1 << 10,
        showpos = 1 << 11,
        uppercase = 1 << 14,
        posspace = 1 << 17, // prepend ' ' to positve numbers
    };
    DECLARE_ENUM_OPERATIONS(ftoa_flags);

    // recomended conversion buffer size
    constexpr size_t FtoaBufferSize = 40;

    /// @brief Converts float value to string representation. Supports normal, fixed and scientific formats.
    /// @param value - float ot conver
    /// @param buffer - buffer to hold the result.
    /// @param buffer_end - end of conversion buffer. If result dies not fit, it is truncated.
    /// @param presc number of digits after decimal point
    /// @param flags - conversion options
    /// @return std::string_view pointing to the converted value in the @param buffer
    std::string_view ftoa_engine(float value, char *buffer, char *buffer_end, int presc, ftoa_flags flags);
}
