#pragma once

#include "ioreg.h"
#include "stm32f4xx.h"
#include "clock.h"
#include <static_assert.h>
#include <enum.h>
#include <data_transfer.h>

namespace Mcucpp
{
	enum class DmaMode
	{
		PriorityLow = 0,
		PriorityMedium = DMA_SxCR_PL_0,
		PriorityHigh = DMA_SxCR_PL_1,
		PriorityVeryHigh = DMA_SxCR_PL_1 | DMA_SxCR_PL_0,
		
		MSize8Bits = 0,
		MSize16Bits = DMA_SxCR_MSIZE_0,
		MSize32Bits = DMA_SxCR_MSIZE_1,
		
		PSize8Bits = 0,
		PSize16Bits = DMA_SxCR_PSIZE_0,
		PSize32Bits = DMA_SxCR_PSIZE_1,
		
		MemIncriment = DMA_SxCR_MINC,
		PeriphIncriment = DMA_SxCR_PINC,
		Circular = DMA_SxCR_CIRC,
		
		Periph2Mem = 0,
		Mem2Periph = DMA_SxCR_DIR_0,
		Mem2Mem = DMA_SxCR_DIR_1,
		
		TransferErrorInterrupt = DMA_SxCR_TEIE,
		HalfTransferInterrupt = DMA_SxCR_HTIE,
		TransferCompleteInterrupt = DMA_SxCR_TCIE
	};
	
	DECLARE_ENUM_OPERATIONS(DmaMode)
		
	struct DmaChannelData
	{
		DmaChannelData()
			:transferCallback(nullptr),
            data(nullptr),
            size(0)
		{}
		TransferCallbackFunc transferCallback;
		
		void *data;
		uint16_t size;
		
		inline void NotifyTransferComplete()
		{
			TransferCallbackFunc callback = transferCallback;
			if(callback)
			{
				callback(data, size, true);
			}
		}
		
		inline void NotifyError()
		{
			TransferCallbackFunc callback = transferCallback;
			//errorCallback = VoidDmaCallback;
			if(callback)
			{
				callback(data, size, false);
			}
		}
	};
	
	template<class Module, class ChannelRegs, int Channel, class RequestT, IRQn IQRNumber>
	class DmaChannel
	{
		STATIC_ASSERT(Channel < Module::Channels);
		static DmaChannelData ChannelData;
		
	public:
		
		typedef RequestT RequestType;
		
		static void Transfer(DmaMode mode, const void *buffer, volatile void *periph, uint16_t bufferSize)
		{
			Module::Enable();
			if(!TransferError())
			{
				while(!Ready())
					;
			}
			ClearFlags();
			ChannelRegs()->CR = 0;
			ChannelRegs()->NDTR = bufferSize;
			ChannelRegs()->PAR = reinterpret_cast<uint32_t>(periph);
			ChannelRegs()->M0AR = reinterpret_cast<uint32_t>(buffer);
			//ChannelRegs()->M1AR = reinterpret_cast<uint32_t>(buffer);
			//ChannelRegs()->FCR
			ChannelData.data = const_cast<void*>(buffer);
			ChannelData.size = bufferSize;
			if(ChannelData.transferCallback)
				mode = mode | DmaMode::TransferCompleteInterrupt | DmaMode::TransferErrorInterrupt;
			ChannelRegs()->CR = (ChannelRegs()->CR & DMA_SxCR_CHSEL) | (uint32_t)mode | DMA_SxCR_EN;
			NVIC_EnableIRQ(IQRNumber);
		}

		static void SetRequest(RequestType request)
		{
			Module::Enable();
			ChannelRegs()->CR = (ChannelRegs()->CR & ~DMA_SxCR_CHSEL) | (uint32_t)request << 25;
		}
		
		static void SetTransferCallback(TransferCallbackFunc callback)
		{
			ChannelData.transferCallback = callback;
		}
		
		static bool Ready()
		{
			return ChannelRegs()->NDTR == 0 || !Enabled() || TrasferComplete();
		}
		
		static bool Enabled()
		{
			return ChannelRegs()->CR & DMA_SxCR_EN;
		}
		
		static void Disable()
		{
			ChannelRegs()->CR &= ~DMA_SxCR_EN;
		}
		
		static uint32_t RemainingTransfers()
		{
			return ChannelRegs()->NDTR;
		}
		
		static void * PeriphAddress()
		{
			return reinterpret_cast<void *>(ChannelRegs()->PAR);
		}
		
		static void * MemAddress()
		{
			return reinterpret_cast<void *>(ChannelRegs()->M0AR);
		}
		
		static bool TransferError()
		{
			return Module::template TransferError<Channel>();
		}
		
		static bool HalfTrasfer()
		{
			return Module::template HalfTrasfer<Channel>();
		}
		
		static bool TrasferComplete()
		{
			return Module::template TrasferComplete<Channel>();
		}
		
		static void ClearFlags()
		{
			Module::template ClearChannelFlags<Channel>();
		}
		
		static void ClearTransferError()
		{
			Module::template ClearTransferError<Channel>();
		}
		
		static void ClearHalfTrasfer()
		{
			Module::template ClearHalfTrasfer<Channel>();
		}
		
		static void ClearTrasferComplete()
		{
			Module::template ClearTrasferComplete<Channel>();
		}
		
		static void IrqHandler()
		{
			if(TrasferComplete())
			{
				ChannelData.NotifyTransferComplete();
			}
			if(TransferError())
			{
				ChannelData.NotifyError();
			}
			ClearFlags();
		}
	};
	
	enum class NoDmaChannelRequest
	{
		NoRequest = -1
	};

	class NoDmaChannel
	{		
	public:
		
		typedef NoDmaChannelRequest RequestType;
		
		static void Transfer(DmaMode mode, const void *buffer, volatile void *periph, uint16_t bufferSize);
		static void SetRequest(RequestType request);
		static void SetTransferCallback(TransferCallbackFunc callback);
		static bool Ready();
		static bool Enabled();
		static void Disable();
		static uint32_t RemainingTransfers();
		static void * PeriphAddress();
		static void * MemAddress();
		static bool TransferError();
		static bool HalfTrasfer();
		static bool TrasferComplete();
		static void ClearFlags();		
		static void ClearTransferError();		
		static void ClearHalfTrasfer();
		static void ClearTrasferComplete();
	};

	template<class DmaRegs, class Clock, int _Channels>
	class DmaModule
	{
		STATIC_ASSERT(_Channels <= 8);
		
		template<int ChannelNum, int Flag>
		static bool ChannelFlag()
		{
			switch(ChannelNum)
			{
				case 0: return (DmaRegs()->LISR & (Flag << 0 )) != 0;
				case 1: return (DmaRegs()->LISR & (Flag << 6 )) != 0;
				case 2: return (DmaRegs()->LISR & (Flag << 16)) != 0;
				case 3: return (DmaRegs()->LISR & (Flag << 22)) != 0;
				case 4: return (DmaRegs()->HISR & (Flag << 0 )) != 0;
				case 5: return (DmaRegs()->HISR & (Flag << 6 )) != 0;
				case 6: return (DmaRegs()->HISR & (Flag << 16)) != 0;
				case 7: return (DmaRegs()->HISR & (Flag << 22)) != 0;
			}
			return false;
		}
		
		template<int ChannelNum, int Flag>
		static void ClearChannelFlag()
		{
			switch(ChannelNum)
			{
				case 0: DmaRegs()->LIFCR |= (Flag << 0 ); break;
				case 1: DmaRegs()->LIFCR |= (Flag << 6 ); break;
				case 2: DmaRegs()->LIFCR |= (Flag << 16); break;
				case 3: DmaRegs()->LIFCR |= (Flag << 22); break;
				case 4: DmaRegs()->HIFCR |= (Flag << 0 ); break;
				case 5: DmaRegs()->HIFCR |= (Flag << 6 ); break;
				case 6: DmaRegs()->HIFCR |= (Flag << 16); break;
				case 7: DmaRegs()->HIFCR |= (Flag << 22); break;
			}
		}
		
	public:
		static const int Channels = _Channels;
				
		template<int ChannelNum>
		static bool TransferError()
		{
			return ChannelFlag<ChannelNum, (1 << 3)>();
		}
		
		template<int ChannelNum>
		static bool HalfTrasfer()
		{
			return ChannelFlag<ChannelNum, (1 << 4)>();
		}
		
		template<int ChannelNum>
		static bool TrasferComplete()
		{
			return ChannelFlag<ChannelNum, (1 << 5)>();
		}
		
		template<int ChannelNum>
		static bool FifoError()
		{
			return ChannelFlag<ChannelNum, (1 << 0)>();
		}
		
		template<int ChannelNum>
		static bool DirectError()
		{
			return ChannelFlag<ChannelNum, (1 << 2)>();
		}
		
		template<int ChannelNum>
		static void ClearChannelFlags()
		{
			STATIC_ASSERT(ChannelNum >= 0 && ChannelNum < Channels);
			ClearChannelFlag<ChannelNum, 0x3d>();
		}
		
		template<int ChannelNum>
		static void ClearTransferError()
		{
			ClearChannelFlag<ChannelNum, (1 << 3)>();
		}
		
		template<int ChannelNum>
		static void ClearHalfTrasfer()
		{
			ClearChannelFlag<ChannelNum, (1 << 4)>();
		}
		
		template<int ChannelNum>
		static void ClearTrasferComplete()
		{
			ClearChannelFlag<ChannelNum, (1 << 5)>();
		}
		
		template<int ChannelNum>
		static void ClearFifoError()
		{
			ClearChannelFlag<ChannelNum, (1 << 0)>();
		}
		
		template<int ChannelNum>
		static void ClearDirectError()
		{
			ClearChannelFlag<ChannelNum, (1 << 2)>();
		}
		
		static void Enable()
		{
			Clock::Enable();
		}
		
		static void Disable()
		{
			Clock::Disable();
		}
	};
	
	enum class Dma1Channel0Request
	{
		Spi3_Rx = 0,
		I2c1_Rx = 1,
		Tim4_Ch1 = 2,
		I2s3_Ext_Rx = 3,
		Uart5_Rx = 4,
		Uart8_Tx = 5,
		Tim5_Ch3 = 6,
		Tim5_Up = 6,
	};

	enum class Dma1Channel1Request
	{
		I2c1_Tx = 0,
		I2c3_Rx = 1,
		Tim2_Up = 3,
		Tim2_Ch3 = 3,
		Usart3_Rx = 4,
		Uart7_Tx = 5,
		Tim5_Ch4 = 6,
		Tim5_Trig = 6,
		Tim6_Up = 7,
	};

	enum class Dma1Channel2Request
	{
		Spi3_Rx = 0,
		Tim7_Up = 1,
		I2s3_Ext_Rx = 2,
		I2c3_Rx = 3,
		Uart4_Rx = 4,
		Tim3_Ch4 = 5,
		Tim3_Up = 5,
		Tim5_Ch1 = 6,
		I2c2_Rx = 7,
	};

	enum class Dma1Channel3Request
	{
		Spi2_Rx = 0,
		// = 1,
		Tim4_Ch2 = 2,
		I2s2_Ext_Rx = 3,
		Usart3_Tx = 4,
		Uart7_Rx = 5,
		Tim5_Ch4 = 6,
		Tim5_Trig= 6,
		I2c2_Rx = 7,
	};


	enum class Dma1Channel4Request
	{
		Spi2_Tx = 0,
		Tim7_Up = 1,
		I2s2_Ext_Tx = 2,
		I2c3_Tx = 3,
		Uart4_Tx = 4,
		Tim3_Ch1 = 5,
		Tim3_Trig = 5, 
		Tim5_Ch2 = 6,
		Usart3_Tx = 7,
	};

	enum class Dma1Channel5Request
	{
		Spi3_Tx = 0,
		I2c1_Rx = 1,
		I2s3_Ext_Tx = 2,
		Tim2_Ch1 = 3,
		Usart2_Rx = 4,
		Tim3_Ch2 = 5,
		I2c3_Tx = 6,
		Dac1 = 7,
	};

	enum class Dma1Channel6Request
	{
		I2c1_Tx = 1,
		Tim4_Up = 2,
		Tim2_Ch2 = 3,
		Tim2_Ch4 = 3,
		Usart2_Tx = 4,
		Uart8_Rx = 5,
		Tim5_Up = 6,
		Dac2 = 7,
	};

	enum class Dma1Channel7Request
	{
		Spi3_Tx = 0,
		I2c1_Tx = 1,
		Tim4_Ch3 = 2,
		Tim2_Up = 3,
		Tim2_Ch4 = 3,
		Uart5_Tx = 4,
		Tim3_Ch3 = 5,
		Usart2_Rx = 6,
		I2c2_Tx = 7,
	};

	enum class Dma2Channel0Request
	{
		Adc1 = 0,
		Adc3 = 2,
		Spi1_Rx = 3,
		Spi4_Rx = 4,
		Tim1_Trig = 6,
	};

	enum class Dma2Channel1Request
	{
		Sai1_A = 0,
		Dcmi = 1,
		Adc3 = 2,
		Spi4_Tx = 4,
		Usart6_Rx = 5,
		Tim1_Ch1 = 6,
		Tim8_Up = 7,
	};

	enum class Dma2Channel2Request
	{
		// Tim8_Ch1 = 0,
		Tim8_Ch2 = 0,
		Tim8_Ch3 = 0,
		Adc2 = 1,
		Spi1_Tx = 2,
		Spi1_Rx = 3,
		Usart1_Rx = 4,
		Usart6_Rx = 5,
		Tim1_Ch2 = 6,
		Tim8_Ch1 = 7,
	};

	enum class Dma2Channel3Request
	{
		Sai1_A = 0,
		Adc2 = 1,
		Spi5_Rx = 2,
		Spi1_Tx = 3, 
		Sdio = 4,
		Spi4_Rx = 5,
		Tim1_Ch1 = 6,
		Tim8_Ch2 = 7,
	};

	enum class Dma2Channel4Request
	{
		Adc1 = 0,
		Sai1_B = 1,
		Spi5_Tx = 2,
		Spi4_Rx = 4,
		Spi4_Tx = 5,
		Tim1_Ch4 = 6,
		Tim1_Trig = 6,
		Tim1_Com = 6,
		Tim8_Ch3 = 7,
	};

	enum class Dma2Channel5Request
	{
		Sai1_B = 0,
		Spi6_Tx = 1,
		Cryp_Out = 2,
		Spi1_Tx = 3,
		Usart1_Rx = 4,
		Spi5_Tx = 5,
		Tim1_Up = 6,
		Spi5_Rx = 7,
	};

	enum class Dma2Channel6Request
	{
		Tim1_Ch1 = 0,
		Tim1_Ch2 = 0,
		// Tim1_Ch3 = 0,
		Spi6_Rx = 1,
		Cryp_In = 2,
		Sdio = 4,
		Usart6_Tx = 5,
		Tim1_Ch3 = 6,
		Spi5_Tx = 7,
	};

	enum class Dma2Channel7Request
	{
		Dcmi = 1,
		Hash_In = 2,
		Usart1_Tx = 4,
		Usart6_Tx = 5,
		Tim8_Ch4 = 7,
		Tim8_Trig = 7,
		Tim8_Com = 7,
	};

	
	namespace Private
	{
		IO_STRUCT_WRAPPER(DMA1, Dma1, DMA_TypeDef);
		IO_STRUCT_WRAPPER(DMA2, Dma2, DMA_TypeDef);
		
		IO_STRUCT_WRAPPER(DMA1_Stream0, Dma1Channel0, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Stream1, Dma1Channel1, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Stream2, Dma1Channel2, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Stream3, Dma1Channel3, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Stream4, Dma1Channel4, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Stream5, Dma1Channel5, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Stream6, Dma1Channel6, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Stream7, Dma1Channel7, DMA_Stream_TypeDef);
		
		IO_STRUCT_WRAPPER(DMA2_Stream0, Dma2Channel0, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Stream1, Dma2Channel1, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Stream2, Dma2Channel2, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Stream3, Dma2Channel3, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Stream4, Dma2Channel4, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Stream5, Dma2Channel5, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Stream6, Dma2Channel6, DMA_Stream_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Stream7, Dma2Channel7, DMA_Stream_TypeDef);
	}
	
	template<class Module, class ChannelRegs, int Channel, class RequestT, IRQn IRQnumber>
	DmaChannelData DmaChannel<Module, ChannelRegs, Channel, RequestT, IRQnumber>::ChannelData;
	
	typedef DmaModule<Private::Dma1, Clock::Dma1Clock, 8> Dma1;
	
	typedef DmaChannel<Dma1, Private::Dma1Channel0, 0, Dma1Channel0Request, DMA1_Stream0_IRQn> Dma1Channel0;
	typedef DmaChannel<Dma1, Private::Dma1Channel1, 1, Dma1Channel1Request, DMA1_Stream1_IRQn> Dma1Channel1;
	typedef DmaChannel<Dma1, Private::Dma1Channel2, 2, Dma1Channel2Request, DMA1_Stream2_IRQn> Dma1Channel2;
	typedef DmaChannel<Dma1, Private::Dma1Channel3, 3, Dma1Channel3Request, DMA1_Stream3_IRQn> Dma1Channel3;
	typedef DmaChannel<Dma1, Private::Dma1Channel4, 4, Dma1Channel4Request, DMA1_Stream4_IRQn> Dma1Channel4;
	typedef DmaChannel<Dma1, Private::Dma1Channel5, 5, Dma1Channel5Request, DMA1_Stream5_IRQn> Dma1Channel5;
	typedef DmaChannel<Dma1, Private::Dma1Channel6, 6, Dma1Channel6Request, DMA1_Stream6_IRQn> Dma1Channel6;
	typedef DmaChannel<Dma1, Private::Dma1Channel7, 7, Dma1Channel7Request, DMA1_Stream7_IRQn> Dma1Channel7;
	

	typedef DmaModule<Private::Dma2, Clock::Dma2Clock, 8> Dma2;
	
	typedef DmaChannel<Dma2, Private::Dma2Channel0, 0, Dma2Channel0Request, DMA2_Stream0_IRQn> Dma2Channel0;
	typedef DmaChannel<Dma2, Private::Dma2Channel1, 1, Dma2Channel1Request, DMA2_Stream1_IRQn> Dma2Channel1;
	typedef DmaChannel<Dma2, Private::Dma2Channel2, 2, Dma2Channel2Request, DMA2_Stream2_IRQn> Dma2Channel2;
	typedef DmaChannel<Dma2, Private::Dma2Channel3, 3, Dma2Channel3Request, DMA2_Stream3_IRQn> Dma2Channel3;
	typedef DmaChannel<Dma2, Private::Dma2Channel4, 4, Dma2Channel4Request, DMA2_Stream4_IRQn> Dma2Channel4;
	typedef DmaChannel<Dma2, Private::Dma2Channel5, 5, Dma2Channel5Request, DMA2_Stream5_IRQn> Dma2Channel5;
	typedef DmaChannel<Dma2, Private::Dma2Channel6, 6, Dma2Channel6Request, DMA2_Stream6_IRQn> Dma2Channel6;
	typedef DmaChannel<Dma2, Private::Dma2Channel7, 7, Dma2Channel7Request, DMA2_Stream7_IRQn> Dma2Channel7;
}
