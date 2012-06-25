
#include <iostream>
#include <string>
#include <gtest.h>
#include <utf8.h>

using namespace Mcucpp;

typedef Utf8Encoding<const char *, wchar_t> Utf8;

void convert(const char *utf8, wchar_t *wchar)
{
    while(*utf8)
        *wchar++ = Utf8::Read(utf8);
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

    convert("\xf0\x90\x80\x80 \xf0\x90x\a1\x9f", result); // U+10000 ...
    EXPECT_STREQ(L"\x10000 \x1085F", result);
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
