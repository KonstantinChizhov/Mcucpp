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

#ifndef TINY_IOS_H__
#define TINY_IOS_H__

#include <enum.h>
#include <char_trates.h>
#include <stddef.h>

namespace Mcucpp
{
	typedef unsigned streamsize_t;
	typedef int32_t pos_type;
	typedef int32_t off_type;

	class ios_base
	{
		ios_base(const ios_base&);
		ios_base& operator=(const ios_base&);
		public:

		ios_base()
		:_flags(fmtflags(right | skipws | dec)), _width(0), _prec(0)
		{}

		enum fmtflags
		{
			boolalpha   = 1 << 0,
			dec         = 1 << 1,
			fixed       = 1 << 2,
			hex         = 1 << 3,
			internal    = 1 << 4,
			left        = 1 << 5,
			oct         = 1 << 6,
			right       = 1 << 7,
			scientific  = 1 << 8,
			showbase    = 1 << 9,
			showpoint   = 1 << 10,
			showpos     = 1 << 11,
			skipws      = 1 << 12,
			unitbuf     = 1 << 13,
			uppercase   = 1 << 14,
			adjustfield = left | right | internal,
			basefield   = dec | oct | hex,
			floatfield  = scientific | fixed
		};

		enum iostate
		{
			goodbit = 0,
			badbit  = 1 << 0,
			eofbit  = 1 << 1,
			failbit = 1 << 2
		};

		enum seekdir
		{
			beg = 0,
			cur = 1,
			end = 2
		};

		enum openmode
		{
			app    = 1 << 0,
			ate    = 1 << 1,
			binary = 1 << 2,
			in     = 1 << 3,
			out    = 1 << 4,
			trunc  = 1 << 5
		};

		inline fmtflags flags ( ) const;
		inline fmtflags flags ( fmtflags fmtfl );
		inline fmtflags setf ( fmtflags fmtfl );
		inline fmtflags setf ( fmtflags fmtfl, fmtflags mask );
		inline fmtflags unsetf ( fmtflags mask );
		inline streamsize_t width ( ) const;
		inline streamsize_t width ( streamsize_t width );
		inline streamsize_t precision ( ) const;
		inline streamsize_t precision ( streamsize_t prec );

		private:
			fmtflags _flags;
			streamsize_t _width;
			streamsize_t _prec;
	};

	DECLARE_ENUM_OPERATIONS(ios_base::fmtflags)
	DECLARE_ENUM_OPERATIONS(ios_base::iostate)

	template<class char_type>
	class basic_ios :public ios_base
	{
	public:
        typedef char_trates<char_type> trates;

		basic_ios()
			:_state(goodbit), _fillch(' ')
		{}

		inline bool good () const;
		inline bool fail () const;
		inline bool bad () const;
		inline bool eof ( ) const;
		inline iostate rdstate ( ) const;
		inline void setstate ( iostate state );
		inline void clear ( iostate state = goodbit );
		inline char_type fill ( ) const;
		inline char_type fill ( char_type fillch );
        inline operator const void * () const;
	protected:
		iostate _state;
		char_type _fillch;
	};


	typedef basic_ios<char> ios;
	typedef basic_ios<wchar_t> wios;

    #define IO_DECLARE_STREAM_MANIPULATOR(NAME, FLAG, MASK) \
	inline ios_base& NAME (ios_base& os)\
	{\
		os.setf(FLAG, MASK);\
		return os;\
	}

	#define IO_DECLARE_STREAM_UNSET_MANIPULATOR(NAME, FLAG) \
	inline ios_base& NAME (ios_base& os)\
	{\
		os.unsetf(FLAG);\
		return os;\
	}

	IO_DECLARE_STREAM_MANIPULATOR(showbase, ios_base::showbase, ios_base::showbase)
	IO_DECLARE_STREAM_MANIPULATOR(boolalpha, ios_base::boolalpha, ios_base::boolalpha)
	IO_DECLARE_STREAM_MANIPULATOR(showpos, ios_base::showpos, ios_base::showpos)
	IO_DECLARE_STREAM_MANIPULATOR(oct, ios_base::oct, ios_base::basefield)
	IO_DECLARE_STREAM_MANIPULATOR(dec, ios_base::dec, ios_base::basefield)
	IO_DECLARE_STREAM_MANIPULATOR(hex, ios_base::hex, ios_base::basefield)
	IO_DECLARE_STREAM_MANIPULATOR(uppercase, ios_base::uppercase, ios_base::uppercase)
	IO_DECLARE_STREAM_MANIPULATOR(unitbuf, ios_base::unitbuf, ios_base::unitbuf)
	IO_DECLARE_STREAM_MANIPULATOR(scientific, ios_base::scientific, ios_base::floatfield)
	IO_DECLARE_STREAM_MANIPULATOR(fixed, ios_base::fixed, ios_base::floatfield)

	IO_DECLARE_STREAM_MANIPULATOR(skipws, ios_base::skipws, ios_base::skipws)

	IO_DECLARE_STREAM_MANIPULATOR(left, ios_base::left, ios_base::adjustfield)
	IO_DECLARE_STREAM_MANIPULATOR(right, ios_base::right, ios_base::adjustfield)
	IO_DECLARE_STREAM_MANIPULATOR(internal, ios_base::internal, ios_base::adjustfield)


	IO_DECLARE_STREAM_UNSET_MANIPULATOR(noshowbase, ios_base::showbase)
	IO_DECLARE_STREAM_UNSET_MANIPULATOR(noboolalpha, ios_base::boolalpha)
	IO_DECLARE_STREAM_UNSET_MANIPULATOR(noshowpos, ios_base::showpos)
	IO_DECLARE_STREAM_UNSET_MANIPULATOR(nouppercase, ios_base::uppercase)
	IO_DECLARE_STREAM_UNSET_MANIPULATOR(nounitbuf, ios_base::unitbuf)
	IO_DECLARE_STREAM_UNSET_MANIPULATOR(noskipws, ios_base::skipws)
}


#include <impl/tiny_ios.h>
#endif
