#pragma once

#include <ioreg.h>
#include <stm32f10x.h>
#include <clock.h>
#include <iopins.h>
#include <pinlist.h>

namespace Mcucpp
{
	namespace Private
	{
		class SpiBase
		{
			public:
			
			enum ClockDivider
			{
				Div2	= 0,
				Div4	= SPI_CR1_BR_0,
				Div8	= SPI_CR1_BR_1,
				Div16	= SPI_CR1_BR_0 | SPI_CR1_BR_1,
				Div32	= SPI_CR1_BR_2,
				Div64	= SPI_CR1_BR_2 | SPI_CR1_BR_0,
				Div128	= SPI_CR1_BR_2 | SPI_CR1_BR_1,
				Div256	= SPI_CR1_BR_2 | SPI_CR1_BR_1 | SPI_CR1_BR_0,
				Fastest = Div2,
				Fast	= Div8,
				Medium	= Div32,
				Slow	= Div128,
				Slowest = Div256
			};
			
			enum SlaveControl
			{
				SoftSlaveControl	= SPI_CR1_SSM,
				AutoSlaveControl	= 0
			};
			
			enum Mode
			{
				Master				= SPI_CR1_MSTR | (SPI_CR2_SSOE << 16),
				MultiMaster			= SPI_CR1_MSTR,
				Slave				= 0
			};
			
			enum DataSize
			{
				DataSize4	= 0x0300,
				DataSize5	= 0x0400,
				DataSize6	= 0x0500,
				DataSize7	= 0x0600,
				DataSize8	= 0x0700,
				DataSize9	= 0x0800,
				DataSize10	= 0x0900,
				DataSize11	= 0x0A00,
				DataSize12	= 0x0B00,
				DataSize13	= 0x0C00,
				DataSize14	= 0x0D00,
				DataSize15	= 0x0E00,
				DataSize16	= 0x0F00
			};
			
			enum ClockPolarity
			{
				ClockPolarityLow	= 0,
				ClockPolarityHigh	= SPI_CR1_CPOL
			};
			
			enum ClockPhase
			{
				ClockPhase1Edge		= 0,
				ClockPhase2Edge		= SPI_CR1_CPHA
			};
			
			enum BitOrder
			{
				LsbFirst			= SPI_CR1_LSBFIRST,
				MsbFirst			= 0
			};
		};
		
		
		template<class Regs, class Clock, class MosiPins, class MisoPins, class ClockPins, class SsPins>
		class Spi :public SpiBase
		{
		public:
			
			static void Enable()
			{
				Regs()->CR1 |= SPI_CR1_SPE;
			}
			
			static void Disable()
			{
				Regs()->CR1 &= ~SPI_CR1_SPE;
			}
			
			static void Init(ClockDivider divider = Medium, Mode mode = Master)
			{
				Clock::Enable();
				Regs()->CR1 = 0;
				Regs()->CR2 = 0;
				Regs()->CR1 = (unsigned)divider | mode;
				Regs()->CR2 = (mode >> 16) | SPI_CR2_SSOE;
				SetDataSize(DataSize8);
				//SetSlaveControl(SoftSlaveControl);
				Regs()->I2SCFGR &= (uint16_t)~((uint16_t)SPI_I2SCFGR_I2SMOD);
				SelectPins<0, 0, 0, 0>();
				Enable();
			}
			
			static void SetDivider(ClockDivider divider)
			{
				Regs()->CR1 = (Regs()->CR1 & ~SPI_CR1_BR) | divider;
			}
			
			static void SetClockPolarity(ClockPolarity clockPolarity)
			{
				Regs()->CR1 = (Regs()->CR1 & ~SPI_CR1_CPOL) | clockPolarity;
			}
			
			static void SetClockPhase(ClockPhase clockPhase)
			{
				Regs()->CR1 = (Regs()->CR1 & ~SPI_CR1_CPHA) | clockPhase;
			}
			
			static void SetBitOrder(BitOrder bitOrder)
			{
				Regs()->CR1 = (Regs()->CR1 & ~SPI_CR1_LSBFIRST) | bitOrder;
			}
			
			static void SetDataSize(DataSize dataSize)
			{
				Regs()->CR2 = (Regs()->CR2 & ~SPI_CR2_DS) | dataSize;
			}
			
			static void SetSlaveControl(SlaveControl slaveControl)
			{
				Regs()->CR1 = (Regs()->CR1 & ~SPI_CR1_SSM) | slaveControl;
			}
			
			static void SetSS()
			{
				//SetSlaveControl(SoftSlaveControl);
				Regs()->CR1 |= SPI_CR1_SSI;
			}
			
			static void ClearSS()
			{
				//SetSlaveControl(SoftSlaveControl);
				Regs()->CR1 &= ~SPI_CR1_SSI;
			}
			
			static void Write(uint16_t outValue)
			{
				while((Regs()->SR & SPI_SR_TXE) == 0);
				if((Regs()->CR2 & SPI_CR2_DS) > DataSize8)
					Regs()->DR = outValue;
				else
					*((volatile uint8_t*)&Regs()->DR) = (uint8_t)outValue;
			}
			
			static uint16_t Read()
			{
				uint16_t timeout = 5000;
				while((Regs()->SR & SPI_SR_RXNE) == 0 && timeout--);
				if(timeout != 0)
					return Regs()->DR;
				return 0xff;
			}
			
			static uint16_t ReadWrite(uint16_t outValue)
			{
				Write(outValue);
				return Read();
			}
			
			template<int Mosi, int Miso, int Sck, int Nss>
			static void SelectPins()
			{
				typedef typename MosiPins:: template Pin<Mosi> MosiPin;
				MosiPin::Port::Enable();
				MosiPin::SetConfiguration(MosiPin::Port::AltFunc);
			
				
				typedef typename MisoPins:: template Pin<Mosi> MisoPin;
				MisoPin::Port::Enable();
				MisoPin::SetConfiguration(MisoPin::Port::AltFunc);
				
				typedef typename ClockPins:: template Pin<Mosi> ClockPin;
				ClockPin::Port::Enable();
				ClockPin::SetConfiguration(ClockPin::Port::AltFunc);
				
				typedef typename SsPins:: template Pin<Mosi> SsPin;
				SsPin::Port::Enable();
				SsPin::SetConfiguration(SsPin::Port::AltFunc);
			}

			static void SelectPins(int Mosi, int Miso, int Sck, int Nss)
			{
				typedef typename MosiPins::ValueType Type;
				
				if(Mosi >= 0)
				{
					Type maskMosi (1 << Mosi);
					MosiPins::SetConfiguration(maskMosi, MosiPins::AltFunc);
				}

				if(Miso >= 0)
				{
					Type maskMiso (1 << Miso);
					MisoPins::SetConfiguration(maskMiso, MisoPins::AltFunc);
				}
				
				if(Sck >= 0)
				{
					Type maskSck (1 << Sck);
					ClockPins::SetConfiguration(maskSck, ClockPins::AltFunc);
				}
				
				if(Nss >= 0)
				{
					Type maskNss (1 << Nss);
					SsPins::SetConfiguration(maskNss, SsPins::AltFunc);
				}
			}
		};
		
		typedef IO::PinList<IO::Pa4, IO::Pa15> Spi1SsPins;
		typedef IO::PinList<IO::Pa5, IO::Pb3> Spi1ClockPins;
		typedef IO::PinList<IO::Pa6, IO::Pb4> Spi1MisoPins;
		typedef IO::PinList<IO::Pa7, IO::Pb5> Spi1MosiPins;
		
		typedef IO::PinList<IO::Pb12> Spi2SsPins;
		typedef IO::PinList<IO::Pb13> Spi2ClockPins;
		typedef IO::PinList<IO::Pb14> Spi2MisoPins;
		typedef IO::PinList<IO::Pb15> Spi2MosiPins;
		
	//	typedef IO::PinList<IO::Pa4, IO::Pa15> Spi3SsPins;
	//	typedef IO::PinList<IO::Pa1, IO::Pb3, IO::Pc10> Spi3ClockPins;
	//	typedef IO::PinList<IO::Pa2, IO::Pb4, IO::Pc11> Spi3MisoPins;
	//	typedef IO::PinList<IO::Pa3, IO::Pb5, IO::Pc12> Spi3MosiPins;
		
		IO_STRUCT_WRAPPER(SPI1, SPI1_REGS, SPI_TypeDef);
		IO_STRUCT_WRAPPER(SPI2, SPI2_REGS, SPI_TypeDef);
//		IO_STRUCT_WRAPPER(SPI3, SPI3_REGS, SPI_TypeDef);
	}
	
	typedef Private::Spi<
		Private::SPI1_REGS, 
		Clock::Spi1Clock, 
		Private::Spi1MosiPins, 
		Private::Spi1MisoPins,
		Private::Spi1ClockPins,
		Private::Spi1SsPins> Spi1;

	typedef Private::Spi<
		Private::SPI2_REGS, 
		Clock::Spi2Clock, 
		Private::Spi2MosiPins, 
		Private::Spi2MisoPins,
		Private::Spi2ClockPins,
		Private::Spi2SsPins> Spi2;
		
	// typedef Private::Spi<
		// Private::SPI3_REGS, 
		// Clock::Spi3Clock, 
		// Private::Spi3MosiPins, 
		// Private::Spi3MisoPins,
		// Private::Spi3ClockPins,
		// Private::Spi3SsPins> Spi3;
}
