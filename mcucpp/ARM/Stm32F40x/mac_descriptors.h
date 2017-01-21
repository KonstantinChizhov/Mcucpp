//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2016
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

#include <stdint.h>
#include <enum.h>
#include <debug.h>
#include <data_transfer.h>
#include <net/net_buffer.h>

namespace Mcucpp
{
namespace Net
{
#if defined(MAX_ETH_DESCROPTORS) && MAX_ETH_DESCROPTORS > 0
	const size_t MaxEthDescriptors = MAX_ETH_DESCROPTORS;
#else
	const size_t MaxEthDescriptors = 8;
#endif

	enum MacDmaTxStatus
	{
		MacDmaTxSuccess            = 0,
		MacDmaTxDefered            = (1 << 0),
		MacDmaTxUnderflow          = (1 << 1),
		MacDmaTxExcessiveDeferral  = (1 << 2),
		MacDmaTxVlanFrame          = (1 << 7),
		MacDmaTxExcessiveCollision = (1 << 8),
		MacDmaTxLateCollision      = (1 << 9),
		MacDmaTxNoCarrier          = (1 << 10),
		MacDmaTxLossOfCarrier      = (1 << 11),
		MacDmaTxIpPayloadErr       = (1 << 12),
		MacDmaTxFrameFlushed       = (1 << 13),
		MacDmaTxJabberTimeout      = (1 << 14),
		MacDmaTxError              = (1 << 15),
		MacDmaTxIpHeaderErr        = (1 << 16)
	};
	
	enum MacDmaRxStatus
	{
		MacDmaRxSuccess         = 0,
		MacDmaRxChecksumErr     = (1 << 0),
		MacDmaRxCrcErr          = (1 << 1),
		MacDmaRxDribbleErr      = (1 << 2),
		MacDmaRxReciveErr       = (1 << 3),
		MacDmaRxReciveTimeout   = (1 << 4),
		MacDmaRxFrameType       = (1 << 5),
		MacDmaRxLateCollision   = (1 << 6),
		MacDmaRxIpHeaderCrcErr  = (1 << 7),
		MacDmaRxLastDescriptor  = (1 << 8),
		MacDmaRxFirstDescriptor = (1 << 9),
		MacDmaRxVlanFrame       = (1 << 10),
		MacDmaRxOverflowErr     = (1 << 11),
		MacDmaRxLengthErr       = (1 << 12),
		MacDmaRxSrcAddrErr      = (1 << 13),
		MacDmaRxDescriptorErr   = (1 << 14),
		MacDmaRxDestAddrErr     = (1 << 30),
		MacDmaRxError           = (1 << 15),
	};
		
	enum MacDmaTxOptions
	{
		MacDmaTxNone,
		MacDmaTxEndOfRing          = (1 << 21),
		MacDmaTxIpHeaderCrc        = (1 << 22),
		MacDmaTxIpHeaderAndDataCrc = (1 << 23),
		MacDmaTxPseudoHeaderCrc    = (3 << 22),
		MacDmaTxTimeStamp          = (1 << 25),
		MacDmaTxDisablePad         = (1 << 26),
		MacDmaTxDisableCrc         = (1 << 27),
		MacDmaTxFirstSegment       = (1 << 28),
		MacDmaTxLastSegment        = (1 << 29),
		MacDmaTxInterrupt          = (1 << 30),
		MacDmaTxReady              = (1 << 31)
	};
	
	DECLARE_ENUM_OPERATIONS(MacDmaTxOptions);
	DECLARE_ENUM_OPERATIONS(MacDmaRxStatus);
	DECLARE_ENUM_OPERATIONS(MacDmaTxStatus);
	
	#pragma pack(push, 1)
	
	struct MacDescriptor
	{
		uint32_t des0;
		uint32_t des1;
		uint32_t des2;
		uint32_t des3;
		
		MacDescriptor()
		:des0(0), des1(0), des2(0), des3(0){}
		
		bool IsFree(){return !des1;}

		bool InUse() { return des0 & (1 << 31); }
		void SetReady() { des0 |= (1 << 31); }
		void Lock() { des0 &= ~(1 << 31); }
		
		
		bool SetBuffer(const void *buffer, size_t size)
		{
			if(size > 0x1fff)
				return false;
			des1 = (des1 & 0x1fff0000) | size;
			des2 = reinterpret_cast<uint32_t>(buffer);
			return true;
		}
		
		bool SetBuffer2(const void *buffer, size_t size)
		{
			if(size > 0x1fff)
				return false;
			des1 = (des1 & 0x00001fff) | (size << 16);
			des3 = reinterpret_cast<uint32_t>(buffer);
			return true;
		}
		
		void * GetBuffer()
		{
			return reinterpret_cast<void*>(des2);
		}
		
		void * GetBuffer2()
		{
			return reinterpret_cast<void*>(des3);
		}
		
		uint32_t GetSize()
		{
			return des1 & 0x00001fff;
		}
		
		uint32_t GetSize2()
		{
			return (des1 & 0x1fff0000) >> 16;
		}
		
		
		MacDescriptor* NextChained() { return (MacDescriptor*)des3; }
	};
	
	struct MacTxDescriptor :public MacDescriptor
	{
		MacTxDescriptor()
		{
		}
		
		void Reset()
		{
			des0 &= MacDmaTxEndOfRing;
			des1 = des2 = des3 = 0;
		}
		
		void ChainNext(MacTxDescriptor *nextDescr)
		{
			des3 = (uint32_t)nextDescr;
			des0 |= (1 << 20);
		}
			
		void SetOptions(MacDmaTxOptions options)
		{ 
			des0 = (des0 & ~0xFFE00000) | options;
		}
		
		MacDmaTxOptions GetOptions()
		{ 
			return (MacDmaTxOptions)(des0 & 0xFFE00000);
		}
		
		void AppendOptions(MacDmaTxOptions options)
		{ 
			des0 |= options;
		}
		
		bool IsNextChained() { return des0 & (1 << 20); }
		MacDmaTxStatus GetStatus(){ return (MacDmaTxStatus)(des0 & 0x1ff87);}
		unsigned GetCollisions(){ return (des0 >> 3) & 0x0f;}
		
		void SetEndOfRing()
		{
			AppendOptions(MacDmaTxEndOfRing);
		}
	};
	
	struct MacRxDescriptor :public MacDescriptor
	{
		void ChainNext(MacRxDescriptor *nextDescr)
		{
			des3 = (uint32_t)nextDescr;
			des1 |= (1 << 14);
		}
		
		void Reset()
		{
			des0 = des2 = des3 = 0;
			des1 &= (1 << 15);
		}
		
		bool IsNextChained() { return des1 & (1 << 14); }
		MacDmaRxStatus GetStatus(){ return (MacDmaRxStatus)(des0 & 0x4000ffff); }
		size_t GetFrameLength()
		{
			if(des0 & MacDmaRxLastDescriptor)
				return (des0 >> 16) & 0x3ff;
			return 0;
		}
		
		void SetEndOfRing()
		{
			des1 |= (1 << 15);
		}
	};
	
	#pragma pack(pop)
	
	struct EthTransferData
	{
		const void *data;
		size_t size;
		
		EthTransferData()
			:data(0), size(0){}
			
		void Reset()
		{
			data = 0;
			size = 0;
		}
	};
	
	class EthTxPool
	{
	public:
		enum {DescriptorCount = MaxEthDescriptors};
		struct TxDescriptorWithBufferPointer : public MacTxDescriptor
		{
			DataBuffer *buffer1;
			DataBuffer *buffer2;
			uint32_t seqNumber;
			void Reset()
			{
				MacTxDescriptor::Reset();
				buffer1 = 0;
				buffer2 = 0;
			}
		};
		
		TxDescriptorWithBufferPointer Descriptors[DescriptorCount];
		size_t _currentDescriptor;
	public:
		EthTxPool()
		:_currentDescriptor(0)
		{
			Descriptors[DescriptorCount - 1].SetEndOfRing();
		}
		
		MacTxDescriptor *StartOfList(){ return &Descriptors[0];}
		
		void Reset()
		{
			_currentDescriptor = 0;
			for(size_t i = 0; i < DescriptorCount; i++)
				Descriptors[i].Reset();
			Descriptors[DescriptorCount - 1].SetEndOfRing();
		}
		
		bool EnqueueBuffer(NetBuffer &buffer, uint32_t seqNumber)
		{
			size_t partsCount = buffer.Parts();
			size_t descriptorsRequired = (partsCount + 1) / 2;
			size_t descrIndex = _currentDescriptor;
			
			for(size_t index = 0; index < descriptorsRequired; index++)
			{
				MacTxDescriptor & descr = Descriptors[descrIndex];
				if(descr.InUse())
				{
					return false; // Not enougth free descriptors
				}
				descrIndex++;
				if(descrIndex >= DescriptorCount)
					descrIndex=0;
			}
			
			descrIndex = _currentDescriptor;
			
			for(size_t i = 0; i < descriptorsRequired; i++)
			{
				TxDescriptorWithBufferPointer & descr = Descriptors[descrIndex];
				DataBuffer *part = buffer.DetachFront();
				descr.Reset();
				descr.SetBuffer(part->Data(), part->Size());
				descr.buffer1 = part;
				descr.seqNumber = seqNumber;
				
				if(i < partsCount)
				{
					DataBuffer *part2 = buffer.DetachFront();
					descr.SetBuffer2(part2->Data(), part2->Size());
					descr.buffer2 = part2;
				}
				
				MacDmaTxOptions options = MacDmaTxNone;
				if(i == 0)
					options |= MacDmaTxFirstSegment | MacDmaTxInterrupt;
				
				if(i == descriptorsRequired - 1)
					options |= MacDmaTxLastSegment;
				
				descrIndex++;
				if(descrIndex >= DescriptorCount)
				{
					descrIndex=0;
					options |= MacDmaTxEndOfRing;
				}
				descr.SetOptions(options | MacDmaTxReady);
			}
			_currentDescriptor = descrIndex;
			
			return true;
		}
	};
	
	class EthRxPool
	{
	public:
		enum {DescriptorCount = 3};
		
		struct RxDescriptorWithBufferPointer : public MacRxDescriptor
		{
			DataBuffer *buffer1;
			DataBuffer *buffer2;
			uint32_t reserved;
			
			void Reset()
			{
				MacRxDescriptor::Reset();
				buffer1 = 0;
				buffer2 = 0;
			}
			
			bool SetBuffer(DataBuffer *buffer)
			{
				bool res = MacRxDescriptor::SetBuffer(buffer->Data(), buffer->Capacity());
				if(!res)
				{
					return false;
				}
				buffer1 = buffer;
				return true;
			}
			
			bool SetBuffer2(DataBuffer *buffer)
			{
				bool res = MacRxDescriptor::SetBuffer2(buffer->Data(), buffer->Capacity());
				if(!res)
				{
					return false;
				}
				buffer1 = buffer;
				return true;
			}
		};
		
		RxDescriptorWithBufferPointer Descriptors[DescriptorCount];
	public:
		EthRxPool()
		{
			Descriptors[DescriptorCount - 1].SetEndOfRing();
		}
		
		void Reset()
		{
			for(size_t i = 0; i < DescriptorCount; i++)
				Descriptors[i].Reset();
			Descriptors[DescriptorCount - 1].SetEndOfRing();
		}
	};
}}