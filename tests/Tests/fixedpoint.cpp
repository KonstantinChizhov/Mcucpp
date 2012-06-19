
#include <fixedpoint.h>
#include <gtest.h>

using namespace Mcucpp;

	typedef FixedPoint<16, 16> fixed16_t;
	typedef FixedPoint<8, 8> fixed8_t;
	const double threshold = 0.0001;

TEST(FixedPoint, Init)
{
	fixed16_t p0;
	EXPECT_EQ(0, p0.IntegerPart());
	EXPECT_EQ(0u, p0.FractionalPart());

	fixed16_t p1(100u);
	EXPECT_EQ(100, p1.IntegerPart());
	EXPECT_EQ(0u, p1.FractionalPart());

	fixed16_t p2(100, 0xff000000);
	EXPECT_EQ(100, p2.IntegerPart());
	EXPECT_EQ(0xff00u, p2.FractionalPart());

	fixed8_t p3(0);
	EXPECT_EQ(0, p3.IntegerPart());
	EXPECT_EQ(0u, p3.FractionalPart());

	fixed8_t p4(100, 0xff000000);
	EXPECT_EQ(100, p4.IntegerPart());
	EXPECT_EQ(0xffu, p4.FractionalPart());

	fixed16_t p5(-100, 0xff000000);
	EXPECT_EQ(-100, p5.IntegerPart());
	EXPECT_EQ(0xff00u, p5.FractionalPart());
}

TEST(FixedPoint, Plus)
{
	fixed16_t p1(50.4), p2(-100.5), p3;
	p3 = p1 + p2;
	EXPECT_NEAR(-50.1f, p3.ToFloat(), threshold);

	p3 += 10;
	EXPECT_NEAR(-40.1f, p3.ToFloat(), threshold);

	p3 += fixed16_t(-100.2f);
	EXPECT_NEAR(-140.3f, p3.ToFloat(), threshold);

	p3 = p1 + 10;
	EXPECT_NEAR(60.4f, p3.ToFloat(), threshold);

	p3 = 10 + p2;
	EXPECT_NEAR(-90.5f, p3.ToFloat(), threshold);
}

TEST(FixedPoint, Minus)
{
	fixed16_t p1(50.4), p2(-100.5), p3;
	p3 = p1 - p2;
	EXPECT_NEAR(-150.9, p3.ToDouble(), threshold);

	p3 -= 10;
	EXPECT_NEAR(-160.9, p3.ToDouble(), threshold);

	p3 -= fixed16_t(-100.2);
	EXPECT_NEAR(-60.7, p3.ToDouble(), threshold);

	p3 = p1 - 10;
	EXPECT_NEAR(40.4, p3.ToDouble(), threshold);

	p3 = 10 - p2;
	EXPECT_NEAR(110.5, p3.ToDouble(), threshold);
}
