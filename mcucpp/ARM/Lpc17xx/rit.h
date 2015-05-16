// Repetitive Interrupt Timer

#pragma once
#include <clock.h>
#include <ioreg.h>
#include <static_if.h>

namespace Mcucpp
{
	struct RitBase
	{
		enum{
			RIT_CTRL_RITINT	= 1<<0, // Interrupt flag
			RIT_CTRL_ENCLR 	= 1<<1, // Clear counter on interrupt
			RIT_CTRL_ENBR	= 1<<2, // Enable timer halting when the processor is halted for debugging
			RIT_CTRL_EN		= 1<<3, // Timer enable
		};
	};

	namespace Private
	{
		template<unsigned n>
		struct NumWrapper{
			enum{ Val = n};
		};

		template<class Regs, class ClockCtrl>
		class Rit :public RitBase
		{
		public:
			template<unsigned prescaler>
			static void Init()
			{
				const unsigned clkdiv = StaticIf<prescaler%2, NumWrapper<1>, 
								typename StaticIf<prescaler%4, NumWrapper<2>, 
								typename StaticIf<prescaler%8, NumWrapper<4>, NumWrapper<8> >::Result 
								>::Result >::Result::Val;
				ClockCtrl::SetDiv( clkdiv );
				ClockCtrl::Enable();
				Regs()->RICOMPVAL = prescaler/clkdiv;
				Regs()->RIMASK = 0;
				Regs()->RICOUNTER = 0;
				Regs()->RICTRL = RIT_CTRL_ENCLR | RIT_CTRL_ENBR;
			}
			
			static void Init(unsigned prescaler)
			{
				unsigned clkdiv = prescaler%2?1:(prescaler%4?2:(prescaler%8?4:8));
				ClockCtrl::SetDiv( clkdiv );
				ClockCtrl::Enable();
				Regs()->RICOMPVAL = prescaler/clkdiv;
				Regs()->RIMASK = 0;
				Regs()->RICOUNTER = 0;
				Regs()->RICTRL = RIT_CTRL_ENCLR | RIT_CTRL_ENBR;
			}
			
			inline static void Enable(){		Regs()->RICTRL |= RIT_CTRL_EN;		}
			inline static void Disable(){		Regs()->RICTRL &= RIT_CTRL_EN;		}
			inline static void IntClear(){		Regs()->RICTRL |= RIT_CTRL_RITINT;	}
			inline static void Set(unsigned n){	Regs()->RICOUNTER = n;				}
			inline static void Clear(){			Regs()->RICOUNTER = 0;				}
		};
	}

	namespace Private{
		IO_STRUCT_WRAPPER(LPC_RIT, Rit_REGS, LPC_RIT_TypeDef);
	}
	typedef Private::Rit< Private::Rit_REGS, Clock::RitClock > Rit;
}
