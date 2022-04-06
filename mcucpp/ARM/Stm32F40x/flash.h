//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2022
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
#include "ioreg.h"
#include "stm32f4xx.h"
#include <power.h>

namespace Mcucpp
{
	class Flash
	{
		static inline void SetParallelism();
		
	public:
		static const uint32_t BaseAddress = 0x08000000;
		
		static inline void ConfigureFreq(uint32_t clockFreq);
		static inline constexpr uint32_t TotalSize();
		static inline constexpr uint32_t PageCount();
		static inline constexpr uint32_t PageAddress(unsigned page);
		static inline constexpr uint32_t PageSize(unsigned page);
		static inline constexpr uint32_t AddrToPage(const void *addr);
		static inline bool ErasePage(unsigned page);
		static inline bool WritePage(unsigned page, const void *data, size_t length, size_t offset = 0);
		static inline bool WritePage(void *dest, const void *src, size_t length);
		static inline bool MassErase();
		static inline bool WriteProtect(bool enable);
		static inline bool ReadoutProtect(bool enable);
		static inline bool WriteProtectEnabled();
		static inline bool ReadoutProtectEnabled();
	};
	
	void Flash::SetParallelism()
	{
		uint32_t vdd = Power::GetVdd();
		if(vdd > 27)
		{
			FLASH->CR = (FLASH->CR & ~FLASH_CR_PSIZE) | FLASH_CR_PSIZE_1;
		}else if(vdd > 21)
		{
			FLASH->CR = (FLASH->CR & ~FLASH_CR_PSIZE) | FLASH_CR_PSIZE_0;
		}
		else
		{
			FLASH->CR &= ~FLASH_CR_PSIZE;
		}
	}
	
	void Flash::ConfigureFreq(uint32_t clockFreq)
	{
		// use PVD to get it
		const uint32_t div = 24000000; // for 2.4 - 2.7 V voltage range
		uint32_t ws = (clockFreq-1) / div;
		if(ws > 7)
			ws = 7;
		FLASH->ACR = FLASH_ACR_PRFTEN | 
					FLASH_ACR_ICEN |
					FLASH_ACR_DCEN |
					ws;
	}
		
	constexpr uint32_t Flash::TotalSize()
	{
	#if defined(STM32F40_41xxx)
		return 1024*1024;
	#elif defined(STM32F429_439xx)
		return 2*1024*1024;
	#else
		#error Not supported device. TODO: add support.
	#endif
	}
	
	constexpr uint32_t Flash::PageCount()
	{
	#if defined(STM32F40_41xxx)
		return 12;
	#elif defined(STM32F429_439xx)
		return 24;
	#else
		#error Not supported device. TODO: add support.
	#endif
	}
	
	constexpr uint32_t Flash::PageAddress(unsigned page)
	{
	#if defined(STM32F40_41xxx)
		if(page < 4)
			return BaseAddress + page * 0x4000;
		if(page == 4)
			return BaseAddress + 0x10000;
		return BaseAddress + 0x20000 + (page-5) * 0x20000;
	#elif defined(STM32F429_439xx)
		// bank 1
		if(page < 4)
			return BaseAddress + page * 0x4000;
		if(page == 4)
			return BaseAddress + 0x10000;
		if(page < 12)
			return BaseAddress + 0x20000 + (page-5) * 0x20000;
		// bank 2
		if(page < 16)
			return BaseAddress + 0x100000 + (page-15) * 0x4000;
		if(page == 16)
			return BaseAddress + 0x110000;
		
		return BaseAddress + 0x120000 + (page-17) * 0x20000;
	#else
		#error Not supported device. TODO: add support.
	#endif
	}
	
	constexpr uint32_t Flash::PageSize(unsigned page)
	{
	#if defined(STM32F40_41xxx)
		if(page < 4)
			return 0x4000;
		if(page == 4)
			return 0x10000;
		return 0x20000;
	#elif defined(STM32F429_439xx)
		// bank 1
		if(page < 4)
			return 0x4000;
		if(page == 4)
			return 0x10000;
        if(page == 5)
            return 0x20000;
		// bank 2
		if(page < 16)
			return 0x4000;
		if(page == 16)
			return 0x10000;
		return 0x20000;
	#else
		#error Not supported device. TODO: add support.
	#endif
	}

	constexpr uint32_t Flash::AddrToPage(const void *addr)
	{
		uint32_t addrI = reinterpret_cast<uint32_t>(addr);
		uint32_t lastPage = PageCount()-1;
		if(addrI < BaseAddress || addrI >= PageAddress(lastPage) + PageSize(lastPage))
		{
			return 0xffffffff;
		}
		for(uint32_t i = 0; i < PageCount(); i++)
		{
			uint32_t pageAddr = PageAddress(i);
			uint32_t size = PageSize(i);
			if(addrI >= pageAddr && addrI < pageAddr + size)
			{
				return i;;
			}
		}
		return 0xffffffff;
	}
	
	bool Flash::ErasePage(unsigned page)
	{
		if(page > PageCount())
			return false;
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
		
		while(FLASH->SR & FLASH_SR_BSY)
			;
		
		FLASH->SR |= FLASH_SR_EOP | FLASH_SR_SOP | FLASH_SR_WRPERR | FLASH_SR_PGAERR | FLASH_SR_PGPERR | FLASH_SR_PGSERR;
		
		FLASH->CR = FLASH_CR_SER | ((page & 0x0f) << 3);
		SetParallelism();
		FLASH->CR |= FLASH_CR_STRT;
		
		while(FLASH->SR & FLASH_SR_BSY)
			;
		FLASH->CR |= FLASH_CR_LOCK;
		return (FLASH->SR & (FLASH_SR_WRPERR | FLASH_SR_PGAERR | FLASH_SR_PGPERR | FLASH_SR_PGSERR)) == 0;
	}

	bool Flash::WritePage(void *dest, const void *src, size_t length)
	{
		unsigned page = AddrToPage(dest);
		size_t offset = (unsigned)dest - PageAddress(page);
		return WritePage(page, src, length, offset);
	}
	
	bool Flash::WritePage(unsigned page, const void *data, size_t length, size_t offset)
	{
		if(page > PageCount())
			return false;
			
		if(offset + length > PageSize(page))
			return false;
		
		uint8_t *dest = (uint8_t *)(PageAddress(page) + offset);
		const uint8_t *src = (const uint8_t *)data;
		
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;
		
		while(FLASH->SR & FLASH_SR_BSY)
			;
		FLASH->SR |= FLASH_SR_EOP | FLASH_SR_SOP | FLASH_SR_WRPERR | FLASH_SR_PGAERR | FLASH_SR_PGPERR | FLASH_SR_PGSERR;
		
		FLASH->CR &= ~FLASH_CR_PSIZE;
		//FLASH->CR |= FLASH_CR_PSIZE_0;
		FLASH->CR |= FLASH_CR_PG;
		
		for(size_t i = 0; i < length; i++)
		{
			dest[i] = src[i];
			while(FLASH->SR & FLASH_SR_BSY)
				;
			if((FLASH->SR & (FLASH_SR_WRPERR | FLASH_SR_PGAERR | FLASH_SR_PGPERR | FLASH_SR_PGSERR)) != 0)
				break;
		}
		
		FLASH->CR &= (~FLASH_CR_PG);
		FLASH->CR |= FLASH_CR_LOCK;
		return (FLASH->SR & (FLASH_SR_WRPERR | FLASH_SR_PGAERR | FLASH_SR_PGPERR | FLASH_SR_PGSERR)) == 0;
	}
}