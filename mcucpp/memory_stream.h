
//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2017
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

#include <binary_stream.h>

namespace Mcucpp
{
	class MemoryStreamSource
	{
		uint8_t *_memory;
		size_t _size;
		size_t _pos;
	public:
	
		MemoryStreamSource(void *memory, size_t size)
		:_memory(static_cast<uint8_t *>(memory)),
		_size(size),
		_pos(0)
		{
			
		}
	
		bool Seek(size_t pos)
		{
			if(pos < _size)
			{
				_pos = pos;
				return true;
			}
			return false;
		}
		
		bool Write(uint8_t value)
		{
			if(_pos < _size)
			{
				_memory[_pos++] = value;
				return true;
			}
			return false;
		}
		
		uint8_t Read()
		{
			if(_pos < _size)
			{
				return _memory[_pos++];
			}
			return 0xff;
		}
	};

	typedef BinaryStream<MemoryStreamSource> MemoryStream;
}