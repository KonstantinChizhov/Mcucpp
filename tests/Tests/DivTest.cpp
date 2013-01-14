#include <gtest\gtest.h>
#include <div.h>
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
