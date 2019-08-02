
//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
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

#if defined (__GNUC__) && defined(__arm__)
	#define MCUCPP_INTERRUPT(ISR_NAME) __attribute__(( __interrupt__)) void ISR_NAME()
#endif

#if __cplusplus <= 199711L // check for c++11
	#define nullptr 0
#endif

#if defined (__GNUC__)
	#define MCUCPP_WEAK __attribute__((weak))
	#define MCUCPP_FORCEINLINE __attribute__((always_inline))
#else
	#define MCUCPP_WEAK
	#define MCUCPP_FORCEINLINE
#endif

#if __cplusplus <= 199711L || (defined(__AVR__) && !__has_include("type_traits")) // check for c++11 of avr-gcc compiler

namespace std
{

    template< class T > struct remove_const          { typedef T type; };
    template< class T > struct remove_const<const T> { typedef T type; };

    template< class T > struct remove_volatile             { typedef T type; };
    template< class T > struct remove_volatile<volatile T> { typedef T type; };

    template< class T >
    struct remove_cv {
        typedef typename std::remove_volatile<typename std::remove_const<T>::type>::type type;
    };
}

#else
    #include <type_traits>
#endif


#if defined(__AVR__)
#include <stdlib.h>
    namespace std
    {
    #if __cplusplus <= 199711L 
        inline void terminate()
    #else
        [[noreturn]] inline void terminate() noexcept
    #endif
        {
            abort();
        }
    }
#endif
