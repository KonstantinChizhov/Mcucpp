#include <string>
#include "tiny_istream.h"
#include <gtest.h>

using namespace Mcucpp;

template<class CharT>
class StringReader :public basic_ios<CharT>
{

public:
	void SetSource(const CharT* str)
	{
		_str = str;
		pos = 0;
	}

	void Reset()
	{
		pos = 0;
	}

	CharT get()
	{
		CharT c = _str[pos];
		if(c)
			pos++;
		else
			basic_ios<CharT>::setstate(ios_base::eofbit);
		return c;
	}


private:
	int pos;
	const CharT* _str;
};

template<class CharT=char>
class basic_istringstream :public basic_istream<StringReader<CharT>, CharT>
{
public:
	basic_istringstream(const CharT *str)
	{
		StringReader<CharT>::SetSource(str);
	}
};

typedef basic_istringstream<char> istringstream;

TEST(Format, Reader)
{
	char buffer[100];
	istringstream in("Hello world\n123");
	in >> buffer;
	EXPECT_STREQ("Hello", buffer);
	in >> buffer;
	EXPECT_STREQ(buffer, "world");
	int val;
	in >> val;
	EXPECT_EQ(val, 123);
}
