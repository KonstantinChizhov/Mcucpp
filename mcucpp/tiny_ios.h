#ifndef TINY_IOS_H__
#define TINY_IOS_H__

#include <string.h>
#include <enum.h>

namespace Mcucpp
{
namespace IO
{
    template<class CharT>
	class CharTrates;

	template<>
	class CharTrates<char>
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
		static int SrtLen(const char *str){return strlen(str);}

		static const char * False(){return "false";}
		static const char * True(){return "true";}
	};

	template<>
	class CharTrates<wchar_t>
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
			return '+';
		}
		static int SrtLen(const wchar_t *str)
		{
		    const wchar_t *ptr = str;
		    while(*ptr++);
		    return ptr - str - 1;
		}

		static const wchar_t * False(){return L"false";}
		static const wchar_t * True(){return L"true";}
	};

#if (defined(__ICCAVR__) || defined(__AVR__) || defined(__AVR) || defined(AVR)) && !defined(TINY_IOS_LARGE_STREAM_SIZE)
	typedef unsigned char streamsize_t;
#else
	typedef unsigned streamsize_t;
#endif

	class ios_base
	{
		ios_base(const ios_base&);
		ios_base& operator=(const ios_base&);
		public:

		ios_base()
		:_flags(right), _width(0), _prec(0)
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

    template<class CharT>
	class basic_ios :public ios_base
	{
	public:
		basic_ios()
			:_fillch(' ')
		{}

		inline bool good () const;
		inline bool fail () const;
		inline bool bad () const;
		inline bool eof ( ) const;
		inline iostate rdstate ( ) const;
		inline void setstate ( iostate state );
		inline void clear ( iostate state = goodbit );
		inline CharT fill ( ) const;
		inline CharT fill ( CharT fillch );

	protected:
		iostate _state;
		CharT _fillch;
	};


	typedef basic_ios<char> ios;
	typedef basic_ios<wchar_t> wios;
}
}
#include <impl/tiny_ios.h>
#endif
