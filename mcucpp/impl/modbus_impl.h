

namespace Mcucpp
{
namespace Modbus
{

	static inline uint16_t CalcModbusCrc(DataBuffer & buffer)
	{

		uint16_t crc = Crc16ModbusTable::Init;
		for(uint8_t c : buffer)
		{
			crc = CrcUpdate<Crc16ModbusTable>(c, crc);
		}
		return crc;
	}

    template<class IODevice>
	ModbusTransportRtu<IODevice>::ModbusTransportRtu()
		:_rxChunk(nullptr)
	{

	}

    template<class IODevice>
	ModbusTransportRtu<IODevice>::~ModbusTransportRtu()
	{
        if(_rxChunk)
        {
            DataChunk::Release(_rxChunk);
        }
	}

	template<class IODevice>
	void ModbusTransportRtu<IODevice>::DiscardReadBuffer()
	{
	     _rxBuffer.Clear();
	}

	template<class IODevice>
	bool ModbusTransportRtu<IODevice>::SendMessage(DataBuffer & buffer)
	{
	    _txBuffer = std::move(buffer);
		uint16_t crc = CalcModbusCrc(_txBuffer);
        _txBuffer.InsertBack(2);
 		_txBuffer.Seek(_txBuffer.Size()-2);
		_txBuffer.WriteU16Le(crc);

		auto chunk = _txBuffer.BufferList();
		if(!chunk)
        {
            return false;
        }

        auto callback = [this](void *data, size_t size, bool success)
        {
            TxHandler(data, size, success);
        };
        bool res = IODevice::WriteAsync(chunk->Data(), chunk->Size(), callback);
		return res;
	}

	template<class IODevice>
	bool ModbusTransportRtu<IODevice>::StartListen()
	{
        _rxBuffer.Clear();
        if(_rxChunk)
        {
            DataChunk::Release(_rxChunk);
        }
        _rxChunk = DataChunk::GetNew(DataChunkSize());

        if(!_rxChunk)
        {
            return false;
        }

        auto callback = [this](void *data, size_t size, bool success)
        {
            RxHandler(data, size, success);
        };
        bool res = IODevice::ReadAsync(_rxChunk->Data(), _rxChunk->Capacity(), callback);
        return res;
	}

	template<class IODevice>
	void ModbusTransportRtu<IODevice>::Stop()
	{
        _rxBuffer.Clear();
	}

	template<class IODevice>
	void ModbusTransportRtu<IODevice>::TxHandler(void *data, size_t size, bool success)
	{
	    (void)data;
	    (void)size;

	    if(success)
        {
            DataChunk::Release(_txBuffer.DetachFront());
            auto chunk = _txBuffer.BufferList();
            if(!chunk)
            {
                return;
            }
            auto callback = [this](void *data, size_t size, bool success)
            {
                TxHandler(data, size, success);
            };

            if(!IODevice::WriteAsync(chunk->Data(), chunk->Size(), callback))
            {
                // handle error
            }
        }
        else
        {
            _txBuffer.Clear();
        }
	}

	template<class IODevice>
    void ModbusTransportRtu<IODevice>::RxHandler(void *data, size_t size, bool success)
    {
        (void)data;

        if(!_rxChunk || !_rxChunk->Resize(size))
        {
            // logic error
            return;
        }

        if(size > 0)
        {
            _rxBuffer.AttachBack(_rxChunk);
            _rxChunk = DataChunk::GetNew(DataChunkSize());
            if(!_rxChunk)
            {
                // out of memory
                return;
            }
            if(!success)
            {
                if(_device)
                {
                    uint16_t crc = CalcModbusCrc(_rxBuffer);

                    if(crc == 0)
                    {
                        _rxBuffer.Seek(0);
                        _device->MessageReceived(_rxBuffer);
                    }
                }
                _rxBuffer.Clear();
            }
        }

        auto callback = [this](void *data, size_t size, bool success)
        {
            RxHandler(data, size, success);
        };
        bool res = IODevice::ReadAsync(_rxChunk->Data(), _rxChunk->Capacity(), callback);
        if(res)
        {
            // handle device busy
        }
    }

}
}
