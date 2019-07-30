
#pragma once

#include <mcu_header.h>
#include <clock.h>
#include <iopins.h>
#include <pinlist.h>
#include <platform_dalay.h>
#include <initializer_list>

namespace Mcucpp
{
namespace Private
{
typedef IO::PinList<
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
	IO::Pc5>
	Adc1Pins;
}

typedef void (*AdcCallbackType)(uint16_t *data, size_t count);

class AdcCommon
{
  public:
  	static const unsigned MaxSequence = 16;
	static const unsigned MaxImmediate = 4;
	enum AdcError
	{
		NoError,
		Overflow,
		TransferError,
		HardwareError,
		ArgumentError,
		SequenceError,
		NotReady
	};

	typedef uint16_t DataT;
	static const uint8_t ChannelCount = 18;

	enum Reference
	{
		External = 0,
		VCC = 0
	};

	static const uint8_t TempSensorChannel = 16;
	static const uint8_t ReferenceChannel = 17;

	enum SequenceTrigger
		{
			SeqTriggerOnce = 0x10,
			SeqTriggerTimer1CC1Event = 0,
			SeqTriggerTimer1CC2Event,
			SeqTriggerTimer1CC3Event,
			SeqTriggerTimer2CC2Event,
			SeqTriggerTimer3TRGOEvent,
			SeqTriggerTimer4CC4Event,
			SeqTriggerEXTIline11
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
		};

		enum TriggerMode{TriggerNone, TriggerRisingEdge};
};

struct AdcData
{
	AdcData()
		: seqCallback(nullptr),
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

#define ADC_BASE_TEMPLATE_ARGS template <class Regs, class ClockCtrl, class InputPins, class DmaChannel>
#define ADC_BASE_TEMPLATE_QUALIFIER AdcBase<Regs, ClockCtrl, InputPins, DmaChannel>

template <class Regs, class ClockCtrl, class InputPins, class DmaChannel>
class AdcBase : public AdcCommon
{
	// Maximum ADC convertion time MaxCockDivider * AdcConvertionCycles / cpu cycle per wait loop
	static const unsigned AdcTimeoutCycles = 2048 * 28 / 4;

  public:
	typedef ClockCtrl Clock;
	typedef InputPins Pins;

	typedef typename ClockCtrl::Prescaller AdcDivider;
	typedef typename ClockCtrl::ClockSource ClockSource;

	// Clock sources
	static const ClockSource AdcClock = ClockCtrl::Apb2;
	static const ClockSource MainClock = ClockCtrl::Apb2;

	// Clock dividers
	static const AdcDivider Div2 = ClockCtrl::Div2;
	static const AdcDivider Div4 = ClockCtrl::Div4;
	static const AdcDivider Div6 = ClockCtrl::Div6;
	static const AdcDivider Div8 = ClockCtrl::Div8;

	static void SelectClockSource(ClockSource);
	static void SetReference(Reference);
	static void SetDivider(AdcDivider divider);
	static void SetSampleTime(uint8_t channel, unsigned sampleTime);
	static void Init();
	static void Init(AdcDivider divider, ClockSource, Reference);
	static uint8_t SetResolution(uint8_t bits);
	static void Disable();
	static void SetImmediateTrigger(ImmediateTrigger trigger, TriggerMode mode);

	// Channels
	static inline uint8_t ChannelsCount() { return 18; }

	static inline bool StartImmediate(const uint8_t *channels, uint16_t *data, uint8_t count, AdcCallbackType callback = nullptr);
	// Blocking conversion
	static inline bool ReadImmediate(const uint8_t *channels, uint16_t *data, uint8_t count);

	//
	static inline bool StartImmediate(uint8_t channel);
	static inline uint16_t ReadImmediate();

	static inline uint16_t ReadImmediate(uint8_t channel);

	template <class Pin>
	static inline uint16_t ReadImmediate()
	{
		const int index = Pins::template PinIndex<Pin>::Value;
		STATIC_ASSERT(index >= 0);
		return ReadImmediate(index);
	}

	static inline bool ImmediateReady();
	static inline void StopImmediate();

	// continues
	static inline void SetSequenceTrigger(SequenceTrigger trigger, TriggerMode mode);
	static inline void SetSequenceCallback(AdcCallbackType callback);
	/// start conversion sequence of channelsCount channels (maximum MaxSequence (16))
	/// dataBuffer has to be channelsCount * scanCount elements
	static inline bool StartSequence(const uint8_t *channels, uint8_t channelsCount, uint16_t *dataBuffer, uint16_t scanCount);
	static inline bool StartSequence(std::initializer_list<uint8_t> channels, uint16_t *dataBuffer, uint16_t scanCount);

	template <class... Inpunts>
	static inline bool StartSequence(uint16_t *dataBuffer, uint16_t scanCount)
	{
		return StartSequence({Pins::template PinIndex<Inpunts>::Value...}, dataBuffer, scanCount);
	}

	template <class Pin>
	constexpr static unsigned ChannelNum()
	{
		return Pins::template PinIndex<Pin>::Value;
	}

	static inline bool SequenceReady();
	static inline void StopSequence();

	//info
	static inline AdcError GetError();
	static inline int ResolutionBits();
	static inline unsigned ToVolts(uint16_t value); // in 10E-4 Volt units
	static inline unsigned ClockFreq();
	static inline unsigned ConvertionTimeCycles(uint8_t channel);
	static inline unsigned AdcPeriodUs10(uint8_t channel);

	static inline int16_t ReadTemperature();

	static void DmaHandler(void *data, size_t size, bool success);
	static void IrqHandler();
	//private:
	static bool VerifyReady(unsigned);
	static unsigned SampleTimeToReg(unsigned sampleTime);
	static AdcData _adcData;
};

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::VerifyReady(unsigned readyMask)
{
	uint32_t timeout = AdcTimeoutCycles * 4;
	while ((Regs()->SR & readyMask) && --timeout)
		;
	return timeout != 0;
}

ADC_BASE_TEMPLATE_ARGS
unsigned ADC_BASE_TEMPLATE_QUALIFIER::ClockFreq()
{
	return ClockCtrl::ClockFreq();
}

ADC_BASE_TEMPLATE_ARGS
unsigned ADC_BASE_TEMPLATE_QUALIFIER::ConvertionTimeCycles(uint8_t channel)
{
	unsigned sampleTimeBits;
	if (channel <= 9)
	{
		unsigned shift = channel * 3;
		sampleTimeBits = (Regs()->SMPR2 >> shift) & 0x07;
	}
	else
	{
		channel -= 10;
		unsigned shift = channel * 3;
		sampleTimeBits = (Regs()->SMPR1 >> shift) & 0x07;
	}

	static const uint16_t sampleTimes[] = {1, 7, 13, 28, 41, 55, 71, 239};

	return ResolutionBits() + sampleTimes[sampleTimeBits] + 1;
}

ADC_BASE_TEMPLATE_ARGS
unsigned ADC_BASE_TEMPLATE_QUALIFIER::AdcPeriodUs10(uint8_t channel)
{
	unsigned adcTickUs4 = (4000000000u / ClockFreq());
	unsigned adcTickUs10 = adcTickUs4 * 2 + adcTickUs4 / 2;
	return (adcTickUs10 * ConvertionTimeCycles(channel) + adcTickUs10 / 2);
}

ADC_BASE_TEMPLATE_ARGS
unsigned ADC_BASE_TEMPLATE_QUALIFIER::SampleTimeToReg(unsigned sampleTime)
{
	if (sampleTime > 239)
		return 7;
	if (sampleTime > 71)
		return 6;
	if (sampleTime > 55)
		return 5;
	if (sampleTime > 41)
		return 4;
	if (sampleTime > 28)
		return 3;
	if (sampleTime > 13)
		return 2;
	if (sampleTime > 7)
		return 1;
	return 0;
}

ADC_BASE_TEMPLATE_ARGS
int ADC_BASE_TEMPLATE_QUALIFIER::ResolutionBits()
{
	return 12;
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SelectClockSource(ClockSource)
{
	// Not supported.
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SetReference(Reference)
{
	// Not supported.
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SetDivider(AdcDivider divider)
{
	ClockCtrl::SetPrescaller(divider);
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SetSampleTime(uint8_t channel, unsigned sampleTime)
{
	if (channel > 18)
		return;

	unsigned bitFieldValue = SampleTimeToReg(sampleTime);

	if (channel <= 9)
	{
		unsigned shift = channel * 3;
		Regs()->SMPR2 = (Regs()->SMPR2 & ~(0x07 << shift)) | (bitFieldValue << shift);
	}
	else
	{
		channel -= 10;
		unsigned shift = channel * 3;
		Regs()->SMPR1 = (Regs()->SMPR1 & ~(0x07 << shift)) | (bitFieldValue << shift);
	}
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::Init()
{
	Init(Div2, AdcClock, External);
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::Init(
	AdcDivider divider,
	ClockSource,
	Reference)
{
	Clock::Enable();
	SelectClockSource(AdcClock);
	SetDivider(divider);
	Regs()->CR1 = 0;
	Regs()->SQR1 = 0;
	Regs()->SQR2 = 0;
	Regs()->SQR3 = 0;
	Regs()->JSQR = 0;

	Regs()->CR2 = ADC_CR2_ADON | ADC_CR2_EXTSEL | ADC_CR2_EXTTRIG;

	Regs()->CR2 |= ADC_CR2_RSTCAL;
	while (Regs()->CR2 & ADC_CR2_RSTCAL)
		;
	DelayLoop(100); // need delay at least one ADC cycle

	Regs()->CR2 |= ADC_CR2_CAL;
	while (Regs()->CR2 & ADC_CR2_CAL)
		;

	NVIC_EnableIRQ(ADC1_IRQn);
}

ADC_BASE_TEMPLATE_ARGS
uint8_t ADC_BASE_TEMPLATE_QUALIFIER::SetResolution(uint8_t bits)
{
	(void)bits;
	return ResolutionBits();
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::Disable()
{
	Regs()->CR1 = 0;
	Regs()->CR2 = 0;
	ClockCtrl::Disable();
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SetImmediateTrigger(
	ImmediateTrigger trigger,
	TriggerMode mode)
{
	if (trigger == ImmTriggerOnce)
	{
		mode = TriggerNone;
	}
	Regs()->CR2 = (Regs()->CR2 & ~(ADC_CR2_JEXTSEL | ADC_CR2_JEXTTRIG)) | ((trigger & 0x07) << 12) | (mode << 15);
}

template <class Pins, class Regs>
static inline void EnableChannel(uint8_t channel)
{
	if (channel == AdcCommon::TempSensorChannel)
	{
		Regs()->CR2 |= ADC_CR2_TSVREFE;
	}
	else
	{
		Pins::SetConfiguration(1u << channel, Pins::Analog);
	}
}

static inline unsigned GetJsqr(const uint8_t *channels, uint8_t count, uint32_t jsqr)
{
	jsqr = ((count - 1) << 20) | (channels[0] << 0);

	if (count > 1)
		jsqr |= (channels[1] << 5);
	if (count > 2)
		jsqr |= (channels[2] << 10);
	if (count > 3)
		jsqr |= (channels[3] << 15);
	return jsqr;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::StartImmediate(const uint8_t *channels, uint16_t *data, uint8_t count, AdcCallbackType callback)
{
	if (count == 0 || count > 4)
	{
		_adcData.error = ArgumentError;
		return false;
	}

	if (!VerifyReady(ADC_SR_JSTRT))
	{
		_adcData.error = HardwareError;
		return false;
	}

	Regs()->CR1 |= ADC_CR1_JDISCEN;

	_adcData.immCallback = callback;
	_adcData.immData = data;

	for (unsigned i = 0; i < count; i++)
		EnableChannel<Pins, Regs>(channels[i]);

	Regs()->JSQR = GetJsqr(channels, count, Regs()->JSQR);
	if (callback)
	{
		Regs()->CR1 |= ADC_CR1_JEOCIE;
	}
	else
	{
		Regs()->CR1 &= ~ADC_CR1_JEOCIE;
	}

	Regs()->CR2 |= ADC_CR2_JEXTTRIG | ADC_CR2_JEXTSEL;
	Regs()->CR2 |= ADC_CR2_JSWSTART;

	return true;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::ReadImmediate(const uint8_t *channels, uint16_t *data, uint8_t count)
{
	bool result = false, alreadyStarted = false;
	if (Regs()->SR & ADC_SR_JSTRT)
	{
		unsigned jsqr = Regs()->JSQR;
		alreadyStarted = (jsqr == GetJsqr(channels, count, jsqr));
		if (!alreadyStarted)
		{
			_adcData.error = SequenceError;
			return result;
		}
	}
	if (alreadyStarted || StartImmediate(channels, data, count, 0))
	{
		unsigned status, timeout = AdcTimeoutCycles;
		do
		{
			status = Regs()->SR;
		} while (((status & ADC_SR_JEOC) == 0) && --timeout);

		if ((status & ADC_SR_JEOC) == 0 || !timeout)
		{
			_adcData.error = HardwareError;
		}
		else
		{
			result = true;

			data[0] = Regs()->JDR1;
			if (count > 1)
				data[1] = Regs()->JDR2;
			if (count > 2)
				data[2] = Regs()->JDR3;
			if (count > 3)
				data[3] = Regs()->JDR4;
			_adcData.error = NoError;
		}
		Regs()->SR &= ~(ADC_SR_JEOC);
	}
	return result;
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::DmaHandler(void *data, size_t size, bool success)
{
	DmaChannel::Disable();
	Regs()->CR2 &= ~(ADC_CR2_DMA);
	Regs()->SR = ~(ADC_SR_EOC);

	if (success)
	{
		if (_adcData.seqCallback)
			_adcData.seqCallback((uint16_t *)data, size);
	}
	else
		_adcData.error = TransferError;
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::IrqHandler()
{
	unsigned sr = Regs()->SR;

	if (sr & ADC_SR_JEOC)
	{
		unsigned count = ((Regs()->JSQR & ADC_JSQR_JL) >> 20) + 1;
		uint16_t *data = _adcData.immData;
		if (data)
		{
			data[0] = Regs()->JDR1;
			if (count > 1)
				data[1] = Regs()->JDR2;
			if (count > 2)
				data[2] = Regs()->JDR3;
			if (count > 3)
				data[3] = Regs()->JDR4;
			_adcData.error = NoError;

			if (_adcData.immCallback)
				_adcData.immCallback(data, count);
		}
	}
	// reset all flags
	Regs()->SR &= ~(ADC_SR_JEOC | ADC_SR_JSTRT);
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::StartImmediate(uint8_t channel)
{
	if (channel > ChannelsCount())
	{
		_adcData.error = ArgumentError;
		return false;
	}

	if (!VerifyReady(ADC_SR_JSTRT))
	{
		_adcData.error = HardwareError;
		return false;
	}

	Regs()->CR1 |= ADC_CR1_DISCEN;
	Regs()->SR = ~(ADC_SR_JEOC);
	Regs()->JSQR = (unsigned)channel;

	EnableChannel<Pins, Regs>(channel);

	Regs()->CR2 |= ADC_CR2_JEXTTRIG | ADC_CR2_JEXTSEL;
	Regs()->CR2 |= ADC_CR2_JSWSTART;
	return true;
}

ADC_BASE_TEMPLATE_ARGS
uint16_t ADC_BASE_TEMPLATE_QUALIFIER::ReadImmediate()
{
	unsigned status, timeout = AdcTimeoutCycles;
	uint16_t result = 0xffff;
	do
	{
		status = Regs()->SR;
	} while (((status & (ADC_SR_JEOC)) == 0) && --timeout);

	if ((status & (ADC_SR_JEOC)) == 0)
	{
		_adcData.error = HardwareError;
	}
	else
	{
		result = Regs()->JDR1;
		_adcData.error = NoError;
	}

	Regs()->SR &= ~(ADC_SR_JEOC);
	return result;
}

ADC_BASE_TEMPLATE_ARGS
uint16_t ADC_BASE_TEMPLATE_QUALIFIER::ReadImmediate(uint8_t channel)
{
	StartImmediate(channel);
	return ReadImmediate();
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::ImmediateReady()
{
	return (Regs()->SR & (ADC_SR_JEOC)) == (ADC_SR_JEOC);
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::StopImmediate()
{
	Regs()->SR &= ~(ADC_SR_JSTRT | ADC_SR_JEOC);
	Regs()->JSQR = 0;
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SetSequenceTrigger(SequenceTrigger trigger, TriggerMode mode)
{
	if (trigger == SeqTriggerOnce)
	{
		mode = TriggerNone;
	}
	Regs()->CR2 = (Regs()->CR2 & ~(ADC_CR2_EXTSEL | ADC_CR2_EXTTRIG)) | ((trigger & 0x0f) << 17) | (mode << 20);
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SetSequenceCallback(AdcCallbackType callback)
{
	_adcData.seqCallback = callback;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::StartSequence(std::initializer_list<uint8_t> channels, uint16_t *dataBuffer, uint16_t scanCount)
{
	if (scanCount == 0 || channels.size() == 0)
	{
		_adcData.error = ArgumentError;
		return false;
	}

	if (!VerifyReady(ADC_SR_STRT))
	{
		_adcData.error = NotReady;
		return false;
	}

	Regs()->SR &= ~(ADC_SR_STRT | ADC_SR_EOC);

	if (channels.size() <= MaxSequence)
	{
		Regs()->SQR1 = ((channels.size() - 1) << 20);
		Regs()->SQR3 = 0;
		Regs()->SQR2 = 0;
		Regs()->SQR4 = 0;
		unsigned i = 0;
		for (uint8_t channel : channels)
		{
			Pins::SetConfiguration(1u << channel, Pins::Analog);
			if (i < 6)
			{
				Regs()->SQR3 |= (channel & 0x1f) << 5 * (i);
			}
			else if (i < 12)
			{
				Regs()->SQR2 |= (channel & 0x1f) << 5 * (i - 6);
			}
			else
			{
				Regs()->SQR1 |= (channel & 0x1f) << 5 * (i - 12);
			}
			i++;
		}

		DmaChannel::SetTransferCallback(DmaHandler);
		//DmaChannel::SetRequest(channelNum);
		DmaChannel::Transfer(DmaBase::Periph2Mem | DmaBase::MemIncriment | DmaBase::PriorityHigh | DmaBase::PSize16Bits | DmaBase::MSize16Bits,
							 dataBuffer, &Regs()->DR, channels.size() * scanCount);

		_adcData.error = NoError;

		uint32_t controlReg = Regs()->CR2;
		controlReg |= ADC_CR2_DMA;
		if (scanCount > 1)
			controlReg |= ADC_CR2_CONT;
		Regs()->CR2 = controlReg;

		// start conversion now if no external trigger selected
		if ((controlReg & ADC_CR2_EXTTRIG) == 0)
			Regs()->CR2 |= ADC_CR2_SWSTART;

		return true;
	}
	else
	{
		_adcData.error = ArgumentError;
		return false;
	}
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::StartSequence(const uint8_t *channels, uint8_t channelsCount, uint16_t *dataBuffer, uint16_t scanCount)
{
	if (scanCount == 0 || channelsCount == 0)
	{
		_adcData.error = ArgumentError;
		return false;
	}

	if (!VerifyReady(ADC_SR_STRT))
	{
		_adcData.error = NotReady;
		return false;
	}

	Regs()->SR &= ADC_SR_STRT | ADC_SR_EOC;

	if (channelsCount <= MaxSequence)
	{
		Regs()->SQR1 = ((channelsCount - 1) << 20);
		Regs()->SQR3 = 0;
		Regs()->SQR2 = 0;
		Regs()->SQR4 = 0;
		for (unsigned i = 0; i < channelsCount; i++)
		{
			Pins::SetConfiguration(1u << channels[i], Pins::Analog);
			if (i < 6)
			{
				Regs()->SQR3 |= (channels[i] & 0x1f) << 5 * (i);
			}
			else if (i < 12)
			{
				Regs()->SQR2 |= (channels[i] & 0x1f) << 5 * (i - 6);
			}
			else
			{
				Regs()->SQR1 |= (channels[i] & 0x1f) << 5 * (i - 12);
			}
		}

		DmaChannel::SetTransferCallback(DmaHandler);
		DmaChannel::Transfer(DmaBase::Periph2Mem | DmaBase::MemIncriment | DmaBase::PriorityHigh | DmaBase::PSize16Bits | DmaBase::MSize16Bits,
							 dataBuffer, &Regs()->DR, channelsCount * scanCount);

		_adcData.error = NoError;

		uint32_t controlReg = Regs()->CR2;
		controlReg |= ADC_CR2_DMA;
		if (scanCount > 1)
			controlReg |= ADC_CR2_CONT;
		Regs()->CR2 = controlReg;

		// start conversion now if no external trigger selected
		if ((controlReg & ADC_CR2_EXTTRIG) == 0)
			Regs()->CR2 |= ADC_CR2_SWSTART;

		return true;
	}
	else
	{
		_adcData.error = ArgumentError;
		return false;
	}
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::SequenceReady()
{
	return DmaChannel::Ready();
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::StopSequence()
{
	DmaChannel::Disable();
	Regs()->SR &= ~(ADC_SR_STRT | ADC_SR_EOC);
	Regs()->SQR1 = 0;
	Regs()->SQR2 = 0;
	Regs()->SQR3 = 0;
}

ADC_BASE_TEMPLATE_ARGS
AdcCommon::AdcError ADC_BASE_TEMPLATE_QUALIFIER::GetError()
{
	return _adcData.error;
}

ADC_BASE_TEMPLATE_ARGS
int16_t ADC_BASE_TEMPLATE_QUALIFIER::ReadTemperature()
{
	SetSampleTime(TempSensorChannel, 250);
	uint16_t rawValue = ReadImmediate(TempSensorChannel);
	int16_t v25 = 14100;	// x10000
	int16_t Avg_Slope = 43; // x10000
	int16_t value = (v25 - ToVolts(rawValue)) / Avg_Slope + 25;
	return value;
}

ADC_BASE_TEMPLATE_ARGS
AdcData ADC_BASE_TEMPLATE_QUALIFIER::_adcData;

namespace Private
{
IO_STRUCT_WRAPPER(ADC1, Adc1Regs, ADC_TypeDef);
}

typedef AdcBase<Private::Adc1Regs, Clock::Adc1Clock, Private::Adc1Pins, Dma1Channel1> Adc1;
} // namespace Mcucpp
