//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2026
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

#include <sys_tick.h>
#include <compiler.h>
// MCU specific clock
#include <clock.h>
#include <mtimer.h>
#include <csr.h>
#include <arch.h>
#include <plic.h>
#include <K1921VG015.h>

namespace Mcucpp
{
	static volatile uint32_t tickCount = 0;

	MCUCPP_WEAK uint32_t GetTickCount()
	{
		return tickCount;
	}

	void SysTickHandler()
	{
		tickCount++;
		SysTickTimer::Reload();
	}
}

void Mcucpp::SysTickTimer::Init(uint32_t periodMilliSec)
	{
		set_csr(mstatus, MSTATUS_MIE);
		period_cycles = Clock::SysClock::ClockFreq() / 1000 * periodMilliSec - 1;
		Reload();
		PLIC_SetIrqHandler(Plic_Mach_Target, IsrVect_IRQ_0, SysTickHandler);
		
	}

	void Mcucpp::SysTickTimer::Reload()
	{
		mtimer_set_raw_time_cmp(period_cycles);
	}

	void Mcucpp::SysTickTimer::EnableInterrupt()
	{
		PLIC_IntEnable(Plic_Mach_Target, IsrVect_IRQ_0);
		set_csr(mie, (1 << 7));
	}
