
#include <gtest/gtest.h>
#include <modbus.h>
#include <data_transfer.h>
#include <data_buffer.h>
#include <vector>
#include <array>

class ModbusIoDevice
{
public:
	
	
	static bool WriteAsync(void *data, size_t size, Mcucpp::TransferCallbackFunc callback)
	{
		std::cout << "WriteAsync" << size << std::endl;
		writes ++;
		uint8_t *ptr = (uint8_t*)data;
		txdata.insert( txdata.end(), ptr, ptr+size);
		callback(data, size, true);
		return true;
	}

	static bool ReadAsync(void *data, size_t size, Mcucpp::TransferCallbackFunc callback)
	{
		std::cout << "ReadAsync: " << size << std::endl;
		reads++;
		uint8_t *ptr = (uint8_t*)data;
		size_t rxsize = 0;
		if(rxiter == rxdata.size())
		{
			return false;
		}
		for(int i = 0; (i < size) && (rxiter < rxdata.size()); i++, rxiter++, rxsize++)
		{
			ptr[i] = rxdata[rxiter];
		}
		std::cout << "rxsize: " << rxsize << std::endl;

		callback(data, rxsize, rxsize == size);

		return true;
	}

	static void Reset()
	{
		txdata.clear();
		rxdata.clear();
		rxiter = 0;
		reads = 0;
		writes = 0;
	}

	static int reads;
	static int writes;
	static int rxiter;
	static std::vector<uint8_t> txdata;
	static std::vector<uint8_t> rxdata;
};

std::vector<uint8_t> ModbusIoDevice::txdata;
std::vector<uint8_t> ModbusIoDevice::rxdata;
int ModbusIoDevice::rxiter = 0;
int ModbusIoDevice::reads = 0;
int ModbusIoDevice::writes = 0;

class TestDevice :public Mcucpp::Modbus::ModbusDevice
{
public:
	TestDevice(Mcucpp::Modbus::ModbusTransport &transport)
		:ModbusDevice(transport)
	{}

	void MessageReceived(Mcucpp::DataBuffer & buffer)
	{
		lastMessage = std::move(buffer);
	}
	Mcucpp::DataBuffer lastMessage;
};

TEST(Modbus, ModbusTransportRtu_Rx_Tx)
{
	using namespace Mcucpp;
	using namespace Mcucpp::Modbus;
	ModbusIoDevice::Reset();

	ModbusTransportRtu<ModbusIoDevice> transport;
	TestDevice device(transport);

	ModbusIoDevice::rxdata = {1, 3, 0, 1, 0, 1, 213, 202};

	EXPECT_TRUE(transport.StartListen());
	EXPECT_GT(device.lastMessage.Size(), 0);

	Mcucpp::DataBuffer message;
	EXPECT_TRUE(message.InsertFront(10));
	EXPECT_TRUE(message.InsertFront(10));
	EXPECT_TRUE(message.Seek(0));
	for(int i = 0; i < 10; i++)
    {
        message.WriteU16Le((uint16_t)i);
    }
	EXPECT_TRUE(transport.SendMessage(message));
    EXPECT_EQ(ModbusIoDevice::txdata.size(), 22);
    for(int i = 0; i < 10; i++)
    {
        EXPECT_EQ(ModbusIoDevice::txdata[i*2], i);
        EXPECT_EQ(ModbusIoDevice::txdata[i*2+1], 0);
    }
    EXPECT_EQ(ModbusIoDevice::txdata[20], 0X93);
    EXPECT_EQ(ModbusIoDevice::txdata[21], 0XF7);

    uint16_t crc = Crc16ModbusTable::Init;
    for(uint8_t c : ModbusIoDevice::txdata)
    {
        crc = CrcUpdate<Crc16ModbusTable>(c, crc);
    }

    EXPECT_EQ(crc, 0);
}


TEST(Modbus, SlaveTest)
{
	using namespace Mcucpp;
	using namespace Mcucpp::Modbus;
	ModbusIoDevice::Reset();

	ModbusTransportRtu<ModbusIoDevice> transport;
	ModbusSlave device(transport);

	ModbusIoDevice::rxdata = {1, 3, 0, 1, 0, 1, 213, 202};
	EXPECT_TRUE(transport.StartListen());
	EXPECT_EQ(ModbusIoDevice::txdata, std::vector<uint8_t>({1, 3 , 2, 0, 0, 0xB8, 0x44}));

	ModbusIoDevice::Reset();
	ModbusIoDevice::rxdata = {0x01, 0x04, 0x00, 0x00, 0x00,  0x7D, 0x30, 0x2B};
	EXPECT_TRUE(transport.StartListen());

	auto expected = std::vector<uint8_t>({
		0x1, 0x4, 0xfa, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xf0, 0xa3});
	
	EXPECT_EQ(ModbusIoDevice::txdata, expected);

	uint16_t crc = Crc16ModbusTable::Init;
	for(uint8_t c : ModbusIoDevice::txdata)
	{
		crc = CrcUpdate<Crc16ModbusTable>(c, crc);
	}

	EXPECT_EQ(crc, 0);
}

TEST(Modbus, SlaveTestStaticBuffer)
{
	using namespace Mcucpp;
	using namespace Mcucpp::Modbus;
	ModbusIoDevice::Reset();

	uint8_t rxBuffer[256];
	DataChunk rxChunk(rxBuffer, 0, sizeof(rxBuffer));
	uint8_t txBuffer[256];
	DataChunk txChunk(txBuffer, 0, sizeof(txBuffer));

	ModbusTransportRtu<ModbusIoDevice> transport;
	ModbusSlave device(transport);
	transport.SetStaticBuffers(&rxChunk, &txChunk);

	ModbusIoDevice::rxdata = {1, 3, 0, 1, 0, 1, 213, 202};
	EXPECT_TRUE(transport.StartListen());
	EXPECT_EQ(ModbusIoDevice::txdata, std::vector<uint8_t>({1, 3 , 2, 0, 0, 0xB8, 0x44}));
	EXPECT_EQ(ModbusIoDevice::reads, 2);
	EXPECT_EQ(ModbusIoDevice::writes, 1);
}
