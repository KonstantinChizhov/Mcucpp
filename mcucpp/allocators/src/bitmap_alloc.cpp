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

#include <allocators/bitmap_alloc.h>
#include <stdint.h>
#include <stddef.h>
#include <atomic.h>
#include <template_utils.h>
#include <bit_functions.h>

namespace Mcucpp
{
	BitMap::BitMap(MapT *map, size_t mapSizeBits)
		:_map(map),
		_mapSizeBits(mapSizeBits),
		_mapElements((mapSizeBits + BitsInWord - 1) / BitsInWord),
		_lastBlock(0)
	{

	}

	size_t BitMap::UsedBlocks() const
	{
		size_t used = 0;
		for(size_t i = 0; i < _mapElements; i++)
			used += Util::GetPopulatedBits(_map[i]);
		return used;
	}



	size_t BitMap::Alloc(unsigned blockCount)
	{
		if(blockCount == 0)
		{
			return size_t(-1);
		}

		if(blockCount < BitsInWord)
		{
			MapT mask = (1 << blockCount) - 1;
            MapT *mapPtr = &_map[_lastBlock];
            MapT *endPtr = &_map[_mapElements];
            const int shiftLimit = BitsInWord - blockCount;
            do
            {
                outerLoop1:
                MapT mapElem = mapPtr[0];
                if(mapElem == BusyBlock)
                {
                    continue;
                }

                int bitShift = CountTrailingZeros((MapT)~mapElem);
                MapT currentMask = mask << bitShift;
                for(; bitShift < shiftLimit; bitShift++, currentMask <<= 1)
                {
                    if(mapElem & currentMask)
                    {
                        continue;
                    }
                    if(Atomic::CompareExchange(mapPtr, mapElem, MapT(mapElem | currentMask)))
                    {
                        size_t mapIndex = mapPtr - _map;
                        _lastBlock = mapIndex;
                        return mapIndex * BitsInWord + bitShift;
                    }
                    goto outerLoop1;
                }

                int leadingZeros = CountLeadingZeros(mapElem);

                if(leadingZeros > 0 && mapPtr + 1 < endPtr)
                {
                    MapT endMask = mask >> leadingZeros;
                    MapT mapElem2 = mapPtr[1];
                    if((mapElem2 & endMask) == 0)
                    {
                        MapT startMask = BusyBlock << (BitsInWord - leadingZeros);
                        if(!Atomic::CompareExchange(mapPtr, mapElem, MapT(mapElem | startMask)))
                        {
                            goto outerLoop1;
                        }
                        if(!Atomic::CompareExchange(mapPtr + 1, mapElem2, MapT(mapElem2 | endMask)))
                        {
                            Atomic::AndAndFetch(mapPtr, ~startMask);
                            goto outerLoop1;
                        }
                        size_t mapIndex = mapPtr - _map;
                        _lastBlock = mapIndex + 1;
                        return mapIndex * BitsInWord + (BitsInWord - leadingZeros);
                    }
                }
            }while(++mapPtr < endPtr);
            return size_t(-1);
		}

        const size_t mapElements = _mapElements;
		for(unsigned mapIndex = _lastBlock; mapIndex < mapElements; mapIndex++)
		{
			MapT mapElem = _map[mapIndex];
			if(mapElem == BusyBlock)
			{
				continue;
			}
			int leadingZeros = CountLeadingZeros(mapElem);
			int bitsInLastElement = (blockCount - leadingZeros) % BitsInWord;

			unsigned blocksAvailable = leadingZeros;

			MapT endMask = (1 << bitsInLastElement) - 1;
			unsigned looksAheadIndex;
			MapT lastMapElement = 0;
			for(looksAheadIndex = mapIndex + 1; looksAheadIndex < mapElements; looksAheadIndex++)
			{
				lastMapElement = _map[looksAheadIndex];
				if(lastMapElement != 0)
				{
					if((lastMapElement & endMask) == 0)
					{
						blocksAvailable += bitsInLastElement;
					}
					break;
				}
				blocksAvailable += BitsInWord;
				if(blocksAvailable >= blockCount)
				{
					break;
				}
			}

			if(blocksAvailable < blockCount)
			{
				continue;
			}
			blocksAvailable = blockCount - leadingZeros;

			MapT startMask = ~MapT(0) << (BitsInWord - leadingZeros);

			if(!Atomic::CompareExchange(&_map[mapIndex], mapElem, MapT(mapElem | startMask)))
			{
				mapIndex--;
				continue;
			}
			bool retry = 0;
			for(looksAheadIndex = mapIndex + 1; blocksAvailable >= BitsInWord; looksAheadIndex++, blocksAvailable -= BitsInWord)
			{
				if(!Atomic::CompareExchange(&_map[looksAheadIndex], (MapT)0, BusyBlock))
				{
					retry = true;
					break;
				}
			}

			if(!retry && !Atomic::CompareExchange(&_map[looksAheadIndex], lastMapElement, endMask))
			{
				Atomic::AndAndFetch(&_map[looksAheadIndex--], endMask);
				retry = true;
			}

			if(!retry)
			{
				_lastBlock = mapIndex;
				return (mapIndex + 1) * BitsInWord - leadingZeros;
			}
			Atomic::AndAndFetch(&_map[mapIndex], startMask);
			for(unsigned freeIndex = mapIndex + 1; freeIndex <= looksAheadIndex; freeIndex++)
			{
				_map[mapIndex] = 0;
			}
			mapIndex = looksAheadIndex + 1;
		}
		return size_t(-1);
	}

	void BitMap::Free(size_t blockIndex, unsigned blockCount)
	{
		if(blockIndex + blockCount > _mapSizeBits)
			return;

		size_t mapStartIndex = (blockIndex) >> MapTBits;
		size_t mapEndIndex = (blockIndex + blockCount) >> MapTBits;

		int bitStartPos = blockIndex & (8 * sizeof(MapT) - 1);
		int bitEndPos = (blockIndex + blockCount ) & (8 * sizeof(MapT) - 1);
		MapT startBitMask = ((MapT(1) << bitStartPos) - 1);
		MapT endBitMask = MapT(1) << bitEndPos;
		endBitMask = ~((endBitMask - 1));

		if(_lastBlock > mapStartIndex)
		{
			_lastBlock = mapStartIndex;
		}
		if(mapStartIndex == mapEndIndex) // all bits in same map element
		{
			Atomic::AndAndFetch(&_map[mapStartIndex], (startBitMask | endBitMask));
			return;
		}
		Atomic::AndAndFetch(&_map[mapStartIndex++], (startBitMask));
		for(; mapStartIndex < mapEndIndex; mapStartIndex++)
		{
			_map[mapStartIndex] = 0;
		}
		Atomic::AndAndFetch(&_map[mapStartIndex], (endBitMask));
	}

	void BitMap::FreeAll()
	{
		for(size_t i = 0; i < _mapElements; i++)
			_map[i] = 0;
	}
}

