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

#include <stm32f4xx.h>
#include <clock.h>
#include <iopins.h>
#include <pinlist.h>
#include <dma.h>
#include <power.h>

namespace Mcucpp
{
	
	typedef void (* AdcCallback)(uint16_t *data, size_t count);
	static inline void VoidAdcCallback(uint16_t *, size_t){}
		
	class AdcCommon
	{
	public:
		enum AdcError{NoError, Overflow, TransferError, HardwareError, ArgumentError};
		static const unsigned AdcTimeoutCycles = 2048 * 28 / 4;
		
		static const unsigned MaxSequence = 16;
		static const unsigned MaxImmediate = 4;
		
		static const unsigned VRefNominal = 12100; // in 10E-4 Volt units
		
		typedef uint16_t DataT;
		
		static const DataT InvalidValue = 0xffff;
		
		enum Reference
		{
			External = 0,
			VCC = 0
		};

		// Clock sources
		enum ClockSource{ AdcClock = 0};
	
		// Clock dividers
		enum AdcDivider
		{
			Div2   = 0,
			Div4   = ADC_CCR_ADCPRE_0,
			Div6   = ADC_CCR_ADCPRE_1,
			Div8   = ADC_CCR_ADCPRE_0 | ADC_CCR_ADCPRE_1
		};
	
		// Channels
		static const uint8_t TempSensorChannel = 16;
		static const uint8_t Battery = 17;
		static const uint8_t ReferenceChannel = 18;
		
		enum TriggerMode{TriggerNone, TriggerRisingEdge, TriggerFallingEdge, TriggerBothEdges};
		enum SequenceTrigger
		{
			SeqTriggerOnce = 0x10, 
			SeqTriggerTimer1CC1Event = 0,
			SeqTriggerTimer1CC2Event,
			SeqTriggerTimer1CC3Event,
			SeqTriggerTimer2CC2Event,
			SeqTriggerTimer2CC3Event,
			SeqTriggerTimer2CC4Event,
			SeqTriggerTimer2TRGOEvent,
			SeqTriggerTimer3CC1Event,
			SeqTriggerTimer3TRGOEven,
			SeqTriggerTimer4CC4Event,
			SeqTriggerTimer5CC1Event,
			SeqTriggerTimer5CC2Event,
			SeqTriggerTimer5CC3Event,
			SeqTriggerTimer8CC1Event,
			SeqTriggerTimer8TRGOEvent,
			SeqTriggerEXTIline11
		};
		
		enum ImmediateTrigger
		{
			ImmTriggerOnce = 0x10, 
			ImmTriggerTimer1CC4Event = 0,
			ImmTriggerTimer1TRGOEvent,
			ImmTriggerTimer2CC1Event,
			ImmTriggerTimer2TRGOEvent,
			ImmTriggerTimer3CC2Event,
			ImmTriggerTimer3CC4Event,
			ImmTriggerTimer4CC1Event,
			ImmTriggerTimer4CC2Event,
			ImmTriggerTimer4CC3Event,
			ImmTriggerTimer4TRGOEvent,
			ImmTriggerTimer5CC4Event,
			ImmTriggerTimer5TRGOEvent,
			ImmTriggerTimer8CC2Event,
			ImmTriggerTimer8CC3Event,
			ImmTriggerTimer8CC4Event,
			ImmTriggerEXTILine15
		};
	};
	
	struct AdcData
	{
		AdcData()
			:seqCallback(VoidAdcCallback),
			immCallback(VoidAdcCallback),
			seqData(0),
			immData(0),
			error(AdcCommon::NoError),
			vRef(0)
		{
		}
		
		AdcCallback seqCallback;
		AdcCallback immCallback;
		uint16_t *seqData;
		uint16_t *immData;
		AdcCommon::AdcError error;
		uint16_t vRef;
	};
	
	template<class Regs, class CommonRegs, class ClockCtrl, class InputPins, class DmaChannel, uint8_t channelNum>
	class AdcBase :public AdcCommon
	{
	public:
		typedef InputPins Pins;
		typedef ClockCtrl Clock;
		
		static inline void Init();
		static inline void Init(AdcDivider divider, ClockSource clock, Reference reference);
		static inline void Disable();
		static inline void SetSampleTime(uint8_t channel, unsigned adcCycles);
		static inline uint8_t SetResolution(uint8_t bits);
		static inline void SetDivider(AdcDivider divider);
		static inline void SelectClockSource(ClockSource);
		static inline void SetReference(Reference reference);
		static inline uint8_t ChannelsCount();
		
		// immediate
		static inline void SetImmediateTrigger(ImmediateTrigger trigger, TriggerMode mode);
		// Start async conversionon group max MaxImmediate channels
		static inline bool StartImmediate(const uint8_t *channels, uint16_t *data, uint8_t count, AdcCallback callback = 0);
		// Blocking conversion
		static inline bool ReadImmediate(const uint8_t *channels, uint16_t *data, uint8_t count);
		// 
		static inline bool StartImmediate(uint8_t channel);
		static inline uint16_t ReadImmediate();
		
		static inline uint16_t ReadImmediate(uint8_t channel);
		
		template<class Pin>
		static inline uint16_t ReadImmediate()
		{
			const int index = Pins::template PinIndex<Pin>::Value;
			STATIC_ASSERT(index >= 0);
			return ReadImmediate(index);
		}
		
		static inline bool ImmediateReady();
		static inline void StopImmediate();
		
		// continiuos
		static inline void SetSequenceTrigger(SequenceTrigger trigger, TriggerMode mode);
		static inline void SetSequenceCallback(AdcCallback callback);
		/// start conversion sequence of channelsCount channels (maximum MaxSequence (16)) 
		/// dataBuffer has to be channelsCount * scanCount elements
		static inline bool StartSequence(const uint8_t *channels, uint8_t channelsCount, uint16_t *dataBuffer, uint16_t scanCount);
		static inline bool SequenceReady();
		static inline void StopSequence();
		
		//info
		static inline AdcError GetError();
		static inline int ResolutionBits();
		static inline unsigned ToVolts(uint16_t value); // in 10E-4 Volt units
		static inline unsigned ClockFreq();
		static inline unsigned ConvertionTimeCycles(uint8_t channel);
		
		static void DmaHandler(void * data, size_t size, bool success);
		static void IrqHandler();
	//private:
		static bool VerifyReady(unsigned);
		static unsigned SampleTimeToReg(unsigned sampleTime);
		static AdcData _adcData;
	};
	
#include <adc_private.h>
	
	typedef AdcBase<Private::Adc1Regs, Private::AdcRegs, Clock::Adc1Clock, Private::Adc1Pins, Dma2Channel0, 0> Adc1;
	typedef AdcBase<Private::Adc2Regs, Private::AdcRegs, Clock::Adc2Clock, Private::Adc2Pins, Dma2Channel3, 1> Adc2;
	typedef AdcBase<Private::Adc3Regs, Private::AdcRegs, Clock::Adc3Clock, Private::Adc3Pins, Dma2Channel1, 2> Adc3;
	
	ADC_BASE_TEMPLATE_ARGS
	unsigned ADC_BASE_TEMPLATE_QUALIFIER::ToVolts(uint16_t value)
	{
		if(_adcData.vRef == 0)
		{
			for(int i = 0; i < 4; i++)
				_adcData.vRef += Adc1::ReadImmediate(ReferenceChannel);
			_adcData.vRef /= 4;
		}
		return VRefNominal * value / _adcData.vRef;
	}

}
