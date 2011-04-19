#pragma once

#include <iopins.h>
#include <static_assert.h>
#include <delay.h>


enum Command
{
	ReadRegCmd        	= 0x00,
	WriteRegCmd       	= 0x20,
	ReadRxDataCmd	   	= 0x61,
	WriteTxDataCmd     	= 0xA0,
	FlushTxCmd	       	= 0xE1,
	FlushRxCmd	       	= 0xE2,
	ReuseTxDataCmd     	= 0xE3,
	WriteTxDataNoAckCmd	= 0xb0,
	WriteAckDataCmd		= 0xa8,
	ActivateCmd			= 0x50,
	ReadRxDataLenghtCmd	= 0x60,
	NopCmd            	= 0xFF
};

enum Registers
{
	ConfigReg			= 0x00,
	EnableAutoAckReg	= 0x01,
	EnableRxAdressReg	= 0x02,
	SetupAdressWidthReg	= 0x03,
	SetupRetryReg		= 0x04,
	RfChannelReg		= 0x05,
	RfSetupReg			= 0x06,
	StatusReg			= 0x07,
	ObserveTxReg		= 0x08,
	CarrierDetectReg	= 0x09,
	RxAddress0			= 0x0A,
	RxAddress1			= 0x0B,
	RxAddress2			= 0x0C,
	RxAddress3			= 0x0D,
	RxAddress4			= 0x0E,
	RxAddress5			= 0x0F,
	TxAddress			= 0x10,
	RxDataLength0		= 0x11,
	RxDataLength1		= 0x12,
	RxDataLength2		= 0x13,
	RxDataLength3		= 0x14,
	RxDataLength4		= 0x15,
	RxDataLength5		= 0x16,
	FifoStatusReg  		= 0x17,
	DynamicPayload		= 0x1c,
	Feature				= 0x1d
};

enum FeatureFlags
{
	EnableDynamicPayloadFlag= 1 << 2,
	EnableAckPayloadFlag	= 1 << 1,
	EnableDynamicAckFlag	= 1 << 0
};


enum FifoStatus
{
	FifoTxReuse		= 0x40,
	FifoTxFull		= 0x20,
	FifoTxEmpty		= 0x10,
	FifoRxFull		= 0x02,
	FifoRxEmpty		= 0x01
};

enum ConfigRegValues
{
	MaskRxDataReady	= 1 << 6,
	MaskTxDataReady	= 1 << 5,
	MaskMaxRetry	= 1 << 4,
	EnableCrc		= 1 << 3,
	Crc2bytes		= 1 << 2,
	PowerUpBit		= 1 << 1,
	TxModeBit		= 1 << 0
};

enum{Reg15Size = 11 };
static const uint8_t Bank1_Reg15[Reg15Size]=
{
	0x41,0x20,0x08,0x04,0x81,0x20,0xCF,0xF7,0xFE,0xFF,0xFF //LSB first
};

enum AddressWidthValues
{
	AW3Bytes = 1,
	AW4Bytes = 2,
	AW5Bytes = 3
};

enum AutoRetransmissionDelayValues
{
	 Wait250us = 0x00,
	 Wait500us = 0x10,
	 Wait750us = 0x20,
	 Wait1000us = 0x30,
	 Wait1250us = 0x40,
	 Wait1500us = 0x50,
	 Wait1750us = 0x60,
	 Wait2000us = 0x70,
	 Wait2250us = 0x80,
	 Wait2500us = 0x90,
	 Wait2750us = 0xA0,
	 Wait3000us = 0xB0,
	 Wait3250us = 0xC0,
	 Wait3500us = 0xD0,
	 Wait3750us = 0xE0,
	 Wait4000us = 0xF0
};

enum RfSetupValues
{
	DataRate1Mbps 		= 0 << 3,
	DataRate2Mbps 		= 1 << 3,

	OutputPowerMinus10dBm = 0 << 1,
	OutputPowerMinus5dBm = 1 << 1,
	OutputPower0dBm 	= 2 << 1,
	OutputPower5dBm 	= 3 << 1,

	LnaLowGain			= 0,
	LnaHighGain			= 1
};

enum StatusValues
{
	RegBank			= 1 << 7,
	RxDataReady		= 1 << 6,
	TxDataSent		= 1 << 5,
	MaxRetransmits	= 1 << 4,
	RxPipeNumberMask= 7 << 1,
	RxPipeNumberShift= 1,
	TxFull			= 1
};

class Rfm70Defaults
{
	static const AutoRetransmissionDelayValues AutoRetransmissionDelay = Wait1000us;
	static const uint8_t AutoRetransmissionCount = 15; //0..15
public:
	static const AddressWidthValues AddressWidth = AW5Bytes;

	static const uint8_t RfChannel	= 50; // 0..83
	static const uint8_t RfSetup 	= DataRate1Mbps | OutputPower5dBm | LnaHighGain;
	static const uint8_t Config 	= EnableCrc | Crc2bytes;

public:
	static const uint8_t RetrySetrup = AutoRetransmissionDelay | AutoRetransmissionCount;

};


template<class Spi, class SlaveSelectPin, class EnablePin, class IrqPin, class Defaults = Rfm70Defaults>
class Rfm70
{
private:

	static void WriteReg(uint8_t reg, uint8_t value)
	{
		SlaveSelectPin::Clear();
		Spi::ReadWrite(reg | WriteRegCmd);
		Spi::ReadWrite(value);
		SlaveSelectPin::Set();
	}

	static uint8_t ReadReg(uint8_t reg)
	{
		SlaveSelectPin::Clear();
		Spi::ReadWrite(reg | ReadRegCmd);
		uint8_t value = Spi::ReadWrite(0);
		SlaveSelectPin::Set();
		return value;
	}

	static void ModifyReg(uint8_t reg, uint8_t clearMask, uint8_t setMask)
	{
		uint8_t value = ReadReg(reg);
		WriteReg(reg, (value & clearMask) | setMask);
	}

	static void WriteReg32(uint8_t reg, uint32_t value)
	{
		SlaveSelectPin::Clear();
		Spi::ReadWrite(reg | WriteRegCmd);
		Write(value);
		SlaveSelectPin::Set();
	}

	static void Write(uint32_t value)
	{
		union
		{
			uint32_t dword;
			uint8_t byte[4];
		}param;
		param.dword = value;

		Spi::ReadWrite(param.byte[0]);
		Spi::ReadWrite(param.byte[1]);
		Spi::ReadWrite(param.byte[2]);
		Spi::ReadWrite(param.byte[3]);
	}

	static uint32_t ReadReg32(uint8_t reg)
	{
		SlaveSelectPin::Clear();
		Spi::ReadWrite(reg | ReadRegCmd);
		uint32_t value = Spi::ReadWrite(0);
		value |= Spi::ReadWrite(0) << 8;
		value |= Spi::ReadWrite(0) << 16;
		value |= Spi::ReadWrite(0) << 24;
		SlaveSelectPin::Set();
		return value;
	}

	static void ReadBuffer(uint8_t command, uint8_t *buffer, uint8_t length)
	{
		SlaveSelectPin::Clear();
		Spi::ReadWrite(command);
		//for(uint8_t *end = buffer + length; buffer != end; ++buffer)
		//	*buffer = Spi::ReadWrite(0);

		for(uint8_t i = 0;i<length; ++i)
		{
		    debug << "i";
		    buffer[i] = Spi::ReadWrite(0);
		}

        debug << "e";
		SlaveSelectPin::Set();
	}

	static void WriteBuffer(uint8_t command, const uint8_t *buffer, uint8_t length)
	{
		SlaveSelectPin::Clear();
		Spi::ReadWrite(command);

		for(const uint8_t *end = buffer + length; buffer != end; ++buffer)
			Spi::ReadWrite(*buffer);

		SlaveSelectPin::Set();
	}

	static void InitBank1Regs()
	{
		SwitchBank(1);
		WriteBuffer(WriteRegCmd | 15, Bank1_Reg15, Reg15Size);

		WriteReg32(0x00, 0xE2014B40);
		WriteReg32(0x01, 0x00004BC0);
		WriteReg32(0x02, 0x028CFCD0);
		WriteReg32(0x03, 0x41390099);
		WriteReg32(0x04, 0x0B869ED9);
		WriteReg32(0x05, 0xA67F0624);
		WriteReg32(0x06, 0x0B869ED9);

		WriteReg32(0x0c, 0x00731200);
		WriteReg32(0x0d, 0x0080B436);
	}

	static void SwitchBank(bool bank)
	{
		bool isBank0 = (ReadReg(StatusReg) & RegBank) != 0;
		if(bank != isBank0)
		{
			WriteReg(ActivateCmd, 0x53);
		}
	}

	static void SetAddresess(uint8_t reg, uint32_t higherBytes, uint8_t lowerByte)
	{
		switch(Defaults::AddressWidth)
		{
			case AW3Bytes:
				SlaveSelectPin::Clear();
				Spi::ReadWrite(reg | WriteRegCmd);
				Spi::ReadWrite((higherBytes >> 0) & 0xff);
				Spi::ReadWrite((higherBytes >> 8) & 0xff);
				Spi::ReadWrite((higherBytes >> 16) & 0xff);
				SlaveSelectPin::Set();
			break;

			case AW4Bytes:
				WriteReg32(reg, higherBytes);
			break;

			case AW5Bytes:
				SlaveSelectPin::Clear();
				Spi::ReadWrite(reg | WriteRegCmd);
				Write(higherBytes);
				Spi::ReadWrite(lowerByte);
				SlaveSelectPin::Set();
			break;
		}
	}

public:

	static void Init()
	{
		SlaveSelectPin::Set();
		EnablePin::Set();
		SlaveSelectPin::SetDirWrite();
		EnablePin::SetDirWrite();
		IrqPin::SetDirRead();
		IrqPin::Clear();

		Util::delay_ms<50, F_CPU>();
		Activate();

		InitBank1Regs();

		SwitchBank(0);

		WriteReg(ConfigReg, Defaults::Config | PowerUpBit);

		WriteReg(SetupAdressWidthReg, Defaults::AddressWidth);
		SetRfChannel(Defaults::RfChannel);
		RfSetup(Defaults::RfSetup);
		WriteReg(SetupRetryReg, Defaults::RetrySetrup);
		EnableDinamicPayload();
		//WriteReg(RxDataLength0, 32);
	}

	static void EnableDinamicPayload()
	{
		WriteReg(Feature, EnableDynamicPayloadFlag);
		WriteReg(DynamicPayload, 0x3f);
	}

	static void PowerUp()
	{
		ModifyReg(ConfigReg, 0xff, PowerUpBit);
	}

	static void PowerDown()
	{
		ModifyReg(ConfigReg, ~PowerUpBit, 0);
	}

	static void SwitchToRxMode()
	{
		FlushRx();
		ClearInterruptStatus();

		EnablePin::Clear();
		ModifyReg(ConfigReg, 0xff, TxModeBit);
		EnablePin::Set();
	}

	static void SwitchToTxMode()
	{
		FlushTx();
		EnablePin::Clear();
		ModifyReg(ConfigReg, ~TxModeBit, 0);
		EnablePin::Set();
	}

	static void SetRfChannel(uint8_t channel)
	{
		WriteReg(RfChannelReg, channel);
	}

	static void RfSetup(uint8_t rfSetup)
	{
		WriteReg(RfSetupReg, rfSetup);
	}

	static void FlushTx()
	{
		Spi::ReadWrite(FlushTxCmd);
	}

	static void FlushRx()
	{
		Spi::ReadWrite(FlushRxCmd);
	}

	static void Activate()
	{
		WriteReg(ActivateCmd, 0x73);
	}

	static uint8_t RecivedDataLength()
	{
		return ReadReg(ReadRxDataLenghtCmd);
	}

	static uint8_t DataLength(uint8_t pipe)
	{
		return ReadReg(RxDataLength0 + pipe & 0x1f);
	}

	static uint8_t ActiveRxPipe()
	{
		return (ReadReg(StatusReg) >> RxPipeNumberShift) & RxPipeNumberMask;
	}

	static uint8_t Status()
	{
		return ReadReg(StatusReg);
	}

	static void ClearInterruptStatus()
	{
		ModifyReg(StatusReg, 0xff, 0);
	}

	// Address for pipes 2-5
	template<int pipeNumber>
	static void SetRxAddress(uint8_t lowerByte)
	{
		BOOST_STATIC_ASSERT(pipeNumber > 1 && pipeNumber < 6);
		WriteReg(RxAddress0 + pipeNumber, lowerByte);
	}

	// Address for pipes 0-1
	template<int pipeNumber>
	static void SetRxAddress(uint32_t higherBytes, uint8_t lowerByte)
	{
		BOOST_STATIC_ASSERT(pipeNumber >= 0 && pipeNumber < 2);
		SetAddresess(RxAddress0 + pipeNumber, higherBytes, lowerByte);
	}

	static void SetTxAddress(uint32_t higherBytes, uint8_t lowerByte)
	{
		SetAddresess(TxAddress, higherBytes, lowerByte);
	}

	static bool Write(const void * buffer, uint8_t size)
	{
		SwitchToTxMode();

		uint8_t fifoStatus = ReadReg(FifoStatusReg);
		if(!(fifoStatus & FifoTxFull))
		{
			WriteBuffer(WriteTxDataCmd, (uint8_t*)buffer, size);
			return true;
		}
		return false;
	}

	/// Reads recived data payload.
	static bool Recive(void * buffer)
	{
		uint8_t length = RecivedDataLength();

		if(Status() & RxDataReady)
		{
			ReadBuffer(ReadRxDataCmd, (uint8_t*)buffer, length);
			ClearInterruptStatus();
			return true;
		}
		return false;
	}

//debug routins
	template<class Debug>
	static void DumpRegs(Debug &debug)
	{
		SwitchBank(0);

		uint8_t buffer[5];

		for(unsigned i = 0; i<32; i++)
		{
			if(i==0x0A || i==0x0B || i==0x10)
			{
				ReadBuffer(ReadRegCmd | i, buffer, 5);
				debug << i << ":\t0x";
				for(unsigned i = 5; i; i--)
					debug  << buffer[i-1];
				debug << "\r\n";
			}
			else
			{
				unsigned val = ReadReg(i);
				debug << i << ":\t" << val << "\r\n";
			}
		}
	}

	template<class Debug>
	static void DumpRegs1(Debug &debug)
	{
		SwitchBank(1);
		debug << "Bank1:\r\n";

		for(unsigned i = 0; i<14; i++)
		{
			unsigned long val;
			ReadBuffer(ReadRegCmd | i, (uint8_t*)(&val), 4);
			debug << "0x" << i << ":\t0x" << val << "\r\n";
		}
		uint8_t buffer[12];
		ReadBuffer(ReadRegCmd | 14, buffer, 11);
		debug << "0xe:\t0x";
		for(unsigned i = 11; i; i--)
			debug  << buffer[i-1];
		debug << "\r\n";
		SwitchBank(0);
	}
};

