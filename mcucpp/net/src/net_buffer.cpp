
#include <net/net_buffer.h>
#include <MemPool.h>
#include <new.h>

using namespace Mcucpp;
using namespace Mcucpp::Net;


MemPool<sizeof(DataBuffer) + SmallPoolBufferSize, 20, unsigned, VoidAtomic> SmallPool;
MemPool<sizeof(DataBuffer) + MedPoolBufferSize,   16, unsigned, VoidAtomic> MedPool;
MemPool<sizeof(DataBuffer) + LargePoolBufferSize, 4, unsigned, VoidAtomic> LargePool;

DataBuffer* DataBuffer::GetNew(size_t size)
{
	size_t sizeReuired = size + sizeof(DataBuffer);
	void * ptr = 0;
	size_t storage;
	if(SmallPool.GetBlockSize() >= sizeReuired)
	{
		ptr = SmallPool.Alloc();
		storage = SmallPool.GetBlockSize();
	}
	
	if(!ptr && MedPool.GetBlockSize() >= sizeReuired)
	{
		ptr = MedPool.Alloc();
		storage = MedPool.GetBlockSize();
	}
	
	if(!ptr && LargePool.GetBlockSize() >= sizeReuired)
	{
		ptr = LargePool.Alloc();
		storage = LargePool.GetBlockSize();
	}
	if(!ptr)
		return 0;
	uint8_t *data = (uint8_t *)ptr + sizeof(DataBuffer);
	storage -= sizeof(DataBuffer);
	
	DataBuffer * dataBuffer = new (ptr)DataBuffer(data, size, storage);
	return dataBuffer;
}

DataBuffer* DataBuffer::GetNew(const void *data, size_t size)
{
	DataBuffer* buffer = GetNew(size);
	if(!buffer)
		return 0;
	memcpy(buffer->_data, data, size);
	return buffer;
}

void DataBuffer::Release(DataBuffer * data)
{
	SmallPool.Free(data);
	MedPool.Free(data);
	LargePool.Free(data);
}

void DataBuffer::ReleaseRecursive(DataBuffer * buffer)
{
	DataBuffer *next;
	while(buffer)
	{
		next = buffer->Next();
		if(buffer)
			DataBuffer::Release(buffer);
		buffer = next;
	}
}


NetBufferBase::NetBufferBase()
	:_first(0),
	_current(0),
	_pos(0)
{
	
}

NetBufferBase::NetBufferBase(NetBufferBase &rhs)
{
	_first = rhs._first;
	_current = rhs._current;
	_pos = rhs._pos;
	rhs._first = 0;
	rhs._current = 0;
	rhs._pos = 0;
	
}

NetBufferBase & NetBufferBase::operator=(NetBufferBase &rhs)
{
	Clear();
	_first = rhs._first;
	_current = rhs._current;
	_pos = rhs._pos;
	rhs._first = 0;
	rhs._current = 0;
	rhs._pos = 0;
	return *this;
}

NetBufferBase::~NetBufferBase()
{
	Clear();
}

void NetBufferBase::Clear()
{
	DataBuffer *buffer = _first;
	_current = _first = 0;
	DataBuffer::ReleaseRecursive(buffer);
}

bool NetBufferBase::InsertFront(size_t size)
{
	DataBuffer* buffer = 0;
	DataBuffer* next;
	do
	{
		if(!buffer)
			buffer = DataBuffer::GetNew(size);
		if(!buffer)
			return false;
		next = _first;
		buffer->_next = next;
	}while(!Atomic::CompareExchange(&_first, next, buffer));
	
	return true;
}

DataBuffer *DataBuffer::FindLast(DataBuffer *first)
{
	if(first)
	{
		while(first->Next())
		{
			first = first->Next();
		}
	}
	return first;
}

bool NetBufferBase::InsertBack(size_t size)
{
	DataBuffer *last, *buffer = 0;
	DataBuffer** pnext;
	do
	{
		last = DataBuffer::FindLast(_first);
		pnext = last ? &(last->_next) : &_first;
		if(last)
		{
			uint16_t lastSize = last->Size();
			if(last->Capacity() >= lastSize + size)
			{
				if(!Atomic::CompareExchange(&last->_size, lastSize, (uint16_t)(lastSize + size)))
					continue;
				if(buffer)
					DataBuffer::Release(buffer);
				return true;
			}
		}
		if(!buffer)
			buffer = DataBuffer::GetNew(size);
		if(!buffer)
			return false;
	}while(!Atomic::CompareExchange(pnext, (DataBuffer *)0, buffer));
	
	return true;
}

void NetBufferBase::AttachBack(DataBuffer* buffer)
{
	DataBuffer *last;
	DataBuffer** pnext;
	do
	{
		last = DataBuffer::FindLast(_first);
		pnext = last ? &(last->_next) : &_first;
	}while(!Atomic::CompareExchange(pnext, (DataBuffer *)0, buffer));
}

void NetBufferBase::AttachFront(DataBuffer* buffer)
{
	DataBuffer* next;
	do
	{
		next = _first;
		buffer->_next = next;
	}while(!Atomic::CompareExchange(&_first, next, buffer));
}

DataBuffer* NetBufferBase::DetachFront()
{
	DataBuffer* first, *next;
	do
	{
		first = _first;
		next = first->_next;
	}while(!Atomic::CompareExchange(&_first, first, next));
	return first;
}


bool NetBufferBase::Seek(size_t pos)
{
	DataBuffer *current = _first;
	while(current && current->Size() < pos)
	{
		pos -= current->Size();
		current = current->Next();
	}
	if(!current)
	{
		return false;
	}
	_pos = pos;
	_current = current;
	return true;
}

size_t NetBufferBase::Tell()
{
	DataBuffer *current = _first;
	size_t result = _pos;

	while(current && current != _current)
	{
		result += current->Size();
		current = current->Next();
	}
	return result;
}

size_t NetBufferBase::Size()
{
	DataBuffer *current = _first;
	size_t totalSize = 0;
	if(current)
	{
		do
		{
			totalSize += current->Size();
			current = current->Next();
		}while(current);
	}
	return totalSize;
}

unsigned NetBufferBase::Parts()
{
	DataBuffer *current = _first;
	unsigned parts = 0;
	while(current)
	{
		++parts;
		current = current->Next();
	}

	return parts;
}