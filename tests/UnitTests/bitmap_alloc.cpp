
#include <gtest/gtest.h>
#include <allocators/bitmap_alloc.h>

using namespace Mcucpp;

void PrintMap(unsigned *map, size_t count)
{
	for(size_t i = 0; i < count; i++)
	{
		std::cout << std::setw(8) << std::setfill('0') << std::hex << map[i] << " ";
	}
}

TEST(BitmapAalloc, SmallBlock)
{
	unsigned map[10] = {0};
	BitMap bitmap(map, sizeof(map) * 8);
	
	ASSERT_EQ(0, bitmap.Alloc(1));
	ASSERT_EQ(1, bitmap.Alloc(1));
	ASSERT_EQ(2, bitmap.Alloc(2));
	ASSERT_EQ(4, bitmap.Alloc(3));
	ASSERT_EQ(7, bitmap.Alloc(3));
	
	bitmap.Free(2, 2);
	ASSERT_EQ(2, bitmap.Alloc(2));
	bitmap.Free(2, 2);
	bitmap.Free(6, 2);
	
	ASSERT_EQ(10, bitmap.Alloc(4));
}
	
TEST(BitmapAalloc, CrossWordBorders)
{
	unsigned map[10] = {0};
	BitMap bitmap(map, sizeof(map) * 8);
	
	ASSERT_EQ(0, bitmap.Alloc(30));
	PrintMap(map, 2);
	ASSERT_EQ(30, bitmap.Alloc(10));
	PrintMap(map, 2);
	bitmap.Free(30, 10);
	PrintMap(map, 2);
	ASSERT_EQ(30, bitmap.Alloc(10));
	PrintMap(map, 2);
}


TEST(BitmapAalloc, LargeBlock)
{
	unsigned map[10] = {0};
	BitMap bitmap(map, sizeof(map) * 8);
	
	ASSERT_EQ(0, bitmap.Alloc(20));
	PrintMap(map, 5);
	ASSERT_EQ(20, bitmap.Alloc(70));
	PrintMap(map, 5);
	ASSERT_EQ(90, bitmap.Alloc(70));
	
	bitmap.Free(20, 70);
	PrintMap(map, 5);
	ASSERT_EQ(20, bitmap.Alloc(70));
	PrintMap(map, 5);
}