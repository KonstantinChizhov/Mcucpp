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
//******

// Modbus implementation

#pragma once

#include <crc.h>
#include <stdint.h>
#include <stddef.h>
#include <data_buffer.h>
#include <functional>
#include <utility>
#include <iopins.h>


namespace Mcucpp
{
namespace Modbus
{
	enum class ModbusFunction :uint8_t
	{
		ReadCoils = 1,
		ReadInputs = 2,
		ReadHoldingRegisters = 3,
		ReadInputRegisters = 4,
		WriteSingleCoil = 5,
		WriteSingleRegister = 6,
		ReadExceptionStatus = 7,
		Diagnostics = 8,
		WriteMultipleCoils = 15,
		WriteMultipleRegisters = 16,
		ReadWriteMultipleRegisters = 23,
		Responce = 0x80
	};

	enum class ModbusError :uint8_t
	{
	    NoError = 0,
		IllegalFunction = 0x01,
		IllegalAddress  = 0x02,
		IllegalValue    = 0x03,
		ServerFailure   = 0x04,
		Acknowledge     = 0x05,
		ServerBusy      = 0x06,
		NotAcknowledge  = 0x07,
		MemoryParityError = 0x08,
		PathNotAvailable  = 0x0a,
		DeviceFailed  = 0x0b,
	};

	class ModbusTransport
	{
    protected:
        class ModbusDevice *_device;
	public:
	    ModbusTransport(const ModbusTransport&)=delete;
	    ModbusTransport(ModbusTransport&&)=delete;
	    ModbusTransport& operator=(const ModbusTransport&)=delete;
        ModbusTransport():_device(nullptr) {}

		virtual void DiscardReadBuffer()=0;
		virtual bool SendMessage(DataBuffer & buffer)=0;
		virtual bool StartListen()=0;
		virtual void Stop()=0;
		virtual Mcucpp::DataBuffer&& GetTxBuffer()=0;
		void SetDevice(class ModbusDevice *device){ _device = device; }
		size_t DataChunkSize(){ return 20; }
	};

	class ModbusDevice
	{
    protected:
	    ModbusTransport &_transport;
	public:
	    ModbusDevice(ModbusTransport &transport);
		virtual void MessageReceived(Mcucpp::DataBuffer & buffer)=0;
	};

    class ModbusMaster :public ModbusDevice
	{
	public:
	    ModbusMaster(ModbusTransport &transport);

		virtual void MessageReceived(DataBuffer &buffer);
	};

	class ModbusSlave :public ModbusDevice
	{
	    uint8_t _address;
	    uint8_t _maxRegsToRead;
	public:
        ModbusSlave(ModbusTransport &transport);
		virtual void MessageReceived(DataBuffer &buffer);
        bool SendError(ModbusFunction function, ModbusError error);
        bool SendResponce(ModbusFunction function, uint16_t param1, uint16_t param2);

		std::function<ModbusError(uint16_t start, uint16_t count, DataBuffer &buffer)> OnReadHoldingRegs;
		std::function<ModbusError(uint16_t start, uint16_t count, DataBuffer &buffer)> OnWriteHoldingRegs;
		std::function<ModbusError(uint16_t start, uint16_t count, DataBuffer &buffer)> OnReadInputs;
		std::function<ModbusError(uint16_t start, uint16_t count, DataBuffer &buffer)> OnReadCoils;
		std::function<ModbusError(uint16_t start, uint16_t count, DataBuffer &buffer)> OnWriteCoils;
		std::function<ModbusError(uint16_t start, uint16_t count, DataBuffer &buffer)> OnReadInputRegisters;

		void SetAddress(uint8_t addr) { _address = addr; }

        ModbusError WriteMultipleCoils(DataBuffer &buffer);
        ModbusError WriteSingleRegister(DataBuffer &buffer);
        ModbusError WriteSingleCoil(DataBuffer &buffer);
        ModbusError ReadInputs(uint16_t start, uint16_t count);
        ModbusError ReadCoils(uint16_t start, uint16_t count);
        ModbusError ReadInputRegisters(uint16_t start, uint16_t count);
        ModbusError ReadHoldingRegisters(uint16_t start, uint16_t count, ModbusFunction function);
        ModbusError ReadWriteMultipleRegisters(DataBuffer &buffer);
        ModbusError WriteMultipleRegisters(DataBuffer &buffer);
	};

	template<class IODevice>
	class ModbusTransportRtu : public ModbusTransport
	{
		DataBuffer _rxBuffer;
		DataBuffer _txBuffer;
		DataChunk *_rxChunk = nullptr;
		DataChunk *_txChunk = nullptr;
        bool _buffersAreStatic = false;
        void TxHandler(void *data, size_t size, bool success);
        void RxHandler(void *data, size_t size, bool success);
        void RxHandlerStatic(void *data, size_t size, bool success);
	public:

		ModbusTransportRtu();
		~ModbusTransportRtu();
		virtual void DiscardReadBuffer();
		virtual bool SendMessage(DataBuffer & buffer);
		virtual bool StartListen();
		virtual void Stop();
		void SetStaticBuffers(DataChunk *rxBuffer, DataChunk *txBuffer);
		virtual Mcucpp::DataBuffer&& GetTxBuffer();
	};
}
}

#include <impl/modbus_impl.h>
