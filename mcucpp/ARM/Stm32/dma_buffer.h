#pragma once

#include "ioreg.h"
#include "stm32f10x.h"
#include "clock.h"
#include <static_assert.h>
#include <enum.h>
#include <ring_buffer.h>

namespace Mcucpp
{
namespace IO
{
	class DmaBase
	{
	public:
		enum Mode
		{
			Mem2Mem = DMA_CCR1_MEM2MEM,
			
			PriorityLow = 0,
			PriorityMedium = DMA_CCR1_PL_0,
			PriorityHigh = DMA_CCR1_PL_1,
			PriorityVeryHigh = DMA_CCR1_PL_1 | DMA_CCR1_PL_0,
			
			MSize8Bits = 0,
			MSize16Bits = DMA_CCR1_MSIZE_0,
			MSize32Bits = DMA_CCR1_MSIZE_1,
			
			PSize8Bits = 0,
			PSize16Bits = DMA_CCR1_PSIZE_0,
			PSize32Bits = DMA_CCR1_PSIZE_1,
			
			MemIncriment = DMA_CCR1_MINC,
			PeriphIncriment = DMA_CCR1_PINC,
			Circular = DMA_CCR1_CIRC,
			
			Periph2Mem = 0,
			Mem2Periph = DMA_CCR1_DIR,
			
			TransferErrorInterrupt = DMA_CCR1_TEIE,
			HalfTransferInterrupt = DMA_CCR1_HTIE,
			TransferCompleteInterrupt = DMA_CCR1_TCIE
		};
	};
	
	DECLARE_ENUM_OPERATIONS(DmaBase::Mode)	
	
	template<class Module, class ChannelRegs, int Channel>
	class DmaChannel :public DmaBase
	{
		BOOST_STATIC_ASSERT(Channel < Module::Channels);
		public:
		using DmaBase::Mode;
		
		static void Init(Mode mode, void *buffer, void *periph, uint32_t bufferSize)
		{
			Module::Enable();
			ChannelRegs()->CCR = 0;
			ChannelRegs()->CNDTR = bufferSize;
			ChannelRegs()->CPAR = reinterpret_cast<uint32_t>(periph);
			ChannelRegs()->CMAR = reinterpret_cast<uint32_t>(buffer);
			ChannelRegs()->CCR = mode | DMA_CCR1_EN;
		}
		
		static void Disable()
		{
			ChannelRegs()->CCR &= DMA_CCR1_EN;
		}
		
		static uint32_t RemainingTransfers()
		{
			return ChannelRegs()->CNDTR;
		}
		
		static void * PeriphAddress()
		{
			return reinterpret_cast<void *>(ChannelRegs()->CPAR);
		}
		
		static void * MemAddress()
		{
			return reinterpret_cast<void *>(ChannelRegs()->CMAR);
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
		
		static bool Interrupt()
		{
			return Module::template Interrupt<Channel>();
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
		
		static void ClearInterrupt()
		{
			Module::template ClearInterrupt<Channel>();
		}
	};
	
	template<class DmaRegs, class Clock, int _Channels>
	class DmaModule :public DmaBase
	{
		BOOST_STATIC_ASSERT(_Channels <= 7);
		
		template<int ChannelNum, int Flag>
		static bool ChannelFlag()
		{
			BOOST_STATIC_ASSERT(ChannelNum > 0 && ChannelNum <= Channels);
			return DmaRegs()->ISR & (1 << (ChannelNum - 1) * 4 + Flag);
		}
		
		template<int ChannelNum, int Flag>
		static void ClearChannelFlag()
		{
			BOOST_STATIC_ASSERT(ChannelNum > 0 && ChannelNum <= Channels);
			DmaRegs()->IFCR | (1 << (ChannelNum - 1) * 4 + Flag);
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
			return ChannelFlag<ChannelNum, 3>();
		}
		
		template<int ChannelNum>
		static bool HalfTrasfer()
		{
			return ChannelFlag<ChannelNum, 2>();
		}
		
		template<int ChannelNum>
		static bool TrasferComplete()
		{
			return ChannelFlag<ChannelNum, 1>();
		}
		
		template<int ChannelNum>
		static bool Interrupt()
		{
			return ChannelFlag<ChannelNum, 0>();
		}
		
		template<int ChannelNum>
		static void ClearChannelFlags()
		{
			BOOST_STATIC_ASSERT(ChannelNum > 0 && ChannelNum < Channels);
			DmaRegs()->IFCR | (0x0f << (ChannelNum - 1) * 4);
		}
		
		template<int ChannelNum>
		static void ClearTransferError()
		{
			ClearChannelFlag<ChannelNum, 3>();
		}
		
		template<int ChannelNum>
		static void ClearHalfTrasfer()
		{
			ClearChannelFlag<ChannelNum, 2>();
		}
		
		template<int ChannelNum>
		static void ClearTrasferComplete()
		{
			ClearChannelFlag<ChannelNum, 1>();
		}
		
		template<int ChannelNum>
		static void ClearInterrupt()
		{
			ClearChannelFlag<ChannelNum, 0>();
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
		
		IO_STRUCT_WRAPPER(DMA1_Channel1, Dma1Channel1, DMA_Channel_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Channel2, Dma1Channel2, DMA_Channel_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Channel3, Dma1Channel3, DMA_Channel_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Channel4, Dma1Channel4, DMA_Channel_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Channel5, Dma1Channel5, DMA_Channel_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Channel6, Dma1Channel6, DMA_Channel_TypeDef);
		IO_STRUCT_WRAPPER(DMA1_Channel7, Dma1Channel7, DMA_Channel_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Channel1, Dma2Channel1, DMA_Channel_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Channel2, Dma2Channel2, DMA_Channel_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Channel3, Dma2Channel3, DMA_Channel_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Channel4, Dma2Channel4, DMA_Channel_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Channel5, Dma2Channel5, DMA_Channel_TypeDef);
		
	}
	
	typedef DmaModule<Private::Dma1, Clock::Dma1Clock, 7> Dma1;
	
	typedef DmaChannel<Dma1, Private::Dma1Channel1, 1> Dma1Channel1;
	typedef DmaChannel<Dma1, Private::Dma1Channel2, 1> Dma1Channel2;
	typedef DmaChannel<Dma1, Private::Dma1Channel3, 1> Dma1Channel3;
	typedef DmaChannel<Dma1, Private::Dma1Channel4, 1> Dma1Channel4;
	typedef DmaChannel<Dma1, Private::Dma1Channel5, 1> Dma1Channel5;
	typedef DmaChannel<Dma1, Private::Dma1Channel6, 1> Dma1Channel6;
	typedef DmaChannel<Dma1, Private::Dma1Channel7, 1> Dma1Channel7;
	
#if defined (STM32F10X_HD) || defined  (STM32F10X_CL) || defined  (STM32F10X_HD_VL)
	typedef DmaModule<Private::Dma2, Clock::Dma2Clock, 5> Dma2;
	typedef DmaChannel<Dma2, Private::Dma2Channel1, 1> Dma1Channel1;
	typedef DmaChannel<Dma2, Private::Dma2Channel2, 1> Dma1Channel2;
	typedef DmaChannel<Dma2, Private::Dma2Channel3, 1> Dma1Channel3;
	typedef DmaChannel<Dma2, Private::Dma2Channel4, 1> Dma1Channel4;
	typedef DmaChannel<Dma2, Private::Dma2Channel5, 1> Dma1Channel5;
#endif
	
	template<class PeriphModule, class _DataType, size_t NBlocks, size_t BlockSize>
	class DmaWriteBuffer
	{
		BOOST_STATIC_ASSERT(sizeof(_DataType) <= 4);
	public:
		typedef _DataType DataT;
		static const DmaBase::Mode PSize = PeriphModule::Dma::ItemSize;
		static const DmaBase::Mode MSize = sizeof(_DataType) > 8 ? 
					(sizeof(_DataType) > 16 ? DmaBase::MSize32Bits : DmaBase::MSize16Bits) 
					: DmaBase::MSize8Bits;
		typedef typename PeriphModule::Dma::ChannelTx UsedDmaChannel;
		
		DmaWriteBuffer()
		{
			_dmaBufferEnd = _writePtr = _buffer;
		}
		
		bool Write(DataT item)
		{
				UsedDmaChannel::Disable();// stop current DMA transaction if any
							
				PeriphModule::Dma::EnableTx();
				
				UsedDmaChannel::Init(DmaBase::Mem2Periph | PSize | MSize | DmaBase::MemIncriment | DmaBase::PriorityMedium, 
					readPtr, 
					PeriphModule::Dma::PAddress(), 
					dmaTransactionSize);
			return true;
		}
		
	private:
		
		typedef Containers::Array<BlockSize, DataT> Block;
		Containers::RingBuffer<NBlocks, Block> _txQueue;
	};
	
}
}
