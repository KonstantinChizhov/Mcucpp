
#include <__compatibility.h>
#include <ioreg.h>
#include <stdint.h>

namespace Mcucpp
{
	namespace Private
	{
#if defined(__AVR_ATxmega128D3__) || defined(__AVR_ATxmega256A3__) || defined(__AVR_ATxmega256A3B__)\
	|| defined(__AVR_ATxmega64A1__) || defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega16A4__)\
	|| defined(__AVR_ATxmega128A1__) || defined(__AVR_ATxmega64D3__) || defined(__AVR_ATxmega192D3__)\
	|| defined(__AVR_ATxmega256D3__) || defined(__AVR_ATxmega128A3__) || defined(__AVR_ATxmega192A3__)\
	|| defined(__AVR_ATxmega32D4__) || defined(__AVR_ATxmega32A4__) || defined(__AVR_ATxmega64A3__) ||\
	\
	defined(__ATxmega128D3__) || defined(__ATxmega256A3__) || defined(__ATxmega256A3B__)\
	|| defined(__ATxmega64A1__) || defined(__ATxmega16D4__) || defined(__ATxmega16A4__)\
	|| defined(__ATxmega128A1__) || defined(__ATxmega64D3__) || defined(__ATxmega192D3__)\
	|| defined(__ATxmega256D3__) || defined(__ATxmega128A3__) || defined(__ATxmega192A3__)\
	|| defined(__ATxmega32D4__) || defined(__ATxmega32A4__) || defined(__ATxmega64A3__)

		IO_REG_WRAPPER(SLEEP_CTRL, SleepCtrlReg, uint8_t);
		static const uint8_t SleepEnableMask = SLEEP_SEN_bm;
#elif defined(__AVR_ATtiny48__) || defined(__AVR_ATmega645A__) || defined(__AVR_ATmega324PA__)\
	|| defined(__AVR_ATmega324A__) || defined(__AVR_ATmega325P__) || defined(__AVR_AT90PWM216__)\
	|| defined(__AVR_ATmega168A__) || defined(__AVR_ATmega1280__) || defined(__AVR_ATmega3250P__)\
	|| defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega649__)\
	|| defined(__AVR_ATmega644__) || defined(__AVR_ATmega6450A__) || defined(__AVR_ATmega32M1__)\
	|| defined(__AVR_AT90SCR100__) || defined(__AVR_ATmega64C1__) || defined(__AVR_ATtiny88__)\
	|| defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB162__) || defined(__AVR_ATmega2561__)\
	|| defined(__AVR_ATmega329__) || defined(__AVR_AT90PWM81__) || defined(__AVR_ATmega640__)\
	|| defined(__AVR_AT90USB82__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega165__)\
	|| defined(__AVR_AT90USB1286__) || defined(__AVR_ATmega64M1__) || defined(__AVR_ATmega16U4__)\
	|| defined(__AVR_ATmega16M1__) || defined(__AVR_ATtiny167__) || defined(__AVR_AT90PWM1__)\
	|| defined(__AVR_ATmega48A__) || defined(__AVR_ATmega32C1__) || defined(__AVR_ATmega32U2__)\
	|| defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega644A__) || defined(__AVR_ATmega3250__)\
	|| defined(__AVR_ATmega32U4__) || defined(__AVR_ATtiny87__) || defined(__AVR_AT90PWM3__)\
	|| defined(__AVR_ATmega168P__) || defined(__AVR_ATmega3290__) || defined(__AVR_AT90PWM2B__)\
	|| defined(__AVR_ATmega645__) || defined(__AVR_ATA6289__) || defined(__AVR_ATmega16HVB__)\
	|| defined(__AVR_ATmega48__) || defined(__AVR_ATmega169__) || defined(__AVR_ATmega644P__)\
	|| defined(__AVR_ATmega6490P__) || defined(__AVR_ATmega169PA__) || defined(__AVR_ATmega16HVA2__)\
	|| defined(__AVR_ATmega2560__) || defined(__AVR_ATmega8U2__) || defined(__AVR_ATmega32HVB__)\
	|| defined(__AVR_ATmega88P__) || defined(__AVR_AT90USB1287__) || defined(__AVR_AT90PWM3B__)\
	|| defined(__AVR_ATmega329P__) || defined(__AVR_ATmega165P__) || defined(__AVR_AT90PWM2__)\
	|| defined(__AVR_ATmega324P__) || defined(__AVR_AT90CAN64__) || defined(__AVR_ATmega32U6__)\
	|| defined(__AVR_ATmega48P__) || defined(__AVR_ATmega164A__) || defined(__AVR_ATmega16HVA__)\
	|| defined(__AVR_ATmega6490__) || defined(__AVR_AT90CAN128__) || defined(__AVR_ATmega169P__)\
	|| defined(__AVR_ATmega6450P__) || defined(__AVR_AT90PWM316__) || defined(__AVR_ATmega169A__)\
	|| defined(__AVR_ATmega649P__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega1281__)\
	|| defined(__AVR_ATmega328P__) || defined(__AVR_ATmega64HVE__) || defined(__AVR_ATmega6450__)\
	|| defined(__AVR_AT90CAN32__) || defined(__AVR_ATmega325__) || defined(__AVR_ATmega406__)\
	|| defined(__AVR_ATmega645P__) || defined(__AVR_ATmega165A__) || defined(__AVR_ATmega8HVA__)\
	|| defined(__AVR_ATmega6490A__) || defined(__AVR_ATmega329PA__) || defined(__AVR_ATmega328__)\
	|| defined(__AVR_AT90USB647__) || defined(__AVR_ATmega649A__) || defined(__AVR_ATmega644PA__)\
	|| defined(__AVR_ATmega3290P__) || defined(__AVR_ATmega128RFA1__) || defined(__AVR_ATmega1284P__)\
	|| defined(__AVR_ATmega164P__) || \
	\
	defined(__ATtiny48__) || defined(__ATmega645A__) || defined(__ATmega324PA__)\
	|| defined(__ATmega324A__) || defined(__ATmega325P__) || defined(__AT90PWM216__)\
	|| defined(__ATmega168A__) || defined(__ATmega1280__) || defined(__ATmega3250P__)\
	|| defined(__ATmega88__) || defined(__ATmega88A__) || defined(__ATmega649__)\
	|| defined(__ATmega644__) || defined(__ATmega6450A__) || defined(__ATmega32M1__)\
	|| defined(__AT90SCR100__) || defined(__ATmega64C1__) || defined(__ATtiny88__)\
	|| defined(__AT90USB646__) || defined(__AT90USB162__) || defined(__ATmega2561__)\
	|| defined(__ATmega329__) || defined(__AT90PWM81__) || defined(__ATmega640__)\
	|| defined(__AT90USB82__) || defined(__ATmega168__) || defined(__ATmega165__)\
	|| defined(__AT90USB1286__) || defined(__ATmega64M1__) || defined(__ATmega16U4__)\
	|| defined(__ATmega16M1__) || defined(__ATtiny167__) || defined(__AT90PWM1__)\
	|| defined(__ATmega48A__) || defined(__ATmega32C1__) || defined(__ATmega32U2__)\
	|| defined(__ATmega88PA__) || defined(__ATmega644A__) || defined(__ATmega3250__)\
	|| defined(__ATmega32U4__) || defined(__ATtiny87__) || defined(__AT90PWM3__)\
	|| defined(__ATmega168P__) || defined(__ATmega3290__) || defined(__AT90PWM2B__)\
	|| defined(__ATmega645__) || defined(__ATA6289__) || defined(__ATmega16HVB__)\
	|| defined(__ATmega48__) || defined(__ATmega169__) || defined(__ATmega644P__)\
	|| defined(__ATmega6490P__) || defined(__ATmega169PA__) || defined(__ATmega16HVA2__)\
	|| defined(__ATmega2560__) || defined(__ATmega8U2__) || defined(__ATmega32HVB__)\
	|| defined(__ATmega88P__) || defined(__AT90USB1287__) || defined(__AT90PWM3B__)\
	|| defined(__ATmega329P__) || defined(__ATmega165P__) || defined(__AT90PWM2__)\
	|| defined(__ATmega324P__) || defined(__AT90CAN64__) || defined(__ATmega32U6__)\
	|| defined(__ATmega48P__) || defined(__ATmega164A__) || defined(__ATmega16HVA__)\
	|| defined(__ATmega6490__) || defined(__AT90CAN128__) || defined(__ATmega169P__)\
	|| defined(__ATmega6450P__) || defined(__AT90PWM316__) || defined(__ATmega169A__)\
	|| defined(__ATmega649P__) || defined(__ATmega16U2__) || defined(__ATmega1281__)\
	|| defined(__ATmega328P__) || defined(__ATmega64HVE__) || defined(__ATmega6450__)\
	|| defined(__AT90CAN32__) || defined(__ATmega325__) || defined(__ATmega406__)\
	|| defined(__ATmega645P__) || defined(__ATmega165A__) || defined(__ATmega8HVA__)\
	|| defined(__ATmega6490A__) || defined(__ATmega329PA__) || defined(__ATmega328__)\
	|| defined(__AT90USB647__) || defined(__ATmega649A__) || defined(__ATmega644PA__)\
	|| defined(__ATmega3290P__) || defined(__ATmega128RFA1__) || defined(__ATmega1284P__)\
	|| defined(__ATmega164P__)

		IO_REG_WRAPPER(SMCR, SleepCtrlReg, uint8_t);
		static const uint8_t SleepEnableMask = 1 << SE;
#elif defined(__AVR_ATmega162__) || defined(__AVR_AT90C8534__) || defined(__AVR_ATmega32__)\
	|| defined(__AVR_AT90S2313__) || defined(__AVR_AT90S8515__) || defined(__AVR_ATmega163__)\
	|| defined(__AVR_AT43USB320__) || defined(__AVR_ATtiny861A__) || defined(__AVR_ATtiny85__)\
	|| defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny24A__) || defined(__AVR_ATtiny2313A__)\
	|| defined(__AVR_AT90S2343__) || defined(__AVR_ATtiny84__) || defined(__AVR_ATmega8__)\
	|| defined(__AVR_ATtiny461__) || defined(__AVR_AT90S8535__) || defined(__AVR_AT90S4433__)\
	|| defined(__AVR_AT90S4434__) || defined(__AVR_ATmega16A__) || defined(__AVR_ATtiny15__)\
	|| defined(__AVR_AT76C711__) || defined(__AVR_ATmega128__) || defined(__AVR_ATtiny12__)\
	|| defined(__AVR_ATtiny4313__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega8515__)\
	|| defined(__AVR_ATmega64__) || defined(__AVR_AT43USB355__) || defined(__AVR_ATmega8535__)\
	|| defined(__AVR_ATtiny44A__) || defined(__AVR_AT90S2333__) || defined(__AVR_ATtiny26__)\
	|| defined(__AVR_ATtiny11__) || defined(__AVR_ATtiny13A__) || defined(__AVR_ATmega323__)\
	|| defined(__AVR_AT90S2323__) || defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny861__)\
	|| defined(__AVR_ATtiny43U__) || defined(__AVR_ATtiny261__) || defined(__AVR_AT90S4414__)\
	|| defined(__AVR_ATtiny22__) || defined(__AVR_ATmega161__) || defined(__AVR_AT90S1200__)\
	|| defined(__AVR_ATmega103__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny13__)\
	|| defined(__AVR_ATtiny461A__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny261A__)\
	|| defined(__AVR_ATtiny44__) || \
	\
	defined(__ATmega162__) || defined(__AT90C8534__) || defined(__ATmega32__)\
	|| defined(__AT90S2313__) || defined(__AT90S8515__) || defined(__ATmega163__)\
	|| defined(__AT43USB320__) || defined(__ATtiny861A__) || defined(__ATtiny85__)\
	|| defined(__ATtiny2313__) || defined(__ATtiny24A__) || defined(__ATtiny2313A__)\
	|| defined(__AT90S2343__) || defined(__ATtiny84__) || defined(__ATmega8__)\
	|| defined(__ATtiny461__) || defined(__AT90S8535__) || defined(__AT90S4433__)\
	|| defined(__AT90S4434__) || defined(__ATmega16A__) || defined(__ATtiny15__)\
	|| defined(__AT76C711__) || defined(__ATmega128__) || defined(__ATtiny12__)\
	|| defined(__ATtiny4313__) || defined(__ATmega16__) || defined(__ATmega8515__)\
	|| defined(__ATmega64__) || defined(__AT43USB355__) || defined(__ATmega8535__)\
	|| defined(__ATtiny44A__) || defined(__AT90S2333__) || defined(__ATtiny26__)\
	|| defined(__ATtiny11__) || defined(__ATtiny13A__) || defined(__ATmega323__)\
	|| defined(__AT90S2323__) || defined(__ATtiny24__) || defined(__ATtiny861__)\
	|| defined(__ATtiny43U__) || defined(__ATtiny261__) || defined(__AT90S4414__)\
	|| defined(__ATtiny22__) || defined(__ATmega161__) || defined(__AT90S1200__)\
	|| defined(__ATmega103__) || defined(__ATtiny25__) || defined(__ATtiny13__)\
	|| defined(__ATtiny461A__) || defined(__ATtiny45__) || defined(__ATtiny261A__)\
	|| defined(__ATtiny44__)

		IO_REG_WRAPPER(MCUCR, SleepCtrlReg, uint8_t);
		static const uint8_t SleepEnableMask = 1 << SE;
#else
	#error "The selected device is not supported"
#endif

#if defined(__AVR_ATmega161__) || defined(__ATmega161__)
	enum SleepMode
	{
		CpuOffMode = 0,			// Idle
		LowFreqMode = 0,		// Idle
		AsyncPeriphMode = 1,	// Power Save
		StandbyMode = 2,
		PowerDownMode = 2 // Power Down
	};
	
	inline void SetSleepMode(SleepMode mode)
	{
		MCUCR = (MCUCR & ~(1 << SM1)) | (mode == PowerDownMode || mode == AsyncPeriphMode ? (1 << SM1) : 0);
		EMCUCR = (EMCUCR & ~(1 << SM0)) | (mode == AsyncPeriphMode ? (1 << SM0) : 0);
	}

#elif defined(__AVR_ATmega162__) || defined(__AVR_ATmega8515__) || defined(__ATmega162__) || defined(__ATmega8515__)

	enum SleepMode
	{
		CpuOffMode = 0,			// Idle
		LowFreqMode = 0,		// Idle
		AsyncPeriphMode = 1,	// Power Save
		StandbyMode = 2,
		PowerDownMode = 3		// Power Down
	};
	inline void SetSleepMode(SleepMode mode)
	{
		MCUCR = ((MCUCR & ~(1 << SM1)) | (mode == CpuOffMode ? 0 : (1 << SM1)));
		MCUCSR = ((MCUCSR & ~(1 << SM2)) | (mode == StandbyMode ? (1 << SM2) : 0));
		EMCUCR = ((EMCUCR & ~(1 << SM0)) | (mode == AsyncPeriphMode || mode == StandbyMode ? (1 << SM0) : 0));
	}

#elif defined(__AVR_AT90S2313__) || defined(__AVR_AT90S2323__) || defined(__AVR_AT90S2333__) \
	|| defined(__AVR_AT90S2343__) || defined(__AVR_AT43USB320__) || defined(__AVR_AT43USB355__) \
	|| defined(__AVR_AT90S4414__) || defined(__AVR_AT90S4433__) || defined(__AVR_AT90S8515__) \
	|| defined(__AVR_ATtiny22__) || \
	\
	defined(__AT90S2313__) || defined(__AT90S2323__) || defined(__AT90S2333__) \
	|| defined(__AT90S2343__) || defined(__AT43USB320__) || defined(__AT43USB355__) \
	|| defined(__AT90S4414__) || defined(__AT90S4433__) || defined(__AT90S8515__) \
	|| defined(__ATtiny22__)

	enum SleepMode
	{
		CpuOffMode = 0,			// Idle
		LowFreqMode = 0,		// Idle
		AsyncPeriphMode = 0,	// Power Save
		StandbyMode = 1 << SM,
		PowerDownMode = 1 << SM	// Power Down
	};
	
	inline void SetSleepMode(SleepMode mode)
	{
		SleepCtrlReg::AndOr(~(1 << SM), mode);
	}

#elif defined(__AVR_ATtiny167__) || defined(__AVR_ATtiny87__) || defined(__ATtiny167__) || defined(__ATtiny87__)

	
	enum SleepMode
	{
		CpuOffMode = 0,			// Idle
		LowFreqMode = 0,		// Idle
		AsyncPeriphMode = 0,	// Power Save
		StandbyMode = 1 << SM1,
		PowerDownMode = 1 << SM1,	// Power Down
		AdcMode = 1 << SM0
	};
	
	inline void SetSleepMode(SleepMode mode)
	{
		SleepCtrlReg::AndOr(~((1 << SM0) | (1 << SM1)), mode);
	}

#elif defined(__AVR_AT90S4434__) || defined(__AVR_AT76C711__) || defined(__AVR_AT90S8535__) \
|| defined(__AVR_ATmega103__) || defined(__AVR_ATmega161__) || defined(__AVR_ATmega163__) \
|| defined(__AVR_ATmega16HVB__) || defined(__AVR_ATmega16HVBREVB__) || defined(__AVR_ATmega32HVB__) \
|| defined(__AVR_ATmega32HVBREVB__) || defined(__AVR_ATtiny13__) || defined(__AVR_ATtiny13A__) \
|| defined(__AVR_ATtiny15__) || defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny24A__) \
|| defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny44A__) || defined(__AVR_ATtiny84__) \
|| defined(__AVR_ATtiny84A__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) \
|| defined(__AVR_ATtiny48__) || defined(__AVR_ATtiny85__) || defined(__AVR_ATtiny261__) \
|| defined(__AVR_ATtiny261A__) || defined(__AVR_ATtiny461__) || defined(__AVR_ATtiny461A__) \
|| defined(__AVR_ATtiny861__) || defined(__AVR_ATtiny861A__) || defined(__AVR_ATtiny88__) \
|| defined(__AVR_ATtiny1634__) || \
\
defined(__AT90S4434__) || defined(__AT76C711__) || defined(__AT90S8535__) \
|| defined(__ATmega103__) || defined(__ATmega161__) || defined(__ATmega163__) \
|| defined(__ATmega16HVB__) || defined(__ATmega16HVBREVB__) || defined(__ATmega32HVB__) \
|| defined(__ATmega32HVBREVB__) || defined(__ATtiny13__) || defined(__ATtiny13A__) \
|| defined(__ATtiny15__) || defined(__ATtiny24__) || defined(__ATtiny24A__) \
|| defined(__ATtiny44__) || defined(__ATtiny44A__) || defined(__ATtiny84__) \
|| defined(__ATtiny84A__) || defined(__ATtiny25__) || defined(__ATtiny45__) \
|| defined(__ATtiny48__) || defined(__ATtiny85__) || defined(__ATtiny261__) \
|| defined(__ATtiny261A__) || defined(__ATtiny461__) || defined(__ATtiny461A__) \
|| defined(__ATtiny861__) || defined(__ATtiny861A__) || defined(__ATtiny88__) \
|| defined(__ATtiny1634__)

	enum SleepMode
	{
		CpuOffMode = 0,			// Idle
		LowFreqMode = 0,		// Idle
		AsyncPeriphMode = (1 << SM0) | (1 << SM1),	// Power Save
		StandbyMode = 1 << SM1,
		PowerDownMode = 1 << SM1,	// Power Down
		AdcMode = 1 << SM0
	};
	
	inline void SetSleepMode(SleepMode mode)
	{
		SleepCtrlReg::AndOr(~((1 << SM0) | (1 << SM1)), mode);
	}

#elif defined(__AVR_ATtiny2313__) || defined(__AVR_ATtiny2313A__) || defined(__AVR_ATtiny4313__) || \
	defined(__ATtiny2313__) || defined(__ATtiny2313A__) || defined(__ATtiny4313__)
	
	enum SleepMode
	{
		CpuOffMode = 0,			// Idle
		LowFreqMode = 0,		// Idle
		AsyncPeriphMode = 0,	// Power Save
		StandbyMode = 1 << SM1,
		PowerDownMode = (1 << SM0) | (1 << SM1),	// Power Down
	};
	
	inline void SetSleepMode(SleepMode mode)
	{
		SleepCtrlReg::AndOr(~((1 << SM0) | (1 << SM1)), mode);
	}

#elif defined(__AVR_ATtiny26__) || defined(__AVR_ATtiny43U__) || defined(__ATtiny26__) || defined(__ATtiny43U__)

	enum SleepMode
	{
		CpuOffMode = 0,			// Idle
		LowFreqMode = 0,		// Idle
		AsyncPeriphMode = 0,	// Power Save
		StandbyMode = (1 << SM0) | (1 << SM1),
		PowerDownMode = 1 << SM1,	// Power Down
		AdcMode = 1 << SM0
	};
	
	inline void SetSleepMode(SleepMode mode)
	{
		SleepCtrlReg::AndOr(~((1 << SM0) | (1 << SM1)), mode);
	}

#elif defined(__AVR_AT90PWM216__) || defined(__AVR_AT90PWM316__) || defined(__AVR_AT90PWM161__) || defined(__AVR_AT90PWM81__) || \
defined(__AT90PWM216__) || defined(__AT90PWM316__) || defined(__AT90PWM161__) || defined(__AT90PWM81__)
	
	enum SleepMode
	{
		CpuOffMode = 0,			// Idle
		LowFreqMode = 0,		// Idle
		AsyncPeriphMode = 0,	// Power Save
		StandbyMode = (1 << SM0) | (1 << SM2),
		PowerDownMode = 1 << SM1,	// Power Down
		AdcMode = 1 << SM0
	};
	
	inline void SetSleepMode(SleepMode mode)
	{
		SleepCtrlReg::AndOr(~((1 << SM0) | (1 << SM1( | (1 << SM2)), mode);
	}

#elif defined(__AVR_AT90CAN128__) || defined(__AVR_AT90CAN32__) || defined(__AVR_AT90CAN64__) \
|| defined(__AVR_AT90PWM1__) || defined(__AVR_AT90PWM2__) || defined(__AVR_AT90PWM2B__) \
|| defined(__AVR_AT90PWM3__) || defined(__AVR_AT90PWM3B__) || defined(__AVR_AT90USB162__) \
|| defined(__AVR_AT90USB82__) || defined(__AVR_AT90USB1286__) || defined(__AVR_AT90USB1287__) \
|| defined(__AVR_AT90USB646__) || defined(__AVR_AT90USB647__) || defined(__AVR_ATmega128__) \
|| defined(__AVR_ATmega1280__) || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega1284P__) \
|| defined(__AVR_ATmega128RFA1__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega16A__) \
|| defined(__AVR_ATmega162__) || defined(__AVR_ATmega164A__) || defined(__AVR_ATmega164P__) \
|| defined(__AVR_ATmega165__) || defined(__AVR_ATmega165A__) || defined(__AVR_ATmega165P__) \
|| defined(__AVR_ATmega168__) || defined(__AVR_ATmega168A__) || defined(__AVR_ATmega168P__) \
|| defined(__AVR_ATmega169__) || defined(__AVR_ATmega169A__) || defined(__AVR_ATmega169P__) \
|| defined(__AVR_ATmega169PA__) || defined(__AVR_ATmega16HVA__) || defined(__AVR_ATmega16HVA2__) \
|| defined(__AVR_ATmega16M1__) || defined(__AVR_ATmega16U2__) || defined(__AVR_ATmega16U4__) \
|| defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega32__) \
|| defined(__AVR_ATmega323__) || defined(__AVR_ATmega324A__) || defined(__AVR_ATmega324P__) \
|| defined(__AVR_ATmega324PA__) || defined(__AVR_ATmega325__) || defined(__AVR_ATmega325A__) \
|| defined(__AVR_ATmega325PA__) || defined(__AVR_ATmega3250__) || defined(__AVR_ATmega3250A__) \
|| defined(__AVR_ATmega3250PA__) || defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__) \
|| defined(__AVR_ATmega329__) || defined(__AVR_ATmega329A__) || defined(__AVR_ATmega329P__) \
|| defined(__AVR_ATmega329PA__) || defined(__AVR_ATmega3290__) || defined(__AVR_ATmega3290A__) \
|| defined(__AVR_ATmega3290P__) || defined(__AVR_ATmega3290PA__) || defined(__AVR_ATmega32C1__) \
|| defined(__AVR_ATmega32M1__) || defined(__AVR_ATmega32U2__) || defined(__AVR_ATmega32U4__) \
|| defined(__AVR_ATmega32U6__) || defined(__AVR_ATmega406__) || defined(__AVR_ATmega48__) \
|| defined(__AVR_ATmega48A__) || defined(__AVR_ATmega48PA__) || defined(__AVR_ATmega48P__) \
|| defined(__AVR_ATmega64__) || defined(__AVR_ATmega640__) || defined(__AVR_ATmega644__) \
|| defined(__AVR_ATmega644A__) || defined(__AVR_ATmega644P__) || defined(__AVR_ATmega644PA__) \
|| defined(__AVR_ATmega645__) || defined(__AVR_ATmega645A__) || defined(__AVR_ATmega645P__) \
|| defined(__AVR_ATmega6450__) || defined(__AVR_ATmega6450A__) || defined(__AVR_ATmega6450P__) \
|| defined(__AVR_ATmega649__) || defined(__AVR_ATmega649A__) || defined(__AVR_ATmega6490__) \
|| defined(__AVR_ATmega6490A__) || defined(__AVR_ATmega6490P__) || defined(__AVR_ATmega649P__) \
|| defined(__AVR_ATmega64C1__) || defined(__AVR_ATmega64HVE__) || defined(__AVR_ATmega64M1__) \
|| defined(__AVR_ATmega8__) || defined(__AVR_ATmega8515__) || defined(__AVR_ATmega8535__) \
|| defined(__AVR_ATmega88__) || defined(__AVR_ATmega88A__) || defined(__AVR_ATmega88P__) \
|| defined(__AVR_ATmega88PA__) || defined(__AVR_ATmega8HVA__) || defined(__AVR_ATmega8U2__) || \
\
 defined(__AT90CAN128__) || defined(__AT90CAN32__) || defined(__AT90CAN64__) \
|| defined(__AT90PWM1__) || defined(__AT90PWM2__) || defined(__AT90PWM2B__) \
|| defined(__AT90PWM3__) || defined(__AT90PWM3B__) || defined(__AT90USB162__) \
|| defined(__AT90USB82__) || defined(__AT90USB1286__) || defined(__AT90USB1287__) \
|| defined(__AT90USB646__) || defined(__AT90USB647__) || defined(__ATmega128__) \
|| defined(__ATmega1280__) || defined(__ATmega1281__) || defined(__ATmega1284P__) \
|| defined(__ATmega128RFA1__) || defined(__ATmega16__) || defined(__ATmega16A__) \
|| defined(__ATmega162__) || defined(__ATmega164A__) || defined(__ATmega164P__) \
|| defined(__ATmega165__) || defined(__ATmega165A__) || defined(__ATmega165P__) \
|| defined(__ATmega168__) || defined(__ATmega168A__) || defined(__ATmega168P__) \
|| defined(__ATmega169__) || defined(__ATmega169A__) || defined(__ATmega169P__) \
|| defined(__ATmega169PA__) || defined(__ATmega16HVA__) || defined(__ATmega16HVA2__) \
|| defined(__ATmega16M1__) || defined(__ATmega16U2__) || defined(__ATmega16U4__) \
|| defined(__ATmega2560__) || defined(__ATmega2561__) || defined(__ATmega32__) \
|| defined(__ATmega323__) || defined(__ATmega324A__) || defined(__ATmega324P__) \
|| defined(__ATmega324PA__) || defined(__ATmega325__) || defined(__ATmega325A__) \
|| defined(__ATmega325PA__) || defined(__ATmega3250__) || defined(__ATmega3250A__) \
|| defined(__ATmega3250PA__) || defined(__ATmega328__) || defined(__ATmega328P__) \
|| defined(__ATmega329__) || defined(__ATmega329A__) || defined(__ATmega329P__) \
|| defined(__ATmega329PA__) || defined(__ATmega3290__) || defined(__ATmega3290A__) \
|| defined(__ATmega3290P__) || defined(__ATmega3290PA__) || defined(__ATmega32C1__) \
|| defined(__ATmega32M1__) || defined(__ATmega32U2__) || defined(__ATmega32U4__) \
|| defined(__ATmega32U6__) || defined(__ATmega406__) || defined(__ATmega48__) \
|| defined(__ATmega48A__) || defined(__ATmega48PA__) || defined(__ATmega48P__) \
|| defined(__ATmega64__) || defined(__ATmega640__) || defined(__ATmega644__) \
|| defined(__ATmega644A__) || defined(__ATmega644P__) || defined(__ATmega644PA__) \
|| defined(__ATmega645__) || defined(__ATmega645A__) || defined(__ATmega645P__) \
|| defined(__ATmega6450__) || defined(__ATmega6450A__) || defined(__ATmega6450P__) \
|| defined(__ATmega649__) || defined(__ATmega649A__) || defined(__ATmega6490__) \
|| defined(__ATmega6490A__) || defined(__ATmega6490P__) || defined(__ATmega649P__) \
|| defined(__ATmega64C1__) || defined(__ATmega64HVE__) || defined(__ATmega64M1__) \
|| defined(__ATmega8__) || defined(__ATmega8515__) || defined(__ATmega8535__) \
|| defined(__ATmega88__) || defined(__ATmega88A__) || defined(__ATmega88P__) \
|| defined(__ATmega88PA__) || defined(__ATmega8HVA__) || defined(__ATmega8U2__)

	enum SleepMode
	{
		CpuOffMode = 0,			// Idle
		LowFreqMode = 0,		// Idle
		AsyncPeriphMode = (1 << SM0) | (1 << SM1),	// Power Save
		StandbyMode = (1 << SM0) | (1 << SM2) | (1 << SM2),
		PowerDownMode = 1 << SM1,	// Power Down
		AdcMode = 1 << SM0
	};
	
	inline void SetSleepMode(SleepMode mode)
	{
		SleepCtrlReg::AndOr(~((1 << SM0) | (1 << SM1) | (1 << SM2)), mode);
	}

#elif defined(__AVR_ATxmega16A4__) || defined(__AVR_ATxmega16D4__) || defined(__AVR_ATxmega32A4__) \
|| defined(__AVR_ATxmega32D4__) || defined(__AVR_ATxmega32X1__) || defined(__AVR_ATxmega64A1__) \
|| defined(__AVR_ATxmega64A1U__) || defined(__AVR_ATxmega64A3__) || defined(__AVR_ATxmega64D3__) \
|| defined(__AVR_ATxmega128A1__) || defined(__AVR_ATxmega128A1U__) || defined(__AVR_ATxmega128A3__) \
|| defined(__AVR_ATxmega128B1__) || defined(__AVR_ATxmega128D3__) || defined(__AVR_ATxmega192A3__) \
|| defined(__AVR_ATxmega192D3__) || defined(__AVR_ATxmega256A3__) || defined(__AVR_ATxmega256D3__) \
|| defined(__AVR_ATxmega256A3B__) || defined(__AVR_ATxmega256A3BU__) || \
\
defined(__ATxmega16A4__) || defined(__ATxmega16D4__) || defined(__ATxmega32A4__) \
|| defined(__ATxmega32D4__) || defined(__ATxmega32X1__) || defined(__ATxmega64A1__) \
|| defined(__ATxmega64A1U__) || defined(__ATxmega64A3__) || defined(__ATxmega64D3__) \
|| defined(__ATxmega128A1__) || defined(__ATxmega128A1U__) || defined(__ATxmega128A3__) \
|| defined(__ATxmega128B1__) || defined(__ATxmega128D3__) || defined(__ATxmega192A3__) \
|| defined(__ATxmega192D3__) || defined(__ATxmega256A3__) || defined(__ATxmega256D3__) \
|| defined(__ATxmega256A3B__) || defined(__ATxmega256A3BU__)

	enum SleepMode
	{
		CpuOffMode = 0,			// Idle
		LowFreqMode = 0,		// Idle
		AsyncPeriphMode = SLEEP_SMODE1_bm | SLEEP_SMODE0_bm,	// Power Save
		StandbyMode = SLEEP_SMODE2_bm | SLEEP_SMODE1_bm | SLEEP_SMODE0_bm,
		PowerDownMode = SLEEP_SMODE1_bm,	// Power Down
	};
	
	inline void SetSleepMode(SleepMode mode)
	{
		SleepCtrlReg::AndOr(~(SLEEP_SMODE2_bm | SLEEP_SMODE1_bm | SLEEP_SMODE0_bm), mode);
	}

#else
    #error #error "The selected device is not supported"
#endif

	inline void Sleep(SleepMode mode)
	{
		SetSleepMode(mode);
		SleepCtrlReg::Or(SleepEnableMask);
		do{
			asm("sleep");
		}
		while(0);
		SleepCtrlReg::And(~SleepEnableMask);
	}
}
	
	inline void Power::CpuOff()
	{
		Private::Sleep(Private::CpuOffMode);
	}
	
	inline void Power::SleepLowFreq()
	{
		// TODO: set lower cpu freq if supported
		Private::Sleep(Private::LowFreqMode);
	}
	
	inline void Power::AsyncPeriphOlny()
	{
		Private::Sleep(Private::AsyncPeriphMode);
	}
	
	inline void Power::Standby()
	{
		Private::Sleep(Private::StandbyMode);
	}
	
	inline void Power::PowerDown()
	{
		Private::Sleep(Private::PowerDownMode);
	}
}
