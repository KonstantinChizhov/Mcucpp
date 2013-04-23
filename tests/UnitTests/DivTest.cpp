#include <gtest\gtest.h>
#include <div.h>
#include <fastdiv10.h>
#include <fstream>
#include <cstdlib>

void GenerateTestUnsigned64x32()
{
	std::ofstream file("div64x32uTests.h", std::ios::out);
	int testCount = 1000;
	for(int i=0; i<testCount; i++)
	{
		uint64_t u = (uint64_t)rand() ^ ((uint64_t)rand() << 16) ^ ((uint64_t)rand() << 32);
		uint32_t v = (uint32_t)rand() ^ ((uint32_t)rand() << 16);
		uint32_t q = (uint32_t)(u / v);
		file << "EXPECT_EQ(" << q << "ul, div64x32u(" << u << "ull, " << v << "ul, 0) );" << std::endl;
	}
}

TEST(Div, Test64x32unsigned)
{
	//GenerateTestUnsigned64x32();
	#include "div64x32uTests.h"
}

void GenerateTestSigned64x32()
{
	std::ofstream file("div64x32sTests.h", std::ios::out);
	int testCount = 1000;
	for(int i=0; i<testCount; i++)
	{
		int64_t u = (int64_t)rand() ^ ((int64_t)rand() << 16) ^ ((int64_t)rand() << 32);
		int32_t v = (int32_t)rand() ^ ((int32_t)rand() << 16);
		if(rand() & 1)
			u *= -1;
		if(rand() & 1)
			v *= -1;
		int32_t q = (int32_t)(u / v);
		file << "EXPECT_EQ(" << q << "l, div64x32s(" << u << "ll, " << v << "l, 0) );" << std::endl;
	}
}

TEST(Div, Test64x32signed)
{
	//GenerateTestSigned64x32();
	#include "div64x32sTests.h"
}

TEST(Div10, FastDiv10)
{
	using namespace Mcucpp;
	for(uint32_t i = 0; i < 256; i++)
	{
		divmod10_t<uint8_t> res8 = divmodu10((uint8_t)i);
		EXPECT_EQ(i/10, res8.quot);
		EXPECT_EQ(i%10, res8.rem);
	}

	for(uint32_t i = 0; i < 0x00010000; i++)
	{
		divmod10_t<uint16_t> res16 = divmodu10((uint16_t)i);
		EXPECT_EQ(i/10, res16.quot);
		EXPECT_EQ(i%10, res16.rem);
	}

	divmodu10((unsigned)100);
	divmodu10((unsigned long)100);
	divmodu10((unsigned short)100);
	divmodu10((unsigned long long)100);
}
