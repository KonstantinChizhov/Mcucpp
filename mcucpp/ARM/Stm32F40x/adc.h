
#pragma once

#include <stm32f4xx.h>
#include <clock.h>
#include <iopins.h>
#include <pinlist.h>
#include <dma.h>

namespace Mcucpp
{
	
	typedef void (* AdcCallback)(uint16_t *data, size_t count);
	static inline void VoidAdcCallback(uint16_t *data, size_t count){}
	
	struct AdcData
	{
		AdcData()
			:callback(VoidAdcCallback)
		{
		}
		
		AdcCallback callback;
	};
	
	template<class Regs, class CommonRegs, class ClockCtrl, class InputPins, class DmaChannel, uint8_t channelNum>
	class AdcBase
	{
	public:
		// Maximum ADC convertion time MaxCockDivider * AdcConvertionCycles / cpu cycle per wait loop
		static const unsigned AdcTimeoutCycles = 2048 * 28 / 4;
		
		static const unsigned MaxSequence = 16;
		static const unsigned MaxImmediate = 4;
		
		static inline void Init(Divider divider, ClockSource clock, Reference reference);
		static inline void Disable();
		static inline void SetSampleTime(uint8_t channel, unsigned adcCycles);
		static inline void SetResolution(uint8_t bits);
		static inline void SetDivider(Divider divider);
		static inline void SetReference(Reference reference);
		static inline uint8_t ChannelsCount();
		
		enum TriggerMode{RisingEdge, FallingEdge, BothEdges};
		enum SequenceTrigger{Once, FreeRun, ...};
		enum ImmediateTrigger{Once, FreeRun, ...};
		enum Error{Overflow, TransferError, ...};
		
		// immediate
		static inline void SetImmediateTrigger(Trigger trigger, TriggerMode mode);
		static inline bool StartImmediate(uint8_t *channels, uint16_t *data, uint8_t count);
		static inline bool ReadImmediate(uint8_t *channels, uint16_t *data, uint8_t count);
		static inline bool StartImmediate(uint8_t channel);
		static inline uint16_t ReadImmediate();
		static inline uint16_t ReadImmediate(uint8_t channel);
		static inline bool ImmediateReady();
		static inline void StopImmediate();
		
		// continiuos
		static inline void SetSequenceTrigger(Trigger trigger, TriggerMode mode);
		static inline void SetSequenceCallback(AdcCallback callback);
		static inline bool StartSequence(uint8_t *channels, uint8_t channelsCount, uint16_t *dataBuffer, uint16_t scanCount);
		static inline bool SequenceReady();
		static inline void StopSequence();
		
		//info
		static inline int ResolutionBits();
		static inline unsigned ToVolts(uint16_t value); // in 10E-4 Volt units
		static inline unsigned SampleRate();

	};
	
#include <adc_private.h>
	
	typedef AdcBase<Private::Adc1Regs, Private::AdcRegs, Clock::Adc1Clock, Private::Adc1Pins, Dma2Channel0, 0> Adc1;
	typedef AdcBase<Private::Adc2Regs, Private::AdcRegs, Clock::Adc2Clock, Private::Adc2Pins, Dma2Channel3, 1> Adc2;
	typedef AdcBase<Private::Adc3Regs, Private::AdcRegs, Clock::Adc3Clock, Private::Adc3Pins, Dma2Channel1, 2> Adc3;
}
