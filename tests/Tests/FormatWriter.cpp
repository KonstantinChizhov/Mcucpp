#include <iostream>
#include <string>
#include "tiny_ostream.h"
#include "tiny_iomanip.h"
#include "format_parser.h"
#include <gtest.h>


class MyWriter
{
	enum{BufSize = 10000};
public:
	MyWriter()
	{
		seekp(0);
	}

	void seekp(int pos)
	{
		this->pos = pos;
		_buffer[0] = '\0';
	}

	void put(char c)
	{
		_buffer[pos++] = c;
	}
	char *c_str()
	{
		_buffer[pos]='\0';
		return  &_buffer[0];
	}
private:
	int pos;
	char _buffer[BufSize];
};

using namespace Mcucpp;

typedef basic_ostream<MyWriter > my_ostream;

TEST(Format, Stream)
{
	my_ostream my_cout;
	my_cout << "Hello, world!";
	EXPECT_STREQ(my_cout.c_str(), "Hello, world!");
	my_cout.seekp(0);

	my_cout << (uint8_t)123;
	EXPECT_STREQ(my_cout.c_str(), "123");
	my_cout.seekp(0);

	my_cout << (uint16_t)1234;
	EXPECT_STREQ(my_cout.c_str(), "1234");
	my_cout.seekp(0);

	my_cout << (uint32_t)12345678;
	EXPECT_STREQ(my_cout.c_str(), "12345678");
	my_cout.seekp(0);

	my_cout << (int16_t)-1234;
	EXPECT_STREQ(my_cout.c_str(), "-1234");
	my_cout.seekp(0);

	my_cout << (int32_t)-12345678;
	EXPECT_STREQ(my_cout.c_str(), "-12345678");
	my_cout.seekp(0);

	my_cout << hex << 0x12345678;
	EXPECT_STREQ(my_cout.c_str(), "12345678");
	my_cout.seekp(0);

	my_cout << hex << showbase << 0x12345678;
	EXPECT_STREQ(my_cout.c_str(), "0x12345678");
	my_cout.seekp(0);

	my_cout << setw(16) << hex << showbase;
	my_cout << left << 0x12345678;
	EXPECT_STREQ(my_cout.c_str(), "0x12345678      ");
	my_cout.seekp(0);

	my_cout << setw(16) << right << 0x12345678;
	EXPECT_STREQ(my_cout.c_str(), "      0x12345678");
	my_cout.seekp(0);

	my_cout.fill('0');
	my_cout << setw(16) << internal << 0x12345678;
	EXPECT_STREQ(my_cout.c_str(), "0x00000012345678");
	my_cout.seekp(0);

	my_cout << setw(0);
	my_cout << true;
	EXPECT_STREQ(my_cout.c_str(), "1");
	my_cout.seekp(0);

	my_cout << false;
	EXPECT_STREQ(my_cout.c_str(), "0");
	my_cout.seekp(0);

	my_cout << boolalpha;
	my_cout << true;
	EXPECT_STREQ(my_cout.c_str(), "true");
	my_cout.seekp(0);

	my_cout << false;
	EXPECT_STREQ(my_cout.c_str(), "false");
	my_cout.seekp(0);
}

TEST(Format, String)
{
	my_ostream my_cout;

	my_cout % Format("Hello, %!") % "world";
	EXPECT_STREQ(my_cout.c_str(), "Hello, world!");
	my_cout.seekp(0);

	my_cout % Format("Foo = %") % 1000;
	EXPECT_STREQ(my_cout.c_str(), "Foo = 1000");
	my_cout.seekp(0);

	my_cout % Format("Foo = %|#0x10|%%") % 0x1234;
	EXPECT_STREQ(my_cout.c_str(), "Foo = 0x00001234%");
	my_cout.seekp(0);
}


TEST(Format, FloatDefault)
{
	my_ostream my_cout;

	my_cout.precision(5);
	my_cout << 0.0f;
	EXPECT_STREQ("0", my_cout.c_str());
	my_cout.seekp(0);

	my_cout << 123.f;
	EXPECT_STREQ("123", my_cout.c_str());
	my_cout.seekp(0);

	my_cout << 12.34f;
	EXPECT_STREQ("12.34", my_cout.c_str());
	my_cout.seekp(0);

	my_cout << 12345.67f;
	EXPECT_STREQ("12346", my_cout.c_str());
	my_cout.seekp(0);

	my_cout << 1234567890.123f;
	EXPECT_STREQ("1.2346e+009", my_cout.c_str());
	my_cout.seekp(0);

	my_cout << 1.234567890e-5f;
	EXPECT_STREQ("1.2346e-005", my_cout.c_str());
	my_cout.seekp(0);

	my_cout << -12.34f;
	EXPECT_STREQ("-12.34", my_cout.c_str());
	my_cout.seekp(0);

	my_cout << -12345.67f;
	EXPECT_STREQ("-12346", my_cout.c_str());
	my_cout.seekp(0);

	my_cout << -1234567890.123f;
	EXPECT_STREQ("-1.2346e+009", my_cout.c_str());
	my_cout.seekp(0);

	my_cout << -1.234567890e-5f;
	EXPECT_STREQ("-1.2346e-005", my_cout.c_str());
	my_cout.seekp(0);
}

TEST(Format, FloatRound)
{
	my_ostream my_cout;

	my_cout.precision(5);
	my_cout << 99999.99f;
	EXPECT_STREQ("1e+005", my_cout.c_str());
	my_cout.seekp(0);

	my_cout << 99999.0f;
	EXPECT_STREQ("99999", my_cout.c_str());
	my_cout.seekp(0);

	my_cout << 9999.99f;
	EXPECT_STREQ("10000", my_cout.c_str());
	my_cout.seekp(0);

	my_cout << 9999.001f;
	EXPECT_STREQ("9999", my_cout.c_str());
	my_cout.seekp(0);
}
