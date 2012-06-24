
#include <iostream>
#include <string>
#include <gtest.h>
#include <utf8.h>

using namespace Mcucpp;

typedef Utf8Encoding<char *, wchar_t> Utf8;

void convert(char *utf8, wchar_t *wchar)
{
    while(*utf8)
        *wchar++ = Utf8::Read(utf8);
    *wchar = 0;
}

TEST(Utf8, Decode)
{
    char str1[] = "Hello";
    wchar_t result[100];
    convert(str1, result);
    EXPECT_STREQ(L"Hello", result);

    // TODO: more tests
}
