#pragma once

#if !defined(_NEW) && !defined(_NEW_)
#define _NEW

#include <stddef.h>

inline void* operator new(size_t, void* __p)
{
	return __p;
}

inline void* operator new[](size_t, void* __p)
{
	return __p;
}

inline void operator delete  (void*, void*) { }
inline void operator delete[](void*, void*) { }

#endif