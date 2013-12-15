#pragma once

#include "ioreg.h"
#include "stm32f4xx.h"
#include "clock.h"
#include <static_assert.h>
#include <enum.h>

namespace Mcucpp
{
	class DmaBase
	{
	public:
		enum Mode
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
	};
	
	DECLARE_ENUM_OPERATIONS(DmaBase::Mode)
	
	template<class Module, class ChannelRegs, int Channel>
	class DmaChannel :public DmaBase
	{
		STATIC_ASSERT(Channel < Module::Channels);
		public:
		using DmaBase::Mode;
		
		static void Transfer(Mode mode, const void *buffer, volatile void *periph, uint32_t bufferSize, uint8_t channel = 0)
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
			ChannelRegs()->CR = mode | DMA_SxCR_EN | ((channel & 0x07) << 25);
		}
		
		static bool Ready()
		{
			return !Enabled() || TrasferComplete();
		}
		
		static bool Enabled()
		{
			return ChannelRegs()->CR & DMA_SxCR_EN;
		}
		
		static void Disable()
		{
			ChannelRegs()->CCR &= DMA_SxCR_EN;
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
		
	};
	
	template<class DmaRegs, class Clock, int _Channels>
	class DmaModule :public DmaBase
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
		
		// Channel - DMA Channel number 1..7
		// NOTE: Channel starts with 1, regarding to ST documentation
		
		// template<int ChannelNum>
		// class Channel : public DmaChannel<DmaModule, TODO, ChannelNum>
		// {};
		
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
	
	typedef DmaModule<Private::Dma1, Clock::Dma1Clock, 8> Dma1;
	
	typedef DmaChannel<Dma1, Private::Dma1Channel0, 0> Dma1Channel0;
	typedef DmaChannel<Dma1, Private::Dma1Channel1, 1> Dma1Channel1;
	typedef DmaChannel<Dma1, Private::Dma1Channel2, 2> Dma1Channel2;
	typedef DmaChannel<Dma1, Private::Dma1Channel3, 3> Dma1Channel3;
	typedef DmaChannel<Dma1, Private::Dma1Channel4, 4> Dma1Channel4;
	typedef DmaChannel<Dma1, Private::Dma1Channel5, 5> Dma1Channel5;
	typedef DmaChannel<Dma1, Private::Dma1Channel6, 6> Dma1Channel6;
	typedef DmaChannel<Dma1, Private::Dma1Channel7, 7> Dma1Channel7;
	

	typedef DmaModule<Private::Dma2, Clock::Dma2Clock, 8> Dma2;
	
	typedef DmaChannel<Dma2, Private::Dma2Channel0, 0> Dma2Channel0;
	typedef DmaChannel<Dma2, Private::Dma2Channel1, 1> Dma2Channel1;
	typedef DmaChannel<Dma2, Private::Dma2Channel2, 2> Dma2Channel2;
	typedef DmaChannel<Dma2, Private::Dma2Channel3, 3> Dma2Channel3;
	typedef DmaChannel<Dma2, Private::Dma2Channel4, 4> Dma2Channel4;
	typedef DmaChannel<Dma2, Private::Dma2Channel5, 5> Dma2Channel5;
	typedef DmaChannel<Dma2, Private::Dma2Channel6, 6> Dma2Channel6;
	typedef DmaChannel<Dma2, Private::Dma2Channel7, 7> Dma2Channel7;
}
