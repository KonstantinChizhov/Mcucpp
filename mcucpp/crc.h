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

#ifndef CRC16_H
#define CRC16_H

#include <stdint.h>
#include <stddef.h>
#include <template_utils.h>

namespace Mcucpp
{
	template<class CrcClass>
	inline typename CrcClass::ResultType CrcTable(typename CrcClass::ResultType v)
	{
		typedef typename CrcClass::ResultType T;
		const T Msb = T(1) << (sizeof(T)*8 - 1);
		uint8_t i = 8;
		if(CrcClass::RefIn)
		{
			do
				v = v & 1 ? (v >> 1) ^ CrcClass::RevPoly : v >> 1;
			while(--i);
		}else
		{
			v = v << (CrcClass::Width - 8);
			do
				v = v & Msb ? (v << 1) ^ CrcClass::Poly : v << 1;
			while(--i);
		}

		return v;
	}

	struct Crc16
	{
		typedef uint16_t ResultType;
		static const unsigned Width = 16;
		static const ResultType Poly = 0x8005u;
		static const ResultType Init = 0;
		static const bool RefIn = true;
		static const bool RefOut = true;
		static const ResultType XorOut = 0;
		static const ResultType Check = 0xBB3Du;
		static inline const char *CheckMessage(){return "123456789";}
		static inline const char *Name(){return "CRC 16";}
		static const ResultType RevPoly = Util::ReverseBits<ResultType, Poly>::value;
		static inline ResultType Table(ResultType v){return CrcTable<Crc16>(v);}
	};

	struct Crc16Table
	{
		typedef uint16_t ResultType;
		static const unsigned Width = 16;
		static const ResultType Poly = 0x8005u;
		static const ResultType Init = 0;
		static const bool RefIn = true;
		static const bool RefOut = true;
		static const ResultType XorOut = 0;
		static const ResultType Check = 0xBB3Du;
		static inline const char *CheckMessage(){return "123456789";}
		static inline const char *Name(){return "CRC 16";}
		static const ResultType RevPoly = Util::ReverseBits<ResultType, Poly>::value;
		static inline ResultType Table(ResultType v)
		{
			static const ResultType table[]=
			{
				000000, 0xc0c1, 0xc181, 0x0140, 0xc301, 0x03c0, 0x0280, 0xc241,
				0xc601, 0x06c0, 0x0780, 0xc741, 0x0500, 0xc5c1, 0xc481, 0x0440,
				0xcc01, 0x0cc0, 0x0d80, 0xcd41, 0x0f00, 0xcfc1, 0xce81, 0x0e40,
				0x0a00, 0xcac1, 0xcb81, 0x0b40, 0xc901, 0x09c0, 0x0880, 0xc841,
				0xd801, 0x18c0, 0x1980, 0xd941, 0x1b00, 0xdbc1, 0xda81, 0x1a40,
				0x1e00, 0xdec1, 0xdf81, 0x1f40, 0xdd01, 0x1dc0, 0x1c80, 0xdc41,
				0x1400, 0xd4c1, 0xd581, 0x1540, 0xd701, 0x17c0, 0x1680, 0xd641,
				0xd201, 0x12c0, 0x1380, 0xd341, 0x1100, 0xd1c1, 0xd081, 0x1040,
				0xf001, 0x30c0, 0x3180, 0xf141, 0x3300, 0xf3c1, 0xf281, 0x3240,
				0x3600, 0xf6c1, 0xf781, 0x3740, 0xf501, 0x35c0, 0x3480, 0xf441,
				0x3c00, 0xfcc1, 0xfd81, 0x3d40, 0xff01, 0x3fc0, 0x3e80, 0xfe41,
				0xfa01, 0x3ac0, 0x3b80, 0xfb41, 0x3900, 0xf9c1, 0xf881, 0x3840,
				0x2800, 0xe8c1, 0xe981, 0x2940, 0xeb01, 0x2bc0, 0x2a80, 0xea41,
				0xee01, 0x2ec0, 0x2f80, 0xef41, 0x2d00, 0xedc1, 0xec81, 0x2c40,
				0xe401, 0x24c0, 0x2580, 0xe541, 0x2700, 0xe7c1, 0xe681, 0x2640,
				0x2200, 0xe2c1, 0xe381, 0x2340, 0xe101, 0x21c0, 0x2080, 0xe041,
				0xa001, 0x60c0, 0x6180, 0xa141, 0x6300, 0xa3c1, 0xa281, 0x6240,
				0x6600, 0xa6c1, 0xa781, 0x6740, 0xa501, 0x65c0, 0x6480, 0xa441,
				0x6c00, 0xacc1, 0xad81, 0x6d40, 0xaf01, 0x6fc0, 0x6e80, 0xae41,
				0xaa01, 0x6ac0, 0x6b80, 0xab41, 0x6900, 0xa9c1, 0xa881, 0x6840,
				0x7800, 0xb8c1, 0xb981, 0x7940, 0xbb01, 0x7bc0, 0x7a80, 0xba41,
				0xbe01, 0x7ec0, 0x7f80, 0xbf41, 0x7d00, 0xbdc1, 0xbc81, 0x7c40,
				0xb401, 0x74c0, 0x7580, 0xb541, 0x7700, 0xb7c1, 0xb681, 0x7640,
				0x7200, 0xb2c1, 0xb381, 0x7340, 0xb101, 0x71c0, 0x7080, 0xb041,
				0x5000, 0x90c1, 0x9181, 0x5140, 0x9301, 0x53c0, 0x5280, 0x9241,
				0x9601, 0x56c0, 0x5780, 0x9741, 0x5500, 0x95c1, 0x9481, 0x5440,
				0x9c01, 0x5cc0, 0x5d80, 0x9d41, 0x5f00, 0x9fc1, 0x9e81, 0x5e40,
				0x5a00, 0x9ac1, 0x9b81, 0x5b40, 0x9901, 0x59c0, 0x5880, 0x9841,
				0x8801, 0x48c0, 0x4980, 0x8941, 0x4b00, 0x8bc1, 0x8a81, 0x4a40,
				0x4e00, 0x8ec1, 0x8f81, 0x4f40, 0x8d01, 0x4dc0, 0x4c80, 0x8c41,
				0x4400, 0x84c1, 0x8581, 0x4540, 0x8701, 0x47c0, 0x4680, 0x8641,
				0x8201, 0x42c0, 0x4380, 0x8341, 0x4100, 0x81c1, 0x8081, 0x4040
			};
			return table[v];
		}
	};

	struct Crc16Modbus
	{
		typedef uint16_t ResultType;
		static const unsigned Width = 16;
		static const ResultType Poly = 0xA001u;
		static const ResultType Init = 0xffff;
		static const bool RefIn = true;
		static const bool RefOut = true;
		static const ResultType XorOut = 0;
		static const ResultType Check = 0x4B37;
		static inline const char *CheckMessage(){return "123456789";}
		static inline const char *Name(){return "Modbus CRC 16";}
		static const ResultType RevPoly = Util::ReverseBits<ResultType, Poly>::value;
		static inline ResultType Table(ResultType v){return CrcTable<Crc16>(v);}
	};

	struct Crc16ModbusTable
	{
		typedef uint16_t ResultType;
		static const unsigned Width = 16;
		static const ResultType Poly = 0xA001u;
		static const ResultType Init = 0xffff;
		static const bool RefIn = true;
		static const bool RefOut = true;
		static const ResultType XorOut = 0;
		static const ResultType Check = 0x4B37;
		static inline const char *CheckMessage(){return "123456789";}
		static inline const char *Name(){return "Modbus CRC 16";}
		static const ResultType RevPoly = Util::ReverseBits<ResultType, Poly>::value;
		static inline ResultType Table(ResultType v){
			static const ResultType table[]=
			{
				0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
				0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
				0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
				0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
				0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
				0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
				0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
				0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
				0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
				0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
				0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
				0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
				0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
				0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
				0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
				0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
				0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
				0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
				0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
				0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
				0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
				0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
				0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
				0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
				0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
				0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
				0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
				0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
				0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
				0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
				0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
				0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040
			};
			return table[v];
		}
	};

	
	struct Crc16Citt
	{
		typedef uint16_t ResultType;
		static const unsigned Width = 16;
		static const ResultType Poly =  0x1021;
		static const ResultType Init = 0xFFFF;
		static const bool RefIn = false;
		static const bool RefOut = false;
		static const ResultType XorOut = 0;
		static const ResultType Check = 0x29B1u;
		static inline const char *CheckMessage(){return "123456789";}
		static inline const char *Name(){return "CRC 16/CITT";}
		static const ResultType RevPoly = Util::ReverseBits<ResultType, Poly>::value;
		static inline ResultType Table(ResultType v){return CrcTable<Crc16Citt>(v);}
	};

	struct XModemCrc
	{
		typedef uint16_t ResultType;
		static const unsigned Width = 16;
		static const ResultType Poly =  0x1021;
		static const ResultType Init = 0x0000;
		static const bool RefIn = false;
		static const bool RefOut = false;
		static const ResultType XorOut = 0;
		static const ResultType Check = 0x31C3;
		static inline const char *CheckMessage(){return "123456789";}
		static inline const char *Name(){return "XMODEM";}
		static const ResultType RevPoly = Util::ReverseBits<ResultType, Poly>::value;
		static inline ResultType Table(ResultType v){return CrcTable<XModemCrc>(v);}
	};

	struct Crc32
	{
		typedef uint32_t ResultType;
		static const unsigned Width = 32;
		static const ResultType Poly = 0x04C11DB7UL;
		static const ResultType Init = 0XFFFFFFFFUL;
		static const bool RefIn = true;
		static const bool RefOut = true;
		static const ResultType XorOut = 0XFFFFFFFFUL;
		static const ResultType Check = 0xCBF43926UL;
		static inline const char *CheckMessage(){return "123456789";}
		static inline const char *Name(){return "CRC 32";}
		static const ResultType RevPoly = Util::ReverseBits<ResultType, Poly>::value;
		static inline ResultType Table(ResultType v){return CrcTable<Crc32>(v);}
	};

	struct DallasCrc
	{
		typedef uint8_t ResultType;
		static const unsigned Width = 8;
		static const ResultType Poly = 0x31;
		static const ResultType Init = 0x0;
		static const bool RefIn = true;
		static const bool RefOut = true;
		static const ResultType XorOut = 0;
		static const ResultType Check = 0xA1;
		static inline const char *CheckMessage(){return "123456789";}
		static inline const char *Name(){return "Dallas CRC";}
		static const ResultType RevPoly = Util::ReverseBits<ResultType, Poly>::value;
		static inline ResultType Table(ResultType v){return CrcTable<DallasCrc>(v);}
	};

	
	template<class CrcClass>
	inline typename CrcClass::ResultType CrcUpdate(
						uint8_t newchar,
						typename CrcClass::ResultType crcval)
	{
		if(CrcClass::RefOut)
		{
			return (crcval >> 8) ^ CrcClass::Table((crcval ^ newchar) & 0xff);
			//return CrcTable<CrcClass>(crcval ^ newchar);
		}
		else
		{
			return CrcClass::Table(newchar ^ (crcval >> (CrcClass::Width - 8))) ^ (crcval << 8);
		}
	}

	template<class CrcClass>
	inline typename CrcClass::ResultType ComputeCrc(
						const uint8_t* message,
						size_t length,
						typename CrcClass::ResultType crc = CrcClass::Init)
	{
		for(size_t i = 0; i < length; i++)
		{
		   crc = CrcUpdate<CrcClass>(message[i], crc);
		}
		return crc ^ CrcClass::XorOut;
	}
}
#endif
