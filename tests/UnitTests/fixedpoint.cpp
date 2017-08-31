
#include <fixedpoint.h>
#include <gtest/gtest.h>

using namespace Mcucpp;

	typedef FixedPoint<int32_t, 17> fixed16_t;
	typedef FixedPoint<int16_t, 8> fixed8_t;
	const double threshold = 0.01;

TEST(FixedPoint, Init)
{
	fixed16_t p0;
	EXPECT_EQ(0, p0.IntegerPart());
	EXPECT_EQ(0u, p0.FractionalPart());

	fixed16_t p1(100u);
	EXPECT_EQ(100, p1.IntegerPart());
	EXPECT_EQ(0u, p1.FractionalPart());

	fixed16_t p2(100);
	EXPECT_EQ(100, p2.IntegerPart());
	EXPECT_EQ(0, p2.FractionalPart());

	fixed8_t p3(0);
	EXPECT_EQ(0, p3.IntegerPart());
	EXPECT_EQ(0u, p3.FractionalPart());

	fixed8_t p4(100);
	EXPECT_EQ(100, p4.IntegerPart());

	fixed16_t p5(-100);
	EXPECT_EQ(-100, p5.IntegerPart());

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
	EXPECT_NEAR(150.9, p3.ToDouble(), threshold);

	p3 -= 10;
	EXPECT_NEAR(140.9, p3.ToDouble(), threshold);

	p3 -= fixed16_t(-100.2);
	EXPECT_NEAR(241.1, p3.ToDouble(), threshold);

	p3 = p1 - 10;
	EXPECT_NEAR(40.4, p3.ToDouble(), threshold);

	p3 = 10 - p2;
	EXPECT_NEAR(110.5, p3.ToDouble(), threshold);
}

TEST(FixedPoint, Multiply)
{
	fixed16_t p1(50.4), p2(-100.5), p3;
	p3 = p1 * p2;
	EXPECT_NEAR(-5065.2, p3.ToDouble(), threshold);

	p3 *= 2;
	EXPECT_NEAR(-10130.4, p3.ToDouble(), threshold);

	p3 *= fixed16_t(0.1);
	EXPECT_NEAR(-1013, p3.ToDouble(), 0.25);

	p3 = p1 * 10;
	EXPECT_NEAR(504.0, p3.ToDouble(), threshold);

	p3 = 10 * p2;
	EXPECT_NEAR(-1005.0, p3.ToDouble(), threshold);

	p3 = fixed16_t(0.1) * fixed16_t(0.1);
	EXPECT_NEAR(0.01, p3.ToDouble(), threshold);
}

TEST(FixedPoint, Divide)
{
	EXPECT_NEAR(1.0, (fixed16_t(0.1) / fixed16_t(0.1)).ToDouble(), threshold);

	fixed16_t p1(100), p2(12), p3;
	p3 = p1 / p2;
	EXPECT_NEAR(8.33333, p3.ToDouble(), threshold);

	p3 /= -2;
	EXPECT_NEAR(-4.16666, p3.ToDouble(), threshold);

	p3 /= fixed16_t(0.1);
	EXPECT_NEAR(-41.6666, p3.ToDouble(), threshold*2);

	p3 = p1 / 10;
	EXPECT_NEAR(10, p3.ToDouble(), threshold);

	p3 = 10 / p2;
	EXPECT_NEAR(10.0 / 12.0, p3.ToDouble(), threshold);

}
