
#include "modbus.h"

using namespace Mcucpp;
using namespace Mcucpp::Modbus;



ModbusDevice::ModbusDevice(class ModbusTransport &transport)
    :_transport(transport)
{
    _transport.SetDevice(this);
}

ModbusSlave::ModbusSlave(ModbusTransport &transport)
    :ModbusDevice(transport),
    _address(1),
    _maxRegsToRead(125)
{

}


ModbusMaster::ModbusMaster(ModbusTransport &transport)
    :ModbusDevice(transport)
{

}

void ModbusSlave::MessageReceived(DataBuffer &buffer)
{

    uint8_t addr = buffer.ReadU8();
    if(addr != _address)
    {
        return;
    }

    ModbusFunction function = static_cast<ModbusFunction>(buffer.ReadU8());
    ModbusError error = ModbusError::NoError;

    switch(function)
    {
        case ModbusFunction::ReadHoldingRegisters:
        {
            uint16_t start = buffer.ReadU16Be();
            uint16_t count = buffer.ReadU16Be();
            error = ReadHoldingRegisters(start, count, ModbusFunction::ReadHoldingRegisters);
            break;
        }
        case ModbusFunction::ReadCoils:
        {
            uint16_t start = buffer.ReadU16Be();
            uint16_t count = buffer.ReadU16Be();
            error = ReadCoils(start, count);
            break;
        }
        case ModbusFunction::ReadInputs:
        {
            uint16_t start = buffer.ReadU16Be();
            uint16_t count = buffer.ReadU16Be();
            error = ReadInputs(start, count);
            break;
        }
        case ModbusFunction::ReadInputRegisters:
        {
            uint16_t start = buffer.ReadU16Be();
            uint16_t count = buffer.ReadU16Be();
            error = ReadInputRegisters(start, count);
            break;
        }
        case ModbusFunction::WriteSingleCoil:
        {
            error = WriteSingleCoil(buffer);
            break;
        }
        case ModbusFunction::WriteSingleRegister:
        {
            error = WriteSingleRegister(buffer);
            break;
        }
        case ModbusFunction::WriteMultipleCoils:
        {
            error = WriteMultipleCoils(buffer);
            break;
        }
        case ModbusFunction::WriteMultipleRegisters:
        {
            error = WriteMultipleRegisters(buffer);
            break;
        }
        case ModbusFunction::ReadWriteMultipleRegisters:
        {
            error = ReadWriteMultipleRegisters(buffer);
            break;
        }

        case ModbusFunction::ReadExceptionStatus:
        case ModbusFunction::Diagnostics:
        default:
           error = ModbusError::IllegalFunction;
    }
    if(error != ModbusError::NoError && !SendError(function, error))
    {
        // handle error
    }
}


ModbusError ModbusSlave::WriteMultipleCoils(DataBuffer &buffer)
{
    uint16_t start = buffer.ReadU16Be();
    uint16_t count = buffer.ReadU16Be();
    uint8_t bytes = buffer.ReadU8();
    if((count+7) / 8 > bytes)
    {
        return ModbusError::IllegalValue;
    }

    if(OnWriteCoils)
    {

        OnWriteCoils(start, count, buffer);
    }
    SendResponce(ModbusFunction::WriteMultipleCoils, start, count);
    return ModbusError::NoError;
}

ModbusError ModbusSlave::WriteSingleRegister(DataBuffer &buffer)
{
    uint16_t start = buffer.ReadU16Be();
    uint16_t value = buffer.ReadU16Be();
    if(OnWriteHoldingRegs)
    {
        --buffer;
        --buffer;
        OnWriteHoldingRegs(start, 1, buffer);
    }
    SendResponce(ModbusFunction::WriteSingleRegister, start, value);
    return ModbusError::NoError;
}

ModbusError ModbusSlave::WriteSingleCoil(DataBuffer &buffer)
{
    uint16_t addr = buffer.ReadU16Be();
    uint16_t state = buffer.ReadU16Be();

    if(OnWriteCoils)
    {
        --buffer;
        --buffer;
        ModbusError error = OnWriteCoils(addr, 1, buffer);
        if(error != ModbusError::NoError)
        {
            return error;
        }
    }
    SendResponce(ModbusFunction::WriteSingleCoil, addr, state);
    return ModbusError::NoError;
}

ModbusError ModbusSlave::ReadInputs(uint16_t start, uint16_t count)
{
    if(count > _maxRegsToRead*16)
    {
        return ModbusError::IllegalValue;
    }

    DataBuffer message;
    uint8_t messageBytes = (count+7)/8;
    if(!message.InsertFront(messageBytes + 3))
    {
        return ModbusError::ServerFailure;
    }

    message.Seek(0);
    message.WriteU8(_address);
    message.WriteU8(static_cast<uint8_t>(ModbusFunction::ReadInputs));
    message.WriteU8(messageBytes);

    if(OnReadInputs)
    {
        ModbusError error = OnReadInputs(start, count, message);
        if(error != ModbusError::NoError)
        {
            return error;
        }
    }
    else
    {
        for(int i = 0; i < messageBytes; i++)
        {
            message.WriteU8(0);
        }
    }
    _transport.SendMessage(message);
    return ModbusError::NoError;
}

ModbusError ModbusSlave::ReadCoils(uint16_t start, uint16_t count)
{
    if(count > _maxRegsToRead*16)
    {
        return ModbusError::IllegalValue;
    }

    DataBuffer message;
    uint8_t messageBytes = (count+7)/8;
    if(!message.InsertFront(messageBytes + 3))
    {
        return ModbusError::ServerFailure;
    }

    message.Seek(0);
    message.WriteU8(_address);
    message.WriteU8(static_cast<uint8_t>(ModbusFunction::ReadCoils));
    message.WriteU8(messageBytes);

    if(OnReadCoils)
    {
        ModbusError error = OnReadCoils(start, count, message);
        if(error != ModbusError::NoError)
        {
            return error;
        }
    }
    else
    {
        for(int i = 0; i < messageBytes; i++)
        {
            message.WriteU8(0);
        }
    }
    _transport.SendMessage(message);
    return ModbusError::NoError;
}

ModbusError ModbusSlave::WriteMultipleRegisters(DataBuffer &buffer)
{
    uint16_t writeAddr = buffer.ReadU16Be();
    uint16_t writeCount = buffer.ReadU16Be();
    uint8_t bytes = buffer.ReadU8();

    if(writeCount > _maxRegsToRead
        || bytes < writeCount*2)
    {
        return ModbusError::IllegalValue;
    }
    if(OnWriteHoldingRegs)
    {
        ModbusError error = OnWriteHoldingRegs(writeAddr, writeCount, buffer);
        if(error != ModbusError::NoError)
        {
            return error;
        }
    }
    SendResponce(ModbusFunction::WriteMultipleRegisters, writeAddr, writeCount);
    return ModbusError::NoError;
}

ModbusError ModbusSlave::ReadWriteMultipleRegisters(DataBuffer &buffer)
{
    uint16_t readAddr = buffer.ReadU16Be();
    uint16_t readCount = buffer.ReadU16Be();
    uint16_t writeAddr = buffer.ReadU16Be();
    uint16_t writeCount = buffer.ReadU16Be();
    uint8_t bytes = buffer.ReadU8();

    if(writeCount > _maxRegsToRead
        || bytes != (writeCount+7) / 8)
    {
        return ModbusError::IllegalValue;
    }

    if(OnWriteHoldingRegs)
    {
        ModbusError error = OnWriteHoldingRegs(writeAddr, writeCount, buffer);
        if(error != ModbusError::NoError)
        {
            return error;
        }
    }

    return ReadHoldingRegisters(readAddr, readCount, ModbusFunction::ReadWriteMultipleRegisters);
}

ModbusError ModbusSlave::ReadInputRegisters(uint16_t start, uint16_t count)
{
    if(count > _maxRegsToRead)
    {
        return ModbusError::IllegalValue;
    }
    DataBuffer message;
    if(!message.InsertFront(count*2 + 3))
    {
        return ModbusError::ServerFailure;
    }

    message.Seek(0);
    message.WriteU8(_address);
    message.WriteU8(static_cast<uint8_t>(ModbusFunction::ReadInputRegisters));
    message.WriteU8(count*2);

    if(OnReadInputRegisters)
    {
        ModbusError error = OnReadInputRegisters(start, count, message);
        if(error != ModbusError::NoError)
        {
            return error;
        }
    }
    else
    {
        for(int i = 0; i < count; i++)
        {
            message.WriteU8(0);
            message.WriteU8(0);
        }
    }
    _transport.SendMessage(message);
    return ModbusError::NoError;
}

ModbusError ModbusSlave::ReadHoldingRegisters(uint16_t start, uint16_t count, ModbusFunction function)
{
    if(count > _maxRegsToRead)
    {
        return ModbusError::IllegalValue;
    }
    DataBuffer message;
    if(!message.InsertFront(count*2 + 3))
    {
        return ModbusError::ServerFailure;
    }

    message.Seek(0);
    message.WriteU8(_address);
    message.WriteU8(static_cast<uint8_t>(function));
    message.WriteU8(count*2);

    if(OnReadHoldingRegs)
    {
        ModbusError error = OnReadHoldingRegs(start, count, message);
        if(error != ModbusError::NoError)
        {
            return error;
        }
    }
    else
    {
        for(int i = 0; i < count; i++)
        {
            message.WriteU8(0);
            message.WriteU8(0);
        }
    }
    _transport.SendMessage(message);
    return ModbusError::NoError;
}


bool ModbusSlave::SendError(ModbusFunction function, ModbusError error)
{
    DataBuffer message;
    if(!message.InsertFront(3))
    {
        return false;
    }
    message.Seek(0);
    message.WriteU8(_address);
    message.WriteU8(static_cast<uint8_t>(function) | 0x80);
    message.WriteU8(static_cast<uint8_t>(error));
    return _transport.SendMessage(message);
}

bool ModbusSlave::SendResponce(ModbusFunction function, uint16_t param1, uint16_t param2)
{
    DataBuffer message;
    if(!message.InsertFront(6))
    {
        return false;
    }
    message.Seek(0);
    message.WriteU8(_address);
    message.WriteU8(static_cast<uint8_t>(function));
    message.WriteU16Be(param1);
    message.WriteU16Be(param2);
    return _transport.SendMessage(message);
}



void ModbusMaster::MessageReceived(DataBuffer &buffer)
{
    (void)buffer;
}
