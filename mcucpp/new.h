#pragma once

void* operator new(size_t, void* __p);
void* operator new[](size_t, void* __p);
void operator delete  (void*, void*);
void operator delete[](void*, void*);