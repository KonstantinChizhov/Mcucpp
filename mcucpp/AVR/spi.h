
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
	
	class SpiBase
	{
	protected:
		enum{SPI2X_shift = 8};
	public:
		enum ClockDivider
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
		};
		
		enum ModeFlags
		{
			SoftSlaveControl = 0,
			LsbFirst = 1 << DORD,
			MsbFirst = 0,
			DataSize8 = 0,
			ClockPolarityLow = 0,
			ClockPolarityHigh = 1 << CPOL,
			ClockPhase1Edge = 0,
			ClockPhase2Edge = 1 << CPHA,
			Master = 1 << MSTR,
			Slave = 0,
		};
	};
	
	inline SpiBase::ModeFlags operator|(SpiBase::ModeFlags left, SpiBase::ModeFlags right)
	{
		return static_cast<SpiBase::ModeFlags>(static_cast<int>(left) | static_cast<int>(right));
	}
		
		
	class Spi :public SpiBase
	{
		public:
		typedef Private::SsPin SsPin;
		typedef Private::MosiPin MosiPin;
		typedef Private::MisoPin MisoPin;
		typedef Private::SckPin SckPin;
		
		static void Enable()
		{
			SPCR |= 1<<SPE;
		}
		
		static void Disable()
		{
			SPCR &= ~(1<<SPE);
		}
		
		static void Init(ClockDivider divider, ModeFlags mode = Master)
		{
			SsPin::SetDirWrite();
			SsPin::Set();
			MosiPin::SetDirWrite();
			SckPin::SetDirWrite();
			MisoPin::SetDirRead();
			
			if(divider & (1 << (SPI2X + SPI2X_shift)))
				SPSR |= 1 << SPI2X;
			else 
				SPSR &= ~(1 << SPI2X);
				
			SPCR = uint8_t(mode) | uint8_t(divider) | 1<<SPE;
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
		
		static void SetSS()
		{
			SsPin::Set();
		}
		
		static void ClearSS()
		{
			SsPin::Clear();
		}
	};
}
#endif
