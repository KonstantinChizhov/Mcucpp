#pragma once

#include "ioreg.h"
#include "stm32f10x.h"
#include "clock.h"

namespace Mcucpp
{
	namespace Private
	{
		class SpiBase
		{
			static const uint16_t SPI_Mode_Select = 0xF7FF;
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
				Slowest = Div256,
			};
			
			enum ModeFlags
			{
				SoftSlaveControl	= SPI_CR1_SSM,
				HardSlaveControl	= 0,
				Master				= SPI_CR1_MSTR | SPI_CR1_SSI,
				Slave				= 0,
				DataSize16			= SPI_CR1_DFF,
				DataSize8			= 0,
				ClockPolarityLow	= 0,
				ClockPolarityHigh	= SPI_CR1_CPOL,
				ClockPhase1Edge		= 0,
				ClockPhase2Edge		= SPI_CR1_CPHA,
				LsbFirst			= SPI_CR1_LSBFIRST,
				MsbFirst			= 0
			};
		};
		
		inline SpiBase::ModeFlags operator|(SpiBase::ModeFlags left, SpiBase::ModeFlags right)
		{
			return static_cast<SpiBase::ModeFlags>(static_cast<int>(left) | static_cast<int>(right));
		}
		
		template<class Cr1, class Cr2, class Sr, class Dr, class Crcpr, class RxCrcr, class TxCrcr, class I2SCfgr, class I2Spr, class ClkEnReg, unsigned ClkEnMask>
		class Spi :public SpiBase
		{
			public:
			
			static void Enable()
			{
				ClkEnReg::Or(ClkEnMask);
			}
			
			static void Disable()
			{
				ClkEnReg::And(~ClkEnMask);
			}
			
			static void Init(ClockDivider divider, ModeFlags mode = Master | SoftSlaveControl)
			{
				Enable();
				Cr1::Set((unsigned)divider | SPI_CR1_SPE | mode);
				Cr2::Or(SPI_CR2_SSOE);
				I2SCfgr::And(SPI_Mode_Select);
			}
			
			static void Write(uint8_t outValue)
			{
				while((Sr::Get() & SPI_SR_TXE) == 0);
				Dr::Set(outValue);
			}
			
			static uint8_t Read()
			{
				uint16_t timeout = 5000;
				while((Sr::Get() & SPI_SR_RXNE) == 0 && timeout--);
				if(timeout != 0)
					return Dr::Get();
				return 0xff;
			}
			
			static uint8_t ReadWrite(uint8_t outValue)
			{
				Write(outValue);
				return Read();
			}
			
			static void EnableSoftSSControl()
			{
				Cr1::Or(SPI_CR1_SSM);
			}
			
			static void SetSS()
			{
				Cr1::Or(SPI_CR1_SSI);
			}
			
			static void ClearSS()
			{
				Cr1::And(~SPI_CR1_SSI);
			}
			
			class Crc
			{
				static void Enable(uint16_t  polynom = 0x0007)
				{
					Crcpr::Set(polynom);
					Cr1::Or(SPI_CR1_CRCEN);
				}
				
				static void Transmit()
				{
					Cr1::Or(SPI_CR1_CRCNEXT);
				}
				
				static uint16_t RxCrc()
				{
					return RxCrcr::Get();
				}
				
				static uint16_t TxCrc()
				{
					return TxCrcr::Get();
				}
			};
		};
		
		
	}

#define DECLARE_SPI(CR1, CR2, SR, DR, CRCPR, RXCRCR, TXCRCR, I2SCFGR, I2SPR, CLK_EN_REG, CLK_EN_MASK, className) \
   namespace Private{\
		IO_REG_WRAPPER(CR1, className ## Cr1, uint32_t);\
		IO_REG_WRAPPER(CR2, className ## Cr2, uint32_t);\
		IO_REG_WRAPPER(SR, className ## Sr, uint32_t);\
		IO_REG_WRAPPER(DR, className ## Dr, uint32_t);\
		IO_REG_WRAPPER(CRCPR, className ## Crcpr, uint32_t);\
		IO_REG_WRAPPER(RXCRCR, className ## RxCrcr, uint32_t);\
		IO_REG_WRAPPER(TXCRCR, className ## TxCrcr, uint32_t);\
		IO_REG_WRAPPER(I2SCFGR, className ## I2SCfgr, uint32_t);\
		IO_REG_WRAPPER(I2SPR, className ## I2Spr, uint32_t);\
	}\
	  typedef Private::Spi<\
			Private::className ## Cr1, \
			Private::className ## Cr2, \
			Private::className ## Sr, \
			Private::className ## Dr, \
			Private::className ## Crcpr, \
			Private::className ## RxCrcr, \
			Private::className ## TxCrcr, \
			Private::className ## I2SCfgr, \
			Private::className ## I2Spr, \
			CLK_EN_REG, CLK_EN_MASK\
			> className; 
			
#ifdef USE_SPI1
	DECLARE_SPI(SPI1->CR1, SPI1->CR2, SPI1->SR, SPI1->DR, SPI1->CRCPR, SPI1->RXCRCR, SPI1->TXCRCR, SPI1->I2SCFGR,  SPI1->I2SPR, Clock::PeriphClockEnable2, RCC_APB2ENR_SPI1EN, Spi1)
#endif

#ifdef USE_SPI2
	DECLARE_SPI(SPI2->CR1, SPI2->CR2, SPI2->SR, SPI2->DR, SPI2->CRCPR, SPI2->RXCRCR, SPI2->TXCRCR, SPI2->I2SCFGR,  SPI2->I2SPR, Clock::PeriphClockEnable1, RCC_APB1ENR_SPI2EN, Spi2)
#endif

#ifdef USE_SPI3
	DECLARE_SPI(SPI3->CR1, SPI3->CR2, SPI3->SR, SPI3->DR, SPI3->CRCPR, SPI3->RXCRCR, SPI3->TXCRCR, SPI3->I2SCFGR,  SPI3->I2SPR, Clock::PeriphClockEnable1, RCC_APB1ENR_SPI3EN, Spi3)
#endif
}