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

#ifndef MCUCPP_MEMPOOL_H
#define MCUCPP_MEMPOOL_H

#include <stdint.h>
#include <stddef.h>
#include <atomic.h>
#include <template_utils.h>

namespace Mcucpp
{

	template<size_t _BlockSize, size_t NBlocks, class MapT = uint8_t, class Atomic=VoidAtomic>
	class MemPool
	{
		static const MapT busyBlock = MapT(-1);
		static const int MapTBits = sizeof(MapT) == 4 ? 5 : sizeof(MapT) == 2 ? 4 : 3;
	public:
		static const size_t MapSize = NBlocks/(8*sizeof(MapT)) + 1;
		static const size_t Size = NBlocks * _BlockSize;
		static const size_t Blocks = NBlocks;
		static const size_t BlockSize = _BlockSize;

		MemPool()
		{
			FreeAll();
		}

		size_t GetBlockSize()
		{
			return BlockSize;
		}

		size_t BlockCount()
		{
			return Blocks;
		}

		size_t UsedBlocks()
		{
			size_t used = 0;
			for(size_t i = 0; i < MapSize; i++)
				used += Util::GetPopulatedBits(_map[i]);
			return used;
		}

		void* Alloc()
		{
			size_t i = 0;
			void *adr;
			MapT mapElem = 0;
			MapT bit = 0;
			do
			{
				for(;i < MapSize; i++)
				{
					mapElem = Atomic::Fetch(&_map[i]);
					if(mapElem != busyBlock)
						break;
				}
				if(i == MapSize)
				{
					return 0;
				}
				MapT bit2;
				bit2 = bit = (~mapElem) & -(~mapElem);
				uint8_t bitPos = 0;
				while (bit2 >>= 1) ++bitPos;
				size_t index = i*8*sizeof(MapT) + bitPos;
				if(index >= NBlocks)
				{
					return 0;
				}
				adr = &_data[index];
			}while(!Atomic::CompareExchange(&_map[i], mapElem, MapT(mapElem | bit)));
			// NOTE: possible ABA problem
			return adr;
		}

		void Free(void* ptr)
		{
			if(!IsInPool(ptr))
				return;
			size_t blockIndex = reinterpret_cast<Block*>(ptr) - _data;
			size_t mapIndex = blockIndex >> MapTBits;
			int bitPos = blockIndex & (8*sizeof(MapT) - 1);
			MapT bit = 1 << bitPos;
			Atomic::AndAndFetch(&_map[mapIndex], ~bit);
		}

		bool IsInPool(void *ptr)
		{
			uint8_t *bPtr =  (uint8_t *)ptr;
			uint8_t *dPtr =  (uint8_t *)(&_data[0]);
			
			if((bPtr < dPtr) || (bPtr > (dPtr + sizeof(_data))))
				return false;
			return true;
		}

		void FreeAll()
		{
			for(size_t i = 0; i < MapSize; i++)
				_map[i] = 0;
		}

	protected:
		struct Block
		{
			unsigned data[(BlockSize + sizeof(unsigned) - 1) / sizeof(unsigned)];
		};
		MapT _map[MapSize];
		Block _data[NBlocks];
	};
}
#endif
