#pragma once	
	
namespace Timers
{

#ifdef TIMSK1
#define InterruptMaskReg TIMSK1
#else
#define InterruptMaskReg TIMSK
#endif

#ifdef TIFR1
#define InterruptFlagsReg TIFR1
#else
#define InterruptFlagsReg TIFR
#endif

//handle headers with outdated definitions
#if defined(PWM10)
#define WGM10 PWM10
#define WGM11 PWM11
#define WGM12 CTC1
#define WGM13 4
#endif

	class BaseTimer1
	{
		public:
		typedef uint16_t DataT;
		enum {MaxValue = 0xffff};
		enum ClockDivider
		{
			DivStop=0, 
			Div1 		= (1<<CS10), 
			Div8 		= (1<<CS11), 
			Div64 		= (1<<CS10) | (1<<CS11), 
			Div256 		= (1<<CS12), 
			Div1024 	= (1<<CS12) | (1<<CS00), 
			ExtFalling	= (1<<CS12) | (1<<CS11), 
			ExtRising	= (1<<CS12) | (1<<CS11) | (1<<CS10)
		};

		enum {ClockDividerMask = ~((1<<CS12) | (1<<CS11) | (1<<CS10))};

		template<unsigned Number> struct Divider;

		static void Set(DataT val)
		{
			TCNT1 = val;
		}

		static DataT Get()
		{
			return TCNT1;
		}

		static void Stop()
		{
			TCCR1B = 0;
		}

		static void Clear()
		{
			TCNT1 = 0;
		}

		static void Start(ClockDivider divider)
		{
			TCCR1B = (TCCR1B & ClockDividerMask) | divider;
		}

		static void EnableInterrupt()
		{
			InterruptMaskReg |= (1 << TOIE1);
		}

		static bool IsInterrupt()
		{
			return InterruptFlagsReg & (1<<TOV1);
		}
		
		static void ClearInterruptFlag()
		{
			InterruptFlagsReg |= (1<<TOV1);
		}
	};

	template<> struct BaseTimer1::Divider <0> { static const ClockDivider value = Div1;		enum {Div = 1}; };
	template<> struct BaseTimer1::Divider <1> { static const ClockDivider value = Div8;		enum {Div = 8}; };
	template<> struct BaseTimer1::Divider <2> { static const ClockDivider value = Div64;	enum {Div = 64}; };
	template<> struct BaseTimer1::Divider <3> { static const ClockDivider value = Div256;	enum {Div = 256}; };
	template<> struct BaseTimer1::Divider <4> { static const ClockDivider value = Div1024;	enum {Div = 1024}; };

#if defined(WGM10)

	class Timer1 : public BaseTimer1
	{		
		public:
	
		enum TimerMode
		{
			Normal 						= (0 << WGM13) | (0 << WGM12) | (0 << WGM11) | (0 << WGM10),
			PwmPhaseCorrect8Bit 		= (0 << WGM13) | (0 << WGM12) | (0 << WGM11) | (1 << WGM10),
			PwmPhaseCorrect9Bit 		= (0 << WGM13) | (0 << WGM12) | (1 << WGM11) | (0 << WGM10),
			PwmPhaseCorrect10Bit 		= (0 << WGM13) | (0 << WGM12) | (1 << WGM11) | (1 << WGM10),
			ClearOnMatchOcr1a 			= (0 << WGM13) | (1 << WGM12) | (0 << WGM11) | (0 << WGM10),
			PwmFast8Bit 				= (0 << WGM13) | (1 << WGM12) | (0 << WGM11) | (1 << WGM10),
			PwmFast9Bit 				= (0 << WGM13) | (1 << WGM12) | (1 << WGM11) | (0 << WGM10),
			PwmFast10Bit 				= (0 << WGM13) | (1 << WGM12) | (1 << WGM11) | (1 << WGM10),
			PwmPhaseAndFreqCorrectToIcr1 	= (1 << WGM13) | (0 << WGM12) | (0 << WGM11) | (0 << WGM10),
			PwmPhaseAndFreqCorrectToOcr1a 	= (1 << WGM13) | (0 << WGM12) | (0 << WGM11) | (1 << WGM10),
			PwmPhaseCorrectToIcr1 		= (1 << WGM13) | (0 << WGM12) | (1 << WGM11) | (0 << WGM10),
			PwmPhaseCorrectToOcr1a 		= (1 << WGM13) | (0 << WGM12) | (1 << WGM11) | (1 << WGM10),
			ClearOnMatchIcr1 			= (1 << WGM13) | (1 << WGM12) | (0 << WGM11) | (0 << WGM10),
			// Reserved 				= (1 << WGM13) | (1 << WGM12) | (0 << WGM11) | (1 << WGM10),
			PwmFastToIcr1 				= (1 << WGM13) | (1 << WGM12) | (1 << WGM11) | (0 << WGM10),
			PwmFastToOcr1a 				= (1 << WGM13) | (1 << WGM12) | (1 << WGM11) | (1 << WGM10)
		};

		enum {TCCR1AClearMask = ~((1 << WGM11) | (1 << WGM10))};
		enum {TCCR1AMask = ((1 << WGM11) | (1 << WGM10))};
		enum {TCCR1BClearMask = ~((1 << WGM12) | (1 << WGM13))};
		enum {TCCR1BMask = ((1 << WGM12) | (1 << WGM13))};

		static void SetMode(TimerMode mode)
		{
			TCCR1A = (TCCR1A & TCCR1AClearMask) | (mode & TCCR1AMask);
			TCCR1B = (TCCR1B & TCCR1BClearMask) | (mode & TCCR1BMask);
		}

		template<int number> class OutputCompare;
	};

	template<> class Timer1::OutputCompare<0>
	{
	public:
		static void Set(DataT val)
		{
			OCR1A = val;
		}

		static DataT Get()
		{
			return OCR1A;
		}

		static void EnableInterrupt()
		{
			InterruptMaskReg |= (1 << OCIE1A);
		}

		static bool IsInterrupt()
		{
			return InterruptFlagsReg & (1<<OCF1A);
		}
	
		static void ClearInterruptFlag()
		{
			InterruptFlagsReg |= (1<<OCF1A);
		}
	};
	
	template<> class Timer1::OutputCompare<1>
	{
	public:
		static void Set(DataT val)
		{
			OCR1B = val;
		}

		static DataT Get()
		{
			return OCR1B;
		}

		static void EnableInterrupt()
		{
			InterruptMaskReg |= (1 << OCIE1B);
		}

		static bool IsInterrupt()
		{
			return InterruptFlagsReg & (1<<OCF1B);
		}
	
		static void ClearInterruptFlag()
		{
			InterruptFlagsReg |= (1<<OCF1B);
		}
	};

#else
	class Timer1 : public BaseTimer1
	{
	};
#endif

#undef InterruptMaskReg 
#undef InterruptFlagsReg 


}
