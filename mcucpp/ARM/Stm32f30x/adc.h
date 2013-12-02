
#pragma once

#include <stm32f30x.h>
#include <clock.h>
#include <iopins.h>
#include <pinlist.h>
#include <platform_dalay.h>

namespace Mcucpp
{
	namespace Private
	{
		typedef IO::PinList
		<
			IO::Pa0,
			IO::Pa1,
			IO::Pa2,
			IO::Pa3,
			IO::Pf4,
			IO::Pc0,
			IO::Pc1,
			IO::Pc2,
			IO::Pc3,
			IO::Pf2
		> Adc1Pins;
		
		typedef IO::PinList
		<
			IO::Pa4,
			IO::Pa5,
			IO::Pa6,
			IO::Pa7,
			IO::Pc4,
			IO::Pc0,
			IO::Pc1,
			IO::Pc2,
			IO::Pc3,
			IO::Pf2,
			IO::Pc5,
			IO::Pb2
		> Adc2Pins;
		
		typedef IO::PinList
		<
			IO::Pb1,
			IO::Pe9,
			IO::Pe13,
			IO::NullPin,
			IO::Pb13,
			IO::Pe8,
			IO::Pd10,
			IO::Pd11,
			IO::Pd12,
			IO::Pd13,
			IO::Pd14,
			IO::Pb0,
			IO::Pe7,
			IO::Pe10,
			IO::Pe11,
			IO::Pe12
		> Adc3Pins;
		
		typedef IO::PinList
		<
			IO::Pe14,
			IO::Pe15,
			IO::Pb12,
			IO::Pb14,
			IO::Pb15,
			IO::Pe8,
			IO::Pd10,
			IO::Pd11,
			IO::Pd12,
			IO::Pd13,
			IO::Pd14,
			IO::Pd8,
			IO::Pd9
		> Adc4Pins;
	}
	
	template<class Regs, class CommonRegs, class ClockCtrl, class InputPins>
	class AdcBase
	{
		// Maximum ADC convertion time MaxCockDivider * AdcConvertionCycles / cpu cycle per wait loop
		static const unsigned AdcTimeoutCycles = 2048 * 28 / 4;
		
		static bool VerifyReady()
		{
			uint32_t timeout = AdcTimeoutCycles * 4;
			while(Regs()->CR & ADC_CR_ADSTART && --timeout)
				;
			return timeout != 0;
		}
	public:
		typedef uint16_t DataT;
		typedef ClockCtrl Clock;
		typedef InputPins Pins;
		static const uint8_t ChannelCount = 18;
		
		enum Vref
		{
			External = 0,
			VCC = 0
		};

		typedef typename ClockCtrl::Prescaller AdcDivider;
		typedef typename ClockCtrl::ClockSource ClockSource;
		
		// Clock sources
		static const ClockSource AdcClock = ClockCtrl::Async;
		static const ClockSource MainClock = ClockCtrl::AhbDiv1;
		static const ClockSource AhbDiv1 = ClockCtrl::AhbDiv1;
		static const ClockSource AhbDiv2 = ClockCtrl::AhbDiv2;
		static const ClockSource AhbDiv4 = ClockCtrl::AhbDiv4;
		
		// Clock dividers
		static const AdcDivider Div1   = ClockCtrl::Div1  ;
		static const AdcDivider Div2   = ClockCtrl::Div2  ;
		static const AdcDivider Div4   = ClockCtrl::Div4  ;
		static const AdcDivider Div6   = ClockCtrl::Div6  ;
		static const AdcDivider Div8   = ClockCtrl::Div8  ;
		static const AdcDivider Div10  = ClockCtrl::Div10 ;
		static const AdcDivider Div12  = ClockCtrl::Div12 ;
		static const AdcDivider Div16  = ClockCtrl::Div16 ;
		static const AdcDivider Div32  = ClockCtrl::Div32 ;
		static const AdcDivider Div64  = ClockCtrl::Div64 ;
		static const AdcDivider Div128 = ClockCtrl::Div128;
		static const AdcDivider Div256 = ClockCtrl::Div256;
		static const AdcDivider NotChange = static_cast<AdcDivider>(0xff);
		
		
		// Channels
		static const uint8_t OpAmp1Ref = 15;
		static const uint8_t TempSensorChannel = 16;
		static const uint8_t Battery = 17;
		static const uint8_t ReferenceChannel = 18;
		
		enum SampleTime
		{
			SampleTime_1_5 = 0,
			SampleTime_2_5 = 0,
			SampleTime_4_5 = 0,
			SampleTime_7_5 = 0,
			SampleTime_19_5 = 0,
			SampleTime_61_5 = 0,
			SampleTime_181_5 = 0,
			SampleTime_601_5 = 0
		};
		
		static DataT ResolotionBits()
		{
			return 12;
		}
		
		static void SelectClockSource(ClockSource clockSrc)
		{
			ClockCtrl::SelectClockSource(clockSrc);
		}
		
		
		static void SetSampleTime(SampleTime sampleTime, uint8_t channel = 1)
		{
			if(channel > 18 || channel == 0)
				return;
				
			if(!VerifyReady())
				return;
				
			if(channel <= 9)
			{
				unsigned shift = channel * 3;
				Regs()->SMPR1 = (Regs()->SMPR1 & ~(0x07 << shift)) | (sampleTime << shift);
			}
			else
			{
				channel -= 10;
				unsigned shift = channel * 3;
				Regs()->SMPR2 = (Regs()->SMPR2 & ~(0x07 << shift)) | (sampleTime << shift);
			}
		}
		
		static void SetClockDivider(AdcDivider divider)
		{
			if(divider != NotChange)
				ClockCtrl::SetPrescaller(divider);
		}

		static void SetVref(Vref)
		{
		}

		static void SetChannel(uint8_t channel)
		{
			if(!VerifyReady())
				return;
			
			if(channel < Pins::Length)
			{
				Pins::SetConfiguration(1u << (channel - 1), Pins::Analog);
			}
			
			if(channel == TempSensorChannel)
			{
				CommonRegs()->CCR |= ADC12_CCR_TSEN;
				SetSampleTime(SampleTime_181_5, TempSensorChannel);
			}
			if(channel > 18)
				return;
			Regs()->SQR1 = (Regs()->SQR1 & ~0x7cf) | (channel << 6);
		}
		 
		static void Init(uint8_t channel = 1, AdcDivider divider = Div2, Vref ref = VCC)
		{
			Clock::Enable();
			SelectClockSource(AdcClock);
			SetClockDivider(divider);
			SetVref(ref);
			Regs()->CR &= ~(ADC_CR_ADVREGEN);
			Regs()->CR |= ADC_CR_ADVREGEN_0;
			DelayLoop(250); // TODO: get rid of delay loop
			Calibrate();
			SetChannel(channel);
			
			Regs()->CR |= ADC_CR_ADEN;
			while((Regs()->ISR & ADC_ISR_ADRD) == 0)
				;
		}
		
		static void Calibrate()
		{
			// calibrate single-ended input
			Regs()->CR = (Regs()->CR & ~ADC_CR_ADCALDIF) | ADC_CR_ADCAL;
			while(Regs()->CR & ADC_CR_ADCAL)
				;
			DelayLoop(100); // need delay at least one ADC cycle
			// calibrate differential input
			Regs()->CR |= ADC_CR_ADCALDIF | ADC_CR_ADCAL;
			while(Regs()->CR & ADC_CR_ADCAL)
				;
		}
		
		static void StartContinuousConversions()
		{
			
		}
		
		static void StartSingleConversion()
		{
			if(!VerifyReady())
				return;
			Regs()->CR |= ADC_CR_ADSTART;
		}

		static void EnableInterrupt()
		{
			
		}
		
		static uint16_t Read()
		{
			return Regs()->DR;
		}
		
		static bool ResultReady()
		{
			return (Regs()->ISR & ADC_ISR_EOC);
		}

		static uint16_t SingleConversion()
		{
			if(!VerifyReady())
				return 0xffff;
			uint32_t timeout = AdcTimeoutCycles;
			Regs()->CR |= ADC_CR_ADSTART;
			while (!ResultReady() && --timeout)
				;
			if(timeout == 0)
				return 0xffff;
			return Read();
		}
		
		static void Stop()
		{
			Regs()->CR |= ADC_CR_ADSTP;
		}
		
		static void Disable()
		{
			Regs()->CR &= ~ADC_CR_ADVREGEN;
			Regs()->CR |= ADC_CR_ADVREGEN_1;
			Regs()->CR |= ADC_CR_ADDIS;
			Regs()->CR = 0;
			Clock::Disable();
		}
		
		static bool ReadSequence(DataT *data, uint8_t *channels, uint8_t channelsCount)
		{
			return false;
		}
		
		static bool SequenceCompleted()
		{
			return false;
		}
	};
	
	namespace Private
	{
		IO_STRUCT_WRAPPER(ADC1, Adc1Regs, ADC_TypeDef);
		IO_STRUCT_WRAPPER(ADC2, Adc2Regs, ADC_TypeDef);
		IO_STRUCT_WRAPPER(ADC3, Adc3Regs, ADC_TypeDef);
		IO_STRUCT_WRAPPER(ADC4, Adc4Regs, ADC_TypeDef);
		IO_STRUCT_WRAPPER(ADC1_2, Adc1_2Regs, ADC_Common_TypeDef);
		IO_STRUCT_WRAPPER(ADC3_4, Adc3_4Regs, ADC_Common_TypeDef);
	}
	
	typedef AdcBase<Private::Adc1Regs, Private::Adc1_2Regs, Clock::Adc12Clock, Private::Adc1Pins> Adc1;
	typedef AdcBase<Private::Adc2Regs, Private::Adc1_2Regs, Clock::Adc12Clock, Private::Adc2Pins> Adc2;
	typedef AdcBase<Private::Adc3Regs, Private::Adc3_4Regs, Clock::Adc34Clock, Private::Adc3Pins> Adc3;
	typedef AdcBase<Private::Adc4Regs, Private::Adc3_4Regs, Clock::Adc34Clock, Private::Adc4Pins> Adc4;	
}
