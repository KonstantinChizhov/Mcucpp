
//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2012
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

#ifndef __SANDISK__
#define __SANDISK__

#include <delay.h>
//#include <debug.h>
#include <binary_stream.h>

namespace Mcucpp
{
	enum SdCardCommands
	{
		GO_IDLE_STATE = 0,
		SEND_OP_COND = 1,
		SEND_IF_COND = 8,
		SEND_CSD = 9,
		SEND_CID  = 10,
		STOP_TRANSMISSION = 12,
		SEND_STATUS = 13,
		SET_BLOCK_LENGTH = 16,
		READ_SINGLE_BLOCK = 17,
		READ_MULTIPLE_BLOCK = 18,
		WRITE_BLOCK = 24,
		WRITE_MULTIPLE_BLOCK = 25,
		PROGRAM_CSD = 27,
		SET_WRITE_PROT = 28,
		CLR_WRITE_PROT = 29,
		SEND_WRITE_PROT = 30,
		ERASE_WR_BLK_START_ADDR = 32,
		ERASE_WR_BLK_END_ADDR = 33,
		ERASE = 38,
		APP_CMD  = 55,
		GEN_CMD = 56,
		READ_OCR = 58,
		CRC_ON_OFF = 59,
		SD_SEND_OP_COND = 0x40 | 41 // ACDM41
	};
	
	enum SdR1R2ResponseBits
	{
		SdR1Idle           = (1 << 0),
		SdR1EraseReset     = (1 << 1),
		SdR1IllegalCommand = (1 << 2),
		SdR1CrcError       = (1 << 3),
		SdR1EraseSeqError  = (1 << 4),
		SdR1AddressError   = (1 << 5),
		SdR1ParameterError = (1 << 6),
		
		CardIsLocked  = (1 << 0),
		WPEraseSkip   = (1 << 1),
		Error         = (1 << 2),
		CCError       = (1 << 3),
		CardECCFailed = (1 << 4),
		WPViolation   = (1 << 5),
		EraseParam    = (1 << 6),
		OutofRange    = (1 << 7)
	};

	enum SdCardType
	{
		SdCardNone = 0,
		SdCardMmc  = 0x01,
		SdCardV1   = 0x02,
		SdCardV2   = 0x04,
		SdhcCard   = 0x06
	};

	enum SdCardDataError
	{
		SdDataError = 0x01,
		SdDataCCError  = 0x02,
		SdDataECCError   = 0x04,
		SdDataOutOfRangeError   = 0x08
	};

	
	template<class SpiModule_, class CsPin>
	class SdCard
	{
		enum
		{
			CommandTimeoutValue = 100
		};
		static const bool useCrc = false;
		SdCardType _type;
		
		typedef BinaryStream<SpiModule_> Spi;
	protected:
		uint16_t SpiCommand(uint8_t index, uint32_t arg, uint8_t crc = 0);
		uint32_t ReadBlocksCount();
		bool WaitWhileBusy();
		template<class ReadIterator>
		bool ReadDataBlock(ReadIterator iter, size_t size)
		{
			CsPin::Clear();

			uint8_t resp;
			resp = Spi::IgnoreWhile(1000, 0xFF);
			if(resp != 0xFE)
			{
				CsPin::Set();
				return false;
			}
			Spi::template Read<ReadIterator>(iter, size);
			uint16_t crc = Spi::ReadU16Le();
			if(useCrc)
			{
				// TBD
			}else
				(void)crc;
			CsPin::Set();
			Spi::Read();
			return true;
		}
	public:

		bool CheckStatus();

		SdCardType Detect();

		uint32_t BlocksCount();

		size_t BlockSize();
		
		template<class WriteIterator>
		bool WriteBlock(WriteIterator iter, uint32_t logicalBlockAddress)
		{
			if(_type != SdhcCard)
				logicalBlockAddress <<= 9;
			if(SpiCommand(WRITE_BLOCK, logicalBlockAddress) == 0)
			{
				CsPin::Clear();
				if(Spi::Ignore(10000u, 0xff) != 0xff)
				{
					return false;
				}

				Spi::Write(0xFE);
				Spi::template Write<WriteIterator>(iter, 512);
				Spi::ReadU16Be();
				uint8_t resp;
				if((resp = Spi::Read() & 0x1F) != 0x05)
				{
					return false;
				}
				CsPin::Set();
				Spi::Read();
				return true;
			}
			return false;
		}

		template<class ReadIterator>
		bool ReadBlock(ReadIterator iter, uint32_t logicalBlockAddress)
		{
			if(_type != SdhcCard)
				logicalBlockAddress <<= 9;
			if(!WaitWhileBusy())
				return false;
			if(SpiCommand(READ_SINGLE_BLOCK, logicalBlockAddress) == 0)
			{
				return ReadDataBlock<ReadIterator>(iter, 512);
			}
			return false;
		}
			
	};


	template<class SpiModule_, class CsPin> 
	uint16_t SdCard<SpiModule_, CsPin>::SpiCommand(uint8_t index, uint32_t arg, uint8_t crc)
	{
		CsPin::Clear();
		Spi::Read();
		Spi::Write(index | (1 << 6));
		Spi::WriteU32Be(arg);
		Spi::Write(crc | 1);
		uint16_t responce = Spi::IgnoreWhile(1000, 0xff);
		if(index == SEND_STATUS && responce !=0xff)
			responce |= Spi::Read() << 8;
		CsPin::Set();
		Spi::Read();
		return responce;
	}


	template<class SpiModule_, class CsPin> 
	bool SdCard<SpiModule_, CsPin>::CheckStatus()
	{
		 return SpiCommand(SEND_STATUS, 0) == 0;
	}

	template<class SpiModule_, class CsPin> 
	SdCardType SdCard<SpiModule_, CsPin>::Detect()
	{
		_type = SdCardNone;

		CsPin::SetDirWrite();
		CsPin::Set();
		Spi::Init(Spi::Fastest, Spi::Master | Spi::MsbFirst);

		for(uint8_t i=0; i<20; i++)
			Spi::Read();

		if(SpiCommand(GO_IDLE_STATE, 0, 0x95) > SdR1Idle) 
			return _type;
	
		uint8_t resp;
		uint16_t timeout = 50;

		// test for SDCv2
		if(SpiCommand(SEND_IF_COND, 0x1aa, 0x87) <= SdR1Idle)
		{
			CsPin::Clear();
			uint32_t voltage = Spi::ReadU32Le();
			CsPin::Set();
		
			// voltage check
			if((voltage & 0x00ffff00) == 0x00aa0100)
			{
				do
				{
					resp = SpiCommand(SEND_OP_COND, 1ul << 30);
					Util::delay_ms<50, F_CPU>();
				}while(resp != 0 && --timeout);

				if(resp == 0 && !SpiCommand(READ_OCR, 0))
				{
					CsPin::Clear();
					uint32_t ocr = Spi::ReadU32Le();
					CsPin::Set();
 					_type = ocr & 0x40 ? SdhcCard : SdCardV2;
				}
			}
		}
		else
		{
			// try SD_SEND_OP_COND for SDSC
			if(SpiCommand(APP_CMD, 0) <= SdR1Idle && (resp = SpiCommand(SD_SEND_OP_COND, 0)) <= SdR1Idle)
			{
				while(resp != 0 && --timeout)
				{
					resp = SpiCommand(SEND_OP_COND, 0);
					Util::delay_ms<50, F_CPU>();
				}

				if(resp == 0)
					_type = SdCardV1;
			}
			else //  MMC
			{
				do
				{
					resp = SpiCommand(SEND_OP_COND, 0);
					Util::delay_ms<50, F_CPU>();
				}while(resp != 0 && --timeout);

				if(resp == SdR1Idle)
					_type = SdCardMmc;
			}
		}
		return _type;
	}

	template<class SpiModule_, class CsPin> 
	uint32_t SdCard<SpiModule_, CsPin>::ReadBlocksCount()
	{
		uint8_t csd[16];
		if(!SpiCommand(SEND_CSD, 0) && ReadDataBlock(csd, 16))
		{
			if(csd[0] & 0xC0) // SD v2
			{
				uint32_t c_size = (((uint32_t)csd[7] & 0x3F) << 16) | ((uint32_t)csd[8] << 8) | csd[9];
				return (c_size + 1) * 1024u - 1u;
			}else // SD v1
			{
				uint32_t c_size = ((((uint32_t)csd[6] << 16) | ((uint32_t)csd[7] << 8) | csd[8]) & 0x0003FFC0) >> 6;
				uint16_t c_size_mult = ((uint16_t)((csd[9] & 0x03) << 1)) | ((uint16_t)((csd[10] & 0x80) >> 7));
				uint16_t block_len = csd[5] & 0x0F;
				block_len = 1u << (block_len - 9); 
				return ((c_size + 1u) * (1u << (c_size_mult + 2u)) * block_len) - 1u;
			}	
		}
		return 0;
	}

	template<class SpiModule_, class CsPin> 
	uint32_t SdCard<SpiModule_, CsPin>::BlocksCount()
	{
		// TODO: cache this value
		return ReadBlocksCount();
	}

	template<class SpiModule_, class CsPin> 
	size_t SdCard<SpiModule_, CsPin>::BlockSize()
	{
		return 512;
	}

	template<class SpiModule_, class CsPin> 
	bool SdCard<SpiModule_, CsPin>::WaitWhileBusy()
	{
		CsPin::Clear();
		return Spi::Ignore(10000u, 0xff) == 0xff;
	}

} // namespace Mcucpp
#endif
