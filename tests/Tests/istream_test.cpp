#include <string>
#include "tiny_istream.h"
#include "tiny_iomanip.h"
#include <gtest.h>
#include <sstream>

using namespace Mcucpp;

template<class char_type>
class StringReader
{
public:
	void SetSource(const char_type* str)
	{
		_str = str;
		pos = 0;
	}

	void seek(size_t p)
	{
		pos = p;
	}

	char_type get(basic_ios<char_type> &thisIos)
	{
		char_type c = _str[pos];
		if(c)
			pos++;
		else
            {
                thisIos.setstate(ios_base::eofbit);
                return basic_ios<char_type>::trates::eof;
            }

		return c;
	}

private:
	int pos;
	const char_type* _str;
};

template<class char_type=char>
class basic_istringstream :public basic_istream<StringReader<char_type>, char_type>
{
public:
	basic_istringstream(const char_type *str)
	{
		basic_istream<StringReader<char_type>, char_type>::_src.SetSource(str);
	}
};

typedef basic_istringstream<char> istringstream;

TEST(Format, Reader)
{
	char buffer[100];
	istringstream in("Hello world\n123 \t0xff\t 033");
	in >> buffer;
	EXPECT_STREQ("Hello", buffer);
	in >> buffer;
	EXPECT_STREQ("world", buffer);
	int val;
	in >> val;
	EXPECT_EQ(123, val);
	in >> hex >> val;
	EXPECT_EQ(255, val);
	in >> oct >> val;
	EXPECT_EQ(27, val);
}

TEST(Format, ReaderGetline)
{
	char buffer[100];
	istringstream in(" Hello world\n123 ff 33");
	in.getline(buffer);
	EXPECT_STREQ("Hello world", buffer);
}

TEST(Format, ReaderSkipWs)
{
	char buffer[100];
	istringstream in(" \tHello    \t    world\n\t123 \t0xff-033");
	//std::stringstream in(" \tHello    \t    world\n\t123 \t0xff-033");
	in >> buffer;
	EXPECT_STREQ("Hello", buffer);
	in >> buffer;
	EXPECT_STREQ("world", buffer);
	int val;
	in >> val;
	EXPECT_EQ(123, val);
	in >> hex >> val;
	EXPECT_EQ(255, val);
	in.ignore() >> oct >> val;
	EXPECT_EQ(27, val);
}

TEST(Format, ReaderGet)
{
	char buffer[100];
	istringstream in(" \tHello    \t    world,123 \t0xff-033");
	in.get(buffer, 100, ',');
	EXPECT_STREQ(" \tHello    \t    world", buffer);
	EXPECT_EQ(sizeof(" \tHello    \t    world")-1, in.gcount());
	char c;
	in.get(c);
	EXPECT_EQ(',', c);

	in.get(buffer, 100);
	EXPECT_STREQ("123 \t0xff-033", buffer);
	EXPECT_TRUE(in.eof());
}

TEST(Format, ReaderRead)
{
	char buffer[100];
	istringstream in(" \tHello    \t    world,123 \t0xff-033");
	if(!in)
        FAIL();
	in.read(buffer, 10);
	buffer[10] = 0;
	EXPECT_STREQ(" \tHello   ", buffer);
	EXPECT_EQ(10u, in.gcount());
}


TEST(Format, ReaderBase)
{
	istringstream in("00008,deadbeef,12345678");
	if(!in) FAIL();
	unsigned val;
	in >> dec >> val;
	EXPECT_EQ(8u, val);
	in.ignore(100, ',');
	in >> hex >> val;
	EXPECT_EQ(0xdeadbeef, val);
	in.ignore(100, ',');
	in >> oct >> val;
	EXPECT_EQ(01234567u, val);
}
