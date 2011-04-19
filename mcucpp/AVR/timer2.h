#pragma once	
	
namespace Timers
{

#ifdef TIMSK2
#define InterruptMaskReg TIMSK2
#else
#define InterruptMaskReg TIMSK
#endif

#ifdef TIFR2
#define InterruptFlagsReg TIFR2
#else
#define InterruptFlagsReg TIFR
#endif

#ifdef TCCR2A
#define ControlRegA TCCR2A
#else
#define ControlRegA TCCR2
#define ControlRegB TCCR2
#endif

#ifdef TCCR2B
#define ControlRegB TCCR2B
#endif

	class BaseTimer2
	{
		public:
		typedef uint8_t DataT;
		enum {MaxValue = 255};
		enum ClockDivider
		{
			DivStop		= (0<<CS22) | (0<<CS21) | (0<<CS20), 
			Div1 		= (0<<CS22) | (0<<CS21) | (1<<CS20),
			Div8 		= (0<<CS22) | (1<<CS21) | (0<<CS20),
			Div32 		= (0<<CS22) | (1<<CS21) | (1<<CS20),
			Div64 		= (1<<CS22) | (0<<CS21) | (0<<CS20),
			Div128 		= (1<<CS22) | (0<<CS21) | (1<<CS20),
			Div256 		= (1<<CS22) | (1<<CS21) | (0<<CS20),
			Div1024 	= (1<<CS22) | (1<<CS21) | (1<<CS20)
		};

		enum {ClockDividerMask = ~((1<<CS22) | (1<<CS21) | (1<<CS20))};

		template<unsigned Number> struct Divider;

		static void Set(DataT val)
		{
			TCNT2 = val;
		}

		static DataT Get()
		{
			return TCNT2;
		}

		static void Stop()
		{
			ControlRegB = 0;
		}

		static void Clear()
		{
			TCNT2 = 0;
		}
		
		static void ClearPrescaller()
		{
			//SFIOR |= (1 << PSR2);
		}

		static void Start(ClockDivider divider)
		{
			ControlRegB = (ControlRegB & ClockDividerMask) | divider;
		}

		static void EnableInterrupt()
		{
			InterruptMaskReg |= (1 << TOIE2);
		}

		static bool IsInterrupt()
		{
			return InterruptFlagsReg & (1<<TOV2);
		}
		
		static void ClearInterruptFlag()
		{
			InterruptFlagsReg |= (1<<TOV2);
		}
	};

	template<> struct BaseTimer2::Divider <0> { static const ClockDivider value = Div1;		enum {Div = 1}; };
	template<> struct BaseTimer2::Divider <1> { static const ClockDivider value = Div8;		enum {Div = 8}; };
	template<> struct BaseTimer2::Divider <2> { static const ClockDivider value = Div64;	enum {Div = 64}; };
	template<> struct BaseTimer2::Divider <3> { static const ClockDivider value = Div256;	enum {Div = 256}; };
	template<> struct BaseTimer2::Divider <4> { static const ClockDivider value = Div1024;	enum {Div = 1024}; };

#if defined WGM20

	class Timer2 : public BaseTimer2
	{		
		public:
	
		enum TimerMode
		{
			Normal 			= (0 << WGM21) | (0 << WGM20),
			PwmPhaseCorrect = (0 << WGM21) | (1 << WGM20),
			ClearOnMatch	= (1 << WGM21) | (0 << WGM20),
			PwmFast 		= (1 << WGM21) | (1 << WGM20)
		};

		enum {TimerModeClearMask = ~(1 << WGM21) | (1 << WGM20)};

		static void SetMode(TimerMode mode)
		{
			TCCR2 = (TCCR2 & TimerModeClearMask) | mode;
		}

		template<int number> class OutputCompare;
	};

	template<> class Timer2::OutputCompare<0>
	{
	public:
		static void Set(DataT val)
		{
			OCR2 = val;
		}

		static DataT Get()
		{
			return OCR2;
		}

		static void EnableInterrupt()
		{
			InterruptMaskReg |= (1 << OCIE2);
		}

		static bool IsInterrupt()
		{
			return InterruptFlagsReg & (1<<OCF2);
		}
	
		static void ClearInterruptFlag()
		{
			InterruptFlagsReg |= (1<<OCF2);
		}
	};
#else
	class Timer2 : public BaseTimer2
	{
	};
#endif

#undef InterruptMaskReg 
#undef InterruptFlagsReg 
#undef ControlRegA
#undef ControlRegB

}
