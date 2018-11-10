//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2018
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

#include <mcu_header.h>
#include <clock.h>
#include <iopins.h>
#include <pinlist.h>
#include <dma.h>
#include <power.h>
#include <delay.h>
#include <initializer_list>


namespace Mcucpp
{

	typedef void (* AdcCallbackType)(uint16_t *data, size_t count);

	class AdcCommon
	{
	public:
		enum AdcError{NoError, Overflow, TransferError, HardwareError, ArgumentError, SequenceError};

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
		enum ClockSource{ AdcClock = 0, MainClock = ADC_CCR_CKMODE_0};

		// Clock dividers
		enum AdcDivider
		{
			Div1   = 0,
			Div2   = 1u << ADC_CCR_PRESC_Pos,
			Div4   = 2u << ADC_CCR_PRESC_Pos,
			Div6   = 3u << ADC_CCR_PRESC_Pos,
			Div8   = 4u << ADC_CCR_PRESC_Pos,
			Div10  = 5u << ADC_CCR_PRESC_Pos,
			Div12  = 6u << ADC_CCR_PRESC_Pos,
			Div16  = 7u << ADC_CCR_PRESC_Pos,
			Div32  = 8u << ADC_CCR_PRESC_Pos,
			Div64  = 9u << ADC_CCR_PRESC_Pos,
			Div128 = 10u << ADC_CCR_PRESC_Pos,
			Div256 = 11u << ADC_CCR_PRESC_Pos
		};

		// Channels
		static const uint8_t TempSensorChannel = 17;
		static const uint8_t Battery = 18;
		static const uint8_t ReferenceChannel = 16;

		enum TriggerMode{TriggerNone, TriggerRisingEdge, TriggerFallingEdge, TriggerBothEdges};

		enum SequenceTrigger
		{
			SeqTriggerOnce = 0x10,
			SeqTriggerTimer1CC1Event = 0,
			SeqTriggerTimer1CC2Event,
			SeqTriggerTimer1CC3Event,
			SeqTriggerTimer2CC2Event,
			SeqTriggerTimer3TRGOEvent,
			SeqTriggerTimer4CC4Event,
			SeqTriggerEXTIline11,
			SeqTriggerTimer8TRGOEvent,
			SeqTriggerTimer8TRGO2Event,
			SeqTriggerTimer1TRGOEvent,
			SeqTriggerTimer1TRGO2Event,
			SeqTriggerTimer2TRGOEvent,
			SeqTriggerTimer4TRGOEvent,
			SeqTriggerTimer6TRGOEvent,
			SeqTriggerTimer15TRGOEvent,
			SeqTriggerTimer3CC4Event
		};

		enum ImmediateTrigger
		{
			ImmTriggerOnce = 0x10,
			ImmTriggerTimer1TRGOEvent = 0,
			ImmTriggerTimer1CC4Event,
			ImmTriggerTimer2TRGOEvent,
			ImmTriggerTimer2CC1Event,
			ImmTriggerTimer3CC4Event,
			ImmTriggerTimer4TRGOEvent,
			ImmTriggerEXTILine15,
			ImmTriggerTimer8CC4Event,
			ImmTriggerTimer1TRGO2Event,
			ImmTriggerTimer8TRGOEvent,
			ImmTriggerTimer8TRGO2Event,
			ImmTriggerTimer3CC3Event,
			ImmTriggerTimer3TRGOEvent,
			ImmTriggerTimer3CC1Event,
			ImmTriggerTimer6TRGOEvent,
			ImmTriggerTimer15TRGOEvent
		};
	};

	struct AdcData
	{
		AdcData()
			:seqCallback(nullptr),
			immCallback(nullptr),
			seqData(0),
			immData(0),
			error(AdcCommon::NoError),
			vRef(0)
		{
		}

		AdcCallbackType seqCallback;
		AdcCallbackType immCallback;
		uint16_t *seqData;
		uint16_t *immData;
		AdcCommon::AdcError error;
		uint16_t vRef;
	};

	template<class Regs, class CommonRegs, class ClockCtrl, class InputPins, class DmaChannel, typename DmaChannel::RequestType channelNum>
	class AdcBase :public AdcCommon
	{
	public:
		typedef InputPins Pins;
		typedef ClockCtrl ClockSrc;

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
		static inline bool StartImmediate(const uint8_t *channels, uint16_t *data, uint8_t count, AdcCallbackType callback = nullptr);
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
		static inline void SetSequenceCallback(AdcCallbackType callback);
		/// start conversion sequence of channelsCount channels (maximum MaxSequence (16))
		/// dataBuffer has to be channelsCount * scanCount elements
		static inline bool StartSequence(const uint8_t *channels, uint8_t channelsCount, uint16_t *dataBuffer, uint16_t scanCount);
		static inline bool StartSequence(std::initializer_list<uint8_t> channels, uint16_t *dataBuffer, uint16_t scanCount);

		template<class ... Inpunts>
        static inline bool StartSequence(uint16_t *dataBuffer, uint16_t scanCount)
        {
             return StartSequence( {Pins::template PinIndex<Inpunts>::Value... }, dataBuffer, scanCount);
        }

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

	typedef AdcBase<Private::Adc1Regs, Private::AdcRegs, Clock::AdcClock, Private::Adc1Pins, Dma1Channel1, Dma1Channel1Request::Adc1> Adc1;
	typedef AdcBase<Private::Adc2Regs, Private::AdcRegs, Clock::AdcClock, Private::Adc2Pins, Dma1Channel2, Dma1Channel2Request::Adc2> Adc2;
	typedef AdcBase<Private::Adc3Regs, Private::AdcRegs, Clock::AdcClock, Private::Adc3Pins, Dma1Channel3, Dma1Channel3Request::Adc3> Adc3;

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
