#pragma once


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
				Div4	= 1,
				Div8	= 2,
				Div16	= 3,
				Div32	= 4,
				Div64	= 5,
				Div128	= 6,
				Div256	= 7,
				Fastest = Div2,
				Fast	= Div8,
				Medium	= Div32,
				Slow	= Div128,
				Slowest = Div256
			};
			
			enum SlaveControl
			{
				SoftSlaveControl	= 1,
				AutoSlaveControl	= 0
			};
			
			enum Mode
			{
				Master				= 3,
				MultiMaster			= 1,
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
				ClockPolarityHigh	= 1
			};
			
			enum ClockPhase
			{
				ClockPhase1Edge		= 0,
				ClockPhase2Edge		= 1
			};
			
			enum BitOrder
			{
				LsbFirst			= 1,
				MsbFirst			= 0
			};
		};
		
		
		class Spi :public SpiBase
		{
		public:
			
			static void Enable()
			{
			
			}
			
			static void Disable()
			{
				
			}
			
			static void Init(ClockDivider divider = Medium, Mode mode = Master)
			{
				
			}
			
			static void SetDivider(ClockDivider divider)
			{
				
			}
			
			static void SetClockPolarity(ClockPolarity clockPolarity)
			{
				
			}
			
			static void SetClockPhase(ClockPhase clockPhase)
			{
				
			}
			
			static void SetBitOrder(BitOrder bitOrder)
			{
				
			}
			
			static void SetDataSize(DataSize dataSize)
			{
				
			}
			
			static void SetSlaveControl(SlaveControl slaveControl)
			{
				
			}
			
			static void SetSS()
			{
				
			}
			
			static void ClearSS()
			{
				
			}
			
			static void Write(uint16_t outValue)
			{
				
			}
			
			static uint16_t Read()
			{
				return 0xff;
			}
			
			static uint16_t ReadWrite(uint16_t outValue)
			{
				return Read();
			}
			
			template<int Mosi, int Miso, int Sck, int Nss>
			static void SelectPins()
			{
				
			}

			static void SelectPins(int Mosi, int Miso, int Sck, int Nss)
			{
				
			}

		};
	}
	
	typedef Private::Spi Spi1;
	typedef Private::Spi Spi2;
	typedef Private::Spi Spi3;
}

