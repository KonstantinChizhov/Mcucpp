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

#include <atomic.h>
#include <iopins.h>
#include <pinlist.h>
#include <clock.h>
#include <dispatcher.h>
#include <enum.h>

namespace Mcucpp
{

	enum class I2cError :uint8_t
	{
		NoError, Overflow, Timeout, BusError, ArbitrationError, ArgumentError, Nack, Busy, 
	};

	enum class I2cState :uint8_t
	{
		Idle = 0, 
		Start = 1, 
		DevAddr = 2, 
		RegAddrNextByte = 4,
		RegAddr = 5, 
		Restart = 8, 
		DevAddrRead = 16,
		Data = 32, 
		Stop = 64,
	};

	enum class I2cMode :uint8_t
	{
		Idle = 0, 
		Read = 1,
		Write = 2,
	};

	enum class I2cOpts :uint8_t
	{
		None = 0, 
		
		DevAddr7Bit = 0,
		DevAddr10Bit = 1,
		
		RegAddr8Bit = 0,
		RegAddr16Bit = 2,
		RegAddrNone = 3,
	};


	typedef void (* I2cCallback)(uint16_t addr, void *data, size_t count, I2cError status);

	struct I2cCallbackData
	{
		
	};
	
	struct I2cData
	{
		I2cData()
			:timeoutSpins(10000),
			callback(nullptr),
			buffer(nullptr),
			regAddr(0),
			size(0),
			bytesTransfered(0),
			addr(0),
			error(I2cError::NoError),
			state(I2cState::Idle),
			mode(I2cMode::Idle),
			opts(I2cOpts::None)
		{
		}
		
		void Reset()
		{
			timeoutSpins = 10000;
			callback = nullptr;
			buffer = nullptr;
			size = 0;
			bytesTransfered = 0;
			addr = 0;
			mode = I2cMode::Idle;
			//error = I2cError::NoError;
			state = I2cState::Idle;
			opts = I2cOpts::None;
		}
		
		void operator ()()
		{
			if(callback)
			{
				uint16_t addrTmp = addr;
				uint8_t *bufferTmp = buffer;
				I2cError errorTmp = error;
				size_t sizeTmp = size;
				I2cCallback callbackTmp = callback;
				Reset();
				callbackTmp(addrTmp, bufferTmp, sizeTmp, errorTmp);
			}else
			{
				Reset();
			}
		}

		uint32_t timeoutSpins;
		I2cCallback callback;
		uint8_t *buffer;
		uint16_t regAddr;
		uint16_t size;
		uint16_t bytesTransfered;
		uint16_t addr;
		I2cError error;
		volatile I2cState state;
		I2cMode mode;
		I2cOpts opts;
	};


	template< 
		class I2Cx, 
		IRQn_Type EvIQRNumber, 
		IRQn_Type ErIQRNumber, 
		class ClockCtrl, 
		class SclPins, 
		class SdaPins, 
		class DmaTxChannel, 
		class DmaRxChannel, 
		uint8_t DmaTxChannelNum,
		uint8_t DmaRxChannelNum>
	class I2cBase
	{
	public:
		static I2cData _data;
	public:
		static const uint8_t AltFuncNum = 0;
		
		static inline I2cError GetError(){return _data.error; }
		
		static void Init(uint32_t i2cClockSpeed = 100000U, bool dutyCycle2 = false);
		
		static I2cError WriteU8(uint16_t devAddr, uint8_t regAddr, uint8_t data, I2cOpts opts = I2cOpts::None);
		
		static int ReadU8(uint16_t devAddr, uint16_t regAddr, I2cOpts opts = I2cOpts::None);

		static I2cError Read(uint16_t devAddr, uint16_t regAddr, uint8_t *data, int size, I2cOpts opts = I2cOpts::None);
		
		static bool Write(uint16_t devAddr, uint16_t regAddr, const uint8_t *data, int size, I2cOpts opts = I2cOpts::None);
		
		static bool WriteRegAddr(uint16_t regAddr, I2cOpts opts);
		
		static bool WaitEvent(uint32_t i2c_event);
		
		static bool WriteDevAddr(uint16_t devAddr, bool read, I2cOpts opts);

		static bool Start();
		
		static bool Busy();
		
		static bool WaitWhileBusy();
		
		static bool WriteAsync(uint16_t addr, uint16_t regAddr, const void *data, size_t size, I2cOpts opts = I2cOpts::None, I2cCallback callback = nullptr);
		
		static bool ReadAsync(uint16_t addr, uint16_t regAddr, void *data, size_t size, I2cOpts opts = I2cOpts::None, I2cCallback callback = nullptr);
		
		static void EventIrqHandler();
		
		static void ErrorIrqHandler();
		
		static I2cError GetErorFromLastEvent(uint32_t lastevent);
		
		template<uint8_t SclPinNumber, uint8_t SdaPinNumber>
		static void SelectPins()
		{
			typedef typename SclPins:: template Pin<SclPinNumber> SclPin;
			SclPin::Port::Enable();
			SclPin::SetConfiguration(SclPin::Port::AltFunc);
			SclPin::AltFuncNumber(AltFuncNum);
			SclPin::SetDriverType(SclPin::Port::OpenDrain);
			
			typedef typename SdaPins:: template Pin<SdaPinNumber> SdaPin;
			SdaPin::Port::Enable();
			SdaPin::SetConfiguration(SdaPin::Port::AltFunc);
			SdaPin::AltFuncNumber(AltFuncNum);
			SdaPin::SetDriverType(SdaPin::Port::OpenDrain);
		}

		static void SelectPins(uint8_t sclPinNumber, uint8_t sdaPinNumber)
		{
			typedef typename SclPins::ValueType Type;
			SclPins::Enable();
			Type maskScl (1 << sclPinNumber);
			SclPins::SetConfiguration(maskScl, SclPins::AltFunc);
			SclPins::AltFuncNumber(maskScl, AltFuncNum);
			SclPins::SetDriverType(maskScl, SclPins::OpenDrain);
			
			SdaPins::Enable();
			Type maskSda (1 << sdaPinNumber);
			SdaPins::SetConfiguration(maskSda, SdaPins::AltFunc);
			SdaPins::AltFuncNumber(maskSda, AltFuncNum);
			SdaPins::SetDriverType(maskSda, SdaPins::OpenDrain);
		}
		
		template<class SclPin, class SdaPin>
		static void SelectPins()
		{
			const int sclPinIndex = SclPins::template PinIndex<SclPin>::Value;
			const int sdaPinIndex = SdaPins::template PinIndex<SdaPin>::Value;
			STATIC_ASSERT(sclPinIndex >= 0);
			STATIC_ASSERT(sdaPinIndex >= 0);
			SelectPins<sclPinIndex, sclPinIndex>();
		}
	};


	#define I2C_TEMPLATE_ARGS template< \
		class I2Cx, \
		IRQn_Type EvIQRNumber, \
		IRQn_Type ErIQRNumber, \
		class ClockCtrl, \
		class SclPins, \
		class SdaPins, \
		class DmaTxChannel, \
		class DmaRxChannel, \
		uint8_t DmaTxChannelNum,\
		uint8_t DmaRxChannelNum>

	#define I2C_TEMPLATE_QUALIFIER I2cBase<\
			I2Cx, \
			EvIQRNumber, \
			ErIQRNumber, \
			ClockCtrl, \
			SclPins, \
			SdaPins, \
			DmaTxChannel, \
			DmaRxChannel, \
			DmaTxChannelNum,\
			DmaRxChannelNum>
	
	I2C_TEMPLATE_ARGS 
	I2cData I2C_TEMPLATE_QUALIFIER::_data;

	typedef IO::PinList<IO::Pb6> I2C1SclPins;
	typedef IO::PinList<IO::Pb7> I2C1SdaPins;

	IO_STRUCT_WRAPPER(I2C1, I2C1Regs, I2C_TypeDef);

	typedef I2cBase<I2C1Regs, I2C1_EV_IRQn, I2C1_ER_IRQn, Clock::I2c1Clock, I2C1SclPins, I2C1SdaPins, void, void, 0, 0> I2c1;

	I2C_TEMPLATE_ARGS
	void I2C_TEMPLATE_QUALIFIER::Init(uint32_t i2cClockSpeed, bool dutyCycle2)
	{
		//ClockCtrl::Reset();
		ClockCtrl::Enable();
		I2Cx()->CR1 = I2C_CR1_PE;
		
		uint32_t apb1Clock = Clock::Apb1Clock::ClockFreq();
		uint32_t APB1ClockMhz = apb1Clock / 1000000;
		I2Cx()->CR2 = APB1ClockMhz & 0x3f;
		I2Cx()->CR1 &= ~I2C_CR1_PE;
		
		uint32_t result = 0;
		uint32_t tmpreg = 0;
		
		if (i2cClockSpeed <= 100000)
		{
			result = (uint16_t)(apb1Clock / (i2cClockSpeed << 1));

			if (result < 0x04)
			{
				result = 0x04;
			}
			tmpreg |= result;
			I2Cx()->TRISE = APB1ClockMhz + 1;
		}
		else
		{
			if (dutyCycle2)
			{
				result = (uint16_t)(apb1Clock / (i2cClockSpeed * 3));
			}
			else
			{
				result = (uint16_t)(apb1Clock / (i2cClockSpeed * 25));
				result |= 0x4000;
			}

			if ((result & I2C_CCR_CCR) == 0)
			{
				result |= (uint16_t)0x0001;
			}
			tmpreg |= (uint16_t)(result | I2C_CCR_FS);
			I2Cx()->TRISE = (uint16_t)(((APB1ClockMhz * (uint16_t)300) / (uint16_t)1000) + (uint16_t)1);  
		}
		I2Cx()->CCR = tmpreg;
		I2Cx()->CR1 |= I2C_CR1_PE;
		I2Cx()->CR1 &= 0xFBF5;
		I2Cx()->CR1 |= I2C_CR1_ACK;
		I2Cx()->OAR1 = 2;
		I2Cx()->OAR2 = 0;
		
		NVIC_EnableIRQ(EvIQRNumber);
		NVIC_EnableIRQ(ErIQRNumber);
		//NVIC_SetPriority(EvIQRNumber, 1);
		//NVIC_SetPriority(ErIQRNumber, 1);
	}
	
	I2C_TEMPLATE_ARGS
	I2cError I2C_TEMPLATE_QUALIFIER::WriteU8(uint16_t devAddr, uint8_t regAddr, uint8_t data, I2cOpts opts)
	{
		I2Cx()->SR1 = 0;
		I2Cx()->SR2 = 0;

		if(!WaitWhileBusy()) return _data.error;

		if(!Start())return _data.error;
		
		if(!WriteDevAddr(devAddr, false, opts))return _data.error;
		if(!HasAnyFlag(opts, I2cOpts::RegAddrNone) )
		{
			if(!WriteRegAddr(regAddr, opts))return _data.error;
		}
		I2Cx()->DR = data;
		if(!WaitEvent(0x00070084))return _data.error;  // TRA, BUSY, MSL, TXE and BTF flags 

		I2Cx()->CR1 &= ~I2C_CR1_ACK;
		I2Cx()->CR1 |= I2C_CR1_STOP;
		return I2cError::NoError;
	}
	
	I2C_TEMPLATE_ARGS
	int I2C_TEMPLATE_QUALIFIER::ReadU8(uint16_t devAddr, uint16_t regAddr, I2cOpts opts)
	{
		_data.error = I2cError::NoError;
		if(devAddr > 1023)
		{
			_data.error = I2cError::ArgumentError;
			return -(int)_data.error;
		}
		
		if(!WaitWhileBusy()) return -(int)_data.error;
		if(!HasAnyFlag(opts, I2cOpts::RegAddrNone) )
		{
			if(!Start()) return -(int)_data.error;
			if(!WriteDevAddr(devAddr, false, opts)) return -(int)_data.error;
			if(!WriteRegAddr(regAddr, opts)) return -(int)_data.error;
		}
		if(!Start())return -(int)_data.error;

		if(!WriteDevAddr(devAddr, true, opts)) return -(int)_data.error;
		
		I2Cx()->CR1 = (I2Cx()->CR1 & (~I2C_CR1_ACK)) | I2C_CR1_STOP;
		uint8_t tmp = (uint8_t)I2Cx()->DR;

		return tmp;
	}
	
	I2C_TEMPLATE_ARGS
	bool I2C_TEMPLATE_QUALIFIER::WriteDevAddr(uint16_t devAddr, bool read, I2cOpts opts)
	{
		(void)opts;
		I2Cx()->DR = (devAddr << 1) | (uint16_t)read;
		return WaitEvent(read ? 0x00030040 : 0x00070082);
	}
	
	I2C_TEMPLATE_ARGS
	bool I2C_TEMPLATE_QUALIFIER::WriteRegAddr(uint16_t regAddr, I2cOpts opts)
	{
		if(HasAnyFlag(opts, I2cOpts::RegAddr16Bit))
		{
			I2Cx()->DR = (uint8_t)regAddr;
			if(!WaitEvent(0x00030040)) return false;  // TRA, BUSY, MSL, TXE and BTF flags 
		
			I2Cx()->DR = (uint8_t)(regAddr >> 8);
			return WaitEvent(0x00070084);
		}else
		{
			I2Cx()->DR = (uint8_t)regAddr;
			return WaitEvent(0x00070084);
		}
		return true;
	}
	
	I2C_TEMPLATE_ARGS
	bool I2C_TEMPLATE_QUALIFIER::WaitWhileBusy()
	{
		for(uint32_t i = 0; i < 1000000 && Busy(); i++)
		{
			GetCurrentDispatcher().Poll();
		}
		if(Busy())
		{
			_data.error = I2cError::Busy;
			return false;
		}
		I2Cx()->SR1 = 0;
		I2Cx()->SR2 = 0;
		return true;
	}
	
	I2C_TEMPLATE_ARGS
	I2cError I2C_TEMPLATE_QUALIFIER::Read(uint16_t devAddr, uint16_t regAddr, uint8_t *data, int size, I2cOpts opts)
	{
		_data.error = I2cError::NoError;
		if(devAddr > 1023 || !data || size <= 2)
		{
			_data.error = I2cError::ArgumentError;
			return (I2cError) _data.error;
		}
		
		if(!WaitWhileBusy()) return _data.error;
		
		if(!Start()) return _data.error;

		if(!WriteDevAddr(devAddr, false, opts)) return _data.error;

		if(!WriteRegAddr(regAddr, opts)) return _data.error;
		
		if(!Start()) return _data.error;

		I2Cx()->CR1 |= I2C_CR1_ACK;

		if(!WriteDevAddr(devAddr, true, opts)) return _data.error;
		
		for(int i = 0; i < size; i++)
		{
			if(!WaitEvent(0x00030040)) return _data.error;
			uint8_t tmp = (uint8_t)I2Cx()->DR;
			//if(!WaitEvent(0x00030040)) return _data.error;
			data[i] = tmp;
		}
		
		I2Cx()->CR1 &= ~I2C_CR1_ACK;
		I2Cx()->CR1 |= I2C_CR1_STOP;
		return _data.error;
	}
	
	I2C_TEMPLATE_ARGS
	bool I2C_TEMPLATE_QUALIFIER::Write(uint16_t devAddr, uint16_t regAddr, const uint8_t *data, int size, I2cOpts opts)
	{
		if(devAddr > 1023 || !data || size <= 2)
		{
			_data.error = I2cError::ArgumentError;
			return 0xff;
		}
		
		if(!WaitWhileBusy()) return false;

		I2Cx()->CR1 |= I2C_CR1_ACK;
		
		if(!Start())return false;
		
		I2Cx()->DR = devAddr & 0xfffe;
		if(!WaitEvent(0x00070082)) return false; // BUSY, MSL, ADDR, TXE TRA

		I2Cx()->DR = (uint8_t)regAddr;
		if(!WaitEvent(0x00070084)) return false;  // TRA, BUSY, MSL, TXE and BTF flags 

		WriteRegAddr(regAddr, opts);
		
		int i = 0;
		
		for(; i < size; i++)
		{
			if(!WaitEvent(0x00030040))return false; // I2C_EVENT_MASTER_BYTE_RECEIVED
			uint8_t tmp = (uint8_t)I2Cx()->DR;
			data[i] = tmp;
		}
		
		I2Cx()->CR1 &= ~I2C_CR1_ACK;
		I2Cx()->CR1 |= I2C_CR1_STOP;
		return true;
	}
	
	I2C_TEMPLATE_ARGS
	bool I2C_TEMPLATE_QUALIFIER::WaitEvent(uint32_t i2c_event)
	{
		bool result;
		int timer=100000;
		uint32_t lastevent;
		do{
			lastevent = I2Cx()->SR1;
			lastevent |= (I2Cx()->SR2 << 16);
			lastevent &= 0x00FFFFFF;
			result = (lastevent & i2c_event) == i2c_event;
		}
		while(!result && timer-- > 0);
		if(!result)
		{
			I2Cx()->CR1 &= ~I2C_CR1_ACK;
			I2Cx()->CR1 |= I2C_CR1_STOP;
			_data.error = GetErorFromLastEvent(lastevent);
			if(_data.error == I2cError::NoError)
			{
				_data.error = I2cError::Timeout;
			}
		}
		return result;
	}

	I2C_TEMPLATE_ARGS
	bool I2C_TEMPLATE_QUALIFIER::Start()
	{
		I2Cx()->SR1 = 0;
		I2Cx()->SR2 = 0;
		I2Cx()->CR1 |= I2C_CR1_START;
		return WaitEvent(0x00030001);
	}
	
	I2C_TEMPLATE_ARGS
	bool I2C_TEMPLATE_QUALIFIER::Busy()
	{
		return (I2Cx()->SR2 & I2C_SR2_BUSY) || (_data.state != I2cState::Idle);
	}

	I2C_TEMPLATE_ARGS
	bool I2C_TEMPLATE_QUALIFIER::WriteAsync(uint16_t addr, uint16_t regAddr, const void *data, size_t size, I2cOpts opts, I2cCallback callback)
	{
		_data.error = I2cError::NoError;
		if(!WaitWhileBusy())
		{
			return false;
		}
		
		if(!Mcucpp::Atomic::CompareExchange(&_data.buffer, (uint8_t*)nullptr, static_cast<uint8_t*>(data)))
		{
			_data.error = I2cError::Busy;
			return false;
		}
		I2Cx()->CR2 |= I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN;
		
		_data.size = size;
		_data.bytesTransfered = 0;
		_data.addr = addr;
		_data.regAddr = regAddr;
		_data.callback = callback;
		_data.state = I2cState::Start;
		_data.opts = opts;
		
		I2Cx()->CR1 |= I2C_CR1_START;
		
		return true;
	}
	
	I2C_TEMPLATE_ARGS
	bool I2C_TEMPLATE_QUALIFIER::ReadAsync(uint16_t addr, uint16_t regAddr, void *data, size_t size, I2cOpts opts, I2cCallback callback)
	{
		_data.error = I2cError::NoError;
		if(!WaitWhileBusy())
		{
			return false;
		}
		
		if(!Mcucpp::Atomic::CompareExchange(&_data.state, I2cState::Idle, I2cState::Start))
		{
			_data.error = I2cError::Busy;
			return false;
		}
		
		_data.buffer = static_cast<uint8_t*>(data);
		_data.size = size;
		_data.bytesTransfered = 0;
		_data.addr = addr;
		_data.regAddr = regAddr;
		_data.callback = callback;
		_data.mode = I2cMode::Read;
		_data.opts = opts;
		
		I2Cx()->CR1 |= I2C_CR1_ACK;
		I2Cx()->CR2 = (I2Cx()->CR2 & ~(I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN)) 
				| I2C_CR2_ITEVTEN | I2C_CR2_ITERREN;
		
		I2Cx()->CR1 |= I2C_CR1_START;
		
		return true;
	}
	
	I2C_TEMPLATE_ARGS
	void I2C_TEMPLATE_QUALIFIER::EventIrqHandler()
	{
		uint32_t lastevent = (I2Cx()->SR1 | (I2Cx()->SR2 << 16)) & 0x00FFFFFF;
	
		if(!_data.buffer)
		{
			I2Cx()->CR1 &= ~I2C_CR1_ACK;
			I2Cx()->CR1 |= I2C_CR1_STOP;
			I2Cx()->CR2 &= ~(I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);
			_data.Reset();
		}
		switch(_data.state)
		{
		case I2cState::Start:
			if(lastevent & I2C_SR1_SB)
			{
				I2Cx()->DR = (_data.addr << 1) & 0xfffe;
				_data.state = I2cState::DevAddr;
			}
		break;
		case I2cState::DevAddr:
			if(lastevent & (I2C_SR1_SB | I2C_SR1_STOPF))
			{
				_data.error = I2cError::BusError;
				break;
			}
			if(lastevent & (I2C_SR1_ADDR | I2C_SR1_ADD10))
			{
				I2Cx()->CR2 |= I2C_CR2_ITBUFEN;
				I2Cx()->DR = (uint8_t)_data.regAddr;
				if(HasAnyFlag(_data.opts, I2cOpts::RegAddr16Bit))
				{
					_data.state = I2cState::RegAddrNextByte;
				}
				else
				{
					_data.state = I2cState::RegAddr;
					if(_data.mode == I2cMode::Read)
					{
						I2Cx()->CR1 |= I2C_CR1_START;
					}
				}
			}
		break;
		case I2cState::RegAddrNextByte:
			if(lastevent & (I2C_SR1_SB | I2C_SR1_STOPF))
			{
				_data.error = I2cError::BusError;
				break;
			}
			if(lastevent & I2C_SR1_TXE)
			{
				I2Cx()->CR2 &= ~I2C_CR2_ITBUFEN;
				I2Cx()->SR1 &= ~(I2C_SR1_TXE);
				I2Cx()->DR = (uint8_t)(_data.regAddr >> 8);
				_data.state = I2cState::RegAddr;
				if(_data.mode == I2cMode::Read)
				{
					I2Cx()->CR1 |= I2C_CR1_START;
				}
			}

		break;
		case I2cState::RegAddr:
			if(lastevent & (I2C_SR1_SB | I2C_SR1_STOPF))
			{
				_data.error = I2cError::BusError;
				break;
			}
			if(lastevent & I2C_SR1_TXE)
			{
				I2Cx()->CR2 &= ~I2C_CR2_ITBUFEN;
				I2Cx()->SR1 &= ~(I2C_SR1_TXE );
			}
			if(lastevent & I2C_SR1_BTF)
			{
				I2Cx()->SR1 &= ~(I2C_SR1_BTF);
				_data.state = I2cState::Restart;
			}
		break;
		case I2cState::Restart: 
			if(lastevent & I2C_SR1_SB)
			{
				I2Cx()->DR = (_data.addr << 1) | 1;
				I2Cx()->CR1 &= ~I2C_CR1_ACK;
				__DSB();
				I2Cx()->CR1 |= I2C_CR1_ACK;
				_data.state = I2cState::DevAddrRead;
			}
		break;
		case I2cState::DevAddrRead: 
			if(lastevent & (I2C_SR1_SB | I2C_SR1_STOPF))
			{
				_data.error = I2cError::BusError;
				break;
			}
			if(lastevent & (I2C_SR1_ADDR | I2C_SR1_ADD10))
			{
				I2Cx()->CR2 |= I2C_CR2_ITBUFEN;
				_data.state = I2cState::Data;
			}
		break;
		case I2cState::Data:
			if(lastevent & (I2C_SR1_SB | I2C_SR1_STOPF))
			{
				_data.error = I2cError::BusError;
				break;
			}
			if(lastevent & I2C_SR1_RXNE)
			{
				uint8_t tmp = (uint8_t)I2Cx()->DR;
				if(_data.bytesTransfered < _data.size - 1)
				{
					_data.buffer[_data.bytesTransfered] = tmp;
					if(_data.bytesTransfered == _data.size - 2)
					{
						I2Cx()->CR1 &= ~I2C_CR1_ACK; // NACK last byte
					}
					_data.bytesTransfered++;
				}
				else
				{
					_data.state = I2cState::Idle;
					I2Cx()->CR1 |= I2C_CR1_STOP;
					I2Cx()->CR2 &= ~(I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);
					_data.buffer[_data.bytesTransfered++] = (uint8_t)I2Cx()->DR;
					_data.error = I2cError::NoError;
					if(_data.callback)
					{
						GetCurrentDispatcher().SetTask(_data);
					}
					else
					{
						_data.Reset();
					}
				}
			}
		break;
		case I2cState::Stop:
		case I2cState::Idle:
			I2Cx()->SR1 = 0;
			I2Cx()->CR2 &= ~(I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);
		break;
		}
		
		if(_data.error != I2cError::NoError)
		{
			I2Cx()->CR1 &= ~I2C_CR1_ACK;
			I2Cx()->CR1 |= I2C_CR1_STOP;
			I2Cx()->CR2 &= ~(I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);
			
			if(_data.callback)
			{
				GetCurrentDispatcher().SetTask(_data);
			}
			else
			{
				_data.Reset();
			}
		}
		NVIC_ClearPendingIRQ(EvIQRNumber);
		__DSB();
	}
	
	
	I2C_TEMPLATE_ARGS
	void I2C_TEMPLATE_QUALIFIER::ErrorIrqHandler()
	{
		uint32_t lastevent = (I2Cx()->SR1 | (I2Cx()->SR2 << 16)) & 0x00FFFFFF;
		
		I2Cx()->CR1 &= ~I2C_CR1_ACK;
		I2Cx()->CR1 |= I2C_CR1_STOP;
		I2Cx()->CR2 &= ~(I2C_CR2_ITBUFEN | I2C_CR2_ITEVTEN | I2C_CR2_ITERREN);
		
		_data.error = GetErorFromLastEvent(lastevent);
		
		if(_data.callback)
		{
			GetCurrentDispatcher().SetTask(_data);
		}
		else
		{
			_data.Reset();
		}
		NVIC_ClearPendingIRQ(ErIQRNumber);
		__DSB();
	}
	
	I2C_TEMPLATE_ARGS
	I2cError I2C_TEMPLATE_QUALIFIER::GetErorFromLastEvent(uint32_t lastevent)
	{
		if(lastevent & I2C_SR1_TIMEOUT)
		{
			return I2cError::Timeout;
		}
		if(lastevent & I2C_SR1_OVR)
		{
			return I2cError::Overflow;
		}
		if(lastevent & I2C_SR1_AF)
		{
			return I2cError::Nack;
		}
		if(lastevent & I2C_SR1_ARLO)
		{
			return I2cError::ArbitrationError;
		}
		if(lastevent & I2C_SR1_BERR)
		{
			return I2cError::BusError;
		}
		return I2cError::NoError;
	}

}