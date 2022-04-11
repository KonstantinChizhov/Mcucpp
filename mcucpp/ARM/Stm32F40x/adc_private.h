
#define ADC_BASE_TEMPLATE_ARGS template<class Regs, class CommonRegs, class ClockCtrl, class InputPins, class DmaChannel, typename DmaChannel::RequestType channelNum>
#define ADC_BASE_TEMPLATE_QUALIFIER AdcBase<Regs, CommonRegs, ClockCtrl, InputPins, DmaChannel, channelNum>

namespace Private
{
	using namespace IO;
	typedef PinList
	<
		Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pb0, Pb1, Pc0, Pc1, Pc2, Pc3, Pc4, Pc5
	> Adc1Pins;
	
	typedef PinList
	<
		Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pb0, Pb1, Pc0, Pc1, Pc2, Pc3, Pc4, Pc5
	> Adc2Pins;
	
	typedef PinList
	<
		Pa0, Pa1, Pa2, Pa3, Pf6, Pf7, Pf8, Pf9, Pf10, Pf3, Pc0, Pc1, Pc2, Pc3, Pf4, Pf5
	> Adc3Pins;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::VerifyReady(unsigned readyMask)
{
	uint32_t timeout = AdcTimeoutCycles * 4;
	while(Regs()->CR2 & readyMask && --timeout)
		;
	return timeout != 0;
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::EnableOversampling(unsigned bits, unsigned shift)
{
	// Not supported
}

ADC_BASE_TEMPLATE_ARGS
unsigned ADC_BASE_TEMPLATE_QUALIFIER::ClockFreq()
{
	unsigned divider = (((CommonRegs()->CCR & ADC_CCR_ADCPRE) >> 16) + 1) * 2;
	return ClockCtrl::ClockFreq() / divider;
}

ADC_BASE_TEMPLATE_ARGS
unsigned ADC_BASE_TEMPLATE_QUALIFIER::ConvertionTimeCycles(uint8_t channel)
{
	unsigned sampleTimeBits;
	if(channel <= 9)
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
	uint16_t sampleTimes[] = {3, 15, 28, 56, 84, 112, 144, 480};
	return ResolutionBits() + sampleTimes[sampleTimeBits];
}

ADC_BASE_TEMPLATE_ARGS
unsigned ADC_BASE_TEMPLATE_QUALIFIER::AdcPeriodUs10(uint8_t channel)
{
	unsigned adcTickNs4 = (4000000000u / ClockFreq());
	unsigned adcTickNs10 = adcTickNs4 * 2 + adcTickNs4 / 2;
	return (adcTickNs10 * ConvertionTimeCycles(channel) + adcTickNs10 / 2);
}

ADC_BASE_TEMPLATE_ARGS
unsigned ADC_BASE_TEMPLATE_QUALIFIER::SampleTimeToReg(unsigned sampleTime)
{
	if(sampleTime > 144)
		return 7;
	if(sampleTime > 112)
		return 6;
	if(sampleTime > 84)
		return 5;
	if(sampleTime > 56)
		return 4;
	if(sampleTime > 28)
		return 3;
	if(sampleTime > 15)
		return 2;
	if(sampleTime > 3)
		return 1;
	return 0;
}


ADC_BASE_TEMPLATE_ARGS
int ADC_BASE_TEMPLATE_QUALIFIER::ResolutionBits()
{
	unsigned resField = ((Regs()->CR1 & ADC_CR1_RES) >> 24);
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
	CommonRegs()->CCR = (CommonRegs()->CCR & ~ADC_CCR_ADCPRE) | divider;
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SetSampleTime(uint8_t channel, unsigned sampleTime)
{
	if(channel > 18)
		return;
		
	if(!VerifyReady(ADC_CR2_SWSTART | ADC_CR2_JSWSTART))
		return;
	
	unsigned bitFieldValue = SampleTimeToReg(sampleTime);
	
	if(channel <= 9)
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
	unsigned srcClock = ClockCtrl::ClockFreq();
	unsigned maxAdcFreq = 15000000;
	unsigned vdd = Power::GetVdd();
	if(vdd > 29)
		maxAdcFreq = 36000000;
	else if(vdd > 24)
		maxAdcFreq = 30000000;
	
	AdcDivider divider = Div8;
	for(unsigned div = 2; div <= 8; div += 2)
	{
		if(srcClock / div <= maxAdcFreq)
		{
			divider = (AdcDivider)((div / 2 - 1) << 16);
			break;
		}
	}
	Init(divider, AdcClock, External);
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::Init(
		AdcDivider divider, 
		ClockSource, 
		Reference
		)
{
	Clock::Enable();
	Regs()->CR1 = 0;
	Regs()->CR2 = 0;
	//CommonRegs()->CCR = 0;
	SetDivider(divider);
	SetSampleTime(TempSensorChannel, 480);
	SetSampleTime(Battery, 480);
	SetSampleTime(ReferenceChannel, 480);
	
	Regs()->CR2 |= ADC_CR2_ADON;
	CommonRegs()->CCR |= ADC_CCR_TSVREFE;
	_adcData.vRef = 0;
	NVIC_EnableIRQ(ADC_IRQn);
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
	Regs()->CR1 = (Regs()->CR1 & ADC_CR1_RES) | (resField << 24);
	return ResolutionBits();
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::Disable()
{
	Regs()->CR1 = 0;
	Regs()->CR2 = 0;
	//CommonRegs()->CCR = 0;
	Clock::Disable();
}

ADC_BASE_TEMPLATE_ARGS
void  ADC_BASE_TEMPLATE_QUALIFIER::SetImmediateTrigger(
	ImmediateTrigger trigger, 
	TriggerMode mode)
{
	if(trigger == ImmTriggerOnce)
	{
		mode = TriggerNone;
	}
	Regs()->CR2 = (Regs()->CR2 & ~(ADC_CR2_JEXTSEL | ADC_CR2_JEXTEN) ) | ((trigger&0x0f) << 16) | (mode << 20);
}

template<class Pins, class CommonRegs>
static inline void EnableChannel(uint8_t channel)
{
	if(channel == AdcCommon::TempSensorChannel || channel == AdcCommon::ReferenceChannel)
	{
		CommonRegs()->CCR = (CommonRegs()->CCR & ~(ADC_CCR_VBATE)) | ADC_CCR_TSVREFE;
	}else
	if(channel == AdcCommon::Battery)
	{
		CommonRegs()->CCR |= ADC_CCR_VBATE;
	}else
	{
		Pins::SetConfiguration(1u << channel, Pins::Analog);
	}
}

static inline unsigned GetJsqr(const uint8_t *channels, uint8_t count)
{
	if(count > 4) return 0;
	unsigned jsqr = ((count-1) << 20) | (channels[0] << 15);
	
	if(count > 1) jsqr |= channels[1] << 10;
	if(count > 2) jsqr |= channels[2] << 5;
	if(count > 3) jsqr |= channels[3];
	return jsqr;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::StartImmediate(const uint8_t *channels, uint16_t *data, uint8_t count, AdcCallback callback)
{
	if(count == 0 || count > 4)
	{
		_adcData.error = ArgumentError;
		return false;
	}
	
	_adcData.immCallback = callback;
	_adcData.immData = data;

	for(unsigned i =0; i< count; i++)
		EnableChannel<Pins, CommonRegs>(channels[i]);
	
	Regs()->JSQR = GetJsqr(channels, count);
	
	Regs()->CR1 = (Regs()->CR1 & ~ADC_CR1_JEOCIE) | ADC_CR1_SCAN | (callback ? ADC_CR1_JEOCIE : 0);
	
	unsigned controlReg = Regs()->CR2;
	if((controlReg & ADC_CR2_JEXTEN) == 0)
		controlReg |= ADC_CR2_JSWSTART;
	Regs()->CR2 = controlReg;
	
	return true;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::ReadImmediate(const uint8_t *channels, uint16_t *data, uint8_t count)
{
	bool result = false, alreadyStarted = false;
	if(Regs()->SR & ADC_SR_JSTRT)
	{
		unsigned jsqr = Regs()->JSQR;
		alreadyStarted = jsqr == GetJsqr(channels, count);
	}
	if(alreadyStarted || StartImmediate(channels, data, count, 0))
	{
		unsigned status, timeout = AdcTimeoutCycles;
		do
		{
			status = Regs()->SR;
		}while(((status & ADC_SR_JEOC) == 0) && --timeout);
		
		if((status & ADC_SR_JEOC) == 0 || !timeout)
		{
			_adcData.error = HardwareError;
		}else
		{
			result = true;
			unsigned index=0;
			if(count > 3) data[index++] = Regs()->JDR4;
			if(count > 2) data[index++] = Regs()->JDR3;
			if(count > 1) data[index++] = Regs()->JDR2;
			data[index] = Regs()->JDR1;
			_adcData.error = NoError;
		}
		Regs()->SR &= ~(ADC_SR_JSTRT | ADC_SR_JEOC);
	}
	return result;
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::DmaHandler(void * data, size_t size, bool success)
{
	DmaChannel::Disable();
	Regs()->CR2 &= ~(ADC_CR2_SWSTART | ADC_CR2_DMA | ADC_CR2_CONT | ADC_CR2_EOCS);
	Regs()->SR &= ~(ADC_SR_EOC | ADC_SR_STRT);
	//Regs()->CR1 &= ~ADC_CR1_SCAN;
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
	unsigned sr = Regs()->SR;
	if(sr & ADC_SR_JEOC)
	{
		Regs()->SR = sr & ~(ADC_SR_JSTRT | ADC_SR_JEOC);
		unsigned count = ((Regs()->JSQR & ADC_JSQR_JL) >> 20) + 1;
		uint16_t *data = _adcData.immData;
		if(data)
		{
			unsigned index=0;
			if(count > 3) data[index++] = Regs()->JDR4;
			if(count > 2) data[index++] = Regs()->JDR3;
			if(count > 1) data[index++] = Regs()->JDR2;
			data[index] = Regs()->JDR1;
			_adcData.error = NoError;
				
			if(_adcData.immCallback)
				_adcData.immCallback(data, count);
		}
	}
	if(sr & ADC_SR_EOC)
	{
		Regs()->SR = sr & ~(ADC_SR_STRT | ADC_SR_EOC | ADC_SR_OVR);
	}
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::StartImmediate(uint8_t channel)
{
	if(channel > ChannelsCount())
		return false;
	
	Regs()->SR &= ~(ADC_SR_JSTRT | ADC_SR_JEOC);
	Regs()->JSQR = (unsigned)channel << 15;
	
	EnableChannel<Pins, CommonRegs>(channel);
	
	Regs()->CR1 = (Regs()->CR1 & ~ADC_CR1_JEOCIE) | ADC_CR1_SCAN;
		
	unsigned controlReg = Regs()->CR2;
	// start conversion now if no external trigger selected
	if((controlReg & ADC_CR2_JEXTEN) == 0)
		controlReg |= ADC_CR2_JSWSTART;
	Regs()->CR2 = controlReg;
	return true;
}

ADC_BASE_TEMPLATE_ARGS
uint16_t ADC_BASE_TEMPLATE_QUALIFIER::ReadImmediate()
{
	unsigned status, timeout = AdcTimeoutCycles;
	uint16_t result = InvalidValue;
	do
	{
		status = Regs()->SR;
	}while(((status & ADC_SR_JEOC) == 0) && --timeout);
	
	if((status & ADC_SR_JEOC) == 0)
	{
		_adcData.error = HardwareError;
	}else
	{
		result = Regs()->JDR1;
		_adcData.error = NoError; 
	}
	Regs()->SR &= ~(ADC_SR_JSTRT | ADC_SR_JEOC);
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
	return (Regs()->SR & ( ADC_SR_JEOC)) == (ADC_SR_JEOC);
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::StopImmediate()
{
	Regs()->SR &= ~(ADC_SR_JSTRT | ADC_SR_JEOC);
	Regs()->CR2 &= ~(ADC_CR2_JSWSTART);
	Regs()->JSQR = 0;
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SetSequenceTrigger(SequenceTrigger trigger, TriggerMode mode)
{
	if(trigger == SeqTriggerOnce)
	{
		mode = TriggerNone;
	}
	Regs()->CR2 = (Regs()->CR2 & ~(ADC_CR2_EXTSEL | ADC_CR2_EXTEN) ) | ((trigger&0x0f) << 24) | (mode << 28);
}

ADC_BASE_TEMPLATE_ARGS
void ADC_BASE_TEMPLATE_QUALIFIER::SetSequenceCallback(AdcCallback callback)
{
	_adcData.seqCallback = callback;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::StartSequence(std::initializer_list<uint8_t> channels, uint16_t *dataBuffer, uint16_t scanCount)
{
	if(scanCount == 0 || channels.size() == 0 || channels.size() > 16)
	{	
		_adcData.error = ArgumentError;
		return false;
	}
	
	if(!_adcData.seqCallback)
	{
		_adcData.seqCallback = VoidAdcCallback;
	}
	
	if(!VerifyReady(ADC_CR2_SWSTART))	
	{
		_adcData.error = NotReady;
		return false;
	}

	Regs()->SR &= ~ADC_SR_OVR;
		
	Regs()->SQR1 = (Regs()->SQR1 & ~ADC_SQR1_L) | (( channels.size() - 1) << 20);
	Regs()->SQR3 = 0;
	Regs()->SQR2 = 0;
	int i = 0;
	for(uint8_t channel : channels)
	{
		Pins::SetConfiguration(1u << channel, Pins::Analog);
		if(i < 6)
		{
			Regs()->SQR3 |= (channel & 0x1f) << 5*i;
		}else if(i < 12)
		{
			Regs()->SQR2 |= (channel & 0x1f) << 5*(i-6);
		}else
		{
			Regs()->SQR1 |= (channel & 0x1f) << 5*(i-12);
		}
		i++;
	}
	
	DmaChannel::SetTransferCallback(DmaHandler);
	DmaChannel::SetRequest(channelNum);
	DmaChannel::Transfer(DmaMode::Periph2Mem | DmaMode::MemIncriment | DmaMode::PriorityHigh | DmaMode::PSize16Bits | DmaMode::MSize16Bits,
			dataBuffer, &Regs()->DR, channels.size() * scanCount);
	
	_adcData.error = NoError;
	
	Regs()->CR1 |= ADC_CR1_SCAN;
	
	unsigned controlReg = Regs()->CR2;
	controlReg |= ADC_CR2_DMA;  //| ADC_CR2_EOCS;
	if(scanCount > 1)
		controlReg |= ADC_CR2_CONT;
	Regs()->CR2 = controlReg;
	
	// start conversion now if no external trigger selected
	if((controlReg & ADC_CR2_EXTEN) == 0)
		Regs()->CR2 |= ADC_CR2_SWSTART;
	
	return true;
}

ADC_BASE_TEMPLATE_ARGS
bool ADC_BASE_TEMPLATE_QUALIFIER::StartSequence(const uint8_t *channels, uint8_t channelsCount, uint16_t *dataBuffer, uint16_t scanCount)
{
	if(scanCount == 0 || channelsCount == 0 || channelsCount > 16)
	{
		_adcData.error = ArgumentError;
		return false;
	}
	
	if(!_adcData.seqCallback)
	{
		_adcData.seqCallback = VoidAdcCallback;
	}	
	
	if(!VerifyReady(ADC_CR2_SWSTART))
	{
		_adcData.error = NotReady;
		return false;
	}

	Regs()->SR &= ~ADC_SR_OVR;
		

	Regs()->SQR1 = (Regs()->SQR1 & ~ADC_SQR1_L) | ((channelsCount - 1) << 20);
	Regs()->SQR3 = 0;
	Regs()->SQR2 = 0;
	for(unsigned i = 0; i < channelsCount; i++)
	{
		Pins::SetConfiguration(1u << channels[i], Pins::Analog);
		if(i < 6)
		{
			Regs()->SQR3 |= (channels[i] & 0x1f) << 5*i;
		}else if(i < 12)
		{
			Regs()->SQR2 |= (channels[i] & 0x1f) << 5*(i-6);
		}else
		{
			Regs()->SQR1 |= (channels[i] & 0x1f) << 5*(i-12);
		}
	}
	
	DmaChannel::SetTransferCallback(DmaHandler);
	DmaChannel::Transfer(DmaChannel::Periph2Mem | DmaChannel::MemIncriment | DmaChannel::PriorityHigh | DmaChannel::PSize16Bits | DmaChannel::MSize16Bits,
			dataBuffer, &Regs()->DR, channelsCount * scanCount, channelNum);
	
	_adcData.error = NoError;
	
	Regs()->CR1 |= ADC_CR1_SCAN;
	
	unsigned controlReg = Regs()->CR2;
	controlReg |= ADC_CR2_DMA;  //| ADC_CR2_EOCS;
	if(scanCount > 1)
		controlReg |= ADC_CR2_CONT;
	Regs()->CR2 = controlReg;
	
	// start conversion now if no external trigger selected
	if((controlReg & ADC_CR2_EXTEN) == 0)
		Regs()->CR2 |= ADC_CR2_SWSTART;
	
	return true;
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
	Regs()->CR2 &= ~(ADC_CR2_SWSTART | ADC_CR2_DMA | ADC_CR2_CONT);
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
int16_t  ADC_BASE_TEMPLATE_QUALIFIER::ReadTemperature()
{
	SetSampleTime(TempSensorChannel, 250);
	uint16_t rawValue = ReadImmediate(TempSensorChannel);
	int16_t TS_CAL1 = *((volatile int16_t*) 0x1FFF7A2C);
	int16_t TS_CAL2 = *((volatile int16_t*)0x1FFF7A2E);
	int16_t value = (rawValue - TS_CAL1) * (110 - 30) / (TS_CAL2 - TS_CAL1) + 30;
	return value;
}

namespace Private
{
	IO_STRUCT_WRAPPER(ADC1, Adc1Regs, ADC_TypeDef);
	IO_STRUCT_WRAPPER(ADC2, Adc2Regs, ADC_TypeDef);
	IO_STRUCT_WRAPPER(ADC3, Adc3Regs, ADC_TypeDef);
	
	IO_STRUCT_WRAPPER(ADC, AdcRegs, ADC_Common_TypeDef);
}

ADC_BASE_TEMPLATE_ARGS
AdcData ADC_BASE_TEMPLATE_QUALIFIER::_adcData;
