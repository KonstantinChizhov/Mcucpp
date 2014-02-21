
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
#include <atomic.h>
#include <__compatibility.h>

namespace Mcucpp
{
	class Watchdog
	{
		public:
			enum Period
			{
				Period15ms = 0,
				Period30ms = 1 << WDP0,
				Period60ms = 1 << WDP1,
				Period120ms = (1 << WDP0) | (1 << WDP1),
				Period250ms = (1 << WDP2),
				Period500ms = (1 << WDP2) | (1 << WDP0),
				Period1s = (1 << WDP2) | (1 << WDP1),
				Period2s = (1 << WDP0) | (1 << WDP1) | (1 << WDP0)
			};
			
			static void Start(unsigned periodMSec)
			{
				Period period = Period15ms;
				if(periodMSec > 1000)
					period = Period2s;
				else if(periodMSec > 500)
					period = Period1s;
				else if(periodMSec > 250)
					period = Period500ms;
				else if(periodMSec > 120)
					period = Period250ms;
				else if(periodMSec > 60)
					period = Period120ms;
				else if(periodMSec > 30)
					period = Period60ms;
				else if(periodMSec > 15)
					period = Period30ms;
					
				WDTCR = period | (1 << WDE);
				Reset();
			}
			
			static void Disable()
			{
				ATOMIC
				{
					Reset();
					WDTCR |= (1 << WDTOE) | (1 << WDE);
					WDTCR = 0;
				}
			}
			
			static void Reset()
			{
				asm("wdr");
			}
			
			static bool CauseReset()
			{
				return false; //TODO
			}
	};
}