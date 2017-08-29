
#include <tlsf_alloc.h>

extern "C"
{
	#include "TLSF-2.4.6/src/tlsf.h"
}

namespace Mcucpp
{
	TlsfAlloc::TlsfAlloc(void * storage, size_t storageSize)
	:_pool(storage)
	{
		init_memory_pool(storageSize, storage);
	}

	void* TlsfAlloc::Alloc(size_t nbytes) noexcept
	{
		return malloc_ex(nbytes, _pool);
	}

	void TlsfAlloc::Free(void* ptr) noexcept
	{
		free_ex(ptr, _pool);
	}
	size_t TlsfAlloc::GetUsedSize()
	{
		return get_used_size(_pool);
	}
}