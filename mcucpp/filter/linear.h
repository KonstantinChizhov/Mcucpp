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
#include <stddef.h>
#include <template_utils.h>
#include <algorithm>
#include <numeric>
#include <iterator>

namespace Mcucpp
{
    namespace Filter
    {
        template<class T1, class T2>
        T1 Clip(T2 value)
        {
            if(Util::IsSigned<T2>::value && Util::MinValue<T1>::value > value)
                return Util::MinValue<T1>::value;
            
            if(!Util::IsSigned<T1>::value && (T2)Util::MinValue<T1>::value > value)
                return Util::MinValue<T1>::value;
                
            if(Util::MaxValue<T1>::value < value)
                return Util::MaxValue<T1>::value;
            
            return T1(value);
        }

        template<class CoeffT, size_t ATaps, size_t BTaps>
        class LinearFilter
        {
            std::array<CoeffT, ATaps> _a;
            std::array<CoeffT, BTaps> _b;
            std::array<CoeffT, BTaps < ATaps ? ATaps : BTaps > _z;
        public:

            void SetA(std::initializer_list<CoeffT> avalue)
            {
                std::copy_n(avalue.begin(), _a.size(), _a.begin());
            }
            void SetB(std::initializer_list<CoeffT> bvalue)
            {
                std::copy_n(bvalue.begin(), _b.size(), _b.begin());
            }
            void Reset()
            {
                std::fill(std::begin(_z), std::end(_z), CoeffT(0));
            }

            template<class InputIter, class OutputIter>
            void Filter(InputIter sx, InputIter ex, OutputIter fx)
            {
                OutputIter filter_x = fx;
                size_t maxLen = std::max(_a.size(), _b.size());
                for (InputIter px = sx; px != ex; ++px)
                {
                    auto x = *px;
                    auto new_x = _b[0] * x + _z[0];
                    for (size_t i = 1; i < maxLen; i++)
                    {
                        _z[i - 1] = (i >= _b.size() ? 0 : _b[i] * x)
                            + _z[i] 
                            - (i >= _a.size() ? 0 : _a[i] * new_x);
                    }
                    *filter_x = new_x;
                    ++filter_x;
                }
            }
        };

    }
}
