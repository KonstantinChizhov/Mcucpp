
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
				Period16ms =  (0 << WDP3) | (0 << WDP2) | (0 << WDP1) | (0 << WDP0),
				Period32ms =  (0 << WDP3) | (0 << WDP2) | (0 << WDP1) | (1 << WDP0),
				Period64ms =  (0 << WDP3) | (0 << WDP2) | (1 << WDP1) | (0 << WDP0),
				Period125ms = (0 << WDP3) | (0 << WDP2) | (1 << WDP1) | (1 << WDP0),
				Period250ms = (0 << WDP3) | (1 << WDP2) | (0 << WDP1) | (0 << WDP0),
				Period500ms = (0 << WDP3) | (1 << WDP2) | (0 << WDP1) | (1 << WDP0),
				
				Period1s =    (0 << WDP3) | (1 << WDP2) | (1 << WDP1) | (0 << WDP0),
				Period2s =    (0 << WDP3) | (1 << WDP2) | (1 << WDP1) | (1 << WDP0),
				Period4s =    (1 << WDP3) | (0 << WDP2) | (0 << WDP1) | (0 << WDP0),
				Period8s =    (1 << WDP3) | (0 << WDP2) | (0 << WDP1) | (1 << WDP0)
			};
			
			static void Start(unsigned periodMSec)
			{
				Period period = Period16ms;
				if(periodMSec > 4000)
					period = Period8s;
				if(periodMSec > 2000)
					period = Period4s;
				if(periodMSec > 1000)
					period = Period2s;
				else if(periodMSec > 500)
					period = Period1s;
				else if(periodMSec > 250)
					period = Period500ms;
				else if(periodMSec > 120)
					period = Period250ms;
				else if(periodMSec > 60)
					period = Period125ms;
				else if(periodMSec > 30)
					period = Period64ms;
				else if(periodMSec > 15)
					period = Period32ms;
					
				WDTCSR = period | (1 << WDE);
				Reset();
			}
			
			static void Disable()
			{
				ATOMIC
				{
					Reset();
					MCUSR &= ~(1<<WDRF);
					WDTCSR |= (1<<WDCE) | (1<<WDE);
					WDTCSR = 0x00;
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