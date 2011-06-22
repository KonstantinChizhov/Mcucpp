
#pragma once
#include "enum.h"
#include "template_utils.h"
#include <string.h>
#include <stdlib.h>

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
	};

	template<class T, class CharT>
	CharT * IntToString(T value, CharT *bufferEnd, unsigned radix)
	{
		CharT *ptr = bufferEnd;
		do
		{
            *--ptr = CharTrates<CharT>::DigitToLit(value % radix);
            value /= radix;
		}
		while (value != 0);
		return ptr;
	}


	class ios_base
	{
		ios_base(const ios_base&);
		ios_base& operator=(const ios_base&);
		public:

		ios_base()
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

	};

	DECLARE_ENUM_OPERATIONS(ios_base::fmtflags)
	DECLARE_ENUM_OPERATIONS(ios_base::iostate)

	class ios :public ios_base
	{
	public:
		ios()
			:_flags(dec)
		{

		}

		bool good () const
		{
			return _state == goodbit;
		}

		bool fail () const
		{
			return _state & failbit;
		}
		bool bad () const
		{
			return _state & badbit;
		}
		bool eof ( ) const
		{
			return _state & eofbit;
		}
		iostate rdstate ( ) const
		{
			return _state;
		}

		void setstate ( iostate state )
		{
			_state |= state;
		}

		void clear ( iostate state = goodbit )
		{
			_state = state;
		}

		fmtflags flags ( ) const
		{return _flags;}

		fmtflags flags ( fmtflags fmtfl )
		{
			fmtflags tmp = _flags;
			_flags = fmtfl;
			return tmp;
		}

		fmtflags setf ( fmtflags fmtfl )
		{
			fmtflags tmp = _flags;
			_flags |= fmtfl;
			return tmp;
		}

		fmtflags setf ( fmtflags fmtfl, fmtflags mask )
		{
			fmtflags tmp = _flags;
			_flags = (_flags & ~mask) | fmtfl;
			return tmp;
		}

		fmtflags unsetf ( fmtflags mask )
		{
			fmtflags tmp = _flags;
			_flags &= ~mask;
			return tmp;
		}

		uint8_t width ( ) const
		{
			//return _width;
			return 0;
		}
		uint8_t width ( uint8_t width )
		{
			//uint8_t tmp = _width;
			//_width = width;
			//return tmp;
			return 0;
		}

		char fill ( ) const
		{
			return ' ';
		}
		char fill ( char fillch )
		{
			return ' ';
		}
	protected:
		fmtflags _flags;
		iostate _state;
		//uint8_t _width;
	};


	template<class OutputPolicy, class IOS = ios, class CharT = char>
	class FormatWriter :public OutputPolicy, public IOS
	{
	private:
		const CharT *_formatSrting;
	public:
		typedef FormatWriter Self;
	protected:

		template<class T>
		struct ConvertBufferSize
		{
			static const int value = sizeof(T) * 3;
		};

		unsigned Base()
		{
			if(IOS::flags() & ios::hex) return 16;
			if(IOS::flags() & ios::oct) return 8;
			return 10;
		}


		template<class T>
		void PutInteger(T value)
		{
			const int bufferSize = ConvertBufferSize<int>::value;
			CharT buffer[bufferSize];
			bool sign = false;

			if(Util::IsSigned<T>::value)
			{
				if(value < 0 && IOS::flags() & ios::dec)
				{
					value = -value;
					sign = true;
				}
			}

			typedef typename Util::Unsigned<T>::Result UT;
			UT uvalue = static_cast<UT>(value);
			CharT * str = IntToString(uvalue, buffer + bufferSize, Base());

			if(sign)
				*--str = CharTrates<CharT>::Minus();

			if((IOS::flags() & (ios::hex | ios::showbase)) == (ios::hex | ios::showbase))
			{
				*--str = 'x';
				*--str = '0';
			}

			OutputPolicy::write(str, buffer + bufferSize - str);
		}

		void PutBool(bool value)
		{
			if(IOS::flags() & ios::boolalpha)
			{
				if(value)
					OutputPolicy::write("true", 4);
				else
					OutputPolicy::write("false", 5);
			}
			else
			{
				if(value)
					OutputPolicy::put('1');
				else
					OutputPolicy::put('0');
			}
		}

		void ProcessFormat()
		{
			if(_formatSrting)
			{
				const CharT *ptr = _formatSrting;
				while(*ptr != '%' && *ptr != '\0')
				{
					ptr++;
				}
				OutputPolicy::write(_formatSrting, ptr - _formatSrting);
				if(*ptr == '%') ptr++;
				if(*ptr == '\0')
					_formatSrting = 0;
				else
					_formatSrting = ptr;
			}
		}
	public:

		FormatWriter()
		{
			_formatSrting = 0;
		}

		Self& Format(const CharT * format)
		{
			_formatSrting = format;
			ProcessFormat();
			return *this;
		}

		Self& operator% (bool value)
		{
			PutBool(value);
			ProcessFormat();
			return *this;
		}

		Self& operator% (int value)
		{
			PutInteger(value);
			ProcessFormat();
			return *this;
		}

		Self& operator% (long value)
		{
			PutInteger(value);
			ProcessFormat();
			return *this;
		}

		Self& operator% (unsigned long value)
		{
			PutInteger(value);
			ProcessFormat();
			return *this;
		}

		Self& operator% (unsigned value)
		{
			PutInteger(value);
			ProcessFormat();
			return *this;
		}

		/*template<class CharPtr>
		Self& operator% (CharPtr value)
		{
			puts(value);
			ProcessFormat();
			return *this;
		}*/

		Self& operator<< (int value)
		{
			PutInteger(value);
			return *this;
		}

		Self& operator<< (long value)
		{
			PutInteger(value);
			return *this;
		}

		Self& operator<< (unsigned long value)
		{
			PutInteger(value);
			return *this;
		}

		Self& operator<< (unsigned value)
		{
			PutInteger(value);
			return *this;
		}

		Self& operator<< (const CharT* value)
		{
			puts(value);
			return *this;
		}

		Self&
		operator<<(Self& (*__pf)(Self&))
		{
			return __pf(*this);
		}

		Self&
		operator<<(ios& (*__pf) (ios&))
		{
			__pf(*this);
			return *this;
		}

		void puts(const CharT *str)
		{
			OutputPolicy::write(str, strlen(str) );
		}

		/*template<class CharPtr>
		void puts(CharPtr str)
		{
			CharT c;
			while(c = *str++)
			{
				OutputPolicy::put(c);
			}
		}*/
	};

	template<class OutputPolicy, class IOS, class CharT>
	FormatWriter<OutputPolicy, IOS, CharT>& endl ( FormatWriter<OutputPolicy, IOS, CharT>& os)
	{
		os.put('\n');
		return os;
	}

	template<class OutputPolicy, class IOS, class CharT>
	FormatWriter<OutputPolicy, IOS, CharT>& ends ( FormatWriter<OutputPolicy, IOS, CharT>& os)
	{
		os.put('\0');
		return os;
	}

	template<class OutputPolicy, class IOS, class CharT>
	FormatWriter<OutputPolicy, IOS, CharT>& dec ( FormatWriter<OutputPolicy, IOS, CharT>& os)
	{
		os.setf(ios::dec, ios::basefield);
		return os;
	}

	template<class OutputPolicy, class IOS, class CharT>
	FormatWriter<OutputPolicy, IOS, CharT>& hex ( FormatWriter<OutputPolicy, IOS, CharT>& os)
	{
		os.setf(ios::hex, ios::basefield);
		return os;
	}

	template<class OutputPolicy, class IOS, class CharT>
	FormatWriter<OutputPolicy, IOS, CharT>& oct ( FormatWriter<OutputPolicy, IOS, CharT>& os)
	{
		os.setf(ios::oct, ios::basefield);
		return os;
	}
}
