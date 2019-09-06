//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2018
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
#include "mcu_header.h"
#include <power.h>
#include <atomic.h>

namespace Mcucpp
{
	class Flash
	{
	public:
		static const uint32_t BaseAddress = 0x08000000;

		static inline void ConfigureFreq(uint32_t clockFreq);
		static inline constexpr uint32_t TotalSize();
		static inline constexpr  uint32_t PageCount();
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


	void Flash::ConfigureFreq(uint32_t clockFreq)
	{
		uint32_t div = ((PWR->CR1 & PWR_CR1_VOS_Msk) == PWR_CR1_VOS_0) ?
			6000000 : // VCORE Range 1
			16000000; // VCORE Range 2

		uint32_t ws = (clockFreq-1) / div;
		if(ws > 4)
			ws = 4;
		FLASH->ACR = FLASH_ACR_PRFTEN |
					FLASH_ACR_ICEN |
					FLASH_ACR_DCEN |
					ws;
	}

	constexpr uint32_t Flash::TotalSize()
	{
	#if defined(STM32L471xx)
		return 1024*1024;
	#else
		#error Not supported device. TODO: add support.
	#endif
	}

	constexpr uint32_t Flash::PageCount()
	{
	#if defined(STM32L471xx)
		return 512;
	#else
		#error Not supported device. TODO: add support.
	#endif
	}

	constexpr uint32_t Flash::PageAddress(unsigned page)
	{
	#if defined(STM32L471xx)
		return BaseAddress + page * 0x0800;
	#else
		#error Not supported device. TODO: add support.
	#endif
	}

	constexpr uint32_t Flash::AddrToPage(const void *addr)
	{
		uint32_t addrI = reinterpret_cast<uint32_t>(addr);
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
	#if defined(STM32L471xx)
		return 0x0800;
	#else
		#error Not supported device. TODO: add support.
	#endif
	}

	bool Flash::ErasePage(unsigned page)
	{
		if(page > PageCount())
			return false;
		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;

		while(FLASH->SR & FLASH_SR_BSY)
			;

		uint32_t errMask = FLASH_SR_OPERR |
					 FLASH_SR_PROGERR |
					 FLASH_SR_WRPERR  |
					 FLASH_SR_PGAERR  |
					 FLASH_SR_SIZERR  |
					 FLASH_SR_PGSERR  |
					 FLASH_SR_MISERR  |
					 FLASH_SR_FASTERR |
					 FLASH_SR_RDERR   |
					 FLASH_SR_OPTVERR;

		FLASH->SR |= FLASH_SR_EOP | errMask;

		FLASH->CR = FLASH_CR_PER | ((page & 0x1ff) << 3);

		FLASH->CR |= FLASH_CR_STRT;

		while(FLASH->SR & FLASH_SR_BSY)
			;
		FLASH->CR |= FLASH_CR_LOCK;
		return (FLASH->SR & errMask) == 0;
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
		if(offset & 0x7) // dest should be aligned with a double word address
			return false;

		if(length & 0x7) // length multiple of 8
			return false;

		length >>= 2;

		uint32_t *dest = (uint32_t *)(PageAddress(page) + offset);
		const uint32_t *src = (const uint32_t *)data;

		FLASH->KEYR = 0x45670123;
		FLASH->KEYR = 0xCDEF89AB;

		while(FLASH->SR & FLASH_SR_BSY)
			;

		uint32_t errMask = FLASH_SR_OPERR |
					 FLASH_SR_PROGERR |
					 FLASH_SR_WRPERR  |
					 FLASH_SR_PGAERR  |
					 FLASH_SR_SIZERR  |
					 FLASH_SR_PGSERR  |
					 FLASH_SR_MISERR  |
					 FLASH_SR_FASTERR |
					 FLASH_SR_RDERR   |
					 FLASH_SR_OPTVERR;

		FLASH->SR |= FLASH_SR_EOP | errMask;

		FLASH->CR |= FLASH_CR_PG;

		for(size_t i = 0; i < length; i+=2)
		{
			DisableInterrupts di;
			dest[i] = src[i];
			dest[i+1] = src[i+1];

			while(FLASH->SR & FLASH_SR_BSY)
				;
			if((FLASH->SR & errMask) != 0)
				break;
		}

		FLASH->CR &= (~FLASH_CR_PG);
		FLASH->CR |= FLASH_CR_LOCK;
		return (FLASH->SR & errMask) == 0;
	}
}
