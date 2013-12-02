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

#ifndef MCUCPP_STATIC_ASSERT
#define MCUCPP_STATIC_ASSERT

#ifndef CONCAT 
#define CONCAT2(First, Second) (First ## Second)
#define CONCAT(First, Second) CONCAT2(First, Second)
#endif 

#ifdef __cplusplus

	#if __cplusplus > 199711L // check for c++11
		#define STATIC_ASSERT(expr) static_assert((expr), "Static assertion failed")
	#else
	namespace Mcucpp
	{
		template<bool> struct StaticAssertionFailed;
		template<> struct StaticAssertionFailed<true> {};
		template<int> struct StaticAssertionTest{};
	}
		#define STATIC_ASSERT(expr) typedef ::Mcucpp::StaticAssertionTest<sizeof(::Mcucpp::StaticAssertionFailed<(expr)>)> (CONCAT(static_assert_failed_at_line_, __LINE__))
	#endif

#else
	#define STATIC_ASSERT(expr) typedef char CONCAT(static_assert_failed_at_line_, __LINE__) [(expr) ? 1 : -1]
#endif

#endif
