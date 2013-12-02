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

namespace Mcucpp
{
	template<class char_type>
	class char_trates;

	template<>
	class char_trates<char>
	{
		public:
		static char DigitToLit(unsigned digit)
		{
			return (digit < 10 ? '0' : 'a' - 10) + digit;
		}

		static char Minus()
		{
			return '-';
		}
		static char Plus()
		{
			return '+';
		}

		static char DecimalDot()
		{
			return '.';
		}
		
		static char Exp()
		{
			return 'e';
		}
		static int StrLen(const char *str)
		{
			const char *ptr = str;
			while(*ptr++);
			return ptr - str - 1;
		}

		static const char * False(){return "false";}
		static const char * True(){return "true";}

		typedef int int_type;
		static const int_type eof = -1;
		static const char Null = 0;
	};

	template<>
	class char_trates<wchar_t>
	{
		public:
		static wchar_t DigitToLit(unsigned digit)
		{
			return (digit < 10 ? L'0' : L'a' - 10) + digit;
		}

		static wchar_t Minus()
		{
			return L'-';
		}

		static wchar_t Plus()
		{
			return L'+';
		}
		static wchar_t DecimalDot()
		{
			return L'.';
		}
		static char Exp()
		{
			return L'e';
		}
		static int StrLen(const wchar_t *str)
		{
			const wchar_t *ptr = str;
			while(*ptr++);
			return ptr - str - 1;
		}

		static const wchar_t * False(){return L"false";}
		static const wchar_t * True(){return L"true";}

		typedef int int_type;
		static const int_type eof = -1;
		static const wchar_t Null = 0;
	};
}
