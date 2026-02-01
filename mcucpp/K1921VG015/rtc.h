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
#include <stddef.h>
#include <chrono>

// MCU specific clock
#include <clock.h>

namespace Mcucpp
{
    struct rtc_clock
    {
        typedef std::chrono::duration<uint32_t> duration;
        typedef duration::rep rep;
        typedef duration::period period;
        typedef std::chrono::time_point<rtc_clock, duration> time_point;

        static constexpr bool is_steady = false;

        static time_point now() noexcept;

        static void set_time(time_point time)noexcept;

        // Map to C API
        static std::time_t to_time_t(const time_point &__t) noexcept
        {
            return std::time_t(duration_cast<std::chrono::seconds>(__t.time_since_epoch()).count());
        }

        static time_point from_time_t(std::time_t __t) noexcept
        {
            typedef std::chrono::time_point<rtc_clock, duration> __from;
            return time_point_cast<rtc_clock::duration>(__from(std::chrono::seconds(__t)));
        }
    };
}
