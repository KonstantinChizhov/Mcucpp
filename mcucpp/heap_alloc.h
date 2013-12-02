//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
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

namespace Mcucpp
{
class HeapAlloc
{
	unsigned *const _pool;
	const unsigned _size;
	unsigned * _firstFreeBlock;
	static const unsigned MinAllocSize = 2;
	static const unsigned AllocGranularity = sizeof(unsigned);
public:
	HeapAlloc(unsigned *addr, unsigned size)
	:_pool(addr), _size(size / AllocGranularity)
	{
		_firstFreeBlock = _pool;
		_firstFreeBlock[0] = _size;
		_firstFreeBlock[1] = 0;
	}
	void* Alloc(unsigned size)
	{
		if(_firstFreeBlock == 0)
			return 0;
			
		unsigned *currentBlock = _firstFreeBlock;
		unsigned *prevBlockPtr = reinterpret_cast<unsigned *>(&_firstFreeBlock);
		size = (size + 2*AllocGranularity - 1) / AllocGranularity;

		do
		{
			unsigned currentBlockSize = currentBlock[0];
			unsigned *nextFreeBlock = reinterpret_cast<unsigned *>(currentBlock[1]);
			if(currentBlockSize >= size)
			{
				if(currentBlock[0] >= size + MinAllocSize)
				{
					currentBlock[0] = size;
					unsigned * newBlockAddr = currentBlock + size;
					*prevBlockPtr = reinterpret_cast<unsigned>(newBlockAddr);
					newBlockAddr[0] = currentBlockSize - size;
					newBlockAddr[1] = reinterpret_cast<unsigned>(nextFreeBlock);
					return &currentBlock[1];
				}
				else
				{
					prevBlockPtr = nextFreeBlock;
					return &currentBlock[1];
				}
			}else
			{
				prevBlockPtr = currentBlock;
				currentBlock = nextFreeBlock;
			}
		}while(currentBlock != 0);
	}
	
	void Free(void* ptr)
	{
		unsigned *uptr = reinterpret_cast<unsigned *>(ptr);
		uptr--;
		if(uptr < _pool || uptr >= _pool + _size)
		{
			return;
		}

		if(_firstFreeBlock == 0)
		{
			_firstFreeBlock = uptr;
			return;
		}
			
		unsigned *currentBlock = _firstFreeBlock;
		unsigned *prevBlockPtr = reinterpret_cast<unsigned *>(&_firstFreeBlock);
		
		do
		{
			unsigned currentBlockSize = currentBlock[0];
			unsigned *nextFreeBlock = reinterpret_cast<unsigned *>(currentBlock[1]);
			// TODO:
			if(true)
			{
				return;
			}else
			{
				prevBlockPtr = currentBlock;
				currentBlock = nextFreeBlock;
			}
		}while(currentBlock != 0);

		uptr[1] = reinterpret_cast<unsigned>(_firstFreeBlock);
	}
	void FreeAll();
};

}
