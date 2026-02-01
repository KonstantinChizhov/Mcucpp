
//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2025
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
#include <enum.h>
#include <noalloc_function.h>


namespace Mcucpp
{

	class DmaChannel
	{
	public:
		using TransferCompleteCallback = noalloc_function<void(void)>;

	private:
		int _number;
		uint32_t _src_ptr = 0;
		uint32_t _dst_ptr = 0;
		uint32_t _ctrl_reg = 0;
		uint16_t _transfer_count = 0;
	public:
		DmaChannel(int number)
			: _number(number)
		{
		}

		~DmaChannel() = default;
		DmaChannel(const DmaChannel &) = delete;
		DmaChannel &operator=(const DmaChannel &) = delete;

		enum class DataSize
		{
			Byte = 0,	  // 8-bit
			HalfWord = 1, // 16-bit
			Word = 2	  // 32-bit
		};

		enum class AddressIncrement
		{
			Increment = 0, // Auto-increment address
			Fixed = 1	   // Keep address fixed
		};

		enum class Priority
		{
			Low = 0,
			Medium = Low,
			High = 1,
			VeryHigh = High
		};

		enum class TransferMode
		{
			None = 0,
			Normal = 0x1,
			AutoTrigger = 0x2,
			DoubleBuffer = 0x3,
			DescriptorChain = 0x6,
			Circular = 0x8,
		};

		void Enable();
		void Disable();
		bool IsEnabled() const;

		void ConfigureSource(uint32_t address, DataSize size, AddressIncrement increment);
		void ConfigureDestination(uint32_t address, DataSize size, AddressIncrement increment);
		bool SetTransferCount(uint16_t count, uint16_t batch_size);
		bool IsTransferComplete() const;
		void ClearTransferCompleteFlag();
		void SetPriority(Priority priority);
		void SetTransferMode(TransferMode mode, bool circular);
		void StartTransfer();
		void SetTransferCompleteCallback(TransferCompleteCallback callback);
		void SwTrigger();
		uint16_t TransferedCount() const;
		bool TransferComplete() const;
	};

}
