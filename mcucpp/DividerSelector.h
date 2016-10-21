//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2016
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
#if !defined(AVR) && !defined (__AVR__)
	#include <div.h>
#endif

namespace Mcucpp
{
namespace Timers
{
	template<typename Timer>
	class DividerSelector
	{
		typename Timer::DataT _ticks;
		unsigned _dividerNumber;
	public:
		DividerSelector()
		:_ticks(0),
		_dividerNumber(0)
		{
			
		}
		
		inline typename Timer::DataT GetReloadValue(){return Timer::MaxValue - _ticks;}
		inline unsigned GetTicks(){return _ticks;}
		inline typename Timer::ClockDivider DividerValue(){return Timer::DividerValue(_dividerNumber);}
		inline unsigned DividerCoeff(){return Timer::DividerCoeff(_dividerNumber);}
		
		inline bool ProcessMkSek(uint32_t srcFreq, uint32_t targetPeriodMkSec)
		{
			unsigned minDivN = 0;
			unsigned maxDivN = Timer::MaxDivider;
			unsigned currentDivN;
			uint32_t ticks;
			
			#if defined(AVR) || defined (__AVR__)
				// less precision but does not require 64-bit intermediate result
				uint32_t timeBase = srcFreq / 1000 * targetPeriodMkSec / 1000;
			#else
				uint32_t timeBase = div64x32u( (uint64_t)srcFreq * targetPeriodMkSec, 1000000, 0);
			#endif
			
			for(int i = 0; i < 32; i++)
			{
				unsigned newDivN = (minDivN + maxDivN) / 2;
				currentDivN = newDivN;
				uint32_t div = Timer::DividerCoeff(currentDivN);
				ticks = timeBase / div;
				
				if(maxDivN == minDivN)
				{
					_dividerNumber = maxDivN;
					break;
				}
				
				if(ticks > Timer::MaxValue)
				{
					minDivN = currentDivN + 1;
				}
				else
				{
					maxDivN = currentDivN;
				}
			}
			if(ticks > Timer::MaxValue)
			{
				_ticks = Timer::MaxValue;
				return false;
			}
			_ticks = (typename Timer::DataT)ticks;
			return true;
		}
	};
	
} //namespace Timers
} //namespace Mcucpp
