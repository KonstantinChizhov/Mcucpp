
#include <avr/io.h>

class Spi
{
	enum{SPI2X_shift = 2};
	public:

	enum ClockDivider
	{
		Div2	= 1 << (SPI2X + SPI2X_shift) | 0 << SPR1 | 0 << SPR0,
		Div4	= 0 << (SPI2X + SPI2X_shift) | 0 << SPR1 | 0 << SPR0,
		Div8	= 1 << (SPI2X + SPI2X_shift) | 0 << SPR1 | 1 << SPR0,
		Div16	= 0 << (SPI2X + SPI2X_shift) | 0 << SPR1 | 1 << SPR0,
		Div32	= 1 << (SPI2X + SPI2X_shift) | 1 << SPR1 | 0 << SPR0,
		Div64	= 0 << (SPI2X + SPI2X_shift) | 1 << SPR1 | 0 << SPR0,
		Div128	= 0 << (SPI2X + SPI2X_shift) | 1 << SPR1 | 1 << SPR0
	};

	enum{ClockDividerMask = 1 << SPR1 | 1 << SPR0};

	static void Init(ClockDivider divider)
	{
		if(divider & (1 << (SPI2X + SPI2X_shift)))
			SPSR |= 1 << SPI2X;
		else 
			SPSR &= ~(1 << SPI2X);
			
		SPCR = (SPCR & ~ClockDividerMask) | 1<<SPE | 1<<MSTR |
				(divider & ClockDividerMask);		
	}

	static uint8_t ReadWrite(uint8_t outValue)
	{
		SPDR = outValue;
		while(!(SPSR & (1<<SPIF)));
		return SPDR;
	}
};