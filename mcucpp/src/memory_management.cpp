#include <stddef.h>
#include <compiler.h>
#include <new>

namespace std
{
	const nothrow_t nothrow;
}

#if defined(TARGET_HEAP_SIZE) && (TARGET_HEAP_SIZE) > 0

static const size_t McucppMemPoolElements = (TARGET_HEAP_SIZE + sizeof(unsigned) - 1) / sizeof(unsigned);
unsigned McucppMemoryPool[McucppMemPoolElements];

#include <allocators/bitmap_alloc.h>
Mcucpp::BitMapAllocator allocator(McucppMemoryPool, sizeof(McucppMemoryPool));


void* operator new(size_t blockSize)noexcept
{
	return allocator.Alloc(blockSize);
}

void* operator new[](size_t blockSize)noexcept
{
	return allocator.Alloc(blockSize);
}

void operator delete(void* ptr)noexcept
{
	return allocator.Free(ptr);
}

void operator delete[](void* ptr)noexcept
{
	return allocator.Free(ptr);
}

void* operator new(size_t blockSize, const std::nothrow_t&)noexcept
{
	return allocator.Alloc(blockSize);
}

void* operator new[](size_t blockSize, const std::nothrow_t&)noexcept
{
	return allocator.Alloc(blockSize);
}

void operator delete(void* ptr, const std::nothrow_t&)noexcept
{
	allocator.Free(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&)noexcept
{
	allocator.Free(ptr);
}

namespace Mcucpp
{
	size_t HeapBytesUsed()
	{
		return allocator.GetUsedSize();
	}
}

#else

void* operator new(size_t blockSize)noexcept
{
    (void)blockSize;
	return nullptr;
}

void* operator new[](size_t blockSize)noexcept
{
    (void)blockSize;
	return nullptr;
}

void operator delete(void* ptr)noexcept
{
    (void)ptr;
}

void operator delete[](void* ptr)noexcept
{
	(void)ptr;
}

void* operator new(size_t blockSize, const std::nothrow_t&) noexcept
{
    (void)blockSize;
	return nullptr;
}

void* operator new[](size_t blockSize, const std::nothrow_t&)noexcept
{
    (void)blockSize;
	return nullptr;
}

void operator delete(void* ptr, const std::nothrow_t&)noexcept
{
	(void)ptr;
}

void operator delete[](void* ptr, const std::nothrow_t&)noexcept
{
	(void)ptr;
}

namespace Mcucpp
{
	size_t HeapBytesUsed()
	{
		return 0;
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
