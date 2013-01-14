#ifndef CRC16_H
#define CRC16_H
#include <stdint.h>
#include <template_utils.h>

namespace Mcucpp
{
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
	};

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

	template<class CrcClass>
	inline typename CrcClass::ResultType CrcUpdate(
						uint8_t newchar,
						typename CrcClass::ResultType crcval)
	{
		if(CrcClass::RefOut)
		{
			//return (crcval >> 8) ^ CrcTable<CrcClass>((crcval ^ newchar) & 0xff);
			return CrcTable<CrcClass>(crcval ^ newchar);
		}
		else
		{
			return CrcTable<CrcClass>(newchar ^ (crcval >> (CrcClass::Width - 8))) ^ (crcval << 8);
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
