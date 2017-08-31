

#include <gtest/gtest.h>
#include <crc.h>
#include <memory_reader.h>



TEST(MemoryReader, Read)
{
	using namespace Mcucpp;
	uint32_t buffer[] = {0x11223344, 0x55667788, 0x99aabbcc};
	MemoryReader reader(buffer, sizeof(buffer));
	
	EXPECT_EQ(0x3344, reader.ReadU16Le());
	EXPECT_EQ(0x1122, reader.ReadU16Le());
}

TEST(MemoryReader, Read2)
{
	using namespace Mcucpp;
	uint32_t buffer[] = {0x11223344, 0x55667788, 0x99aabbcc};
	MemoryReader reader(buffer, sizeof(buffer));
	
	EXPECT_EQ(0x3344, reader.ReadU16Le());
	EXPECT_EQ(0x1122, reader.ReadU16Le());
}