
namespace IO
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

    uint8_t ios_base::width ( ) const
    {
        return _width;
    }

    uint8_t ios_base::width ( uint8_t width )
    {
        uint8_t tmp = _width;
        _width = width;
        return tmp;
    }

	uint8_t ios_base::precision ( ) const
	{
		return _prec;
	}
	uint8_t ios_base::precision ( uint8_t prec )
	{
		uint8_t tmp = _prec;
        _prec = prec;
        return tmp;
	}

	template<class CharT>
	bool basic_ios<CharT>::good () const
    {
        return _state == goodbit;
    }

    template<class CharT>
    bool basic_ios<CharT>::fail () const
    {
        return _state & failbit;
    }

    template<class CharT>
    bool basic_ios<CharT>::bad () const
    {
        return _state & badbit;
    }

    template<class CharT>
    bool basic_ios<CharT>::eof ( ) const
    {
        return _state & eofbit;
    }

    template<class CharT>
    typename basic_ios<CharT>::iostate basic_ios<CharT>::rdstate ( ) const
    {
        return _state;
    }

    template<class CharT>
    void basic_ios<CharT>::setstate (typename basic_ios<CharT>::iostate state )
    {
        _state |= state;
    }

    template<class CharT>
    void basic_ios<CharT>::clear (typename basic_ios<CharT>::iostate state )
    {
        _state = state;
    }

    template<class CharT>
    CharT basic_ios<CharT>::fill ( ) const
    {
        return _fillch;
    }

    template<class CharT>
    CharT basic_ios<CharT>::fill ( CharT fillch )
    {
        uint8_t tmp = _fillch;
        _fillch = fillch;
        return tmp;
    }
}
