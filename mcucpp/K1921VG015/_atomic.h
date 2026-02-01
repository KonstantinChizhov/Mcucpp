#pragma once

#include <csr.h>
#include <arch.h>


namespace Mcucpp
{
	class DisableInterrupts
	{
	public:
		DisableInterrupts()
		{
			clear_csr(mstatus, MSTATUS_MIE);
		}
		~DisableInterrupts()
		{
			set_csr(mstatus, MSTATUS_MIE);
		}
		constexpr operator bool()
		{return false;}
	private:

	};

    #define ATOMIC if(Mcucpp::DisableInterrupts di = Mcucpp::DisableInterrupts()){}else

	static inline void memory_barrier(){ fence(); }

}