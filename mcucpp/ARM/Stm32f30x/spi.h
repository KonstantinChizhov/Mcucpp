#pragma once

#include <ioreg.h>
#include <stm32f30x.h>
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
				Master				= SPI_CR1_MSTR | SPI_CR2_SSOE << 16,
				MultiMaster			= SPI_CR1_MSTR,
				Slave				= 0
			};
			
			enum DataSize
			{
				DataSize4	= 0x3 << 8,
				DataSize5	= 0x4 << 8,
				DataSize6	= 0x5 << 8,
				DataSize7	= 0x6 << 8,
				DataSize8	= 0x7 << 8,
				DataSize9	= 0x8 << 8,
				DataSize10	= 0x9 << 8,
				DataSize11	= 0xA << 8,
				DataSize12	= 0xB << 8,
				DataSize13	= 0xC << 8,
				DataSize14	= 0xD << 8,
				DataSize15	= 0xE << 8,
				DataSize16	= 0xF << 8
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
		
		
		template<class Regs, class Clock, class MosiPins, class MisoPins, class ClockPins, class SsPins, int AlfFunc>
		class Spi :public SpiBase
		{
		public:
			
			static void Enable()
			{
				Clock::Enable();
			}
			
			static void Disable()
			{
				Clock::Disable();
			}
			
			static void Init(ClockDivider divider = Medium, Mode mode = Master)
			{
				Enable();
				Regs()->CR1 = 0;
				Regs()->CR2 = 0;
				Regs()->CR1 = (unsigned)divider | SPI_CR1_SPE | mode;
				Regs()->CR2 = mode >> 16;
				SetDataSize(DataSize8);
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
				SetSlaveControl(SoftSlaveControl);
				Regs()->CR1 |= SPI_CR1_SSI;
			}
			
			static void ClearSS()
			{
				SetSlaveControl(SoftSlaveControl);
				Regs()->CR1 &= ~SPI_CR1_SSI;
			}
			
			static void Write(uint16_t outValue)
			{
				while((Regs()->SR & SPI_SR_TXE) == 0);
				Regs()->DR = outValue;
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
			
			template<uint8_t Mosi, uint8_t Miso, uint8_t Sck, uint8_t Nss>
			static void SelectPins()
			{
				typedef typename MosiPins:: template Pin<Mosi> MosiPin;
				MosiPin::Port::Enable();
				MosiPin::SetConfiguration(MosiPin::Port::AltFunc);
				if(AlfFunc != -1)
					MosiPin::AltFuncNumber(AlfFunc);
				else MosiPin::AltFuncNumber(5);
				
				typedef typename MisoPins:: template Pin<Mosi> MisoPin;
				MisoPin::Port::Enable();
				MisoPin::SetConfiguration(MisoPin::Port::AltFunc);
				if(AlfFunc != -1)
					MisoPin::AltFuncNumber(AlfFunc);
				else if(MisoPin::Number == 13 )
					MisoPin::AltFuncNumber(6);
				else MisoPin::AltFuncNumber(5);
				
				typedef typename ClockPins:: template Pin<Mosi> ClockPin;
				ClockPin::Port::Enable();
				ClockPin::SetConfiguration(ClockPin::Port::AltFunc);
				if(AlfFunc != -1)
					ClockPin::AltFuncNumber(AlfFunc);
				else if(ClockPin::Number == 12 )
					ClockPin::AltFuncNumber(6);
				else ClockPin::AltFuncNumber(5);
				
				typedef typename SsPins:: template Pin<Mosi> SsPin;
				SsPin::Port::Enable();
				SsPin::SetConfiguration(SsPin::Port::AltFunc);
				if(AlfFunc != -1)
					SsPin::AltFuncNumber(AlfFunc);
				else if(SsPin::Number == 11 )
					SsPin::AltFuncNumber(6);
				else SsPin::AltFuncNumber(5);
			}

			static void SelectPins(uint8_t Mosi, uint8_t Miso, uint8_t Sck, uint8_t Nss)
			{
				typedef typename MosiPins::ValueType Type;
				
				Type maskMosi (1 << Mosi);
				MosiPins::SetConfiguration(maskMosi, MosiPins::AltFunc);
				if(AlfFunc != -1)
					MosiPins::AltFuncNumber(maskMosi, AlfFunc);
				else MosiPins::AltFuncNumber(maskMosi, 5);

				Type maskMiso (1 << Miso);
				MisoPins::SetConfiguration(maskMiso, MisoPins::AltFunc);
				if(AlfFunc != -1)
					MisoPins::AltFuncNumber(maskMiso, AlfFunc);
				else if(Miso == 1 )
					MisoPins::AltFuncNumber(maskMiso, 6);
				else MisoPins::AltFuncNumber(maskMiso, 5);
				
				Type maskSck (1 << Sck);
				ClockPins::SetConfiguration(maskSck, ClockPins::AltFunc);
				if(AlfFunc != -1)
					ClockPins::AltFuncNumber(maskSck, AlfFunc);
				else if(Sck == 1 )
					ClockPins::AltFuncNumber(maskSck, 6);
				else ClockPins::AltFuncNumber(maskSck, 5);
				
				Type maskNss (1 << Nss);
				SsPins::SetConfiguration(maskNss, SsPins::AltFunc);
				if(AlfFunc != -1)
					SsPins::AltFuncNumber(maskNss, AlfFunc);
				else if(Nss == 1 )
					SsPins::AltFuncNumber(maskNss, 6);
				else SsPins::AltFuncNumber(maskNss, 5);
			}
			/*
			class CrcUnit
			{
				uint16_t Init = 0;
				
				static void Enable(uint16_t  polynom = 0x0007)
				{
					Regs()->CRCPR = polynom;
					Regs()->CR1 |= (SPI_CR1_CRCEN);
				}
				
				void ResetRx()
				{
					_rxCrc = Init;
				}
				
				void ResetTx()
				{
					_txCrc = Init;
				}
				
				CrcType GetRx()
				{
					return Regs()->RXCRCR;
				}
				
				CrcType GetTx()
				{
					return Regs()->TXCRCR;
				}
				
				bool ReadMsbAndValidate()
				{
					CrcType expectedCrc = 0;
					for(size_t i = 0; i < sizeof(CrcType); i++)
					{
						expectedCrc |= (uint8_t)_source.Read();
						expectedCrc <<= 8;
					}
					
					return expectedCrc != _rxCrc;
				}
				
				bool ReadLsbAndValidate()
				{
					CrcType expectedCrc = 0;
					for(size_t i = 0; i < sizeof(CrcType); i++)
					{
						expectedCrc |= (CrcType)_source.Read() << ((sizeof(CrcType) - 1) * 8);
						expectedCrc >>= 8;
					}	
					return expectedCrc != _rxCrc;
				}
				
				// Send accumulated CRC to communication device LSB
				void SendLsb()
				{
					CrcType crc = _txCrc;
					for(size_t i = 0; i < sizeof(CrcType); i++)
					{
						_source.Write((uint8_t)crc);
						crc >>= 8;
					}
				}
				
				// Send accumulated CRC to communication device MSB
				void SendMsb()
				{
					CrcType crc = _txCrc;
					for(size_t i = 0; i < sizeof(CrcType); i++)
					{
						_source.Write((uint8_t)(crc >> ((sizeof(CrcType) - 1) * 8)));
						crc <<= 8;
					}
				}
			}Crc;*/
		};
		
		typedef IO::PinList<IO::Pa4, IO::Pa15> Spi1SsPins;
		typedef IO::PinList<IO::Pa5, IO::Pb3> Spi1ClockPins;
		typedef IO::PinList<IO::Pa6, IO::Pb4> Spi1MisoPins;
		typedef IO::PinList<IO::Pa7, IO::Pb5> Spi1MosiPins;
		
		/* typedef IO::PinList<IO::Pa11, IO::Pb9,  IO::Pd6> Spi2SsPins;
		typedef IO::PinList<IO::Pa8,  IO::Pb8,  IO::Pb10, IO::Pd7, IO::Pd8> Spi2ClockPins;
		typedef IO::PinList<IO::Pa9,  IO::Pb14, IO::Pc2,  IO::Pd3> Spi2MisoPins;
		typedef IO::PinList<IO::Pa10, IO::Pb15, IO::Pc3,  IO::Pd4> Spi2MosiPins;
		
		typedef IO::PinList<IO::Pa4, IO::Pa15> Spi3SsPins;
		typedef IO::PinList<IO::Pa1, IO::Pb3, IO::Pc10> Spi3ClockPins;
		typedef IO::PinList<IO::Pa2, IO::Pb4, IO::Pc11> Spi3MisoPins;
		typedef IO::PinList<IO::Pa3, IO::Pb5, IO::Pc12> Spi3MosiPins; */
		
		IO_STRUCT_WRAPPER(SPI1, SPI1_REGS, SPI_TypeDef);
		IO_STRUCT_WRAPPER(SPI2, SPI2_REGS, SPI_TypeDef);
		IO_STRUCT_WRAPPER(SPI3, SPI3_REGS, SPI_TypeDef);
	}
	
	typedef Private::Spi<
		Private::SPI1_REGS, 
		Clock::Spi1Clock, 
		Private::Spi1MosiPins, 
		Private::Spi1MisoPins,
		Private::Spi1ClockPins,
		Private::Spi1SsPins, 0> Spi1;

	/* typedef Private::Spi<
		Private::SPI2_REGS, 
		Clock::Spi2Clock, 
		Private::Spi2MosiPins, 
		Private::Spi2MisoPins,
		Private::Spi2ClockPins,
		Private::Spi2SsPins, 5> Spi2;
		
	typedef Private::Spi<
		Private::SPI3_REGS, 
		Clock::Spi3Clock, 
		Private::Spi3MosiPins, 
		Private::Spi3MisoPins,
		Private::Spi3ClockPins,
		Private::Spi3SsPins, 6> Spi3; */
}
