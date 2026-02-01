//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2025
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

#pragma once

#include "ioreg.h"
#include "K1921VG015.h"
#include <algorithm>

#ifndef F_OSC
#warning F_OSC is not defined. F_OSC is in its default value 8 MHZ. Verify that external cristal freq is correct.
#define F_OSC 16000000u
#endif

namespace Mcucpp
{
	namespace Clock
	{
		enum class PllClockSource
		{
			Internal = 0,
			External = 1
		};

		enum class SysClockSource
		{
			Lsi = 3,
			Internal = 0,
			External = 1,
			Pll = 2
		};

		enum class ClockErrorCode
		{
			Success = 0,
			ClockSourceFailed = 1,
			InvalidClockSource = 2,
			ClockSelectFailed = 3
		};

		class ClockBase
		{
		protected:
			static constexpr uint32_t ClockStartTimeout = 100000;
		};

		class HseClock : public ClockBase
		{
		public:
			static uint32_t SrcClockFreq() { return F_OSC; }
			static uint32_t SetClockFreq(uint32_t) { return ClockFreq(); }
			static uint32_t ClockFreq() { return SrcClockFreq(); }
			static bool Enable() { return true; } // always enabled
			static bool Disable() { return false; }
			static bool Enabled() { return true; }
		};

		class HsiClock : public ClockBase
		{
		public:
			static uint32_t SrcClockFreq() { return 1'000'000u; }
			static uint32_t SetClockFreq(uint32_t) { return ClockFreq(); }
			static uint32_t ClockFreq() { return SrcClockFreq(); }
			static bool Enable() { return true; }
			static bool Disable() { return false; }
			static bool Enabled() { return true; }
		};

		class LsiClock : public ClockBase
		{
		public:
			static uint32_t SrcClockFreq() { return 32768u; }
			static uint32_t SetClockFreq(uint32_t) { return ClockFreq(); }
			static uint32_t ClockFreq() { return SrcClockFreq(); }
			static bool Enable() { return true; }
			static bool Disable() { return false; }
			static bool Enabled() { return true; }
		};

		struct PllParams
		{
			uint32_t best_freq;
			uint8_t FBDIV;
			uint8_t REFDIV;
			uint8_t PD0A;
			uint8_t PD0B;
		};

		class PllClock : public ClockBase
		{
		public:
			// PLL clock limits
			static const uint32_t VCO_MAX = 1'600'000'000ul;
			static const uint32_t VCO_MIN = 200'000'000ul;
			static const uint32_t REFDIV_MAX = 63ul;
			static const uint32_t REFDIV_MIN = 1ul;
			static const uint32_t FBDIV_MAX = 160ul;
			static const uint32_t FBDIV_MIN = 20ul;
			static const uint32_t PD0A_MAX = 8ul;
			static const uint32_t PD0A_MIN = 1ul;
			static const uint32_t PD0B_MAX = 64ul;
			static const uint32_t PD0B_MIN = 1ul;

			static const uint32_t PllMaxFreq = 60'000'000;
			static const uint32_t PllMinFreq = 390'000ul;

			static inline uint32_t SrcClockFreq();
			static inline void SelectClockSource(PllClockSource clockSource);
			static inline uint32_t SetClockFreq(uint32_t freq);
			static inline uint32_t ClockFreq();
			static inline bool Enable();
			static inline void Disable();
			static bool Enabled() { return (RCU->PLLSYSCFG0 & RCU_PLLSYSCFG0_PLLEN_Msk) != 0; }
			static inline PllParams CalcParams(uint32_t target_freq);
		};

		class SysClock
		{
			static constexpr int cycles_to_ns_shift = 4;
			static inline uint32_t clock_freq = 0;
			static inline uint32_t cycle_period_ns = (1'000'000'000ull << cycles_to_ns_shift) / F_CPU;
		public:
			static uint32_t MaxFreq() { return 60'000'000; }
			static inline ClockErrorCode SelectClockSource(SysClockSource clockSource);
			static inline uint32_t SetClockFreq(uint32_t freq);
			static inline uint32_t ClockFreq(bool force_update = false);
			static inline uint32_t SrcClockFreq();
			static inline uint64_t CyclesToNs(uint64_t cycles) { return (cycles * cycle_period_ns) >> cycles_to_ns_shift;}
		};
		

		template <class Reg, unsigned Mask, class ResetReg, unsigned ResetMask, class ClockSrc>
		class ClockResetControl : public ClockSrc
		{
		public:
			static inline void Enable()
			{
				Reg::Or(Mask);
			}
			static inline void Disable()
			{
				Reg::And(~Mask);
			}
			static inline void Reset()
			{
				ResetReg::Or(ResetMask);
				ResetReg::And(~ResetMask);
			}
			static inline void SelectClockSource(SysClockSource clockSource);
		};

		using HClk = SysClock;
		using PClk = SysClock;

		uint32_t PllClock::SrcClockFreq()
		{
			if (RCU->PLLSYSCFG3_bit.REFSEL == 0)
			{
				return HsiClock::ClockFreq();
			}
			return HseClock::ClockFreq();
		}

		void PllClock::SelectClockSource(PllClockSource clockSource)
		{
			RCU->PLLSYSCFG3_bit.REFSEL = (uint32_t)clockSource;
		}

		PllParams PllClock::CalcParams(uint32_t target_freq)
		{
			uint32_t best_error = target_freq;
			uint32_t best_freq = 0;
			uint32_t best_FBDIV = 0, best_REFDIV = 0, best_PD0A = 0, best_PD0B = 0;
			uint32_t clock_src = SrcClockFreq();

			for (uint32_t refdiv = REFDIV_MIN; refdiv <= REFDIV_MAX; refdiv++)
			{
				for (uint32_t pd0a = PD0A_MAX - 1; pd0a >= PD0A_MIN; pd0a--)
				{
					for (uint32_t pd0b = PD0B_MIN; pd0b <= PD0B_MAX; pd0b++)
					{
						uint32_t divider = (1 + pd0a) * (1 + pd0b);
						uint32_t required_fVCO = target_freq * divider;

						if (required_fVCO < VCO_MIN || required_fVCO > VCO_MAX)
							continue;

						uint32_t fbdiv = required_fVCO / (clock_src / refdiv);

						if (fbdiv < FBDIV_MIN || fbdiv > FBDIV_MAX)
							continue;

						uint32_t real_fVCO = clock_src / refdiv * fbdiv;

						uint32_t real_pll_clock = real_fVCO / divider;

						uint32_t error = (real_pll_clock > target_freq) ? real_pll_clock - target_freq : target_freq - real_pll_clock;

						if (error < best_error)
						{
							best_freq = real_pll_clock;
							best_error = error;
							best_FBDIV = fbdiv;
							best_REFDIV = refdiv;
							best_PD0A = pd0a;
							best_PD0B = pd0b;
						}

						if (error == 0)
							return PllParams{best_freq, (uint8_t)best_FBDIV, (uint8_t)best_REFDIV, (uint8_t)best_PD0A, (uint8_t)best_PD0B};
					}
				}
			}

			return PllParams{best_freq, (uint8_t)best_FBDIV, (uint8_t)best_REFDIV, (uint8_t)best_PD0A, (uint8_t)best_PD0B};
		}
		uint32_t PllClock::SetClockFreq(uint32_t target_freq)
		{
			if (Enabled())
				return 0;

			if (target_freq > PllMaxFreq)
				target_freq = PllMaxFreq;

			auto params = CalcParams(target_freq);
			if (!params.best_freq)
				return 0;

			RCU->PLLSYSCFG0 = (0 << RCU_PLLSYSCFG0_PD1B_Pos) |				 // PD1B
							  (0 << RCU_PLLSYSCFG0_PD1A_Pos) |				 // PD1A
							  (params.PD0B << RCU_PLLSYSCFG0_PD0B_Pos) |	 // PD0B
							  (params.PD0A << RCU_PLLSYSCFG0_PD0A_Pos) |	 // PD0A
							  (params.REFDIV << RCU_PLLSYSCFG0_REFDIV_Pos) | // refdiv
							  (0 << RCU_PLLSYSCFG0_FOUTEN_Pos) |			 // fouten
							  (0 << RCU_PLLSYSCFG0_DSMEN_Pos) |				 // dsmen
							  (0 << RCU_PLLSYSCFG0_DACEN_Pos) |				 // dacen
							  (0 << RCU_PLLSYSCFG0_BYP_Pos) |				 // bypass
							  (1 << RCU_PLLSYSCFG0_PLLEN_Pos);				 // en

			RCU->PLLSYSCFG1 = 0; // FRAC = 0
			RCU->PLLSYSCFG2_bit.FBDIV = params.FBDIV;

			return params.best_freq;
		}

		uint32_t PllClock::ClockFreq()
		{
			uint32_t fbdiv = RCU->PLLSYSCFG2_bit.FBDIV;
			uint32_t refdiv = RCU->PLLSYSCFG0_bit.REFDIV;
			uint32_t pd0a = RCU->PLLSYSCFG0_bit.PD0A;
			uint32_t pd0b = RCU->PLLSYSCFG0_bit.PD0B;
			uint32_t frac = RCU->PLLSYSCFG0_bit.DSMEN ? RCU->PLLSYSCFG1_bit.FRAC << 8 : 0; // shift 8 for extra precision with 32 bit fraction

			uint32_t src_clk = SrcClockFreq();

			uint32_t divider = refdiv * (1 + pd0a) * (1 + pd0b);
			if (divider == 0)
				return 0;

			uint32_t fract_add = ((uint64_t)(frac / divider) * src_clk) >> 32;
			uint32_t clock = src_clk * fbdiv / divider + fract_add;

			return clock;
		}

		bool PllClock::Enable()
		{
			if (RCU->PLLSYSCFG3_bit.REFSEL == 0)
			{
				if (!HsiClock::Enable())
					return false;
			}
			else if (!HseClock::Enable())
				return false;

			RCU->PLLSYSCFG0_bit.FOUTEN = 1;

			uint32_t timeoutCounter = 0;

			while ((RCU->PLLSYSSTAT_bit.LOCK) != 1 && (timeoutCounter < ClockStartTimeout))
			{
				timeoutCounter++;
			}
			return RCU->PLLSYSSTAT_bit.LOCK;
		}

		void PllClock::Disable()
		{
			RCU->PLLSYSCFG0_bit.PLLEN = 0;
		}

		ClockErrorCode SysClock::SelectClockSource(SysClockSource clockSource)
		{
			uint32_t currentFreq = ClockFreq();
			uint32_t targetFreq;

			if (clockSource == SysClockSource::Lsi)
			{
				if (!LsiClock::Enable())
				{
					return ClockErrorCode::ClockSourceFailed;
				}
				targetFreq = LsiClock::ClockFreq();
			}
			else if (clockSource == SysClockSource::Internal)
			{
				if (!HsiClock::Enable())
				{
					return ClockErrorCode::ClockSourceFailed;
				}
				targetFreq = HsiClock::ClockFreq();
			}
			else if (clockSource == SysClockSource::External)
			{
				if (!HseClock::Enable())
				{
					return ClockErrorCode::ClockSourceFailed;
				}
				targetFreq = HseClock::ClockFreq();
			}
			else if (clockSource == SysClockSource::Pll)
			{
				if (!PllClock::Enable())
				{
					return ClockErrorCode::ClockSourceFailed;
				}
				targetFreq = PllClock::ClockFreq();
			}
			else
				return ClockErrorCode::InvalidClockSource;

			if (currentFreq < targetFreq)
			{
				FLASH->CTRL_bit.LAT = 3;
				FLASH->CTRL_bit.CEN = 1;
			}

			RCU->SYSCLKCFG = ((uint32_t)clockSource << RCU_SYSCLKCFG_SRC_Pos);

			uint32_t timeout = 100000;
			while ((RCU->CLKSTAT_bit.SRC != RCU->SYSCLKCFG_bit.SRC) && --timeout)
			{
			}
			if (timeout == 0)
			{
				return ClockErrorCode::ClockSelectFailed;
			}
			if (currentFreq > targetFreq)
			{
				// Flash::ConfigureFreq(targetFreq);
			}
			return ClockErrorCode::Success;
		}

		uint32_t SysClock::SetClockFreq(uint32_t freq)
		{
			SelectClockSource(SysClockSource::Internal);
			PllClock::Disable();
			PllClock::SelectClockSource(PllClockSource::External);
			if (PllClock::SetClockFreq(freq) > 0)
			{
				SelectClockSource(SysClockSource::Pll);
			}

			return ClockFreq(true);
		}

		uint32_t SysClock::ClockFreq(bool force_update)
		{
			if (clock_freq > 0 && !force_update)
				return clock_freq;

			switch ((SysClockSource)RCU->CLKSTAT_bit.SRC)
			{
			case SysClockSource::Lsi:
				clock_freq = LsiClock::ClockFreq();
				break;
			case SysClockSource::Internal:
				clock_freq = HsiClock::ClockFreq();
				break;
			case SysClockSource::External:
				clock_freq = HseClock::ClockFreq();
				break;
			case SysClockSource::Pll:
				clock_freq = PllClock::ClockFreq();
				break;
			}
			cycle_period_ns = (1'000'000'000ull << cycles_to_ns_shift) / clock_freq;
			return clock_freq;
		}

		uint32_t SysClock::SrcClockFreq()
		{
			return ClockFreq();
		}
	}
}
