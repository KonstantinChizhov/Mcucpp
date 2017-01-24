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
#include <stddef.h>
#include <array.h>
#include <binary_stream.h>
#include <net/net_addr.h>

namespace Mcucpp
{
namespace Net
{
	const size_t SmallPoolBufferSize = 32;
	const size_t MedPoolBufferSize = 128;
	const size_t LargePoolBufferSize = 1396;
	
	class DataBuffer
	{
		uint8_t *_data;
		DataBuffer * _next;
		uint16_t _size;
		uint16_t _storage;
		friend class NetBufferBase;
		friend class BufferChain;
	public:
		
		~DataBuffer()
		{
			Release(this);
		}
	
		DataBuffer(uint8_t *data, size_t size)
			:_data(data), _next(0), _size(size), _storage(0)
		{
			
		}
		
		DataBuffer(uint8_t *data, size_t size, size_t storageSize)
			:_data(data), _next(0), _size(size), _storage(storageSize)
		{
			
		}
		
		uint8_t &operator[](size_t index)
		{
			return _data[index];
		}
		
		uint8_t operator[](size_t index)const
		{
			return _data[index];
		}
		
		bool Resize(size_t s) 
		{
			if(s > _storage)
				return false;
			_size = s; 
			return true; 
		}
		
		uint8_t* Data() const {return _data; }
		size_t Size() const {return _size; }
		
		size_t Capacity() const {return _storage ? _storage : _size; }
		DataBuffer* Next() const {return _next; }
		
		static DataBuffer* GetNew(size_t size);
		static DataBuffer* GetNew(const void *data, size_t size);
		
		static void Release(DataBuffer * data);
		static void ReleaseRecursive(DataBuffer * data);
		static DataBuffer *FindLast(DataBuffer *first);
	};
	

	class NetBufferBase
	{
		DataBuffer *_first;
		DataBuffer *_current;
		size_t _pos;
	public:
		NetBufferBase();
		~NetBufferBase();
		
		// move semantic, like std::auto_ptr
		// transfers buffers ownership to target object
		NetBufferBase(NetBufferBase &);
		NetBufferBase & operator=(NetBufferBase &);
		
		NetBufferBase(DataBuffer* chain)
		:_first(chain), _current(0), _pos(0)
		{
		
		}
		
		DataBuffer* MoveToBufferList()
		{
			DataBuffer* result = _first;
			_first = 0;
			_current = 0;
			_pos = 0;
			return result;
		}
		
		DataBuffer* BufferList(){ return _first;}
		
		uint8_t Read()
		{
			if(!_current)
				return 0;
			uint8_t value = (*_current)[_pos++];
			if(_pos >= _current->Size())
			{
				_pos = 0;
				_current = _current->Next();
			}
			return value;
		}
		
		void Write(uint8_t byte)
		{
			if(!_current)
				return;
			(*_current)[_pos++] = byte;
			if(_pos >= _current->Size())
			{
				_pos = 0;
				_current = _current->Next();
			}
		}
		
		void WriteMac(const Net::MacAddr &addr)
		{
			for(size_t i = 0; i < addr.Length(); i++)
			{
				Write(addr[i]);
			}
		}
		
		void WriteIp(const Net::IpAddr &addr)
		{
			for(size_t i = 0; i < addr.Length(); i++)
			{
				Write(addr[i]);
			}
		}
		
		Net::MacAddr ReadMac()
		{
			Net::MacAddr addr;
			for(size_t i = 0; i < addr.Length(); i++)
			{
				addr[i] = Read();
			}
			return addr;
		}
		
		Net::IpAddr ReadIp()
		{
			uint8_t a = Read();
			uint8_t b = Read();
			uint8_t c = Read();
			uint8_t d = Read();
			return Net::IpAddr(a, b, c, d);
		}
		
		void Clear();
		bool InsertFront(size_t size);
		bool InsertBack(size_t size);
		bool Insert(size_t pos, size_t size);
		void AttachBack(DataBuffer* buffer);
		void AttachFront(DataBuffer* buffer);
		DataBuffer* DetachFront();
		
		bool Seek(size_t pos);
		size_t Size();
		unsigned Parts();
	};
	
	typedef Mcucpp::BinaryStream<NetBufferBase> NetBuffer;
}
}