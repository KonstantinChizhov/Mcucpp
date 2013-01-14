#include <string_util.h>

namespace Mcucpp
{
    ios_base::fmtflags ios_base::flags ( ) const
    {return _flags;}

    ios_base::fmtflags ios_base::flags ( ios_base::fmtflags fmtfl )
    {
        fmtflags tmp = _flags;
        _flags = fmtfl;
        return tmp;
    }

    ios_base::fmtflags ios_base::setf ( ios_base::fmtflags fmtfl )
    {
        fmtflags tmp = _flags;
        _flags |= fmtfl;
        return tmp;
    }

    ios_base::fmtflags ios_base::setf ( ios_base::fmtflags fmtfl, ios_base::fmtflags mask )
    {
        fmtflags tmp = _flags;
        _flags = (_flags & ~mask) | fmtfl;
        return tmp;
    }

    ios_base::fmtflags ios_base::unsetf ( ios_base::fmtflags mask )
    {
        fmtflags tmp = _flags;
        _flags &= ~mask;
        return tmp;
    }

    streamsize_t ios_base::width ( ) const
    {
        return _width;
    }

    streamsize_t ios_base::width ( streamsize_t width )
    {
        streamsize_t tmp = _width;
        _width = width;
        return tmp;
    }

	streamsize_t ios_base::precision ( ) const
	{
		return _prec;
	}
	streamsize_t ios_base::precision ( streamsize_t prec )
	{
		streamsize_t tmp = _prec;
        _prec = prec;
        return tmp;
	}

	template<class char_type>
	bool basic_ios<char_type>::good () const
    {
        return _state == goodbit;
    }

    template<class char_type>
    bool basic_ios<char_type>::fail () const
    {
        return _state & failbit;
    }

    template<class char_type>
    bool basic_ios<char_type>::bad () const
    {
        return _state & badbit;
    }

    template<class char_type>
    bool basic_ios<char_type>::eof ( ) const
    {
        return (_state & eofbit) != 0;
    }

    template<class char_type>
    typename basic_ios<char_type>::iostate basic_ios<char_type>::rdstate ( ) const
    {
        return _state;
    }

    template<class char_type>
    void basic_ios<char_type>::setstate (typename basic_ios<char_type>::iostate state )
    {
        _state |= state;
    }

    template<class char_type>
    void basic_ios<char_type>::clear (typename basic_ios<char_type>::iostate state )
    {
        _state = state;
    }

    template<class char_type>
    char_type basic_ios<char_type>::fill ( ) const
    {
        return _fillch;
    }

    template<class char_type>
    char_type basic_ios<char_type>::fill ( char_type fillch )
    {
        uint8_t tmp = _fillch;
        _fillch = fillch;
        return tmp;
    }

    template<class char_type>
    basic_ios<char_type>::operator const void * () const
    {
        return _state & (badbit | failbit) ? 0 : this;
    }
}
