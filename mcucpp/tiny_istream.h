
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
	class basic_istream :public InputPolicy, public IOS
	{
    public:
	    typedef typename IOS::trates trates;
		using IOS::eof;
		using IOS::good;
    protected:
	    char_type _lastChar;
        streamsize_t _lastReadCount;

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

            if(reader.current == '0')
            {
                reader++;

                if((reader.current == 'x' || reader.current == 'X') || (IOS::flags() & IOS::hex))
                {
                    reader++;
                    value = StringToIntHex<T, Reader>(reader);
                }
                else if(isoctdigit(reader.current))
                {
                    value = StringToIntOct<T, Reader>(reader);
                }
                else
                {
                    return *this;
                }
            }
			else
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
			if(_lastChar != 0)
            {
                char_type tmp = _lastChar;
                _lastChar = 0;
                return tmp;
            }
			return InputPolicy::get(*this);
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
			for(i = 0; c != delim && i < size && good(); c=get(), i++)
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

		basic_istream& operator>> (unsigned value)
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

    protected:
        struct Reader
        {
            basic_istream &_stream;
            char_type current;

            Reader(basic_istream &stream)
            :_stream(stream), current( _stream.SkipWs())
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
                current = _stream.get();
                return &current;
            }
        };
	};
}
