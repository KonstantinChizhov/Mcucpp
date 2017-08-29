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

#include <stdint.h>
#include <stddef.h>
#include <template_utils.h>

namespace Mcucpp
{
	class BitMap
	{
	public:
		typedef uint32_t MapT;
		static const MapT BusyBlock = MapT(-1);
		static const int MapTBits = sizeof(MapT) == 8 ? 6 :
									sizeof(MapT) == 4 ? 5 :
									sizeof(MapT) == 2 ? 4 :
									3;
		static const int BitsInWord = 8 * sizeof(MapT);
	private:
		MapT *const _map;
		const size_t _mapSizeBits;
		const size_t _mapElements;
		unsigned _lastBlock;

	public:
		BitMap(MapT *map, size_t mapSizeBits);
		size_t UsedBlocks() const;
		size_t Alloc(unsigned blockCount);
		void Free(size_t blockIndex, unsigned blockCount);
		void FreeAll();

		size_t TotalBlocks() const
		{
			return _mapSizeBits;
		}
		
		size_t TotalMapSizeBytes() const
		{
			return _mapElements * sizeof(MapT);
		}
	};

	class BitMapAllocator
	{
	public:
		BitMap Bitmap;
	private:
		size_t _size;
		uint8_t * _storage;
		
		static inline size_t CalcBitmapSize(size_t storageSize, size_t blockSize)
		{
			return storageSize * 8 / (1 + 8 * blockSize);
		}
		
		enum {DefaultBlockSize = 8 };
		
	public:
		BitMapAllocator(void * storage, size_t storageSize)
			:Bitmap((BitMap::MapT*)storage, CalcBitmapSize(storageSize, DefaultBlockSize))
		{
			Util::fill_n((unsigned*)storage, storageSize / sizeof(unsigned), 0);
			_size = storageSize - Bitmap.TotalMapSizeBytes();
			_storage = (uint8_t*)storage + Bitmap.TotalMapSizeBytes();
		}

		void* Alloc(size_t nbytes) noexcept
		{
			size_t nblocks = (nbytes + sizeof(size_t) + DefaultBlockSize - 1) / DefaultBlockSize;
			size_t blockIndex = Bitmap.Alloc(nblocks);
			if(blockIndex == size_t(-1))
			{
				return nullptr;
			}
			uint8_t *ptr = _storage + blockIndex * DefaultBlockSize;
			*((size_t*)ptr) = nblocks;
			return ptr + sizeof(size_t);
		}
		
		void Free(void* ptr) noexcept
		{
			if((uint8_t*)ptr < (uint8_t*)_storage)
				return;
			
			size_t blockIndex = (((uint8_t*)ptr) - _storage - sizeof(size_t)) / DefaultBlockSize;
			size_t nblocks = *((size_t*)ptr - 1);
			Bitmap.Free(blockIndex, nblocks);
		}
		
		size_t GetUsedSize()
		{
			return sizeof(Bitmap) + Bitmap.UsedBlocks() * DefaultBlockSize + Bitmap.TotalMapSizeBytes();
		}
	};
}

