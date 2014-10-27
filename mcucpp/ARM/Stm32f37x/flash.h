//*****************************************************************************
//
// Author                : Konstantin Chizhov
// Date                        : 2013
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
#include <stm32f37x.h>

namespace Mcucpp
{
	class Flash
	{
		static const uint32_t FixedPageSize = 2048;
	public:
		static const uint32_t BaseAddress = 0x08000000;
		
		static inline void ConfigureFreq(uint32_t clockFreq);
		static inline uint32_t TotalSize();
		static inline uint32_t PageCount();
		static inline uint32_t PageAddress(unsigned page);
		static inline uint32_t PageSize(unsigned page);
		static inline bool ErasePage(unsigned page);
		static inline bool WritePage(unsigned page, void *data, size_t length, size_t offset = 0);
		static inline bool MassErase();
		static inline bool WriteProtect(unsigned page, bool enable);
		static inline bool ReadoutProtect(bool enable);
		static inline bool WriteProtectEnabled(unsigned page);
		static inline bool ReadoutProtectEnabled();
	};
	
	void Flash::ConfigureFreq(uint32_t clockFreq)
	{
		uint32_t ws = 2;

		if(clockFreq < 24000000)
		{
			ws = 0;
		}else if(clockFreq < 48000000)
		{
			ws = 1;
		}
		FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_PRFTBS | ws;
	}
			
	uint32_t Flash::TotalSize()
	{
		return PageCount()*FixedPageSize;
	}
	
	uint32_t Flash::PageCount()
	{
		return 128;
	}
	
	uint32_t Flash::PageAddress(unsigned page)
	{
		if(page > PageCount())
			return 0;
		return BaseAddress + FixedPageSize * page;
	}
	
	uint32_t Flash::PageSize(unsigned)
	{
		return FixedPageSize;
	}
	
	bool Flash::ErasePage(unsigned page)
	{
		if(page > PageCount())
				return false;
		
		while(FLASH->SR & FLASH_SR_BSY)
			;
				
		if(FLASH->CR & FLASH_CR_LOCK)
		{
			FLASH->KEYR = 0x45670123;
			FLASH->KEYR = 0xCDEF89AB;
		}
		
		FLASH->CR &= ~(FLASH_CR_PG | FLASH_CR_PER | FLASH_CR_MER | FLASH_CR_OPTPG | 
					FLASH_CR_OPTER | FLASH_CR_OPTWRE | FLASH_CR_ERRIE | FLASH_CR_EOPIE | FLASH_CR_OBL_LAUNCH);
		FLASH->CR = FLASH_CR_PER;
		FLASH->AR = PageAddress(page);
		FLASH->CR |= FLASH_CR_STRT;
		
		while(FLASH->SR & FLASH_SR_BSY)
			;
		
		FLASH->CR &= ~FLASH_CR_PER;
		FLASH->CR |= FLASH_CR_LOCK;
		bool result = FLASH->SR & FLASH_SR_EOP;
		FLASH->SR &= ~FLASH_SR_EOP;
		return result;
	}
	
	bool Flash::WritePage(unsigned page, void *data, size_t length, size_t offset)
	{
		if(page > PageCount())
			return false;
		
		if(offset + length > PageSize(page))
			return false;
		
		//bool dataAligned = ((uint32_t)data & 1) == 0;
		// TODO: deal with data and offset alignment
		uint16_t *dest = (uint16_t *)(PageAddress(page) + offset);
		uint16_t *src = (uint16_t *)(data);
		
		if(FLASH->CR & FLASH_CR_LOCK)
		{
			FLASH->KEYR = 0x45670123;
			FLASH->KEYR = 0xCDEF89AB;
		}
		while(FLASH->SR & FLASH_SR_BSY)
			;
		FLASH->CR &= ~(FLASH_CR_PER | FLASH_CR_MER | FLASH_CR_OPTPG | FLASH_CR_OPTER | FLASH_CR_OPTWRE | FLASH_CR_ERRIE | FLASH_CR_EOPIE | FLASH_CR_OBL_LAUNCH);
		FLASH->CR |= FLASH_CR_PG;
		
		length >>= 1;
		
		for(size_t i = 0; i < length; i++)
		{
			dest[i] = src[i];
			while(FLASH->SR & FLASH_SR_BSY)
				;
			if((FLASH->SR & ( FLASH_SR_WRPERR | FLASH_SR_PGERR)) != 0)
				break;
		}
		FLASH->CR &= ~FLASH_CR_PG;
		FLASH->CR |= FLASH_CR_LOCK;
		bool result = FLASH->SR & FLASH_SR_EOP;
		FLASH->SR &= ~FLASH_SR_EOP;
		return (FLASH->SR & ( FLASH_SR_WRPERR | FLASH_SR_PGERR)) == 0;
	}
}