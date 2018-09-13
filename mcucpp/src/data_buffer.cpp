
#include <data_buffer.h>
#include <new>

using namespace Mcucpp;

uint8_t DataBufferBase::_dummy = 0;

DataChunk* DataChunk::GetNew(size_t size)
{
	size_t sizeRequired = (size + sizeof(DataChunk) + 15) & ~0x0f;
	void * ptr = new (std::nothrow) uint8_t[sizeRequired];
	size_t storage = sizeRequired - sizeof(DataChunk);
	if(!ptr)
		return nullptr;
	uint8_t *data = (uint8_t *)ptr + sizeof(DataChunk);
	
	DataChunk * dataChunk = new (ptr)DataChunk(data, size, storage);
	return dataChunk;
}

DataChunk* DataChunk::GetNew(const void *data, size_t size)
{
	DataChunk* buffer = GetNew(size);
	if(!buffer)
		return 0;
	memcpy(buffer->_data, data, size);
	return buffer;
}

void DataChunk::Release(DataChunk * data)
{
	delete [] (uint8_t *)data;
}

void DataChunk::ReleaseRecursive(DataChunk * buffer)
{
	DataChunk *next;
	while(buffer)
	{
		next = buffer->Next();
		if(buffer)
			DataChunk::Release(buffer);
		buffer = next;
	}
}


DataBufferBase::DataBufferBase()
	:_first(0),
	_current(0),
	_pos(0)
{
	
}

DataBufferBase::DataBufferBase(DataBufferBase &rhs)
{
	_first = rhs._first;
	_current = rhs._current;
	_pos = rhs._pos;
	rhs._first = 0;
	rhs._current = 0;
	rhs._pos = 0;
	
}

DataBufferBase & DataBufferBase::operator=(DataBufferBase &rhs)
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

DataBufferBase::~DataBufferBase()
{
	Clear();
}

void DataBufferBase::Clear()
{
	DataChunk *buffer = _first;
	_current = _first = 0;
	DataChunk::ReleaseRecursive(buffer);
}

bool DataBufferBase::InsertFront(size_t size)
{
	DataChunk* buffer = 0;
	DataChunk* next;
	do
	{
		if(!buffer)
			buffer = DataChunk::GetNew(size);
		if(!buffer)
			return false;
		next = _first;
		buffer->_next = next;
	}while(!Atomic::CompareExchange(&_first, next, buffer));
	
	return true;
}

DataChunk *DataChunk::FindLast(DataChunk *first)
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

bool DataBufferBase::InsertBack(size_t size)
{
	DataChunk *last, *buffer = 0;
	DataChunk** pnext;
	do
	{
		last = DataChunk::FindLast(_first);
		pnext = last ? &(last->_next) : &_first;
		if(last)
		{
			uint16_t lastSize = last->Size();
			if(last->Capacity() >= lastSize + size)
			{
				if(!Atomic::CompareExchange(&last->_size, lastSize, (uint16_t)(lastSize + size)))
					continue;
				if(buffer)
					DataChunk::Release(buffer);
				return true;
			}
		}
		if(!buffer)
			buffer = DataChunk::GetNew(size);
		if(!buffer)
			return false;
	}while(!Atomic::CompareExchange(pnext, (DataChunk *)0, buffer));
	
	return true;
}

void DataBufferBase::AttachBack(DataChunk* buffer)
{
	DataChunk *last;
	DataChunk** pnext;
	do
	{
		last = DataChunk::FindLast(_first);
		pnext = last ? &(last->_next) : &_first;
	}while(!Atomic::CompareExchange(pnext, (DataChunk *)0, buffer));
}

void DataBufferBase::AttachFront(DataChunk* buffer)
{
	DataChunk* next;
	do
	{
		next = _first;
		buffer->_next = next;
	}while(!Atomic::CompareExchange(&_first, next, buffer));
}

DataChunk* DataBufferBase::DetachFront()
{
	DataChunk* first, *next;
	do
	{
		first = _first;
		next = first->_next;
	}while(!Atomic::CompareExchange(&_first, first, next));
	return first;
}


bool DataBufferBase::Seek(size_t pos)
{
	DataChunk *current = _first;
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

size_t DataBufferBase::Tell()
{
	DataChunk *current = _first;
	size_t result = _pos;

	while(current && current != _current)
	{
		result += current->Size();
		current = current->Next();
	}
	return result;
}

size_t DataBufferBase::Size()
{
	DataChunk *current = _first;
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

unsigned DataBufferBase::Parts()
{
	DataChunk *current = _first;
	unsigned parts = 0;
	while(current)
	{
		++parts;
		current = current->Next();
	}

	return parts;
}