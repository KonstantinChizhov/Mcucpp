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

#include <tiny_iomanip.h>

namespace Mcucpp
{

	enum FormatMode
	{
		FmMinimal,
		FmNormal,
		FmFull
	};

	template<class Stream, FormatMode Mode = FmNormal, class FormatStrPtrType = char *>
	class FormatParser
	{
		static const bool ScanFloatPrecision = Mode == FmFull;
		static const bool ScanFieldWidth = (Mode == FmNormal || Mode == FmFull);
		static const bool ScanFlags = (Mode == FmNormal || Mode == FmFull);

		FormatStrPtrType _formatSrting;
		typedef FormatParser Self;
	public:
		Stream & out;
	private:
		void inline ClearFmt()
		{
			out.setf(Stream::right | Stream::dec, Stream::unitbuf | Stream::showpos |
											Stream::boolalpha | Stream::adjustfield |
											Stream::basefield | Stream::floatfield |
											Stream::skipws | Stream::showbase |
											Stream::showpoint | Stream::uppercase);
		}

		inline void ProcessFormat();

	public:
		FormatParser(Stream &stream, FormatStrPtrType format)
			:_formatSrting(format),
			out(stream)
		{
			if(_formatSrting)
				ProcessFormat();
		}

		Self&
		operator% (Stream& (*__pf)(Stream&))
		{
			__pf(out);
			return *this;
		}

		Self&
		operator% (SetwT f)
		{
			out.width(f.width);
			return *this;
		}

		template<class T>
		Self& operator % (T value)
		{
			if(_formatSrting)
			{
				out << value;
				ProcessFormat();
			}
			return *this;
		}
	};

	template< FormatMode Mode, class FormatStr>
	struct FormatT
	{
		FormatStr FormatSrting;

		FormatT(FormatStr formatSrting)
			:FormatSrting(formatSrting)
		{}
	};

	template<FormatMode Mode, class FormatStr>
	FormatT<Mode, FormatStr> Format(FormatStr formatStr)
	{
		return FormatT<Mode, FormatStr>(formatStr);
	}

	template<class FormatStr>
	FormatT<FmNormal, FormatStr> Format(FormatStr formatStr)
	{
		return FormatT<FmNormal, FormatStr>(formatStr);
	}

	template<class Stream, FormatMode Mode, class FormatStr>
	FormatParser<Stream, Mode, FormatStr> operator% (Stream &stream, FormatT<Mode, FormatStr> format)
	{
		return FormatParser<Stream, Mode, FormatStr>(stream, format.FormatSrting);
	}
}
#include <impl/format_parser.h>
