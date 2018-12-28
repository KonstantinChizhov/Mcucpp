#include <stddef.h>
#include <compiler.h>
#include <new>

namespace std
{
	const nothrow_t nothrow;
}

#if defined(TARGET_HEAP_SIZE) && (TARGET_HEAP_SIZE) > 0

static const size_t McucppMemPoolElements = (TARGET_HEAP_SIZE + sizeof(unsigned) - 1) / sizeof(unsigned);
static unsigned McucppMemoryPool[McucppMemPoolElements];

#include <allocators/bitmap_alloc.h>
static Mcucpp::BitMapAllocator allocator(McucppMemoryPool, sizeof(McucppMemoryPool));


void* operator new(size_t blockSize)
{
	return allocator.Alloc(blockSize);
}

void* operator new[](size_t blockSize)
{
	return allocator.Alloc(blockSize);
}

void operator delete(void* ptr)
{
	return allocator.Free(ptr);
}

void operator delete[](void* ptr)
{
	return allocator.Free(ptr);
}

void* operator new(size_t blockSize, const std::nothrow_t&)
{
	return allocator.Alloc(blockSize);
}

void* operator new[](size_t blockSize, const std::nothrow_t&)
{
	return allocator.Alloc(blockSize);
}

void operator delete(void* ptr, const std::nothrow_t&)
{
	return allocator.Free(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&)
{
	return allocator.Free(ptr);
}

namespace Mcucpp
{
	size_t HeapBytesUsed()
	{
		return allocator.GetUsedSize();
	}
}

#endif

#if defined (__GNUC__)
void* __dso_handle;

namespace std
{
	void __throw_bad_function_call()
	{
		while(1);
	}
}

#endif
