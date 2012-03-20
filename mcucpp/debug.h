//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2012
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

#ifndef MCUCPP_DEBUG_H
#define MCUCPP_DEBUG_H


#if defined(DEBUG_INTERFACE)
#include <tiny_ostream.h>
namespace Mcucpp
{
	typedef basic_ostream<DEBUG_INTERFACE> DebugStream
	extern DebugStream debugOut;

	class SystemDebug
	{
		SystemDebug();
	public:
		static void Assert(bool condition, const char *message)
		{
			if(!condition)
			{
				debugOut << "Assertion failed: " << message << "\n";
				assert(false);
			}
		}
		static DebugStream& Out(){return debugOut;}
	};
}

#else
// include platform dependent header
#include <_debug.h>
#endif

namespace Mcucpp
{
	class NullStream
	{
	public:
		template<class T> NullStream& operator<<(T &value){return *this;}
	};
}

namespace Mcucpp
{
	class NullDebug
	{
		NullDebug();
	public:
		static void Assert(bool condition, const char *message){}
		static NullStream Out(){return NullStream();}
	};

	#if defined(DEBUG)
		typedef Mcucpp::SystemDebug Debug;
	#else
		typedef Mcucpp::NullDebug Debug;
	#endif

#ifndef CONCAT
	#define CONCAT2(First, Second) (First ## Second)
	#define CONCAT(First, Second) CONCAT2(First, Second)
#endif

#ifndef TO_STR
	#define TO_STR2(ARG) #ARG
	#define TO_STR(ARG) TO_STR2(ARG)
#endif

#if defined(DEBUG)
	#define MCUCPP_ASSERT(COND) Debug::Assert(COND, TO_STR(__FILE__) ":"  TO_STR(__LINE__) ":" TO_STR(COND))
#else
	#define MCUCPP_ASSERT(COND) 
#endif

}
#endif
