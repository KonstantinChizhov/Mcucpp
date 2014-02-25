

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
#include <ring_buffer.h>
#include <Timeout.h>

namespace Mcucpp
{
	template<class Source, size_t BufferSizeParam, class TimeoutMonitor = NeverTimeout>
	class BufferedOutput :public Source
	{
		Containers::RingBuffer<BufferSizeParam, uint8_t, Atomic> _buffer;
		TimeoutMonitor _timeoutMonitor;
	public:
		BufferedOutput(unsigned timeout = -1)
			:_timeoutMonitor(timeout)
		{
		
		}

		void SetTimeout(unsigned timeout)
		{
			_timeoutMonitor.Set(timeout);
		}

		void Write(uint8_t c)
		{
			Source::EnableInterrupt(Source::TxEmptyInt);
			if(Source::WriteReady())
			{
				Source::Write(c);
			}
			else
			{
				_timeoutMonitor.Reset();
				while(!_buffer.push_back(c) && _timeoutMonitor.Tick())
					;
			}
		}
		
		void TxInterruptHandler()
		{
			if(!Source::WriteReady())
				return;
			if(_buffer.empty())
			{
				Source::DisableInterrupt(Source::TxEmptyInt);
			}
			else
			{
				Source::Write(_buffer.front());
				_buffer.pop_front();
			}
		}
	};
}
