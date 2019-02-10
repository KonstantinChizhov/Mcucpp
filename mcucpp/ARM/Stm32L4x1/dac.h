
//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2018
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
#include <mcu_header.h>
#include <clock.h>
#include <iopins.h>
#include <pinlist.h>

namespace Mcucpp
{
typedef IO::Pa4 Dac1Out1;
typedef IO::Pa5 Dac1Out2;

class Dac1_1
{
  public:
    typedef uint16_t DataT;
    static constexpr DataT MaxValue = 4095;

    static inline void Init()
    {
        Clock::Dac1Clock::Enable();
        Dac1Out1::SetConfiguration(Dac1Out1::Port::Analog);
        DAC->CR |= DAC_CR_EN1;
    }

    static inline void Disable()
    {
        DAC->CR &= ~DAC_CR_EN1;
    }

    static inline void Write(DataT value)
    {
        DAC->DHR12R1 = value;
    }

    static inline DataT Read()
    {
        return DAC->DHR12R1;
    }
};

class Dac1_2
{
  public:
    typedef uint16_t DataT;
    static constexpr DataT MaxValue = 4096;

    static inline void Init()
    {
        Clock::Dac1Clock::Enable();
        Dac1Out2::SetConfiguration(Dac1Out1::Port::Analog);
        DAC->CR |= DAC_CR_EN2;
    }

    static inline void Disable()
    {
        DAC->CR &= ~DAC_CR_EN2;
    }

    static inline void Write(DataT value)
    {
        DAC->DHR12R2 = value;
    }

     static inline DataT Read()
    {
        return DAC->DHR12R2;
    }
};

} // namespace Mcucpp
