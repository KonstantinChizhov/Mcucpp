
#pragma once

#include <stm32f4xx.h>
#include <clock.h>
#include <iopins.h>
#include <pinlist.h>
#include <dma.h>

namespace Mcucpp
{
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
	
	typedef void (* AdcCallback)(uint16_t *data, size_t count);
	static inline void VoidAdcCallback(uint16_t *data, size_t count);
	
	struct AdcData
	{
		AdcData()
			:callback(VoidAdcCallback), count(0), data(0)
		{
		}
		
		AdcCallback callback;
		size_t count;
		uint16_t *data;
	};
	
	template<class Regs, class CommonRegs, class ClockCtrl, class InputPins, class DmaChannel, uint8_t channelNum>
	class AdcBase
	{
		// Maximum ADC convertion time MaxCockDivider * AdcConvertionCycles / cpu cycle per wait loop
		static const unsigned AdcTimeoutCycles = 2048 * 28 / 4;
		
		static bool VerifyReady(unsigned readyMask = ADC_CR2_SWSTART | ADC_CR2_JSWSTART)
		{
			uint32_t timeout = AdcTimeoutCycles * 4;
			while(Regs()->CR2 & readyMask && --timeout)
				;
			return timeout != 0;
		}
		
		static unsigned SampleTimeToReg(unsigned sampleTime)
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
		
		static DataT ResolotionBits()
		{
			return 12;
		}
		static DataT ToVolts(DataT value);
		static void SelectClockSource(ClockSource) {}
		static void SetVref(Vref){}
		
		static void SetClockDivider(AdcDivider divider)
		{
			CommonRegs()->CCR = (CommonRegs()->CCR & ~ADC_CCR_ADCPRE) | divider;
		}
		
		static void SetSampleTime(uint8_t channel, unsigned sampleTime)
		{
			if(channel > 18)
				return;
				
			if(!VerifyReady())
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
		 
		static void Init(AdcDivider divider = Div2, ClockSource = AdcClock, Vref = VCC)
		{
			Clock::Enable();
			Regs()->CR1 = 0;
			Regs()->CR2 = 0;
			CommonRegs()->CCR = 0;
			SetClockDivider(divider);
			SetSampleTime(TempSensorChannel, 480);
			SetSampleTime(Battery, 480);
			SetSampleTime(ReferenceChannel, 480);
			
			Regs()->CR2 |= ADC_CR2_ADON;
		}

		static void StartDirect(uint8_t channel)
		{
			if(channel > 18)
				return;
			
			uint32_t timeout = AdcTimeoutCycles * 4;
			while(Regs()->SR & ADC_SR_JSTRT && --timeout)
				;
			if(timeout == 0)
				return;
			
			Regs()->JSQR = (unsigned)channel << 15;
			
			if(channel == TempSensorChannel || channel == ReferenceChannel)
			{
				CommonRegs()->CCR = (CommonRegs()->CCR & ~(ADC_CCR_VBATE)) | ADC_CCR_TSVREFE;
			}else
			if(channel == Battery)
			{
				CommonRegs()->CCR = (CommonRegs()->CCR & ~(ADC_CCR_TSVREFE)) | ADC_CCR_VBATE;
			}else
			{
				Pins::SetConfiguration(1u << channel, Pins::Analog);
			}
			Regs()->CR2 |= ADC_CR2_JSWSTART;
		}
		
		static uint16_t Read(uint8_t channel)
		{
			StartDirect(channel);
			return Read();
		}
		
		static uint16_t Read()
		{
			if((Regs()->SR & ADC_SR_JSTRT) == 0)
				return 0x7fff;
			uint32_t timeout = AdcTimeoutCycles;
			while ((Regs()->SR & ADC_SR_JEOC) == 0 && --timeout)
				;
			Regs()->SR &= ~(ADC_SR_JSTRT | ADC_SR_JEOC);
			if(timeout == 0)
				return 0xffff;
			return Regs()->JDR1;
		}
		
		static bool DataReady()
		{
			return (Regs()->SR & ADC_SR_JEOC);
		}
		
		static void Stop()
		{
			Regs()->CR2 &= ~ADC_CR2_ADON;
		}
		
		static void Disable()
		{
			Regs()->CR1 = 0;
			Regs()->CR2 = 0;
			CommonRegs()->CCR = 0;
			Clock::Disable();
		}
		
		enum TriggerMode{RisingEdge = 1, FallingEdge = 2, BothEdges = 3};
		enum Trigger{Once, FreeRun};
		
		static void SetTrigger(Trigger trigger, TriggerMode mode)
		{
		}
		
		static bool Start(uint8_t *channels, uint8_t channelsCount, uint16_t *dataBuffer, uint16_t scanCount, AdcCallback callback = VoidAdcCallback)
		{
			if(scanCount == 0 || channelsCount == 0)
				return false;
			
			if(!callback)
				callback = VoidAdcCallback;
			
			_adcData.callback = callback;
			_adcData.data = dataBuffer;
			_adcData.count = channelsCount * scanCount;
			
			if(channelsCount <= 16)
			{
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
						_adcData.data, &Regs()->DR, _adcData.count, channelNum);
				
				Regs()->CR1 |= ADC_CR1_SCAN;
				Regs()->CR2 |= ADC_CR2_SWSTART | ADC_CR2_DMA | (scanCount > 1 ? ADC_CR2_CONT : 0);
			}
			else return false;
		}
		
	private:
		static void DmaHandler(void *, size_t)
		{
			Regs()->CR1 ~= ~ADC_CR1_SCAN;
			Regs()->CR2 &= ~(ADC_CR2_SWSTART | ADC_CR2_DMA | ADC_CR2_CONT);
			_adcData.callback(_adcData.data, _adcData.count);
		}
		
		static AdcData _adcData;
		
		static bool ReadSequence(DataT *data, uint8_t *channels, uint8_t channelsCount)
		{
			if(!VerifyReady())
				return false;
			if(channelsCount <= 16)
			{
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
				Regs()->CR1 |= ADC_CR1_SCAN;
				Regs()->CR2 |= ADC_CR2_SWSTART | ADC_CR2_EOCS;
				for(unsigned i = 0; i < channelsCount; i++)
				{
					while ((Regs()->SR & ADC_SR_EOC) == 0)
						;
					data[i] = Regs()->DR;
				}
				Regs()->CR1 &= ~ADC_CR1_SCAN;
				return true;
			}
			return false;
		}
	};
	
	namespace Private
	{
		IO_STRUCT_WRAPPER(ADC1, Adc1Regs, ADC_TypeDef);
		IO_STRUCT_WRAPPER(ADC2, Adc2Regs, ADC_TypeDef);
		IO_STRUCT_WRAPPER(ADC3, Adc3Regs, ADC_TypeDef);
		
		IO_STRUCT_WRAPPER(ADC, AdcRegs, ADC_Common_TypeDef);
	}
	
	template<class Regs, class CommonRegs, class ClockCtrl, class InputPins, class DmaChannel, uint8_t channelNum>
	AdcData AdcBase<Regs, CommonRegs, ClockCtrl, InputPins, DmaChannel, channelNum>::_adcData;
	
	typedef AdcBase<Private::Adc1Regs, Private::AdcRegs, Clock::Adc1Clock, Private::Adc1Pins, Dma2Channel0, 0> Adc1;
	typedef AdcBase<Private::Adc2Regs, Private::AdcRegs, Clock::Adc2Clock, Private::Adc2Pins, Dma2Channel3, 1> Adc2;
	typedef AdcBase<Private::Adc3Regs, Private::AdcRegs, Clock::Adc3Clock, Private::Adc3Pins, Dma2Channel1, 2> Adc3;
}
