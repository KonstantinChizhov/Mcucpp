
#include "__compatibility.h"
#include <iopins.h>
#include <enum.h>

#ifndef _MCUCPP_AVR_SPI_H
#define _MCUCPP_AVR_SPI_H

namespace Mcucpp
{
	namespace Private
	{
		using namespace IO;
		#include "__spi_pins.h"
	}
	
	class Spi
	{
		enum{SPI2X_shift = 8};
		public:

		typedef Private::SsPin SsPin;
		typedef Private::MosiPin MosiPin;
		typedef Private::MisoPin MisoPin;
		typedef Private::SckPin SckPin;
		
		enum Flags
		{
			Div2	= 1 << (SPI2X + SPI2X_shift) | 0 << SPR1 | 0 << SPR0,
			Div4	= 0 << (SPI2X + SPI2X_shift) | 0 << SPR1 | 0 << SPR0,
			Div8	= 1 << (SPI2X + SPI2X_shift) | 0 << SPR1 | 1 << SPR0,
			Div16	= 0 << (SPI2X + SPI2X_shift) | 0 << SPR1 | 1 << SPR0,
			Div32	= 1 << (SPI2X + SPI2X_shift) | 1 << SPR1 | 0 << SPR0,
			Div64	= 1 << (SPI2X + SPI2X_shift) | 1 << SPR1 | 1 << SPR0,
			Div128	= 0 << (SPI2X + SPI2X_shift) | 1 << SPR1 | 1 << SPR0,
			Fastest = Div2,
			Fast	= Div4,
			Medium	= Div16,
			Slow	= Div64,
			Slowest = Div128,
			LSB = 1 << DORD,
			MSB = 0,
			ClockPolarityRising = 0,
			ClockPolarityFalling = 1 << CPOL,
			ClockPhaseSample = 0,
			ClockPhaseSetup = 1 << CPHA,

		};

		static void Init(Flags flags)
		{
			SsPin::SetDirWrite();
			SsPin::Set();
			MosiPin::SetDirWrite();
			SckPin::SetDirWrite();
			MisoPin::SetDirRead();
			
			if(flags & (1 << (SPI2X + SPI2X_shift)))
				SPSR |= 1 << SPI2X;
			else 
				SPSR &= ~(1 << SPI2X);
				
			SPCR = uint8_t(flags) | 1<<SPE | 1<<MSTR;
		}

		static uint8_t ReadWrite(uint8_t outValue)
		{
			SPDR = outValue;
			while(!(SPSR & (1<<SPIF)));
			return SPDR;
		}
		
		static uint8_t Read()
		{
			return ReadWrite(0xff);
		}
		
		static void Write(uint8_t outValue)
		{
			ReadWrite(outValue);
		}

		static void InterruptEnable()
		{
			SPCR |= (1 << SPIE);
		}

		static void InterruptDisable()
		{
			SPCR &= ~(1 << SPIE);
		}
	};

	DECLARE_ENUM_OPERATIONS(Spi::Flags)
}
#endif
