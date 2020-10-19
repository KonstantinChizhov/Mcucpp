//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2016
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
#include "mcu_header.h"
#include "clock.h"

namespace Mcucpp
{
	namespace Timers
	{
		namespace Private
		{
			class TimerBase
			{
			public:
				enum TimerMode
				{
					Normal = 0,
					OnePulse = TIM_CR1_OPM,
					Downcounting = TIM_CR1_DIR,
					UpdateDisable = TIM_CR1_UDIS
				};

				enum CapturePolarity
				{
					RisingEdge = 0,
					FallingEdge = TIM_CCER_CC1P,
					BothEdges = TIM_CCER_CC1P | TIM_CCER_CC1NP
				};

				enum CaptureMode
				{
					CaptureInput1 = TIM_CCMR1_CC1S_0,
					CaptureInput2 = TIM_CCMR1_CC1S_1,
					CaptureTrc = TIM_CCMR1_CC1S_0 | TIM_CCMR1_CC1S_1
				};

				enum OutPolarity
				{
					ActiveHigh = 0,
					ActiveLow = TIM_CCER_CC1P
				};

				enum OutputMode
				{
					Disconnected = 0,
					TogleOutput = TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1,
					ClearOutput = TIM_CCMR1_OC1M_1,
					SetOutput = TIM_CCMR1_OC1M_0,
					Pwm = TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1,
					InvertedPwm = TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2
				};

				typedef uint16_t DataT;
				static const uint16_t MaxValue = 0xffff;
				static const uint16_t MaxDivider = 0xffff;

				typedef uint32_t ClockDivider;
				static const ClockDivider Div1 = 0;
				static const ClockDivider Div2 = 1;
				static const ClockDivider Div4 = 3;
				static const ClockDivider Div8 = 7;
				static const ClockDivider Div16 = 15;
				static const ClockDivider Div32 = 31;
				static const ClockDivider Div64 = 63;
				static const ClockDivider Div128 = 127;
				static const ClockDivider Div256 = 255;
				static const ClockDivider Div1024 = 1023;
				static const ClockDivider Div2048 = 2047;
				static const ClockDivider Div4096 = 4095;
				static const ClockDivider Div8192 = 8191;
				static const ClockDivider Div16384 = 16383;
				static const ClockDivider Div32768 = 32767;

				static ClockDivider DividerValue(unsigned number)
				{
					return (ClockDivider)number;
				}

				static uint32_t DividerCoeff(unsigned number)
				{
					return number + 1;
				}

				enum InterruptFlags
				{
					OverflowInt = TIM_SR_UIF,
					UpdateInt = TIM_SR_UIF,
				};
			};

			DECLARE_ENUM_OPERATIONS(TimerBase::InterruptFlags)
			DECLARE_ENUM_OPERATIONS(TimerBase::TimerMode)

			template <class Regs, class ClockEnReg, IRQn_Type IQRNumber>
			class BaseTimer : public TimerBase
			{
			public:
				typedef uint16_t DataT;
				static const uint16_t MaxValue = 0xffff;
				static const uint16_t MaxDivider = 0xffff;

				typedef uint32_t ClockDivider;
				static const ClockDivider Div1 = 0;
				static const ClockDivider Div2 = 1;
				static const ClockDivider Div4 = 3;
				static const ClockDivider Div8 = 7;
				static const ClockDivider Div16 = 15;
				static const ClockDivider Div32 = 31;
				static const ClockDivider Div64 = 63;
				static const ClockDivider Div128 = 127;
				static const ClockDivider Div256 = 255;
				static const ClockDivider Div1024 = 1023;
				static const ClockDivider Div2048 = 2047;
				static const ClockDivider Div4096 = 4095;
				static const ClockDivider Div8192 = 8191;
				static const ClockDivider Div16384 = 16383;
				static const ClockDivider Div32768 = 32767;

				static ClockDivider DividerValue(unsigned number)
				{
					return (ClockDivider)number;
				}

				static uint32_t DividerCoeff(unsigned number)
				{
					return number + 1;
				}

				enum InterruptFlags
				{
					OverflowInt = TIM_SR_UIF,
					UpdateInt = TIM_SR_UIF,
				};

				static clock_freq_t ClockFreq() { return ClockEnReg::ClockFreq(); }
				static void Enable() { ClockEnReg::Enable(); }
				static void Disable() { ClockEnReg::Disable(); }
				static void Set(DataT val) { Regs()->CNT = val; }
				static void Clear() { Regs()->CNT = 0; }
				static void SetDivider(ClockDivider divider) { Regs()->PSC = divider; }
				static DataT Get() { return Regs()->CNT; }

				static void Stop()
				{
					Regs()->CR1 = 0;
					Regs()->CR2 = 0;
				}

				static void Start(ClockDivider divider = Div1, DataT reloadValue = BaseTimer::MaxValue)
				{
					Enable();
					Regs()->CR1 = 0;
					Regs()->CR2 = 0;
					Regs()->DIER = 0;
					Regs()->PSC = divider;
					Regs()->ARR = reloadValue;
					Regs()->EGR |= TIM_EGR_UG;
					Regs()->CR1 = (TIM_CR1_CEN | TIM_CR1_URS);
				}

				static void EnableInterrupt(InterruptFlags interrupt = UpdateInt)
				{
					if (interrupt & UpdateInt)
						Regs()->DIER |= TIM_DIER_UIE;
					if (interrupt)
						NVIC_EnableIRQ(IQRNumber);
				}

				static void DisableInterrupt(InterruptFlags interrupt = UpdateInt)
				{
					if (interrupt & UpdateInt)
						Regs()->DIER &= ~TIM_DIER_UIE;
				}

				static InterruptFlags IsInterrupt()
				{
					return (InterruptFlags)(Regs()->SR & TIM_SR_UIF);
				}

				static void ClearInterruptFlag(InterruptFlags interrupt = UpdateInt)
				{
					(void)interrupt;
					Regs()->SR = 0;
					NVIC_ClearPendingIRQ(IQRNumber);
				}

				static void SetAutoReload(DataT value)
				{
					Regs()->ARR = value;
					Regs()->EGR |= TIM_EGR_UG;
				}
			};

			template <class Regs, class ClockEnReg, IRQn_Type IQRNumber, template <unsigned> class ChPins>
			class GPTimer : public BaseTimer<Regs, ClockEnReg, IQRNumber>
			{
				IO_BITFIELD_WRAPPER(Regs()->CCMR1, Channel1Mode, uint32_t, 0, 8);
				IO_BITFIELD_WRAPPER(Regs()->CCMR1, Channel2Mode, uint32_t, 8, 8);
				IO_BITFIELD_WRAPPER(Regs()->CCMR2, Channel3Mode, uint32_t, 0, 8);
				IO_BITFIELD_WRAPPER(Regs()->CCMR2, Channel4Mode, uint32_t, 8, 8);
				typedef LOKI_TYPELIST_4(Channel1Mode, Channel2Mode, Channel3Mode, Channel4Mode) ModeList;

			public:
				static void SetMode(TimerBase::TimerMode mode)
				{
					Regs()->CR1 = (Regs()->CR1 & ~(TIM_CR1_DIR | TIM_CR1_OPM | TIM_CR1_UDIS)) | mode;
				}

				template <unsigned Number>
				class OutputCompare
				{
					typedef typename ::Loki::TL::TypeAt<ModeList, Number>::Result ModeBitField;

				public:
					STATIC_ASSERT(Number < 4);
					typedef typename ChPins<Number>::Pins Pins;

					static void Set(TimerBase::DataT val)
					{
						(&Regs()->CCR1)[Number] = val;
					}

					static TimerBase::DataT Get()
					{
						return (&Regs()->CCR1)[Number];
					}

					static void EnableInterrupt()
					{
						STATIC_ASSERT(IQRNumber > 0); // Check if interrupt is available for this timer
						Regs()->DIER |= (TIM_DIER_CC1IE << Number);
						NVIC_EnableIRQ(IQRNumber);
					}

					static void DisableInterrupt()
					{
						Regs()->DIER &= ~(TIM_DIER_CC1IE << Number);
					}

					static bool IsInterrupt()
					{
						return Regs()->SR & (TIM_SR_CC1IF << Number);
					}

					static void ClearInterruptFlag()
					{
						Regs()->SR &= ~(TIM_SR_CC1IF << Number);
					}

					static void Enable()
					{
						Regs()->CCER |= (TIM_CCER_CC1E << Number * 4);
					}

					static void Disable()
					{
						Regs()->CCER &= ~(TIM_CCER_CC1E << Number * 4);
					}

					static void SetOutputPolarity(TimerBase::OutPolarity polarity)
					{
						Regs()->CCER = (Regs()->CCER & ~((TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC1NP) << Number * 4)) | (polarity << Number * 4);
					}

					static void SetCapturePolarity(TimerBase::CapturePolarity polarity)
					{
						Regs()->CCER = (Regs()->CCER & ~((TIM_CCER_CC1E | TIM_CCER_CC1P | TIM_CCER_CC1NP) << Number * 4)) | (polarity << Number * 4);
					}

					static void SetOutputMode(TimerBase::OutputMode mode)
					{
						Regs()->CCER = (Regs()->CCER & ~(TIM_CCER_CC1NP << Number * 4)) | (TIM_CCER_CC1E << Number * 4);
						ModeBitField::Set(mode);
						Regs()->BDTR |= TIM_BDTR_MOE;
					}

					static void SetCaptureMode(TimerBase::CaptureMode mode)
					{
						// if(mode & CaptureTrc)
						// {
						// Regs()->SMCR |= ??
						// }
						ModeBitField::Set(TIM_CCMR1_CC1S_0 | mode);
					}

					static void SelectPins(int pin)
					{
						typedef typename Pins::ValueType Type;
						Type mask = 1 << pin;
						Pins::Enable();
						Pins::SetConfiguration(mask, Pins::AltFunc);
						Pins::AltFuncNumber(mask, ChPins<Number>::AltFunc);
					}

					template <class Pin>
					static void SelectPins(Pin = Pin())
					{
						const int index = Pins::template PinIndex<Pin>::Value;
						STATIC_ASSERT(index >= 0);
						SelectPins(index);
					}
				};
			};

			template <class Regs, class ClockEnReg, IRQn_Type IQRNumber, template <unsigned> class ChPins>
			class AdvancedTimer
				: public GPTimer<Regs, ClockEnReg, IQRNumber, ChPins>
			{
				typedef GPTimer<Regs, ClockEnReg, IQRNumber, ChPins> Base;

			public:
				template <unsigned Number>
				class OutputCompare
					: public Base::template OutputCompare<Number>
				{
					typedef typename Base::template OutputCompare<Number> BaseChannel;

				public:
					typedef typename ChPins<Number>::PinsN PinsN;

					static void SetOutputIdleState(bool directOutput, bool inverseOutput)
					{
						const unsigned clearMask = (TIM_CR2_OIS1 | TIM_CR2_OIS1N) << (Number * 2);
						unsigned setMask = ((directOutput ? TIM_CR2_OIS1 : 0) |
											(inverseOutput ? TIM_CR2_OIS1N : 0))
										   << (Number * 2);
						Regs()->CR2 = (Regs()->CR2 & clearMask) | setMask;
					}

					static void SelectPins(int pin, int inversePin = -1)
					{
						typedef typename BaseChannel::Pins::ValueType Type;
						Type mask = 1 << pin;
						BaseChannel::SelectPins(pin);

						if (inversePin >= 0)
						{
							Type inverseMask = 1 << inversePin;
							PinsN::Enable();
							PinsN::SetConfiguration(inverseMask, PinsN::AltFunc);
							PinsN::AltFuncNumber(inverseMask, ChPins<Number>::AltFunc);
						}
					}

					template <class Pin, class InversePin>
					static void SelectPins(Pin = Pin(), InversePin = InversePin())
					{
						const int index = BaseChannel::Pins::template PinIndex<Pin>::Value;
						STATIC_ASSERT(index >= 0);

						const int indexN = PinsN::template PinIndex<InversePin>::Value;
						STATIC_ASSERT(indexN >= 0);

						SelectPins(index, indexN);
					}

					template <class Pin>
					static void SelectPins(Pin = Pin())
					{
						const int index = BaseChannel::Pins::template PinIndex<Pin>::Value;
						STATIC_ASSERT(index >= 0);

						SelectPins(index, -1);
					}
				};
			};

			using namespace IO;

			typedef PinList<Pa12> Tim1EtrPins;
			typedef PinList<Pa6, Pb12> Tim1BkInPins;
			template <unsigned ChannelNumber>
			struct Tim1ChPins;
			template <>
			struct Tim1ChPins<0>
			{
				enum
				{
					AltFunc = 1
				};
				typedef PinList<Pa8> Pins;
				typedef PinList<Pa7, Pb13> PinsN;
			};
			template <>
			struct Tim1ChPins<1>
			{
				enum
				{
					AltFunc = 1
				};
				typedef PinList<Pa9> Pins;
				typedef PinList<Pb0, Pb14> PinsN;
			};
			template <>
			struct Tim1ChPins<2>
			{
				enum
				{
					AltFunc = 1
				};
				typedef PinList<Pa10> Pins;
				typedef PinList<Pb1, Pb15> PinsN;
			};
			template <>
			struct Tim1ChPins<3>
			{
				enum
				{
					AltFunc = 1
				};
				typedef PinList<Pa11> Pins;
				typedef PinList<NullPin> PinsN;
			};

			typedef PinList<Pa0, Pa5, Pa15> Tim2EtrPins;
			template<unsigned ChannelNumber> struct Tim2ChPins;
			template<> struct Tim2ChPins<0>{ enum{AltFunc = 1}; typedef PinList<Pa0, Pa5, Pa15> Pins; };
			template<> struct Tim2ChPins<1>{ enum{AltFunc = 1}; typedef PinList<Pa1, Pb3> Pins; };
			template<> struct Tim2ChPins<2>{ enum{AltFunc = 1}; typedef PinList<Pa2, Pb10> Pins; };
			template<> struct Tim2ChPins<3>{ enum{AltFunc = 1}; typedef PinList<Pa3, Pb11> Pins; };

			IO_STRUCT_WRAPPER(TIM1, Tim1Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM2, Tim2Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM3, Tim3Regs, TIM_TypeDef);
			IO_STRUCT_WRAPPER(TIM4, Tim4Regs, TIM_TypeDef);
#if defined(TIM6)
			IO_STRUCT_WRAPPER(TIM6, Tim6Regs, TIM_TypeDef);
#endif
#if defined(TIM7)
			IO_STRUCT_WRAPPER(TIM7, Tim7Regs, TIM_TypeDef);
#endif
#if defined(TIM8)
			IO_STRUCT_WRAPPER(TIM8, Tim8Regs, TIM_TypeDef);
#endif
#if defined(TIM9)
			IO_STRUCT_WRAPPER(TIM9, Tim9Regs, TIM_TypeDef);
#endif
#if defined(TIM10)
			IO_STRUCT_WRAPPER(TIM10, Tim10Regs, TIM_TypeDef);
#endif
#if defined(TIM11)
			IO_STRUCT_WRAPPER(TIM11, Tim11Regs, TIM_TypeDef);
#endif
#if defined(TIM12)
			IO_STRUCT_WRAPPER(TIM12, Tim12Regs, TIM_TypeDef);
#endif
#if defined(TIM13)
			IO_STRUCT_WRAPPER(TIM13, Tim13Regs, TIM_TypeDef);
#endif
#if defined(TIM14)
			IO_STRUCT_WRAPPER(TIM14, Tim14Regs, TIM_TypeDef);
#endif
#if defined(TIM15)
			IO_STRUCT_WRAPPER(TIM15, Tim15Regs, TIM_TypeDef);
#endif
#if defined(TIM16)
			IO_STRUCT_WRAPPER(TIM16, Tim16Regs, TIM_TypeDef);
#endif
#if defined(TIM17)
			IO_STRUCT_WRAPPER(TIM17, Tim17Regs, TIM_TypeDef);
#endif
		} // namespace Private

#if defined(TIM1)
		class Timer1 : public Private::AdvancedTimer<Private::Tim1Regs, Clock::Tim1Clock, TIM1_UP_TIM10_IRQn, Private::Tim1ChPins>
		{
		};
#endif

#if defined(TIM2)
		class Timer2 : public Private::GPTimer<Private::Tim2Regs, Clock::Tim2Clock, TIM2_IRQn, Private::Tim2ChPins>
		{
		};
#endif

#if defined(TIM6)
		class Timer6 : public Private::BaseTimer<Private::Tim6Regs, Clock::Tim6Clock, TIM6_DAC_IRQn>
		{
		};
#endif
#if defined(TIM7)
		class Timer7 : public Private::BaseTimer<Private::Tim7Regs, Clock::Tim7Clock, TIM7_IRQn>
		{
		};
#endif

	} // namespace Timers
} // namespace Mcucpp
#undef DECLARE_STM32_BASIC_TIMER