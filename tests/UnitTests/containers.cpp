#include <iostream>

#define DEBUG 1

#include <ring_buffer.h>
#include <stack.h>
#include <array.h>

#include <gtest/gtest.h>

using namespace Mcucpp;
using namespace  Containers;

TEST(Containers, RingBuffer1)
{
	RingBufferPO2<16, int> buf1;
	const RingBufferPO2<16, int> &cref = buf1;

	EXPECT_TRUE(buf1.empty());
	EXPECT_TRUE(buf1.push_back(100));
	EXPECT_FALSE(buf1.empty());
	EXPECT_EQ(buf1.front(), 100);
	EXPECT_TRUE(buf1.pop_front());
	EXPECT_TRUE(buf1.empty());
	EXPECT_FALSE(buf1.pop_front());

	EXPECT_TRUE(buf1.push_back(200));
	EXPECT_TRUE(buf1.push_back(300));

	EXPECT_EQ(buf1.front(), 200);
	EXPECT_EQ(buf1.back(), 300);
	EXPECT_EQ(buf1.size(), 2);

	EXPECT_EQ(cref.front(), 200);
	EXPECT_EQ(cref.back(), 300);
	EXPECT_EQ(cref.size(), 2);
	buf1.clear();
	EXPECT_EQ(buf1.size(), 0);
	EXPECT_TRUE(buf1.empty());

	for(unsigned i=0; i < buf1.capacity(); i++)
		EXPECT_TRUE(buf1.push_back(i));
	EXPECT_FALSE(buf1.push_back(-1));
	EXPECT_TRUE(buf1.full());
	EXPECT_TRUE(cref.full());

	for(unsigned i=0; i < buf1.capacity(); i++)
	{
		EXPECT_EQ(buf1[i], int(i));
		EXPECT_EQ(cref[i], int(i));
	}

	for(unsigned i=0; i < buf1.capacity(); i++)
	{
		EXPECT_EQ(buf1.front(), int(i));
		EXPECT_EQ(cref.front(), int(i));
		EXPECT_TRUE(buf1.pop_front());
	}
}

TEST(Containers, RingBufferPO2FullCondition)
{
	RingBufferPO2<16, int> buf1;
	
	for(unsigned i=0; i < buf1.capacity(); i++)
		EXPECT_TRUE(buf1.push_back(i));
	ASSERT_EQ(16, buf1.size());
	EXPECT_TRUE(buf1.full());
	
	for(unsigned i=0; i < 0xffff; i++)
	{
		ASSERT_TRUE(buf1.pop_front());
		ASSERT_TRUE(buf1.push_back(i));
		ASSERT_EQ(16, buf1.size());
	}
}

TEST(Containers, RingBufferFullCondition)
{
	RingBuffer<16, int> buf1;

	for(unsigned i=0; i < buf1.capacity(); i++)
		EXPECT_TRUE(buf1.push_back(i));
	ASSERT_EQ(16, buf1.size());
	EXPECT_TRUE(buf1.full());
	
	for(unsigned i=0; i < 0xffff; i++)
	{
		ASSERT_TRUE(buf1.pop_front());
		ASSERT_TRUE(buf1.push_back(i));
		ASSERT_EQ(16, buf1.size());
	}
}

size_t RoundToWordBoundary(size_t size)
{
	return (size + sizeof(unsigned) - 1) & ~(sizeof(unsigned) - 1);
}

TEST(Containers, RingBufferPO2StorageSize)
{
	EXPECT_EQ(sizeof(uint_fast8_t), sizeof(RingBufferPO2<16, uint8_t>::size_type));
	EXPECT_EQ(RoundToWordBoundary(16 + 2 * sizeof(uint_fast8_t)), sizeof(RingBufferPO2<16, uint8_t>));
	
	EXPECT_EQ(sizeof(uint_fast16_t), sizeof(RingBufferPO2<256, uint8_t>::size_type));
	EXPECT_EQ(RoundToWordBoundary(256 + 2 * sizeof(uint_fast16_t)), sizeof(RingBufferPO2<256, uint8_t>));
	
	EXPECT_EQ(sizeof(uint_fast8_t), sizeof(RingBufferPO2<16, uint16_t>::size_type));
	EXPECT_EQ(RoundToWordBoundary(16 * 2 + 2 * sizeof(uint_fast8_t)), sizeof(RingBufferPO2<16, uint16_t>));
	
	EXPECT_EQ(sizeof(uint_fast16_t), sizeof(RingBufferPO2<256, uint16_t>::size_type));
	EXPECT_EQ(RoundToWordBoundary(256 * 2 + 2 * sizeof(uint_fast16_t)), sizeof(RingBufferPO2<256, uint16_t>));
	
	EXPECT_EQ(sizeof(uint_fast8_t), sizeof(RingBufferPO2<16, uint16_t>::size_type));
	EXPECT_EQ(RoundToWordBoundary(16 * 4 + 2 * sizeof(uint_fast8_t)), sizeof(RingBufferPO2<16, uint32_t>));
	
	EXPECT_EQ(sizeof(uint_fast16_t), sizeof(RingBufferPO2<256, uint32_t>::size_type));
	EXPECT_EQ(RoundToWordBoundary(256 * 4 + 2 * sizeof(uint_fast16_t)), sizeof(RingBufferPO2<256, uint32_t>));
}

TEST(Containers, RingBuffer2)
{
	RingBuffer<20, int> buf1;
	const RingBuffer<20, int> &cref = buf1;

	EXPECT_TRUE(buf1.empty());
	EXPECT_TRUE(buf1.push_back(100));
	EXPECT_FALSE(buf1.empty());
	EXPECT_EQ(buf1.front(), 100);
	EXPECT_TRUE(buf1.push_back());
	EXPECT_EQ(buf1.back(), 0);
	EXPECT_TRUE(buf1.pop_front());
	EXPECT_TRUE(buf1.pop_front());
	EXPECT_TRUE(buf1.empty());
	EXPECT_FALSE(buf1.pop_front());

	EXPECT_TRUE(buf1.push_back(200));
	EXPECT_TRUE(buf1.push_back(300));

	EXPECT_EQ(buf1.front(), 200);
	EXPECT_EQ(buf1.back(), 300);
	EXPECT_EQ(buf1.size(), 2);

	EXPECT_EQ(cref.front(), 200);
	EXPECT_EQ(cref.back(), 300);
	EXPECT_EQ(cref.size(), 2);
	buf1.clear();
	EXPECT_EQ(buf1.size(), 0);
	EXPECT_TRUE(buf1.empty());

	for(unsigned i=0; i< buf1.capacity(); i++)
		EXPECT_TRUE(buf1.push_back(i));
	EXPECT_FALSE(buf1.push_back(-1));
	EXPECT_TRUE(buf1.full());
	EXPECT_TRUE(cref.full());

	for(unsigned i=0; i < buf1.capacity(); i++)
	{
		EXPECT_EQ(buf1[i], int(i));
		EXPECT_EQ(cref[i], int(i));
	}

	for(unsigned i=0; i < buf1.capacity(); i++)
	{
		EXPECT_EQ(buf1.front(), int(i));
		EXPECT_EQ(cref.front(), int(i));
		EXPECT_TRUE(buf1.pop_front());
	}
}


TEST(Containers, Stack)
{
	FixedStack<20, int> stack;
	FixedStack<20, int> &cref = stack;

	EXPECT_TRUE(stack.empty());
	EXPECT_TRUE(cref.empty());
	EXPECT_EQ(stack.size(), 0);
	EXPECT_EQ(cref.size(), 0);
	EXPECT_TRUE(stack.push_front(100));
	EXPECT_FALSE(stack.empty());
	EXPECT_FALSE(cref.empty());
	EXPECT_EQ(stack.front(), 100);
	EXPECT_EQ(cref.front(), 100);
	EXPECT_TRUE(stack.pop_front());
	EXPECT_FALSE(stack.pop_front());
	EXPECT_TRUE(stack.empty());
	EXPECT_TRUE(cref.empty());

	EXPECT_TRUE(stack.push_front(200));
	EXPECT_TRUE(stack.push_front(300));
	EXPECT_EQ(stack.front(), 300);
	EXPECT_EQ(cref.front(), 300);
	EXPECT_EQ(stack.back(), 200);
	EXPECT_EQ(cref.back(), 200);
	EXPECT_EQ(stack.size(), 2);
	EXPECT_EQ(cref.size(), 2);

	stack.clear();
	EXPECT_EQ(stack.size(), 0);
	EXPECT_TRUE(stack.empty());

	for(unsigned i=0; i< stack.capacity(); i++)
		EXPECT_TRUE(stack.push_front(i));
	EXPECT_FALSE(stack.push_front(-1));

	for(unsigned i=0; i < stack.capacity(); i++)
	{
		EXPECT_EQ(stack[i], int(stack.capacity() - 1 - i));
		EXPECT_EQ(cref[i], int(cref.capacity() - 1 - i));
	}

	for(unsigned i=0; i < stack.capacity(); i++)
	{
		EXPECT_EQ(stack.front(), int(stack.capacity() - 1 - i));
		EXPECT_EQ(cref.front(), int(cref.capacity() - 1 - i));
		EXPECT_TRUE(stack.pop_front());
	}
}


TEST(Containers, Array)
{
	FixedArray<20, int> array;
	FixedArray<20, int> &cref = array;

	EXPECT_TRUE(array.empty());
	EXPECT_TRUE(cref.empty());
	EXPECT_EQ(array.size(), 0);
	EXPECT_EQ(cref.size(), 0);

	EXPECT_TRUE(array.push_back(100));
	EXPECT_FALSE(array.empty());
	EXPECT_FALSE(cref.empty());
	EXPECT_EQ(array.front(), 100);
	EXPECT_EQ(cref.front(), 100);
	EXPECT_TRUE(array.pop_back());
	EXPECT_FALSE(array.pop_back());
	EXPECT_TRUE(array.empty());
	EXPECT_TRUE(cref.empty());

	EXPECT_TRUE(array.push_back(200));
	EXPECT_TRUE(array.push_back(300));
	EXPECT_EQ(array.front(), 200);
	EXPECT_EQ(cref.front(), 200);
	EXPECT_EQ(array.back(), 300);
	EXPECT_EQ(cref.back(), 300);
	EXPECT_EQ(array.size(), 2);
	EXPECT_EQ(cref.size(), 2);

	array.clear();
	EXPECT_EQ(array.size(), 0);
	EXPECT_TRUE(array.empty());

	for(unsigned i=0; i< array.capacity(); i++)
		EXPECT_TRUE(array.push_back(i));
	EXPECT_FALSE(array.push_back(-1));

	for(unsigned i=0; i < array.capacity(); i++)
	{
		EXPECT_EQ(array[i], int(i));
		EXPECT_EQ(cref[i], int(i));
	}

	for(unsigned i=0; i < array.capacity(); i++)
	{
		EXPECT_EQ(array.back(), int(array.capacity() - 1 - i));
		EXPECT_EQ(cref.back(), int(cref.capacity() - 1 - i));
		EXPECT_TRUE(array.pop_back());
	}
}


TEST(Containers, ArrayBool)
{
	FixedArray<20, bool> array;
	FixedArray<20, bool> &cref = array;

	EXPECT_TRUE(array.empty());
	EXPECT_TRUE(cref.empty());
	EXPECT_EQ(array.size(), 0);
	EXPECT_EQ(cref.size(), 0);

	EXPECT_TRUE(array.push_back(true));
	EXPECT_FALSE(array.empty());
	EXPECT_FALSE(cref.empty());
	EXPECT_EQ(array.front(), true);
	EXPECT_EQ(cref.front(), true);
	EXPECT_TRUE(array.pop_back());
	EXPECT_FALSE(array.pop_back());
	EXPECT_TRUE(array.empty());
	EXPECT_TRUE(cref.empty());

	EXPECT_TRUE(array.push_back(false));
	EXPECT_TRUE(array.push_back(true));
	EXPECT_EQ(array.front(), false);
	EXPECT_EQ(cref.front(), false);
	EXPECT_EQ(array.back(), true);
	EXPECT_EQ(cref.back(), true);
	EXPECT_EQ(array.size(), 2);
	EXPECT_EQ(cref.size(), 2);

	array.clear();
	EXPECT_EQ(array.size(), 0);
	EXPECT_TRUE(array.empty());

	for(unsigned i=0; i< array.capacity(); i++)
		EXPECT_TRUE(array.push_back(bool(i & 1)));
	EXPECT_FALSE(array.push_back(false)!=0);

	for(unsigned i=0; i < array.capacity(); i++)
	{
		EXPECT_EQ(array[i], bool(i & 1));
		EXPECT_EQ(cref[i], bool(i & 1));
	}

	for(unsigned i=0; i < array.capacity(); i++)
	{
		EXPECT_EQ(array.back(), bool(int(array.capacity() - 1 - i) & 1) );
		EXPECT_EQ(cref.back(), bool(int(cref.capacity() - 1 - i) & 1) );
		EXPECT_TRUE(array.pop_back());
	}
}

class ArrayBoolInitClass
{
	public:
	FixedArray<20, bool> array;
	ArrayBoolInitClass();
};


ArrayBoolInitClass::ArrayBoolInitClass()
		:array()
{
	array.push_back(false);
}

TEST(Containers, ArrayBoolInit)
{
	ArrayBoolInitClass foo;

	EXPECT_EQ(foo.array.size(), 1);
	ArrayBoolInitClass *pfoo = new ArrayBoolInitClass();
	EXPECT_EQ(pfoo->array.size(), 1);
	delete pfoo;
}

class ConstructionTest
{
	static int _count;
	int _data;
public:
	ConstructionTest()
		:_data(_count)
	{
		_count ++;
	}
	static void Reset()
	{
		_count = 0;
	}

	static int Count()
	{
		return _count;
	}
};

int ConstructionTest::_count = 0;

TEST(Array, ConstructionTest)
{
	ConstructionTest::Reset();
	FixedArray<20, ConstructionTest> array;
	EXPECT_EQ(0, ConstructionTest::Count());
	for(int i = 0; i<10; i++)
	{
		array.push_back( );
		EXPECT_EQ(i+1, ConstructionTest::Count());
	}
}

TEST(RingBufferPO2, ConstructionTest)
{
	ConstructionTest::Reset();
	RingBufferPO2<16, ConstructionTest> array;
	EXPECT_EQ(0, ConstructionTest::Count());
	for(int i = 0; i<10; i++)
	{
		array.push_back( );
		EXPECT_EQ(i+1, ConstructionTest::Count());
	}
}

TEST(RingBuffer, ConstructionTest)
{
	ConstructionTest::Reset();
	RingBuffer<22, ConstructionTest> array;
	EXPECT_EQ(0, ConstructionTest::Count());
	for(int i = 0; i<10; i++)
	{
		array.push_back( );
		EXPECT_EQ(i+1, ConstructionTest::Count());
	}
}

TEST(FixedStack, ConstructionTest)
{
	ConstructionTest::Reset();
	FixedStack<22, ConstructionTest> array;
	EXPECT_EQ(0, ConstructionTest::Count());
	for(int i = 0; i<10; i++)
	{
		array.push_front(ConstructionTest() );
		EXPECT_EQ(i+1, ConstructionTest::Count());
	}
}