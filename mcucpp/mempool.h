#ifndef MCUCPP_MEMPOOL_H
#define MCUCPP_MEMPOOL_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "util.h"
#include <atomic.h>

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
			memset(_map, 0, sizeof(_map));
			memset(_data, 0, sizeof(_data));
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
				used += PopulatedBits(_map[i]);
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
			if(ptr < _data || ptr > _data + sizeof(_data))
				return false;
			return true;
		}

		void FreeAll()
		{
            memset(_map, 0, sizeof(_map));
			memset(_data, 0, sizeof(_data));
		}

	protected:
		struct Block
		{
			uint8_t data[BlockSize];
		};
		MapT _map[MapSize];
		Block _data[NBlocks];
	};
}
#endif
