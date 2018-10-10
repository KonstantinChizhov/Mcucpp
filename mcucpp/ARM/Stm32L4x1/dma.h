
//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2018
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice,
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation and/or
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************


#pragma once

#include "ioreg.h"
#include "mcu_header.h"
#include "clock.h"
#include <static_assert.h>
#include <enum.h>
#include <data_transfer.h>

namespace Mcucpp
{
	enum DmaMode
	{
		Mem2Mem = DMA_CCR_MEM2MEM,
		
		PriorityLow = 0,
		PriorityMedium = DMA_CCR_PL_0,
		PriorityHigh = DMA_CCR_PL_1,
		PriorityVeryHigh = DMA_CCR_PL_1 | DMA_CCR_PL_0,
		
		MSize8Bits = 0,
		MSize16Bits = DMA_CCR_MSIZE_0,
		MSize32Bits = DMA_CCR_MSIZE_1,
		
		PSize8Bits = 0,
		PSize16Bits = DMA_CCR_PSIZE_0,
		PSize32Bits = DMA_CCR_PSIZE_1,
		
		MemIncriment = DMA_CCR_MINC,
		PeriphIncrement = DMA_CCR_PINC,
		Circular = DMA_CCR_CIRC,
		
		Periph2Mem = 0,
		Mem2Periph = DMA_CCR_DIR,
		
		TransferErrorInterrupt = DMA_CCR_TEIE,
		HalfTransferInterrupt = DMA_CCR_HTIE,
		TransferCompleteInterrupt = DMA_CCR_TCIE
	};
	
	
	DECLARE_ENUM_OPERATIONS(DmaMode)
	
	struct DmaChannelData
	{
		DmaChannelData()
			:transferCallback(nullptr)
		{}
		TransferCallback transferCallback;
		
		void *data;
		uint16_t size;
		
		inline void NotifyTransferComplete()
		{
			TransferCallback callback = transferCallback;
			//transferCallback = VoidDmaCallback;
			if(callback)
			{
				callback(data, size, true);
			}
		}
		
		inline void NotifyError()
		{
			TransferCallback callback = transferCallback;
			//errorCallback = VoidDmaCallback;
			if(callback)
			{
				callback(data, size, false);
			}
		}
	};

	
	template<class Module, class ChannelRegs, int Channel, class RequestType, IRQn_Type IRQNumber>
	class DmaChannel
	{
		STATIC_ASSERT(Channel <= Module::Channels);
		static DmaChannelData ChannelData;
	public:
		
		static void Transfer(DmaMode mode, const void *buffer, volatile void *periph, uint32_t bufferSize)
		{
			Module::Enable();
			if(!TransferError())
			{
				while(!Ready())
					;
			}
			ClearFlags();
			ChannelRegs()->CCR = 0;
			ChannelRegs()->CNDTR = bufferSize;
			ChannelRegs()->CPAR = reinterpret_cast<uint32_t>(periph);
			ChannelRegs()->CMAR = reinterpret_cast<uint32_t>(buffer);
			
			ChannelData.data = const_cast<void*>(buffer);
			ChannelData.size = bufferSize;
			if(ChannelData.transferCallback)
				mode = mode | TransferCompleteInterrupt | TransferErrorInterrupt;
		
			ChannelRegs()->CCR = mode | DMA_CCR_EN;
			NVIC_EnableIRQ(IRQNumber);
		}
		
		static void SetTransferCallback(TransferCallback callback)
		{
			ChannelData.transferCallback = callback;
		}
		
		static void SetRequest(RequestType request)
		{
			Module::template SetRequest<Channel>(static_cast<uint32_t>(request));
		}
		
		static bool Enabled()
		{
			return ChannelRegs()->CCR & DMA_CCR_EN;
		}
		
		static void Disable()
		{
			ChannelRegs()->CCR &= DMA_CCR_EN;
		}
		
		static bool Ready()
		{
			return ChannelRegs()->CNDTR == 0 || !Enabled() || TrasferComplete();
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
	
	template<class Module, class ChannelRegs, int Channel, class RequestType, IRQn_Type IRQNumber>
	DmaChannelData DmaChannel<Module, ChannelRegs, Channel, RequestType, IRQNumber>::ChannelData;
	
	template<class DmaRegs, class DmaRequestReg, class ClockEn, int _Channels>
	class DmaModule
	{
		STATIC_ASSERT(_Channels <= 7);
		
		template<int ChannelNum, int Flag>
		static bool ChannelFlag()
		{
			STATIC_ASSERT(ChannelNum > 0 && ChannelNum <= Channels);
			return DmaRegs()->ISR & (1 << ((ChannelNum - 1) * 4 + Flag));
		}
		
		template<int ChannelNum, int Flag>
		static void ClearChannelFlag()
		{
			STATIC_ASSERT(ChannelNum > 0 && ChannelNum <= Channels);
			DmaRegs()->IFCR |= (1 << ((ChannelNum - 1) * 4 + Flag));
		}
		
		
	public:
		static const int Channels = _Channels;
		
		// Channel - DMA Channel number 1..7
		// NOTE: Channel starts with 1, regarding to ST documentation
		
		// template<int ChannelNum>
		// class Channel : public DmaChannel<DmaModule, TODO, ChannelNum>
		// {};
		
		template<int ChannelNum>
		static void SetRequest(uint32_t request)
		{
			STATIC_ASSERT(ChannelNum > 0 && ChannelNum <= Channels);
			DmaRequestReg()->CSELR = (DmaRequestReg()->CSELR & ~(0x0f << ((ChannelNum - 1) * 4))) | (request << ((ChannelNum - 1) * 4));
		}
		
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
			STATIC_ASSERT(ChannelNum > 0 && ChannelNum <= Channels);
			DmaRegs()->IFCR |= (0x0f << (ChannelNum - 1) * 4);
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
			ClockEn::Enable();
		}
		
		static void Disable()
		{
			ClockEn::Disable();
		}
	};
	
	enum class Dma1Channel1Request
	{
		Adc1 = 0,
		Tim2_Ch3 = 4,
		Tim17_Ch1 = 5,
		Tim17_Up = 5,
		Tim4_Ch1 = 6
	};
	
	enum class Dma1Channel2Request
	{
		Adc2 = 0,
		Spi1_Rx = 1,
		Usart3_Rx = 2,
		I2c3_Rx = 3,
		Tim2_Up = 4,
		Tim3_Ch3 = 5,
		Tim1_Ch1 = 7
	};
	
	enum class Dma1Channel3Request
	{
		Adc3 = 0,
		Spi_Rx = 1,
		Usart3_Rx = 2,
		I2c3_Rx = 3,
		Tim16_Up = 4,
		Tim16_Ch1 = 4,
		Tim3_Ch4 = 5,
		Tim3_Up = 5,
		Tim6_Up = 6,
		Dac1 = 6,
		Tim1_Ch2 = 7
	};
	
	
	enum class Dma1Channel4Request
	{
		Dfsdm1_Flt0 = 0,
		Spi2_Rx = 1,
		Usart1_Rx = 2,
		I2c2_Rx = 3,
		Dac2 = 5,
		Tim4_Ch2 = 6,
		Tim1_Ch4 = 7,
		Tim1_Trig = 7,
		Tim1_Com = 7
	};
	
	enum class Dma1Channel5Request
	{
		
	};
	
	enum class Dma1Channel6Request
	{
		
	};
	
	enum class Dma1Channel7Request
	{
		
	};
	
	
	enum class Dma2Channel1Request
	{
		
	};
	
	enum class Dma2Channel2Request
	{
		
	};
	
	enum class Dma2Channel3Request
	{
		
	};
	
	enum class Dma2Channel4Request
	{
		
	};
	
	enum class Dma2Channel5Request
	{
		
	};
	
	enum class Dma2Channel6Request
	{
		
	};
	
	enum class Dma2Channel7Request
	{
		
	};
	
	namespace Private
	{
		IO_STRUCT_WRAPPER(DMA1, Dma1, DMA_TypeDef);
		IO_STRUCT_WRAPPER(DMA2, Dma2, DMA_TypeDef);
		
		IO_STRUCT_WRAPPER(DMA1_CSELR, Dma1Request, DMA_Request_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_CSELR, Dma2Request, DMA_Request_TypeDef);
		
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
		IO_STRUCT_WRAPPER(DMA2_Channel6, Dma2Channel6, DMA_Channel_TypeDef);
		IO_STRUCT_WRAPPER(DMA2_Channel7, Dma2Channel7, DMA_Channel_TypeDef);
	}
	
	typedef DmaModule<Private::Dma1, Private::Dma1Request, Clock::Dma1Clock, 7> Dma1;
	typedef DmaModule<Private::Dma2, Private::Dma2Request, Clock::Dma2Clock, 7> Dma2;
	
	typedef DmaChannel<Dma1, Private::Dma1Channel1, 1, Dma1Channel1Request, DMA1_Channel1_IRQn> Dma1Channel1;
	typedef DmaChannel<Dma1, Private::Dma1Channel2, 2, Dma1Channel2Request, DMA1_Channel2_IRQn> Dma1Channel2;
	typedef DmaChannel<Dma1, Private::Dma1Channel3, 3, Dma1Channel3Request, DMA1_Channel3_IRQn> Dma1Channel3;
	typedef DmaChannel<Dma1, Private::Dma1Channel4, 4, Dma1Channel4Request, DMA1_Channel4_IRQn> Dma1Channel4;
	typedef DmaChannel<Dma1, Private::Dma1Channel5, 5, Dma1Channel5Request, DMA1_Channel5_IRQn> Dma1Channel5;
	typedef DmaChannel<Dma1, Private::Dma1Channel6, 6, Dma1Channel6Request, DMA1_Channel6_IRQn> Dma1Channel6;
	typedef DmaChannel<Dma1, Private::Dma1Channel7, 7, Dma1Channel7Request, DMA1_Channel7_IRQn> Dma1Channel7;

	typedef DmaChannel<Dma2, Private::Dma2Channel1, 1, Dma2Channel1Request, DMA1_Channel1_IRQn> Dma2Channel1;
	typedef DmaChannel<Dma2, Private::Dma2Channel2, 2, Dma2Channel2Request, DMA1_Channel2_IRQn> Dma2Channel2;
	typedef DmaChannel<Dma2, Private::Dma2Channel3, 3, Dma2Channel3Request, DMA1_Channel3_IRQn> Dma2Channel3;
	typedef DmaChannel<Dma2, Private::Dma2Channel4, 4, Dma2Channel4Request, DMA1_Channel4_IRQn> Dma2Channel4;
	typedef DmaChannel<Dma2, Private::Dma2Channel5, 5, Dma2Channel5Request, DMA1_Channel5_IRQn> Dma2Channel5;
	typedef DmaChannel<Dma2, Private::Dma2Channel6, 6, Dma2Channel6Request, DMA1_Channel6_IRQn> Dma2Channel6;
	typedef DmaChannel<Dma2, Private::Dma2Channel7, 7, Dma2Channel7Request, DMA1_Channel7_IRQn> Dma2Channel7;
}
