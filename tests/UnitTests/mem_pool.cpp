
#include <gtest/gtest.h>
#include <mempool.h>
#include <vector>

TEST(Mempool, Test)
{
	using namespace Mcucpp;
	MemPool<10, 35, uint32_t> pool;
	std::vector<void*> ptrs;
	
	for(unsigned i = 0; i < 35; i++)
	{
		EXPECT_EQ(i, pool.UsedBlocks());
		void * ptr = pool.Alloc();
		EXPECT_NE(NULL, (size_t)ptr);
		ptrs.push_back(ptr);
	}
	void * ptr2 = pool.Alloc();
	EXPECT_EQ(NULL, (size_t)ptr2);
	
	for(unsigned i = 0; i < 35; i++)
	{
		EXPECT_EQ(35-i, pool.UsedBlocks());
		EXPECT_TRUE(pool.IsInPool(ptrs[i]));
		pool.Free(ptrs[i]);
	}
	
	
}