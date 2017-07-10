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
#include <net/net_buffer.h>
#include <ring_buffer.h>
#include <net/INetDispatch.h>
#include <ARM/Stm32F40x/mac_descriptors.h>
#include <ARM/Stm32F40x/MacEnums.h>

void Print(const char*str, uint32_t t);

namespace Mcucpp
{
namespace Net
{
	class EtherRxTxQueue
	{
	public:
		Net::EthTxPool _txPool;
		Net::EthRxPool _rxPool;
			
		Containers::RingBuffer<Net::EthRxPool::DescriptorCount, Net::DataBuffer *> _rxQueue;
		
		struct TxQueueItem
		{
			TxQueueItem(uint32_t n, bool s):seqNumber(n), success(s){}
			uint32_t seqNumber;
			bool success;
		};
		
		Containers::RingBuffer<Net::EthTxPool::DescriptorCount, TxQueueItem> _txQueue;
		
	public:
	
		uint32_t TxDescriptorListStartAddr()
		{
			return (uint32_t)&_txPool.Descriptors[0];
		}
		
		uint32_t RxDescriptorListStartAddr()
		{
			return (uint32_t)&_rxPool.Descriptors[0];
		}
		
		unsigned ExtraDescriptorDWords() const
		{
			MCUCPP_STATIC_ASSERT(sizeof(_txPool.Descriptors[0]) == sizeof(_rxPool.Descriptors[0]));
			return (sizeof(_txPool.Descriptors[0]) + 3) / 4 - 4;
		}
	
		bool EnqueueBuffer(NetBuffer &buffer, uint32_t seqNumber)
		{
			return _txPool.EnqueueBuffer(buffer, seqNumber);
		}
		
		EthMacState PrepareQueues()
		{
			EthMacState state = EthOk;
			for(unsigned i = 0; i < Net::EthTxPool::DescriptorCount; i++)
			{
				Net::EthTxPool::TxDescriptorWithBufferPointer &tx = _txPool.Descriptors[i];
				if(tx.buffer1) DataBuffer::Release(tx.buffer1);
				if(tx.buffer2) DataBuffer::Release(tx.buffer2);
				tx.Reset();
			}
			
			for(unsigned i = 0; i < Net::EthRxPool::DescriptorCount; i++)
			{
				Net::EthRxPool::RxDescriptorWithBufferPointer &rx = _rxPool.Descriptors[i];
				if(rx.buffer1) DataBuffer::Release(rx.buffer1);
				if(rx.buffer2) DataBuffer::Release(rx.buffer2);
				
				rx.Reset();
				DataBuffer *smallBuffer = DataBuffer::GetNew(MedPoolBufferSize);
				if(!smallBuffer)
				{
					state = EthOutOfMem; break;
				}
				DataBuffer *largeBuffer = DataBuffer::GetNew(LargePoolBufferSize);
				if(!largeBuffer)
				{
					 DataBuffer::Release(smallBuffer);
					state = EthOutOfMem; break;
				}
				if(!rx.SetBuffer(smallBuffer))
				{
					state = EthDriverError; break;
				}
				if(!rx.SetBuffer2(largeBuffer))
				{
					state = EthDriverError; break;
				}
				rx.SetReady();
			}
			return state;
		}
		
		EthMacState ProcessInterrupt()
		{
			EthMacState state = EthOk;
			
			for(unsigned i = 0; i < Net::EthTxPool::DescriptorCount; i++)
			{
				Net::EthTxPool::TxDescriptorWithBufferPointer &descr = _txPool.Descriptors[i];
				if(!descr.InUse())
				{
					if(descr.GetOptions() & MacDmaTxFirstSegment)
					{
						MacDmaTxStatus status = descr.GetStatus();
						if(status & MacDmaTxError)
						{
							//_sendErrors++;
						}
						else
						{
							//_framesSend++;
						}
						TxQueueItem transferStatus(descr.seqNumber, (status & MacDmaTxError) == MacDmaTxSuccess);
						(void)_txQueue.push_back(transferStatus);
					}
					
					if(descr.buffer1)
					{
						DataBuffer::Release(descr.buffer1);
					}
					if(descr.buffer2)
					{
						DataBuffer::Release(descr.buffer2);
					}
					descr.Reset();
				}
			}
			
			bool frameError = false;
			NetBuffer netBuffer;
			size_t currentFrameSize = 0;
			
			for(unsigned i = 0; i < Net::EthRxPool::DescriptorCount; i++)
			{
				Net::EthRxPool::RxDescriptorWithBufferPointer &descr = _rxPool.Descriptors[i];
				
				if(!descr.InUse())
				{
					MacDmaRxStatus rxStatus = descr.GetStatus();
					if(rxStatus & MacDmaRxFirstDescriptor)
					{
						frameError = false;
						currentFrameSize = 0;
						netBuffer.Clear();
					}
					
					bool chunkError = rxStatus & MacDmaRxError;
					frameError = frameError || chunkError;
					
					if(chunkError || frameError)
					{
						//_reciveErrors++;
						DataBuffer::Release(descr.buffer1);
						DataBuffer::Release(descr.buffer2);
						netBuffer.Clear();
					}else
					{
						size_t buf1Size = descr.GetSize();
						size_t buf2Size = descr.GetSize2();
						bool lastDescriptor = rxStatus & MacDmaRxLastDescriptor;
						if(lastDescriptor)
						{
							size_t frameSize = descr.GetFrameLength();
							if(currentFrameSize + buf1Size >= frameSize)
							{
								buf1Size = frameSize - currentFrameSize;
								buf2Size = 0;
							}
							else
							{
								buf2Size = frameSize - currentFrameSize - buf1Size;
							}
						}
						
						currentFrameSize += buf1Size;
						descr.buffer1->Resize(buf1Size);
						netBuffer.AttachBack(descr.buffer1);
						
						if(buf2Size > 0)
						{
							currentFrameSize += buf2Size;
							descr.buffer2->Resize(buf2Size);
							netBuffer.AttachBack(descr.buffer2);
						}else
						{
							DataBuffer::Release(descr.buffer2);
						}
						
						if(lastDescriptor)
						{
							DataBuffer *bufferList = netBuffer.MoveToBufferList();
							if(!_rxQueue.push_back(bufferList))
							{
								DataBuffer::ReleaseRecursive(bufferList);
								//_framesAppMissed++;
							}
							else
							{
								//_framesRecived++;
							}
							currentFrameSize = 0;
						}
					}
					
					descr.Reset();
					DataBuffer *smallBuffer = DataBuffer::GetNew(MedPoolBufferSize);
					if(!smallBuffer)
					{
						state = EthOutOfMem; continue;
					}
					DataBuffer *largeBuffer = DataBuffer::GetNew(LargePoolBufferSize);
					if(!largeBuffer)
					{
						DataBuffer::Release(smallBuffer);
						state = EthOutOfMem; continue;
					}
					if(!descr.SetBuffer(smallBuffer))
					{
						state = EthDriverError; continue;
					}
					if(!descr.SetBuffer2(largeBuffer))
					{
						state = EthDriverError; continue;
					}
					descr.SetReady();
				}
			}
			return state;
		}
		
		void InvokeCallbacks(Net::INetDispatch *dispatch)
		{
			while(!_rxQueue.empty())
			{
				Net::NetBuffer buffer(_rxQueue.front()); // move buffer chain from queue
				_rxQueue.pop_front();
				buffer.Seek(0);
				Net::MacAddr dest = buffer.ReadMac();
				Net::MacAddr src = buffer.ReadMac();
				uint16_t protocoId = buffer.ReadU16Be();
				// TODO: VLAN handling
				if(dispatch)
				{
					dispatch->RxComplete(src, dest, protocoId, buffer);
				}
				else
				{
					//_framesAppMissed++;
				}
				buffer.Clear();
				Print("Rx handled ", protocoId);
			}
			
			while(!_txQueue.empty())
			{
				TxQueueItem transferStatus = _txQueue.front();
				_txQueue.pop_front();
				if(dispatch)
				{
					Print("Tx complete ", transferStatus.seqNumber);
					dispatch->TxComplete(transferStatus.seqNumber, transferStatus.success);
				}
			}
		}
	};

}}