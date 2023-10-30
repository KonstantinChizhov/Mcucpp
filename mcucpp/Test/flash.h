//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2020
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
#include <stdlib.h>
#include <string.h>
#include <atomic.h>

namespace Mcucpp
{
	class Flash
	{
	public:
	
		static inline void ConfigureFreq(uint32_t clockFreq);
		static constexpr inline uint32_t TotalSize();
		static constexpr inline uint32_t PageCount();
		static inline size_t PageAddress(unsigned page);
		static constexpr inline uint32_t PageSize(unsigned page);
		static inline uint32_t AddrToPage(const void *addr);
		static inline bool ErasePage(unsigned page);
		static inline bool WritePage(unsigned page, const void *data, size_t length, size_t offset = 0);
		static inline bool WritePage(void *dest, const void *src, size_t length);

		static inline bool MassErase();
		static inline bool WriteProtect(bool enable);
		static inline bool ReadoutProtect(bool enable);
		static inline bool WriteProtectEnabled();
		static inline bool ReadoutProtectEnabled();
		static uint8_t FlashMemoryBuffer[1024*1024];
		static size_t BaseAddress;
	};
	
	inline uint8_t Flash::FlashMemoryBuffer[1024*1024];
	inline size_t Flash::BaseAddress;
	
	namespace
	{
		struct InitFlashBuffer
		{
			InitFlashBuffer()
			{
				memset(Flash::FlashMemoryBuffer, 0xff, sizeof(Flash::FlashMemoryBuffer));
				Flash::BaseAddress = reinterpret_cast<size_t>(&Flash::FlashMemoryBuffer[0]);
			}
		} initFlashBuffer;
	}
	
	void Flash::ConfigureFreq(uint32_t clockFreq)
	{

	}

	constexpr uint32_t Flash::TotalSize()
	{
		return sizeof(FlashMemoryBuffer);
	}

	constexpr uint32_t Flash::PageCount()
	{
		return 512;
	}

	size_t Flash::PageAddress(unsigned page)
	{
		return BaseAddress + page * 0x0800;
	}

	uint32_t Flash::AddrToPage(const void *addr)
	{
		size_t addrI = reinterpret_cast<size_t>(addr);
		if(addrI < BaseAddress)
		{
			return 0xffffffff;
		}
		uint32_t page = (addrI - BaseAddress) / 0x0800;
		if(page >= PageCount())
		{
			return 0xffffffff;
		}
		return page;
	}

	constexpr uint32_t Flash::PageSize(unsigned page)
	{
		(void)page;
		return 0x0800;
	}

	bool Flash::ErasePage(unsigned page)
	{
		if(page > PageCount())
			return false;
		
		memset(&FlashMemoryBuffer[PageAddress(page)], 0xff, PageSize(page));
		return true;
	}

	bool Flash::WritePage(void *dest, const void *src, size_t length)
	{
		unsigned page = AddrToPage(dest);
		size_t offset = (size_t)dest - PageAddress(page);
		return WritePage(page, src, length, offset);
	}

	bool Flash::WritePage(unsigned page, const void *data, size_t length, size_t offset)
	{
		if(page > PageCount())
			return false;

		if(offset + length > PageSize(page))
			return false;
		if(offset & 0x7) // dest should be aligned with a double word address
			return false;

		if(length & 0x7) // length multiple of 8
			return false;

		memcpy((void *)PageAddress(page), data, length);
		return true;
	}
}
