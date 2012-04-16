//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2012
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

namespace Mcucpp
{
	class Power
	{
	public:
		// Stop the CPU but all peripherals remains active.
		static inline void CpuOff(); 
		
		// Stop the CPU but all peripherals running at lower freq if supported by traget MCU, if not its equ to CpuSleep.
		static inline void SleepLowFreq();
		
		// Stop the CPU and all peripherals exept that rinning asynchronously.
		//	Wakeup only by async events such as external interrupt,
		//	or interrupts from asynchronously running modules such as timers with external clock.
		//	External Reset or Watchdog Reset.
		static inline void AsyncPeriphOlny();
		
		// Stop the CPU and all peripherals clocks. Main clock remains running for faster wakeup. 
		//	If not supported by target MCU this mode is equ to PowerDown
		//	Wakeup only by async events such as external interrupt.
		//	External Reset or Watchdog Reset.
		static inline void Standby();
		
		// Stop the CPU and all peripherals clocks. Main clock is source disabled. 
		//	Wakeup only by async events such as external interrupt.
		//	External Reset or Watchdog Reset.
		static inline void PowerDown();
	};
}

#include <power_impl.h>