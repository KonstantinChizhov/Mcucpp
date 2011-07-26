#pragma once
namespace IO
{
	template<class Stream, class FormatStrPtrType = char *>
	class FormatParser
	{
		static const bool ScanFloatPrecision = false;
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
			:out(stream)
		{
			_formatSrting = format;
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
				ClearFmt();
				ProcessFormat();
			}
			return *this;
		}
	};

	template<class FormatStr>
	struct FormatT
	{
		FormatStr FormatSrting;

		FormatT(FormatStr formatSrting)
			:FormatSrting(formatSrting)
		{}
	};

	template<class FormatStr>
	FormatT<FormatStr> Format(FormatStr formatStr)
	{
		return FormatT<FormatStr>(formatStr);
	}

	template<class Stream, class FormatStr>
	FormatParser<Stream, FormatStr> operator% (Stream &stream, FormatT<FormatStr> format)
	{
		return FormatParser<Stream, FormatStr>(stream, format.FormatSrting);
	}
}

#include <impl/format_parser.h>
