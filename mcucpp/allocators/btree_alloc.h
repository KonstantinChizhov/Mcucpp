
//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2017
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************
#pragma once

#include <stdint.h>
#include <new.h>
#include <compiler.h>

namespace Mcucpp
{
	namespace BTreeAlloc
	{
		struct Block
		{
			Block(size_t newsize)
				:size(newsize)
			{}
			size_t size; // size of the block minus size of this field
		};

		struct FreeHeader : public Block
		{
			FreeHeader(size_t newsize)
				:Block(newsize),
				less(nullptr),
				more(nullptr)
			{}
			FreeHeader *less;
			FreeHeader *more;
		};

		static inline void DeleteNode(FreeHeader *node, FreeHeader **parent)
		{
			if(!node->less && !node->more)
			{
				*parent = nullptr;
			}
			if(node->less && !node->more)
			{
				*parent = node->less;
			}
			if(!node->less && node->more)
			{
				*parent = node->more;
			}
			if(node->less && node->more)
			{
				FreeHeader *tmpLess = node->less;
				FreeHeader *tmpMore = node->more;
				if(!tmpLess->more)
				{
				 	*parent = tmpLess;
					tmpLess->more = tmpMore;
				}
				else
				{
					FreeHeader *largestParent = tmpLess->more;
					while(largestParent->more)
					{
						largestParent = largestParent->more;
					}
					FreeHeader *largest = largestParent->more;
					*parent = largest;
					largest->more = tmpMore;
					largestParent->more = largest->less;
					largest->less = tmpLess;
				}
			}
			node->less = nullptr;
			node->more = nullptr;
		}

		static inline void InsertNode(FreeHeader *root, FreeHeader *node)
		{
			FreeHeader *current = root;
			while(current)
			{
				if(current->size < node->size)
				{
					if(current->more)
					{
						current = current->more;
					}
					else
					{
						current->more = node;
						return;
					}
				}
				else
				{
					if(current->less)
					{
						current = current->less;
					}
					else
					{
						current->less = node;
						return;
					}
				}
			}
		}
	}

	/*
	class BTreeAllocator
	{
		using namespace BTreeAlloc;
		void *_storage;
		size_t _size;
		FreeHeader * _freeTree;
	public:
		BTreeAllocator(void *storage, size_t size)
			:_storage(storage),
			_size(size)
		{
			_freeTree = new (storage) FreeHeader(size - sizeof(Block));
		}

		void * Alloc(size_t size)
		{
			FreeHeader * current = _freeTree;
			FreeHeader ** ptrToCurrent = &_freeTree;
			do
			{
				if(current->size >= size)
				{
					FreeHeader * currentLess = current->less;
					FreeHeader * currentMore = current->more;

					size_t leftoverSize = current->size - size - sizeof(Block);
					if(leftoverSize < sizeof(FreeHeader) - sizeof(Block))
					{

					}
					else
					{

					}
				}
			}
			while(current);
			return nullptr;
		}

	};
	*/

}
