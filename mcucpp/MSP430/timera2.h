
#pragma once

#ifndef _MSP43_TIMER_H
#define _MSP43_TIMER_H

namespace Timers
{
	class Timer0
	{
		public:
		typedef unsigned short DataT;
		static const unsigned short MaxValue = 0xffff;
                
		enum ClockDivider
		{
			DivStop = 0, 
			Div1 = ID_0, 
			Div2 = ID_1, 
			Div4 = ID_2, 
			Div8 = ID_3 
		};

		enum {ClockDividerMask = ~Div8};
                
                enum ClockSrc
                { 
                  ExtRising = TASSEL_0,
                  AuxClock = TASSEL_1,
                  MainClock = TASSEL_2,          
                  ExtFailing = TASSEL_3
                };
                
                enum {ClockSrcMask = ~ExtFailing};
                
		enum TimerMode
		{
			Normal = MC_2,
			ClearOnMatch0 = MC_1,
			UpDown = MC_3
		};
                
		enum {TimerModeMask = ~UpDown};

		template<unsigned Number> struct Divider;

		static void Set(DataT val)
		{
			TAR = val;
		}

		static DataT Get()
		{
			return TAR;
		}

		static void Stop()
		{
			TACTL = 0;
		}

		static void Clear()
		{
			TACTL |= TACLR;
		}

		static void Start(ClockDivider divider, ClockSrc clockSrc = MainClock, TimerMode mode = Normal)
		{
			TACTL = (TACTL & (ClockDividerMask | ClockSrcMask | TimerModeMask)) 
                          | divider | clockSrc | mode;
		}

		static void EnableInterrupt()
		{
			TACTL |= TAIE;
		}

		static bool IsInterrupt()
		{
			return TACTL & TAIFG;
		}
		
		static void ClearInterruptFlag()
		{
			TACTL |= TAIFG;
		}
		
		static void SetMode(TimerMode mode)
		{
			TACTL = (TACTL & TimerModeMask) | mode;
		}

		template<int number> class OutputCompare;
		template<int number> class InputCapture;
	};
	
	template<> struct Timer0::Divider <0> { static const ClockDivider value = Div1;	enum {Div = 1}; };
	template<> struct Timer0::Divider <1> { static const ClockDivider value = Div2;	enum {Div = 2}; };
	template<> struct Timer0::Divider <2> { static const ClockDivider value = Div4;	enum {Div = 4}; };
	template<> struct Timer0::Divider <3> { static const ClockDivider value = Div8;	enum {Div = 8}; };
	
	template<> class Timer0::OutputCompare<0>
	{
	public:
		enum OutputMode
		{
			OutNone = OUTMOD_0,
			OutSet = OUTMOD_1,
			OutTogleReset = OUTMOD_2,
			OutSetReset = OUTMOD_3,
			OutTogle = OUTMOD_4,
			OutReset = OUTMOD_5,
			OutTogleSet = OUTMOD_6,
			OutResetSet = OUTMOD_7
		};
		enum {OutputModeMask = ~OutResetSet};
		
		static void Set(DataT val)
		{
			TACCR0 = val;
		}

		static DataT Get()
		{
			return TACCR0;
		}

		static void EnableInterrupt()
		{
			TACCTL0 |= CCIE;
		}

		static bool IsInterrupt()
		{
			return TACCTL0 & CCIFG;
		}
	
		static void ClearInterruptFlag()
		{
			TACCTL0 |= CCIFG;
		}
		
		static void SetMode(OutputMode mode)
		{
			TACCTL0 = (TACCTL0 & OutputModeMask) | mode;
		}
	};
	
	template<> class Timer0::OutputCompare<1>
	{
	public:
		enum OutputMode
		{
			OutNone = OUTMOD_0,
			OutSet = OUTMOD_1,
			OutTogleReset = OUTMOD_2,
			OutSetReset = OUTMOD_3,
			OutTogle = OUTMOD_4,
			OutReset = OUTMOD_5,
			OutTogleSet = OUTMOD_6,
			OutResetSet = OUTMOD_7
		};
		enum {OutputModeMask = ~OutResetSet};
		
		static void Set(DataT val)
		{
			TACCR1 = val;
		}

		static DataT Get()
		{
			return TACCR1;
		}

		static void EnableInterrupt()
		{
			TACCTL1 |= CCIE;
		}

		static bool IsInterrupt()
		{
			return TACCTL1 & CCIFG;
		}
	
		static void ClearInterruptFlag()
		{
			TACCTL1 |= CCIFG;
		}
		
		static void SetMode(OutputMode mode)
		{
			TACCTL1 = (TACCTL1 & OutputModeMask) | mode;
		}
	};
	
}

#endif