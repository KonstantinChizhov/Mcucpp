
#include <saturated.h>
#include <gtest.h>

using namespace Mcucpp;


TEST(Saturated, Init)
{
	Saturated<int> i0;
	EXPECT_EQ(0, (int)i0);

	Saturated<int> i1(100);
	EXPECT_EQ(100, (int)i1);

	Saturated<int> i2(100u);
	EXPECT_EQ(100, (int)i2);
	
	Saturated<unsigned> i3(-100);
	EXPECT_EQ(0, (unsigned)i3);
	
	Saturated<unsigned char> i4(-100);
	EXPECT_EQ(0, (int)i4);
	
	Saturated<signed char> i5(-100);
	EXPECT_EQ(-100, (int)i5);
	
	Saturated<signed char> i6(150);
	EXPECT_EQ(127, (int)i6);
	
	Saturated<signed char> i7(-150);
	EXPECT_EQ(-128, (int)i7);
}

TEST(Saturated, PlusSigned)
{
	Saturated<signed char> i1(100), i2(50), i3;
	i3 = i1 + i2;
	EXPECT_EQ(127, (int)i3);
	i3 = i1 + 20;
	EXPECT_EQ(120, (int)i3);
	i3 += 255;
	EXPECT_EQ(127, (int)i3);
	i3 = -128;
	i3 += i1;
	EXPECT_EQ(-28, (int)i3);
	i3 += -120;
	EXPECT_EQ(-128, (int)i3);
}

TEST(Saturated, PlusUnsigned)
{
	Saturated<unsigned char> i1(200), i2(60), i3;
	i3 = i1 + i2;
	EXPECT_EQ(255, (int)i3);
	i3 = i1 + -300;
	EXPECT_EQ(0, (int)i3);
}

TEST(Saturated, MinusSigned)
{
	Saturated<signed char> i1(100), i2(50), i3;
	i3 = i1 - i2;
	EXPECT_EQ(50, (int)i3);
	i3 = i1 - 230;
	EXPECT_EQ(-128, (int)i3);
	i3 -= -255;
	EXPECT_EQ(127, (int)i3);
	i3 = -128;
	i3 -= -i1;
	EXPECT_EQ(-28, (int)i3);
	i3 -= 120;
	EXPECT_EQ(-128, (int)i3);
}

TEST(Saturated, Multiply)
{
	
}

