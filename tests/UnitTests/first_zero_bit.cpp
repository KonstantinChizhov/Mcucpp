
#include <gtest.h>
#include <first_zero_bit.h>

TEST(FirstZeroBit, Calculate)
{
	using namespace Mcucpp;
	EXPECT_EQ(0, FirstZeroBitPosition(0x0u));
	EXPECT_EQ(1, FirstZeroBitPosition(0x1u));
	EXPECT_EQ(2, FirstZeroBitPosition(0x3u));
	EXPECT_EQ(3, FirstZeroBitPosition(0x7u));
	EXPECT_EQ(4, FirstZeroBitPosition(0xfu));
	EXPECT_EQ(5, FirstZeroBitPosition(0x1fu));
	EXPECT_EQ(6, FirstZeroBitPosition(0x3fu));
	EXPECT_EQ(7, FirstZeroBitPosition(0x7fu));
	EXPECT_EQ(8, FirstZeroBitPosition(0xffu));
	EXPECT_EQ(9, FirstZeroBitPosition(0x1ffu));
	EXPECT_EQ(10, FirstZeroBitPosition(0x3ffu));
	EXPECT_EQ(11, FirstZeroBitPosition(0x7ffu));
	EXPECT_EQ(12, FirstZeroBitPosition(0xfffu));
	EXPECT_EQ(13, FirstZeroBitPosition(0x1fffu));
	EXPECT_EQ(14, FirstZeroBitPosition(0x3fffu));
	EXPECT_EQ(15, FirstZeroBitPosition(0x7fffu));
	EXPECT_EQ(16, FirstZeroBitPosition(0xffffu));
	EXPECT_EQ(17, FirstZeroBitPosition(0x1ffffu));
	EXPECT_EQ(18, FirstZeroBitPosition(0x3ffffu));
	EXPECT_EQ(19, FirstZeroBitPosition(0x7ffffu));
	EXPECT_EQ(20, FirstZeroBitPosition(0xfffffu));
	EXPECT_EQ(21, FirstZeroBitPosition(0x1fffffu));
	EXPECT_EQ(22, FirstZeroBitPosition(0x3fffffu));
	EXPECT_EQ(23, FirstZeroBitPosition(0x7fffffu));
	EXPECT_EQ(24, FirstZeroBitPosition(0xffffffu));
	EXPECT_EQ(25, FirstZeroBitPosition(0x1ffffffu));
	EXPECT_EQ(26, FirstZeroBitPosition(0x3ffffffu));
	EXPECT_EQ(27, FirstZeroBitPosition(0x7ffffffu));
	EXPECT_EQ(28, FirstZeroBitPosition(0xfffffffu));
	EXPECT_EQ(29, FirstZeroBitPosition(0x1fffffffu));
	EXPECT_EQ(30, FirstZeroBitPosition(0x3fffffffu));
	EXPECT_EQ(31, FirstZeroBitPosition(0x7fffffffu));
	EXPECT_EQ(255, FirstZeroBitPosition(0xffffffffu));
}