//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2012
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

#ifndef MCUCPP_ARRAY_H
#define MCUCPP_ARRAY_H

#include <stdint.h>
#include <stddef.h>
#include <atomic.h>
#include <debug.h>
#include <select_size.h>
#include <iterator.h>

// TODO: STL consistent iterators begin(), end(), rbegin(), rend() and etc.

namespace Mcucpp
{
namespace Containers
{
template<size_t SIZE, class T, class Atomic = VoidAtomic>
class FixedArray
{
public:
	//typedef typename SelectSizeForLength<SIZE>::Result size_type;
	typedef size_t size_type;
	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* iterator;
	typedef Mcucpp::reverse_iterator<T*> reverse_iterator;
private:
	//value_type _data[SIZE];
	unsigned _buffer[(sizeof(value_type) * (SIZE + 1) - 1) / sizeof(unsigned)];
	value_type *_data(){return reinterpret_cast<value_type*>(_buffer);}
	const value_type *_data()const{return reinterpret_cast<const value_type*>(_buffer);}
	size_type _bottom;
public:
	FixedArray();

	size_type size() const;
	size_type max_size() const
	{
		return SIZE;
	}
	size_type capacity() const
	{
		return SIZE;
	}
	bool empty() const;
	bool full() const;
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;
	bool push_back(const T& x);
	bool push_back();
	bool pop_back();
	void clear();
	inline reference operator[](size_type i);
	inline const_reference operator[](size_type i) const;
	inline void set_all(const_reference value);
	inline void assign (size_type n, const value_type& val);
	inline bool resize(size_type sz);
	iterator insert (iterator position, const value_type& val);
    void insert (iterator position, size_type n, const value_type& val);
	iterator begin(){return _data();}
	iterator end(){return _data() + Atomic::Fetch(&_bottom);}
	reverse_iterator rbegin(){return reverse_iterator(end());}
	reverse_iterator rend(){return reverse_iterator(begin());}
};

template<size_t SIZE, class T, class Atomic>
typename FixedArray<SIZE, T, Atomic>::iterator 
	FixedArray<SIZE, T, Atomic>::insert (iterator position, const value_type& val)
{
	size_t size = Atomic::Fetch(&_bottom);
	MCUCPP_ASSERT(position >= _data() && position <=  _data() + size);
	if(size >= SIZE)
		return position;

	Atomic::FetchAndAdd(&_bottom, 1);
	iterator end = _data() + size;
	for(iterator i = end; i != position; i--)
	{
		*i = *(i - 1);
	}
	*position = val;
	return position;
}

template<size_t SIZE, class T, class Atomic>
void FixedArray<SIZE, T, Atomic>::insert (iterator position, size_type n, const value_type& val)
{
	size_t size = Atomic::Fetch(&_bottom);
	MCUCPP_ASSERT(position > _data() && position <=  _data() + size);

	if(size + n > SIZE)
		return;

	Atomic::FetchAndAdd(&_bottom, n);
	iterator end = _data() + size;
	for(iterator i = end; i != position; i--)
	{
		*i = *(i - n);
	}
	end = position + n;
	for(iterator i = position; i != end; i++)
	{
		*i = val;
	}
	return;
}

template<size_t SIZE, class T, class Atomic>
void FixedArray<SIZE, T, Atomic>::assign (size_type newSize, const_reference value)
{
	if (newSize > SIZE)
		newSize = SIZE;
	size_type oldSize;
	do
	{
		oldSize = Atomic::Fetch(&_bottom);
	} while (!Atomic::CompareExchange(&_bottom, oldSize, newSize));
	
	for (size_type i = 0; i < newSize; i++)
		_data()[i] = value;
}

template<size_t SIZE, class T, class Atomic>
void FixedArray<SIZE, T, Atomic>::set_all(const_reference value)
{
	for (size_type i = 0; i < size(); i++)
		_data()[i] = value;
}

template<size_t SIZE, class T, class Atomic>
FixedArray<SIZE, T, Atomic>::FixedArray() :
		_bottom(0)
{
}

template<size_t SIZE, class T, class Atomic>
bool FixedArray<SIZE, T, Atomic>::resize(size_type newSize)
{
	if (newSize > SIZE)
		return false;
	size_type oldSize;
	do
	{
		oldSize = Atomic::Fetch(&_bottom);
		for (size_type i = oldSize; i < newSize; i++)
			new (&_data()[i]) T();
	} while (!Atomic::CompareExchange(&_bottom, oldSize, newSize));
	return true;
}

template<size_t SIZE, class T, class Atomic>
bool FixedArray<SIZE, T, Atomic>::push_back(const T& value)
{
	if (full())
		return false;
	_data()[Atomic::FetchAndAdd(&_bottom, 1)] = value;
	return true;
}

template<size_t SIZE, class T, class Atomic>
bool FixedArray<SIZE, T, Atomic>::push_back()
{
	if (full())
		return false;
	new (&_data()[Atomic::FetchAndAdd(&_bottom, 1)]) T();
	return true;
}

template<size_t SIZE, class T, class Atomic>
bool FixedArray<SIZE, T, Atomic>::pop_back()
{
	if (empty())
		return false;
	Atomic::SubAndFetch(&_bottom, 1);
	return true;
}

template<size_t SIZE, class T, class Atomic>
const T& FixedArray<SIZE, T, Atomic>::front() const
{
	MCUCPP_ASSERT(!empty());
	return _data()[0];
}

template<size_t SIZE, class T, class Atomic>
const T& FixedArray<SIZE, T, Atomic>::back() const
{
	MCUCPP_ASSERT(!empty());
	return _data()[Atomic::Fetch(&_bottom) - 1];
}

template<size_t SIZE, class T, class Atomic>
T& FixedArray<SIZE, T, Atomic>::front()
{
	MCUCPP_ASSERT(!empty());
	return _data()[0];
}

template<size_t SIZE, class T, class Atomic>
T& FixedArray<SIZE, T, Atomic>::back()
{
	MCUCPP_ASSERT(!empty());
	return _data()[Atomic::Fetch(&_bottom) - 1];
}

template<size_t SIZE, class T, class Atomic>
T& FixedArray<SIZE, T, Atomic>::operator[](size_type i)
{
	MCUCPP_ASSERT(i < _bottom);
	return _data()[i];
}

template<size_t SIZE, class T, class Atomic>
const T& FixedArray<SIZE, T, Atomic>::operator[](size_type i) const
{
	MCUCPP_ASSERT(i < _bottom);
	return _data()[i];
}

template<size_t SIZE, class T, class Atomic>
bool FixedArray<SIZE, T, Atomic>::empty() const
{
	return Atomic::Fetch(&_bottom) == 0;
}

template<size_t SIZE, class T, class Atomic>
bool FixedArray<SIZE, T, Atomic>::full() const
{
	return Atomic::Fetch(&_bottom) == SIZE;
}

template<size_t SIZE, class T, class Atomic>
typename FixedArray<SIZE, T, Atomic>::size_type FixedArray<SIZE, T, Atomic>::size() const
{
	return Atomic::Fetch(&_bottom);
}

template<size_t SIZE, class T, class Atomic>
void FixedArray<SIZE, T, Atomic>::clear()
{
	_bottom = 0;
}

//==========================================================================================
template<size_t SIZE, class Atomic>
class FixedArray<SIZE, bool, Atomic>
{
	static const unsigned IndexMask = sizeof(unsigned) == 32 ? 0x1f : 0x0f;
	static const unsigned IndexShift = sizeof(unsigned) == 32 ? 5 : 4;
public:
	//typedef typename SelectSizeForLength<SIZE>::Result size_type;
	typedef size_t size_type;
	typedef bool value_type;
	class reference
	{
	public:
		reference(unsigned * byte, unsigned mask) :
				_byte(byte), _mask(mask)
		{
		}
		operator bool() const
		{
			return (*_byte & _mask) != 0;
		}
		reference &operator=(bool value)
		{
			if (value)
				*_byte |= _mask;
			else
				*_byte &= ~_mask;
			return *this;
		}
	private:
		unsigned * _byte;
		unsigned _mask;
	};

	typedef bool const_reference;
private:
	unsigned _data[SIZE / sizeof(unsigned) + 1];
	size_type _bottom;
public:
	FixedArray() :
			_bottom(0)
	{
	}

	size_type size() const;
	size_type max_size() const
	{
		return SIZE;
	}
	size_type capacity() const
	{
		return SIZE;
	}
	bool empty() const;
	bool full() const;
	reference front();
	const_reference front() const;
	reference back();
	const_reference back() const;
	bool push_back(bool x);
	bool pop_back();
	void clear();
	inline reference operator[](size_type i);
	inline const_reference operator[](size_type i) const;
	inline void set_all(const_reference value);
	inline void assign (size_type newSize, const_reference value);
	inline bool resize(size_type sz);
};

template<size_t SIZE, class Atomic>
void FixedArray<SIZE, bool, Atomic>::assign (size_type newSize, const_reference value)
{
	resize(newSize);
	set_all(value);
}

template<size_t SIZE, class Atomic>
void FixedArray<SIZE, bool, Atomic>::set_all(const_reference value)
{
	size_type bottom = Atomic::Fetch(&_bottom);
	size_type sz = bottom >> IndexShift;
	unsigned v = value ? unsigned(-1) : 0;
	for (size_type i = 0; i < sz; i++)
		_data[i] = v;
	unsigned mask = (1 << ((bottom & IndexMask ) + 1)) - 1;
	if (value)
		_data[sz] |= mask;
	else
		_data[sz] &= ~mask;
}

template<size_t SIZE, class Atomic>
bool FixedArray<SIZE, bool, Atomic>::resize(size_type newSize)
{
	if (newSize > SIZE)
		return false;
	size_type oldBottom;
	do
	{
		oldBottom = Atomic::Fetch(&_bottom);
	} while (!Atomic::CompareExchange(&_bottom, oldBottom, newSize));
	return true;
}

template<size_t SIZE, class Atomic>
bool FixedArray<SIZE, bool, Atomic>::push_back(bool value)
{
	if (full())
		return false;
	size_type bottom = Atomic::FetchAndAdd(&_bottom, 1);
	unsigned mask = 1 << (bottom & IndexMask);
	size_type index = bottom >> IndexShift;
	if (value)
		_data[index] |= mask;
	else
		_data[index] &= ~mask;
	return true;
}

template<size_t SIZE, class Atomic>
bool FixedArray<SIZE, bool, Atomic>::pop_back()
{
	if (empty())
		return false;
	Atomic::SubAndFetch(&_bottom, 1);
	return true;
}

template<size_t SIZE, class Atomic>
typename FixedArray<SIZE, bool, Atomic>::const_reference FixedArray<SIZE, bool,
		Atomic>::front() const
{
	MCUCPP_ASSERT(!empty());
	return reference(&_data[0], 1);
}

template<size_t SIZE, class Atomic>
typename FixedArray<SIZE, bool, Atomic>::const_reference FixedArray<SIZE, bool,
		Atomic>::back() const
{
	MCUCPP_ASSERT(!empty());
	size_type backItem = Atomic::Fetch(&_bottom) - 1;
	return reference(&_data[backItem >> IndexShift], 1 << (backItem & IndexMask));
}

template<size_t SIZE, class Atomic>
typename FixedArray<SIZE, bool, Atomic>::reference FixedArray<SIZE, bool, Atomic>::front()
{
	MCUCPP_ASSERT(!empty());
	return reference(&_data[0], 1);
}

template<size_t SIZE, class Atomic>
typename FixedArray<SIZE, bool, Atomic>::reference FixedArray<SIZE, bool, Atomic>::back()
{
	MCUCPP_ASSERT(!empty());
	size_type backItem = Atomic::Fetch(&_bottom) - 1;
	return reference(&_data[backItem >> IndexShift], 1 << (backItem & IndexMask));
}

template<size_t SIZE, class Atomic>
typename FixedArray<SIZE, bool, Atomic>::reference FixedArray<SIZE, bool, Atomic>::operator[](
		size_type i)
{
	MCUCPP_ASSERT(i < SIZE);
	return reference(&_data[i >> IndexShift], 1 << (i & IndexMask));
}

template<size_t SIZE, class Atomic>
typename FixedArray<SIZE, bool, Atomic>::const_reference FixedArray<SIZE, bool,
		Atomic>::operator[](size_type i) const
{
	MCUCPP_ASSERT(i < SIZE);
	return reference(&_data[i >> IndexShift], 1 << (i & IndexMask));
}

template<size_t SIZE, class Atomic>
bool FixedArray<SIZE, bool, Atomic>::empty() const
{
	return Atomic::Fetch(&_bottom) == 0;
}

template<size_t SIZE, class Atomic>
bool FixedArray<SIZE, bool, Atomic>::full() const
{
	return Atomic::Fetch(&_bottom) == SIZE;
}

template<size_t SIZE, class Atomic>
typename FixedArray<SIZE, bool, Atomic>::size_type FixedArray<SIZE, bool, Atomic>::size() const
{
	size_type sz = Atomic::Fetch(&_bottom);
	return sz;
}

template<size_t SIZE, class Atomic>
void FixedArray<SIZE, bool, Atomic>::clear()
{
	_bottom = 0;
}
}
}

#endif
