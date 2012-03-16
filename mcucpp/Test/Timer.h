
namespace Timers
{

	class Timer1
	{
	public:
		typedef uint16_t DataT;
		enum {MaxValue = 0xff};
		enum ClockDivider
		{
			DivStop=0,
			Div1,
			Div8,
			Div64,
			Div256,
			Div1024,
			ExtFalling,
			ExtRising
		};

		template<unsigned Number> struct Divider;
		enum {MaxDivider = 4};

		static void Set(DataT val)
		{
			Counter = val;
		}

		static DataT Get()
		{
			return Counter;
		}

		static void Stop()
		{
			CurrentDivider = DivStop;
		}

		static void Clear()
		{
			Counter = 0;
		}

		static void Start(ClockDivider divider)
		{
			CurrentDivider = divider;
		}

		static void EnableInterrupt()
		{

		}

		static bool IsInterrupt()
		{
			return false;
		}

		static void ClearInterruptFlag()
		{

		}

		enum TimerMode
		{
			Normal = 0,
			PwmPhaseCorrect8Bit,
			PwmPhaseCorrect9Bit,
			PwmPhaseCorrect10Bit,
			ClearOnMatchOcr1,
			PwmFast8Bit,
			PwmFast9Bit,
			PwmFast10Bit,
			ClearOnMatchOcr2 ,
			PwmFastToOcr1,
			PwmFastToOcr2
		};

		static void SetMode(TimerMode mode)
		{

		}

		template<int number> class OutputCompare;

		static volatile DataT Counter;
		static ClockDivider CurrentDivider;
	};

	template<> class Timer1::OutputCompare<0>
	{
	public:
		static void Set(DataT val)
		{
			CompareValue = val;
		}

		static DataT Get()
		{
			return CompareValue;
		}

		static void EnableInterrupt()
		{

		}

		static bool IsInterrupt()
		{
			return false;
		}

		static void ClearInterruptFlag()
		{

		}

		static DataT CompareValue;
	};

	template<> class Timer1::OutputCompare<1>
	{
	public:
		static void Set(DataT val)
		{
			CompareValue = val;
		}

		static DataT Get()
		{
			return CompareValue;
		}

		static void EnableInterrupt()
		{

		}

		static bool IsInterrupt()
		{
			return false;
		}

		static void ClearInterruptFlag()
		{

		}

		static DataT CompareValue;
	};

	volatile Timer1::DataT Timer1::Counter = 0;
	Timer1::DataT Timer1::OutputCompare<0>::CompareValue = 0;
	Timer1::DataT Timer1::OutputCompare<1>::CompareValue = 0;

	template<> struct Timer1::Divider <0> { static const ClockDivider value = Div1;		enum {Div = 1}; };
	template<> struct Timer1::Divider <1> { static const ClockDivider value = Div8;		enum {Div = 8}; };
	template<> struct Timer1::Divider <2> { static const ClockDivider value = Div64;	enum {Div = 64}; };
	template<> struct Timer1::Divider <3> { static const ClockDivider value = Div256;	enum {Div = 256}; };
	template<> struct Timer1::Divider <4> { static const ClockDivider value = Div1024;	enum {Div = 1024}; };

} // namespace Timers
