#include <string>
#include "tiny_istream.h"
#include "tiny_iomanip.h"
#include <gtest/gtest.h>
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

	void seek(basic_ios<char_type> &thisIos, ptrdiff_t p,  ios_base::seekdir dir)
	{
		ptrdiff_t len = basic_ios<char_type>::trates::StrLen(_str);
		if(dir == ios_base::beg)
			pos = p;
		else if(dir == ios_base::cur)
			pos += p;
		else
			pos = len - p - 1;

		if(pos < 0)
			pos = 0;
		if(pos >= len)
			pos = len - 1;

	}

	ptrdiff_t tell(basic_ios<char_type> &thisIos)
	{
		return pos;
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
	ptrdiff_t pos;
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

TEST(Format, ReaderSeek)
{
	char str[] = "00008,deadbeef,12345678";
	istringstream in(str);
	if(!in) FAIL();
	unsigned val;
	in >> dec >> val;
	EXPECT_EQ(8u, val);
	in.seekg(0);
	in >> dec >> val;
	EXPECT_EQ(8u, val);
	in.seekg(7, ios_base::end);
	in >> val;
	EXPECT_EQ(12345678u, val);
	EXPECT_EQ(sizeof(str)-1, (size_t)in.tellg());
}

TEST(Format, ReaderBool)
{
	istringstream in("0 1 true false");
	if(!in) FAIL();
	in.setf(istringstream::boolalpha);
	bool val = true;
	in >> val;
	EXPECT_EQ(false, val);
	val = false;
	in >> val;
	EXPECT_EQ(true, val);
	val = false;
	in >> val;
	EXPECT_EQ(true, val);
	val = true;
	in >> val;
	EXPECT_EQ(false, val);
}