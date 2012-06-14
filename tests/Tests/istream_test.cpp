#include <string>
#include "tiny_istream.h"
#include "tiny_iomanip.h"
#include <gtest.h>

using namespace Mcucpp;

template<class CharT>
class StringReader
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

	CharT get(basic_ios<CharT> &thisIos)
	{
		CharT c = _str[pos];
		if(c)
			pos++;
		else
			thisIos.setstate(ios_base::eofbit);
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
	istringstream in("Hello world\n123 0xff 033");
	in >> buffer;
	EXPECT_STREQ("Hello", buffer);
	in >> buffer;
	EXPECT_STREQ("world", buffer);
	int val;
	in >> val;
	EXPECT_EQ(123, val);
	in >> val;
	EXPECT_EQ(255, val);
	in >> val;
	EXPECT_EQ(27, val);
}

TEST(Format, ReaderFlags)
{
	char buffer[100];
	istringstream in("Hello world\n123 ff 33");
	in.getline(buffer);
	EXPECT_STREQ("Hello world", buffer);
	int val;
	in >> dec >> val;
	EXPECT_EQ(123, val);
	in >> hex >> val;
	EXPECT_EQ(255, val);
	in >> oct >> val;
	EXPECT_EQ(27, val);
}
