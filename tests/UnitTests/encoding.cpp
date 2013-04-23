
#include <iostream>
#include <string>
#include <gtest.h>
#include <utf8.h>

using namespace Mcucpp;

typedef Utf8Encoding<uint32_t> Utf8;

void convert(const char *utf8, wchar_t *wchar)
{
    while(*utf8)
        *wchar++ = Utf8::Decode(utf8);
    *wchar = 0;
}

TEST(Utf8, Decode)
{
    wchar_t result[100];

    convert("Hello~\xc2\x80 \xc3\xbf", result);	// U+0000 to U+00FF
    EXPECT_STREQ(L"Hello~\x0080 \x00FF" , result);

	convert("\xc4\x80 \xc4\x81 \xc7\xbe \xc7\xbf", result); // U+0100 to U+01FF
    EXPECT_STREQ(L"\x0100 \x0101 \x01fe \x01ff", result);

	convert("\xc8\x80 \xca\x87 \xcb\xbf", result); // U+0200 to U+02FF
    EXPECT_STREQ(L"\x0200 \x0287 \x02ff", result);

    convert("\xd0\x90 \xd0\xaf \xd1\x8f \xd3\xbf", result); // U+0400 ... U+04FF
    EXPECT_STREQ(L"\x0410 \x042f \x044f \x04ff", result);

    convert("\xdf\xbf \xdc\x80", result); // U+0700 to U+07FF
    EXPECT_STREQ(L"\x07FF \x0700", result);

    convert("\xe0\xa0\x80 \xe1\x87\xbf \xe1\xa7\xbf \xef\xbf\xbf", result); // U+0800 ...
    EXPECT_STREQ(L"\x0800 \x11FF \x19ff \xffff", result);
#if 0 // enable if wchar_t is more then 16 bits
    convert("\xf0\x90\x80\x80 \xf0\x90\xa1\x9f", result); // U+10000 ...
    EXPECT_STREQ(L"\x10000 \x1085F", result);
#endif
}


TEST(Utf8, Errors)
{
    wchar_t result[100];

	// unexpected continuation mark
    convert("Hello \x85world", result);
    EXPECT_STREQ(L"Hello ?world" , result);

	// missed continuation mark
    convert("Hello worl\xc4\x5f", result);
    EXPECT_STREQ(L"Hello worl?_" , result);

}


TEST(Utf8, Encode)
{
	#define TEST_ENCODE_CHAR(W, S) ptr = result; Utf8::Encode(ptr, W); *ptr = 0; EXPECT_STREQ(S , result)

	char result[100], *ptr;

	TEST_ENCODE_CHAR(0x0030, "\x30");
	TEST_ENCODE_CHAR(0x007e, "\x7e");
	TEST_ENCODE_CHAR(0x0080, "\xc2\x80");
	TEST_ENCODE_CHAR(0x0080, "\xc2\x80");
	TEST_ENCODE_CHAR(0x00ff, "\xc3\xbf");
	TEST_ENCODE_CHAR(0x0100, "\xc4\x80");
	TEST_ENCODE_CHAR(0x01FF, "\xc7\xbf");

	TEST_ENCODE_CHAR(0x19ff, "\xe1\xa7\xbf");
	TEST_ENCODE_CHAR(0x0800, "\xe0\xa0\x80");
	TEST_ENCODE_CHAR(0x11D00, "\xf0\x91\xb4\x80");
	TEST_ENCODE_CHAR(0x120FF, "\xf0\x92\x83\xbf");

}


TEST(Utf8, StrLen)
{
   	EXPECT_EQ(5u, Utf8::StrLen("Hello"));
	EXPECT_EQ(13u, Utf8::StrLen("Hello, \xd0\x9c\xd0\xb5\xd0\xb4\xd0\xb2\xd0\xb5\xd0\xb4"));
}
