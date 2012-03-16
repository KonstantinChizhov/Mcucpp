#pragma once

#include <tiny_iomanip.h>

namespace IO
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
			:out(stream),
			_formatSrting(format)
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
