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
		Clear();
	}

	void Clear()
	{
		pos = 0;
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

typedef IO::basic_ostream<MyWriter > my_ostream;

TEST(Format, Stream)
{
	my_ostream my_cout;
	my_cout << "Hello, world!";
	EXPECT_STREQ(my_cout.c_str(), "Hello, world!");
	my_cout.Clear();

	my_cout << (uint8_t)123;
	EXPECT_STREQ(my_cout.c_str(), "123");
	my_cout.Clear();

	my_cout << (uint16_t)1234;
	EXPECT_STREQ(my_cout.c_str(), "1234");
	my_cout.Clear();

	my_cout << (uint32_t)12345678;
	EXPECT_STREQ(my_cout.c_str(), "12345678");
	my_cout.Clear();

	my_cout << (int16_t)-1234;
	EXPECT_STREQ(my_cout.c_str(), "-1234");
	my_cout.Clear();

	my_cout << (int32_t)-12345678;
	EXPECT_STREQ(my_cout.c_str(), "-12345678");
	my_cout.Clear();

	my_cout << IO::hex << 0x12345678;
	EXPECT_STREQ(my_cout.c_str(), "12345678");
	my_cout.Clear();

	my_cout << IO::hex << IO::showbase << 0x12345678;
	EXPECT_STREQ(my_cout.c_str(), "0x12345678");
	my_cout.Clear();

	my_cout << IO::setw(16) << IO::hex << IO::showbase;
	my_cout << IO::left << 0x12345678;
	EXPECT_STREQ(my_cout.c_str(), "0x12345678      ");
	my_cout.Clear();

	my_cout << IO::setw(16) << IO::right << 0x12345678;
	EXPECT_STREQ(my_cout.c_str(), "      0x12345678");
	my_cout.Clear();

	my_cout.fill('0');
	my_cout << IO::setw(16) << IO::internal << 0x12345678;
	EXPECT_STREQ(my_cout.c_str(), "0x00000012345678");
	my_cout.Clear();

	my_cout << IO::setw(0);
	my_cout << true;
	EXPECT_STREQ(my_cout.c_str(), "1");
	my_cout.Clear();

	my_cout << false;
	EXPECT_STREQ(my_cout.c_str(), "0");
	my_cout.Clear();

	my_cout << IO::boolalpha;
	my_cout << true;
	EXPECT_STREQ(my_cout.c_str(), "true");
	my_cout.Clear();

	my_cout << false;
	EXPECT_STREQ(my_cout.c_str(), "false");
	my_cout.Clear();
}

TEST(Format, String)
{
	my_ostream my_cout;

	my_cout % IO::Format("Hello, %!") % "world";
	EXPECT_STREQ(my_cout.c_str(), "Hello, world!");
	my_cout.Clear();

	my_cout % IO::Format("Foo = %") % 1000;
	EXPECT_STREQ(my_cout.c_str(), "Foo = 1000");
	my_cout.Clear();

	my_cout % IO::Format("Foo = %|#0x10|%%") % 0x1234;
	EXPECT_STREQ(my_cout.c_str(), "Foo = 0x00001234%");
	my_cout.Clear();
}
