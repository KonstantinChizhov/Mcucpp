
#pragma once

#include <stm32f10x.h>
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
			IO::Pa4,
			IO::Pa5,
			IO::Pa6,
			IO::Pa7,
			IO::Pb0, // 8
			IO::Pb1,
			IO::Pc0, // 10
			IO::Pc1,
			IO::Pc2,
			IO::Pc3, // 13
			IO::Pc4,
			IO::Pc5
		> Adc1Pins;
	}
	
	template<class Regs, class ClockCtrl, class InputPins>
	class AdcBase
	{
		// Maximum ADC convertion time MaxCockDivider * AdcConvertionCycles / cpu cycle per wait loop
		static const unsigned AdcTimeoutCycles = 2048 * 28 / 4;
		
		static bool VerifyReady()
		{
			uint32_t timeout = AdcTimeoutCycles * 4;
			while((Regs()->SR & ADC_SR_STRT) && --timeout)
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
		static const ClockSource AdcClock = ClockCtrl::Apb2;
		static const ClockSource MainClock = ClockCtrl::Apb2;
		
		// Clock dividers
		static const AdcDivider Div2   = ClockCtrl::Div2;
		static const AdcDivider Div4   = ClockCtrl::Div4;
		static const AdcDivider Div6   = ClockCtrl::Div6;
		static const AdcDivider Div8   = ClockCtrl::Div8;
		
		// Channels
		static const uint8_t TempSensorChannel = 16;
		static const uint8_t ReferenceChannel = 17;
		
		enum SampleTime
		{
			SampleTime_1_5 = 0,
			SampleTime_7_5 = 1,
			SampleTime_13_5 = 2,
			SampleTime_28_5 = 3,
			SampleTime_41_5 = 4,
			SampleTime_55_5 = 5,
			SampleTime_71_5 = 6,
			SampleTime_239_5 = 7,
		};
		
		static DataT ResolutionBits()
		{
			return 12;
		}
		
		static void SelectClockSource(ClockSource clockSrc)
		{
			ClockCtrl::SelectClockSource(clockSrc);
		}
		
		
		static void SetSampleTime(SampleTime sampleTime, uint8_t channel = 1)
		{
			if(channel > 17 || channel == 0)
				return;
				
			if(!VerifyReady())
				return;
				
			if(channel <= 8)
			{
				unsigned shift = channel * 3;
				Regs()->SMPR1 = (Regs()->SMPR1 & ~(0x07 << shift)) | (sampleTime << shift);
			}
			else
			{
				channel -= 9;
				unsigned shift = channel * 3;
				Regs()->SMPR2 = (Regs()->SMPR2 & ~(0x07 << shift)) | (sampleTime << shift);
			}
		}
		
		static void SetClockDivider(AdcDivider divider)
		{
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
				Pins::SetConfiguration(1u << (channel), Pins::Analog);
			}
			
			if(channel == TempSensorChannel)
			{
				Regs()->CR2 |= ADC_CR2_TSVREFE;
				SetSampleTime(SampleTime_239_5, TempSensorChannel);
			}
			if(channel > 18)
				return;
			Regs()->SQR1 = (Regs()->SQR3 & ~0x001f) | (channel );
		}
		 
		static void Init(uint8_t channel = 0, AdcDivider divider = Div2, Vref ref = VCC)
		{
			IO::Porta::Enable();
			Pins::SetConfiguration(1u << (channel), Pins::Analog);
			SetVref(ref);
			Clock::Enable();
			SelectClockSource(AdcClock);
			SetClockDivider(divider);
			ADC1->CR1     =  0;
			ADC1->SQR1    =  0;
			
			ADC1->CR2     =  ADC_CR2_EXTSEL;
			ADC1->CR2    |=  ADC_CR2_EXTTRIG;
			ADC1->CR2    |=  ADC_CR2_ADON;
			ADC1->SQR3 = channel;
			Calibrate();
		}
		
		static void Calibrate()
		{
			//if(!VerifyReady())
			//	return;
			// calibrate single-ended input
			Regs()->CR2 |= ADC_CR2_RSTCAL;
			while(Regs()->CR2 & ADC_CR2_RSTCAL)
				;
			DelayLoop(100); // need delay at least one ADC cycle
			
			Regs()->CR2 |= ADC_CR2_CAL;
			while(Regs()->CR2 & ADC_CR2_CAL)
				;
		}
		
		static void StartContinuousConversions()
		{
			
		}
		
		static void StartSingleConversion()
		{
			//while(!VerifyReady())
				//;
			ADC1->CR2 |=  ADC_CR2_EXTSEL;
			ADC1->CR2 |=  ADC_CR2_EXTTRIG;
			Regs()->CR2 |= ADC_CR2_ADON | ADC_CR2_SWSTART;
		}

		static void EnableInterrupt()
		{
			
		}
		
		static uint16_t Read()
		{
			// uint32_t timeout = AdcTimeoutCycles;
			// while (!ResultReady() && --timeout)
				// ;
			// if(timeout == 0)
				// return 0xffff;
			// return Regs()->DR;
			
			ADC1->CR2 |= ADC_CR2_SWSTART;     //запустить преобразование в регулярном канале   
			while(!(ADC1->SR & ADC_SR_EOC)){};//дождаться окончания  преобразования
			return ADC1->DR;                  //прочитать результат измерения
		}
		
		static bool ResultReady()
		{
			return (Regs()->SR & ADC_SR_EOC);
		}

		static uint16_t SingleConversion()
		{
			StartSingleConversion();
			return Read();
		}
		
		static void Stop()
		{
			Regs()->CR2 &= ~ADC_CR2_ADON;
		}
		
		static void Enable()
		{
			Clock::Enable();
		}
		
		static void Disable()
		{
			Regs()->CR1 = 0;
			Regs()->CR2 = 0;
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
	}
	
	typedef AdcBase<Private::Adc1Regs, Clock::Adc1Clock, Private::Adc1Pins> Adc1;
}
