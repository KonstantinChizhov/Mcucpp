
#pragma once
#include "enum.h"
#include "template_utils.h"
#include <string.h>
#include <stdlib.h>
#include <tiny_ios.h>

namespace Mcucpp
{

	template<class InputPolicy,
				class char_type = char,
				class IOS=basic_ios<char_type>
				>
	class basic_istream : public IOS
	{
    public:
	    typedef typename IOS::trates trates;
		using IOS::eof;
		using IOS::good;
    protected:
	    char_type _lastChar;
        streamsize_t _lastReadCount;
        InputPolicy _src;

		char_type SkipWs(bool skip=true)
		{
			char_type c;
			if(IOS::flags() & IOS::skipws && skip)
			{
                for(c = get(); isspace(c) && good(); c = get());
                return c;
			}
			return get();
		}

		template<class T>
		basic_istream& ReadInteger(T &value)
		{
            value = 0;
			Reader reader(*this);

            if((IOS::flags() & IOS::hex))
            {
                if(reader.current == '0')
                {
                    reader++;
                    if(reader.current == 'x' || reader.current == 'X')
                        reader++;
                }
                value = StringToIntHex<T, Reader>(reader);
            }
            else if(IOS::flags() & IOS::oct)
            {
                value = StringToIntOct<T, Reader>(reader);
            }
            else //if(IOS::flags() & IOS::dec)
            {
                T sign = T(1);
                if(reader.current == '-')
                {
                    reader++;
                    sign = T(-1);
                }
                value = StringToIntDec<T, Reader>(reader) * sign;
            }
            return *this;
		}
		typedef typename trates::int_type int_type;

	 public:

		basic_istream()
		:_lastChar(0), _lastReadCount(0)
		{

		}

		streamsize_t gcount()const
		{
            return _lastReadCount;
		}

		streamsize_t readsome(char_type* s, streamsize_t n)
		{
			// TODO: review
			read(s, n);
			return n;
		}

		int_type peek()
		{
            return _lastChar = InputPolicy::get(*this);
		}

		basic_istream& ignore(streamsize_t n = 1, int delim = trates::eof)
		{
		    streamsize_t i;
		    for(i = 0; i < n && get() != delim; i++){}
		    _lastReadCount = i;
		    return *this;
		}

		basic_istream& putback(char_type c)
		{
            _lastChar = c;
            return *this;
		}

		int_type get()
		{
			// TODO: smarter buffering?
			if(_lastChar != 0)
            {
                char_type tmp = _lastChar;
                _lastChar = 0;
                return tmp;
            }
			return _src.get(*this);
		}

		basic_istream& get(char_type& c )
		{
		    c = get();
		    return *this;
		}

        basic_istream& get(char_type* s, streamsize_t n)
        {
            return get(s, n, '\n');
        }

        basic_istream& get(char_type* s, streamsize_t size, char_type delim)
        {
            streamsize_t i;
            char_type c = get();
			for(i = 0; !((c == delim) || (delim == ' ' && isspace(c)))
				&& i < size && good(); 
				c=get(), i++)
			{
			    *s++ = c;
			}
			putback(c);
			_lastReadCount = i;
			*s = 0;
		    return *this;
        }

		basic_istream& operator>> (bool &value)
		{
			char_type c = SkipWs();
			if(c == trates::DigitToLit(1))
			{
				value = true;
			}
			else if(c == trates::DigitToLit(0))
			{
				value = false;
			}
			else if((IOS::flags() & IOS::boolalpha) )
			{
				putback(c);
				char_type buffer[6];
				get(buffer, 5, ' ');
				if(StringMatch(buffer, trates::True()))
					value = true;
				if(StringMatch(buffer, trates::False()))
					value = false;
			}
			return *this;
		}

		basic_istream& operator>> (int &value)
		{
			return ReadInteger<int>(value);
		}

		basic_istream& operator>> (long &value)
		{
			return ReadInteger<long>(value);
		}

		basic_istream& operator>> (unsigned long &value)
		{
			return ReadInteger<unsigned long>(value);
		}

		basic_istream& operator>> (unsigned &value)
		{
			return ReadInteger<unsigned>(value);
		}

		basic_istream& operator>> (char_type* value)
		{
			for(char_type c = SkipWs(); !isspace(c) && good(); c = get())
			{
				*value++ = c;
			}
			*value = 0;
			return *this;
		}

		basic_istream& operator>> (char_type &value)
		{
			value = get();
			return *this;
		}

		basic_istream&
		operator>>(basic_istream& (*__pf)(basic_istream&))
		{
			return __pf(*this);
		}

		basic_istream&
		operator>>(ios_base& (*__pf) (ios_base&))
		{
			__pf(*this);
			return *this;
		}

		basic_istream& getline(char_type *str)
		{
            for(char_type c = SkipWs(); c!='\n' && c !='\r' && good(); c = get())
			{
				*str++ = c;
			}
			*str = 0;
			return *this;
		}

		basic_istream& read(char_type *str, streamsize_t size)
		{
		    streamsize_t i;
			for(i = 0; i < size && good(); i++)
			{
			    *str++ = get();
			}
			_lastReadCount = i;
		    return *this;
		}

		pos_type tellg()
		{
			return _src.tell(*this);
		}

		basic_istream& seekg(pos_type pos)
		{
			return seekg(pos, ios_base::beg);
		}

		basic_istream& seekg(off_type off, ios_base::seekdir dir)
		{
			_src.seek(*this, off, dir);
			sync();
			return *this;
		}

		int sync()
		{
			_lastChar = 0;
			return 0;
		}

    protected:
        struct Reader
        {
            basic_istream &_stream;
            char_type current, prev;

            Reader(basic_istream &stream)
            :_stream(stream), current( _stream.SkipWs()), prev(0)
            {}
            ~Reader()
            {
                _stream.putback(current);
            }

            char_type operator*()
            {
                return current;
            }
            char_type* operator++(int)
            {
                prev = current;
                current = _stream.get();
                return &prev;
            }
            char_type* operator++()
            {
                prev = current;
                current = _stream.get();
                return &current;
            }
        };
    };
}
