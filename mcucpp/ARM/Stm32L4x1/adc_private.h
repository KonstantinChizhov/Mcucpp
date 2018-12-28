
#define ADC_BASE_TEMPLATE_ARGS template<class Regs, class CommonRegs, class ClockCtrl, class InputPins, class DmaChannel, typename DmaChannel::RequestType channelNum>
#define ADC_BASE_TEMPLATE_QUALIFIER AdcBase<Regs, CommonRegs, ClockCtrl, InputPins, DmaChannel, channelNum>

constexpr int T_ADCVREG_STUP = 20;

namespace Private
{
	using namespace IO;
	typedef PinList
	<
		NullPin, Pc0, Pc1, Pc2, Pc3,  Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pc4, Pc5, Pb0, Pb1
	> Adc1Pins;

	typedef PinList
	<
		NullPin, Pc0, Pc1, Pc2, Pc3,  Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pc4, Pc5, Pb0, Pb1
	> Adc2Pins;

	typedef PinList
	<
		NullPin, Pc0, Pc1, Pc2, Pc3, NullPin, Pf3, Pf4, Pf5, Pf6, Pf7, Pf8, Pf9, Pf10
	> Adc3Pins;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::VerifyReady(unsigned readyMask)
{
	uint32_t timeout = AdcTimeoutCycles * 4;
	while(Regs()->CR & readyMask && --timeout)
		;
	return timeout != 0;
}
ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::EnableOversampling(unsigned bits, unsigned shift)
{
    if(bits > 8)
        bits = 8;

    if(shift > 8)
        shift = 8;

	Regs()->CFGR2 = ADC_CFGR2_ROVSE
        | (bits << ADC_CFGR2_OVSR_Pos)
        | (shift << ADC_CFGR2_OVSS_Pos);
}

ADC_BASE_TEMPLATE_ARGS
unsigned ADC_BASE_TEMPLATE_QUALIFIER::ClockFreq()
{
	unsigned dividerIndex = ((CommonRegs()->CCR & ADC_CCR_PRESC) >> ADC_CCR_PRESC_Pos);
	static const uint16_t dividers[] = {1, 2, 4, 6, 8, 10, 12, 16, 32, 64, 128, 256};
	return ClockCtrl::ClockFreq() / dividers[dividerIndex];
}

ADC_BASE_TEMPLATE_ARGS
unsigned ADC_BASE_TEMPLATE_QUALIFIER::ConvertionTimeCycles(uint8_t channel)
{
	unsigned sampleTimeBits;
	if(channel <= 9)
	{
		unsigned shift = channel * 3;
		sampleTimeBits = (Regs()->SMPR1 >> shift) & 0x07;
	}
	else
	{
		channel -= 10;
		unsigned shift = channel * 3;
		sampleTimeBits = (Regs()->SMPR2 >> shift) & 0x07;
	}

	static const uint16_t sampleTimes[] = {2, 6, 12, 24, 47, 92, 247, 640};

	return ResolutionBits() + sampleTimes[sampleTimeBits] + 1;
}

ADC_BASE_TEMPLATE_ARGS
unsigned ADC_BASE_TEMPLATE_QUALIFIER::SampleTimeToReg(unsigned sampleTime)
{
	if(sampleTime > 247)
		return 7;
	if(sampleTime > 92)
		return 6;
	if(sampleTime > 47)
		return 5;
	if(sampleTime > 24)
		return 4;
	if(sampleTime > 12)
		return 3;
	if(sampleTime > 6)
		return 2;
	if(sampleTime > 3)
		return 1;
	return 0;
}


ADC_BASE_TEMPLATE_ARGS
int ADC_BASE_TEMPLATE_QUALIFIER::ResolutionBits()
{
	unsigned resField = ((Regs()->CFGR & ADC_CFGR_RES) >> ADC_CFGR_RES_Pos);
	return 12 - resField * 2;
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
	CommonRegs()->CCR = (CommonRegs()->CCR & ~ADC_CCR_PRESC) | divider;
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SetSampleTime(uint8_t channel, unsigned sampleTime)
{
	if(channel > 18)
		return;

	if(!VerifyReady(ADC_CR_JADSTART | ADC_CR_ADSTART))
		return;

	unsigned bitFieldValue = SampleTimeToReg(sampleTime);

	if(channel <= 9)
	{
		unsigned shift = channel * 3;
		Regs()->SMPR1 = (Regs()->SMPR1 & ~(0x07 << shift)) | (bitFieldValue << shift);
	}
	else
	{
		channel -= 10;
		unsigned shift = channel * 3;
		Regs()->SMPR2 = (Regs()->SMPR2 & ~(0x07 << shift)) | (bitFieldValue << shift);
	}
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::Init()
{
	Init(Div1, AdcClock, External);
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::Init(
		AdcDivider divider,
		ClockSource,
		Reference
		)
{
	ClockCtrl::Enable();
	Clock::AdcSel::Set(Clock::AdcClockSel::SysClock);

	Regs()->CR = 0;
	Regs()->CFGR = 0;
	__DSB();
	Regs()->CR = ADC_CR_ADVREGEN;
	delay_us<T_ADCVREG_STUP, F_CPU>();

	SetDivider(divider);
	SetSampleTime(TempSensorChannel, 247);
	SetSampleTime(Battery, 247);
	SetSampleTime(ReferenceChannel, 247);

	Regs()->CR |= ADC_CR_ADCAL;
	if(!VerifyReady(ADC_CR_ADCAL))
	{
		_adcData.error = HardwareError;
	}
	Regs()->CR |= ADC_CR_ADCALDIF;
	Regs()->CR |= ADC_CR_ADCAL;
	if(!VerifyReady(ADC_CR_ADCAL))
	{
		_adcData.error = HardwareError;
	}

    Regs()->CR &= ~ADC_CR_ADCALDIF;
	Regs()->CR |= ADC_CR_ADEN;
	CommonRegs()->CCR |= ADC_CCR_VREFEN;
	_adcData.vRef = 0;

	NVIC_EnableIRQ(ADC1_2_IRQn);
	NVIC_EnableIRQ(ADC3_IRQn);
}

ADC_BASE_TEMPLATE_ARGS
uint8_t ADC_BASE_TEMPLATE_QUALIFIER::ChannelsCount()
{
	return 18;
}

ADC_BASE_TEMPLATE_ARGS
uint8_t ADC_BASE_TEMPLATE_QUALIFIER::SetResolution(uint8_t bits)
{
	if(bits > 12)
		bits = 12;
	if(bits < 6)
		bits = 6;
	unsigned resField = (12 - bits) / 2 ;
	Regs()->CFGR = (Regs()->CFGR & ADC_CFGR_RES) | (resField << ADC_CFGR_RES_Pos);
	return ResolutionBits();
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::Disable()
{
	Regs()->CR = ADC_CR_ADDIS;
	__DSB();
	Regs()->CR = ADC_CR_DEEPPWD;
	Regs()->CFGR = 0;
	//CommonRegs()->CCR = 0;
	ClockCtrl::Disable();
}

ADC_BASE_TEMPLATE_ARGS
void  ADC_BASE_TEMPLATE_QUALIFIER::SetImmediateTrigger(
	ImmediateTrigger trigger,
	TriggerMode mode)
{
	Regs()->CR |= ADC_CR_JADSTP;
	if(!VerifyReady(ADC_CR_JADSTP))
	{
		_adcData.error = HardwareError;
	}

	if(trigger == ImmTriggerOnce)
	{
		mode = TriggerNone;
	}
	Regs()->JSQR = (Regs()->JSQR & ~(ADC_JSQR_JEXTSEL | ADC_JSQR_JEXTEN) ) | ((trigger&0x0f) << 16) | (mode << ADC_JSQR_JEXTEN_Pos);
}

template<class Pins, class CommonRegs>
static inline void EnableChannel(uint8_t channel)
{
	if(channel == AdcCommon::TempSensorChannel)
	{
		CommonRegs()->CCR |= ADC_CCR_TSEN;
	}
	else if(channel == AdcCommon::ReferenceChannel)
	{
		CommonRegs()->CCR |= ADC_CCR_VREFEN;
	}
	else if(channel == AdcCommon::Battery)
	{
		CommonRegs()->CCR |= ADC_CCR_VBATEN;
	}else
	{
		Pins::SetConfiguration(1u << channel, Pins::Analog);
	}
}

static inline unsigned GetJsqr(const uint8_t *channels, uint8_t count, uint32_t jsqr)
{
	jsqr &= (ADC_JSQR_JEXTEN | ADC_JSQR_JEXTSEL);
	jsqr |= ((count-1) << ADC_JSQR_JL_Pos) | (channels[0] << ADC_JSQR_JSQ1_Pos);

	if(count > 1) jsqr |= (channels[1] << ADC_JSQR_JSQ2_Pos);
	if(count > 2) jsqr |= (channels[2] << ADC_JSQR_JSQ3_Pos);
	if(count > 3) jsqr |= (channels[3] << ADC_JSQR_JSQ4_Pos);
	return jsqr;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::StartImmediate(const uint8_t *channels, uint16_t *data, uint8_t count, AdcCallbackType callback)
{
	if(count == 0 || count > 4)
	{
		_adcData.error = ArgumentError;
		return false;
	}

	//Regs()->CR |= ADC_CR_JADSTP;
	if(!VerifyReady(ADC_CR_JADSTP | ADC_CR_JADSTART))
	{
		_adcData.error = HardwareError;
		return false;
	}

    Regs()->CFGR |= ADC_CFGR_JQDIS;

	_adcData.immCallback = callback;
	_adcData.immData = data;

	for(unsigned i =0; i< count; i++)
		EnableChannel<Pins, CommonRegs>(channels[i]);

	Regs()->JSQR = GetJsqr(channels, count, Regs()->JSQR);
	if(callback)
	{
		Regs()->IER |= ADC_IER_JEOSIE;
	}
	else
	{
		Regs()->IER &= ~ADC_IER_JEOSIE;
	}

	Regs()->CR |= ADC_CR_JADSTART;

	return true;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::ReadImmediate(const uint8_t *channels, uint16_t *data, uint8_t count)
{
	bool result = false, alreadyStarted = false;
	if(Regs()->CR & ADC_CR_JADSTART)
	{
		unsigned jsqr = Regs()->JSQR;
		alreadyStarted = (jsqr == GetJsqr(channels, count, jsqr));
		if(!alreadyStarted)
		{
			_adcData.error = SequenceError;
			return result;
		}
	}
	if(alreadyStarted || StartImmediate(channels, data, count, 0))
	{
		unsigned status, timeout = AdcTimeoutCycles;
		do
		{
			status = Regs()->ISR;
		}while(((status & ADC_ISR_JEOS) == 0) && --timeout);

		if((status & ADC_ISR_JEOS) == 0 || !timeout)
		{
			_adcData.error = HardwareError;
		}else
		{
			result = true;

			data[0] = Regs()->JDR1;
			if(count > 1) data[1] = Regs()->JDR2;
			if(count > 2) data[2] = Regs()->JDR3;
			if(count > 3)data[3] = Regs()->JDR4;
			_adcData.error = NoError;
		}
		Regs()->ISR |= ADC_ISR_JEOS | ADC_ISR_JEOC;
	}
	return result;
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::DmaHandler(void * data, size_t size, bool success)
{
	DmaChannel::Disable();
	Regs()->CFGR &= ~(ADC_CFGR_DMAEN);
	Regs()->ISR |= (ADC_ISR_EOC | ADC_ISR_EOS);

	if(success)
	{
		if(_adcData.seqCallback)
			_adcData.seqCallback((uint16_t*)data, size);
	}
	else
		_adcData.error = TransferError;
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::IrqHandler()
{
	unsigned sr = Regs()->ISR;
	if(sr & ADC_ISR_OVR)
	{
		_adcData.error = Overflow;
	}
	if(sr & ADC_ISR_JEOC)
	{
		//
	}
	if(sr & ADC_ISR_JEOS)
	{
		unsigned count = ((Regs()->JSQR & ADC_JSQR_JL) >> ADC_JSQR_JL_Pos) + 1;
		uint16_t *data = _adcData.immData;
		if(data)
		{
			data[0] = Regs()->JDR1;
			if(count > 1) data[1] = Regs()->JDR2;
			if(count > 2) data[2] = Regs()->JDR3;
			if(count > 3) data[3] = Regs()->JDR4;
			_adcData.error = NoError;

			if(_adcData.immCallback)
				_adcData.immCallback(data, count);
		}
	}
	// reset all flags
	Regs()->ISR = sr;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::StartImmediate(uint8_t channel)
{
	if(channel > ChannelsCount())
    {
        _adcData.error = ArgumentError;
        return false;
    }

    if(!VerifyReady(ADC_CR_JADSTP | ADC_CR_JADSTART))
	{
		_adcData.error = HardwareError;
		return false;
	}

    Regs()->CFGR |= ADC_CFGR_JQDIS;
	Regs()->ISR |= (ADC_ISR_JEOS | ADC_ISR_JEOC | ADC_ISR_JQOVF);
	Regs()->JSQR = (unsigned)channel << ADC_JSQR_JSQ1_Pos;

	EnableChannel<Pins, CommonRegs>(channel);

	Regs()->CR |= ADC_CR_JADSTART;
	return true;
}

ADC_BASE_TEMPLATE_ARGS
uint16_t ADC_BASE_TEMPLATE_QUALIFIER::ReadImmediate()
{
	unsigned status, timeout = AdcTimeoutCycles;
	uint16_t result = InvalidValue;
	do
	{
		status = Regs()->ISR;
	}while(((status & (ADC_ISR_JEOS | ADC_ISR_JEOC)) == 0) && --timeout);

	if((status & (ADC_ISR_JEOS | ADC_ISR_JEOC)) == 0)
	{
		_adcData.error = HardwareError;
	}else
	{
		result = Regs()->JDR1;
		_adcData.error = NoError;
	}

	Regs()->ISR |= (ADC_ISR_JEOS | ADC_ISR_JEOC | ADC_ISR_JQOVF);
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
	return (Regs()->ISR & ( ADC_ISR_JEOS)) == (ADC_ISR_JEOS);
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::StopImmediate()
{
	Regs()->CR |= ADC_CR_JADSTP;
	if(!VerifyReady(ADC_CR_JADSTP))
	{
		_adcData.error = HardwareError;
	}
	Regs()->ISR &= ~(ADC_ISR_JEOS | ADC_ISR_JEOC);
	Regs()->JSQR = 0;
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SetSequenceTrigger(SequenceTrigger trigger, TriggerMode mode)
{
	if(trigger == SeqTriggerOnce)
	{
		mode = TriggerNone;
	}
	Regs()->CFGR = (Regs()->CFGR & ~(ADC_CFGR_EXTSEL | ADC_CFGR_EXTEN) ) | ((trigger&0x0f) << ADC_CFGR_EXTSEL_Pos) | (mode << ADC_CFGR_EXTEN_Pos);
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SetSequenceCallback(AdcCallbackType callback)
{
	_adcData.seqCallback = callback;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::StartSequence(std::initializer_list<uint8_t> channels, uint16_t *dataBuffer, uint16_t scanCount)
{
	if(scanCount == 0 || channels.size() == 0)
		return false;

	if(!VerifyReady(ADC_CR_ADSTART))
		return false;
	Regs()->ISR |= ADC_ISR_OVR | ADC_ISR_EOC | ADC_ISR_EOS;

	if(channels.size() <= 16)
	{
		Regs()->SQR1 = ((channels.size() - 1) << ADC_SQR1_L_Pos);
		Regs()->SQR3 = 0;
		Regs()->SQR2 = 0;
		unsigned i = 0;
		for(uint8_t channel : channels)
		{
			Pins::SetConfiguration(1u << channel, Pins::Analog);
			if(i < 4)
			{
				Regs()->SQR1 |= (channel & 0x1f) << 6*(i+1);
			}else if(i < 9)
			{
				Regs()->SQR2 |= (channel & 0x1f) << 6*(i-4);
			}else if(i < 14)
			{
				Regs()->SQR3 |= (channel & 0x1f) << 6*(i-9);
			}else
			{
				Regs()->SQR4 |= (channel & 0x1f) << 6*(i-14);
			}
			i++;
		}

		DmaChannel::SetTransferCallback(DmaHandler);
		DmaChannel::SetRequest(channelNum);
		DmaChannel::Transfer(DmaMode::Periph2Mem | DmaMode::MemIncriment | DmaMode::PriorityHigh | DmaMode::PSize16Bits | DmaMode::MSize16Bits,
				dataBuffer, &Regs()->DR, channels.size() * scanCount);

		_adcData.error = NoError;

		uint32_t controlReg = Regs()->CFGR;
		controlReg |= ADC_CFGR_DMAEN;
		if(scanCount > 1)
			controlReg |= ADC_CFGR_CONT;
		Regs()->CFGR = controlReg;

		// start conversion now if no external trigger selected
		if((controlReg & ADC_CFGR_EXTEN) == 0)
			Regs()->CR |= ADC_CR_ADSTART;

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
	if(scanCount == 0 || channelsCount == 0)
		return false;

	if(!VerifyReady(ADC_CR_ADSTART))
		return false;
	Regs()->ISR |= ADC_ISR_OVR | ADC_ISR_EOC | ADC_ISR_EOS;

	if(channelsCount <= 16)
	{
		Regs()->SQR1 = ((channelsCount - 1) << ADC_SQR1_L_Pos);
		Regs()->SQR3 = 0;
		Regs()->SQR2 = 0;
		Regs()->SQR4 = 0;
		for(unsigned i = 0; i < channelsCount; i++)
		{
			Pins::SetConfiguration(1u << channels[i], Pins::Analog);
			if(i < 4)
			{
				Regs()->SQR1 |= (channels[i] & 0x1f) << 6*(i+1);
			}else if(i < 9)
			{
				Regs()->SQR2 |= (channels[i] & 0x1f) << 6*(i-4);
			}else if(i < 14)
			{
				Regs()->SQR3 |= (channels[i] & 0x1f) << 6*(i-9);
			}else
			{
				Regs()->SQR4 |= (channels[i] & 0x1f) << 6*(i-14);
			}
		}

		DmaChannel::SetTransferCallback(DmaHandler);
		DmaChannel::SetRequest(channelNum);
		DmaChannel::Transfer(DmaMode::Periph2Mem | DmaMode::MemIncriment | DmaMode::PriorityHigh | DmaMode::PSize16Bits | DmaMode::MSize16Bits,
				dataBuffer, &Regs()->DR, channelsCount * scanCount);

		_adcData.error = NoError;

		uint32_t controlReg = Regs()->CFGR;
		controlReg |= ADC_CFGR_DMAEN;
		if(scanCount > 1)
			controlReg |= ADC_CFGR_CONT;
		Regs()->CFGR = controlReg;

		// start conversion now if no external trigger selected
		if((controlReg & ADC_CFGR_EXTEN) == 0)
			Regs()->CR |= ADC_CR_ADSTART;

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
	Regs()->CR |= (ADC_CR_ADSTP);
	VerifyReady(ADC_CR_ADSTP);
	Regs()->SQR1 = 0;
	Regs()->SQR2 = 0;
	Regs()->SQR3 = 0;
	Regs()->SQR4 = 0;
}

ADC_BASE_TEMPLATE_ARGS
AdcCommon::AdcError ADC_BASE_TEMPLATE_QUALIFIER::GetError()
{
	return _adcData.error;
}

namespace Private
{
	IO_STRUCT_WRAPPER(ADC1, Adc1Regs, ADC_TypeDef);
	IO_STRUCT_WRAPPER(ADC2, Adc2Regs, ADC_TypeDef);
	IO_STRUCT_WRAPPER(ADC3, Adc3Regs, ADC_TypeDef);

	IO_STRUCT_WRAPPER(ADC123_COMMON, AdcRegs, ADC_Common_TypeDef);
}

ADC_BASE_TEMPLATE_ARGS
AdcData ADC_BASE_TEMPLATE_QUALIFIER::_adcData;
