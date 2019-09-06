#include <clock.h>

namespace Mcucpp
{
typedef uint32_t clock_freq_t;
namespace Clock
{

bool ClockBase::EnableClockSource(unsigned turnOnMask, unsigned waitReadyMask)
{
	uint32_t timeoutCounter = 0;
	RCC->CR |= turnOnMask;
	while (((RCC->CR & waitReadyMask) == 0) && (timeoutCounter < ClockStartTimeout))
	{
		timeoutCounter++;
	}
	return (RCC->CR & waitReadyMask) != 0;
}

bool ClockBase::DisablelockSource(unsigned turnOnMask, unsigned waitReadyMask)
{
	uint32_t timeoutCounter = 0;
	RCC->CR &= ~turnOnMask;
	while (((RCC->CR & waitReadyMask) != 0) && (timeoutCounter < ClockStartTimeout))
	{
		timeoutCounter++;
	}
	return (RCC->CR & waitReadyMask) == 0;
}

clock_freq_t PllClock::SrcClockFreq()
{
	if ((RCC->CFGR & RCC_CFGR_PLLSRC) == 0)
		return HsiClock::ClockFreq();
	else
		return HseClock::ClockFreq();
}

clock_freq_t PllClock::GetDivider()
{
	if ((RCC->CFGR & RCC_CFGR_PLLSRC) == 0)
		return 2;
	else
#if defined(STM32F10X_CL)
		return (RCC->CFGR2 & RCC_CFGR2_PREDIV1) + 1;
#else
		return ((RCC->CFGR & RCC_CFGR_PLLXTPRE) >> 17) + 1;
#endif
}

clock_freq_t PllClock::GetMultipler()
{
#if defined(STM32F10X_CL)
	clock_freq_t mul = ((RCC->CFGR & RCC_CFGR_PLLMULL) >> 18);
	if (mul == 13)
		return 6;
	return mul + 2;
#else
	return ((RCC->CFGR & RCC_CFGR_PLLMULL) >> 18) + 2;
#endif
}

void PllClock::SetMultipler(uint8_t multiler)
{

#if defined(STM32F10X_CL)
	if (multiler > 9)
		multiler = 9;
	if (multiler < 4)
		multiler = 4;
#else
	if (multiler > 16)
		multiler = 16;
#endif
	multiler -= 2;
	RCC->CFGR = (RCC->CFGR & RCC_CFGR_PLLMULL) | (multiler << 18);
}

void PllClock::SetDivider(uint8_t divider)
{
#if defined(STM32F10X_CL)
	if (divider > 15)
		divider = 15;
	divider -= 1;
	RCC->CFGR2 = (RCC->CFGR2 & ~RCC_CFGR2_PREDIV1) | (divider);
#else
	if (divider > 2)
		divider = 2;
	divider -= 1;
	RCC->CFGR = (RCC->CFGR & RCC_CFGR_PLLXTPRE) | (divider << 17);
#endif
}

void PllClock::SelectClockSource(ClockSource clockSource)
{
	RCC->CFGR = (RCC->CFGR & ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE)) | clockSource;
}

clock_freq_t PllClock::ClockFreq()
{
	return SrcClockFreq() / GetDivider() * GetMultipler();
}

bool PllClock::Enable()
{
	if ((RCC->CFGR & RCC_CFGR_PLLSRC) == 0)
	{
		if (!HsiClock::Enable())
			return false;
	}
	else if (!HseClock::Enable())
		return false;
	return ClockBase::EnableClockSource(RCC_CR_PLLON, RCC_CR_PLLRDY);
}

void PllClock::Disable()
{
	ClockBase::DisablelockSource(RCC_CR_PLLON, RCC_CR_PLLRDY);
}

clock_freq_t PllClock::SetClockFreq(clock_freq_t freq)
{
	if (freq > SysClock::MaxFreq())
		freq = SysClock::MaxFreq();

	uint32_t resPllDiv = 0,
			 resPllMul = 0;

	uint32_t srcFreq = SrcClockFreq();

#if defined(STM32F10X_CL)
	resPllDiv = 16;
#else
	resPllDiv = 2;
#endif

	while (resPllDiv > 1)
	{
		uint32_t pllMul = resPllDiv * freq / srcFreq;

#if defined(STM32F10X_CL)
		if (pllMul <= 9 && pllMul >= 4)
		{
			resPllMul = pllMul;
			break;
		}
#else
		if (pllMul <= 16)
		{
			resPllMul = pllMul;
			break;
		}
#endif
		resPllDiv--;
	}

	SetDivider(resPllDiv);
	SetMultipler(resPllMul);

	return ClockFreq();
}

clock_freq_t SysClock::MaxFreq()
{
	return F_CPU;
}

inline SysClock::ErrorCode SysClock::SelectClockSource(ClockSource clockSource)
{
	uint32_t clockStatusValue;
	uint32_t clockSelectMask;

	if (clockSource == Internal)
	{
		clockStatusValue = RCC_CFGR_SWS_HSI;
		clockSelectMask = RCC_CFGR_SW_HSI;
		if (!HsiClock::Enable())
			return ClockSourceFailed;
	}
	else if (clockSource == External)
	{
		clockStatusValue = RCC_CFGR_SWS_HSE;
		clockSelectMask = RCC_CFGR_SW_HSE;
		if (!HseClock::Enable())
			return ClockSourceFailed;
	}
	else if (clockSource == Pll)
	{
		clockStatusValue = RCC_CFGR_SWS_PLL;
		clockSelectMask = RCC_CFGR_SW_PLL;
		if (!PllClock::Enable())
			return ClockSourceFailed;
	}
	else
		return InvalidClockSource;

	RCC->CFGR = (RCC->CFGR & ~RCC_CFGR_SW) | clockSelectMask;

	uint32_t timeout = 10000;
	while ((RCC->CFGR & RCC_CFGR_SWS) != clockStatusValue && --timeout)
	{
		;
	}
	if (timeout == 0)
	{
		return ClockSelectFailed;
	}
	return Success;
}

clock_freq_t SysClock::ClockFreq()
{
	uint32_t clockSrc = RCC->CFGR & RCC_CFGR_SWS;
	switch (clockSrc)
	{
	case 0:
		return HsiClock::ClockFreq();
	case RCC_CFGR_SWS_0:
		return HseClock::ClockFreq();
	case RCC_CFGR_SWS_1:
		return PllClock::ClockFreq();
	}
	return 0;
}

clock_freq_t SysClock::SetClockFreq(clock_freq_t freq)
{
	SelectClockSource(Internal);
#if defined(FLASH_ACR_LATENCY)
	FLASH->ACR &= ~FLASH_ACR_LATENCY;
#endif
	PllClock::Disable();
	PllClock::SelectClockSource(PllClock::External);
	PllClock::SetClockFreq(freq);
#if defined(FLASH_ACR_LATENCY)
	if (freq > 48000000)
	{
		FLASH->ACR |= FLASH_ACR_LATENCY_2;
	}
	else if (freq > 24000000)
	{
		FLASH->ACR |= FLASH_ACR_LATENCY_1;
	}
#endif
	SelectClockSource(Pll);
	return ClockFreq();
}

} // namespace Clock
} // namespace Mcucpp