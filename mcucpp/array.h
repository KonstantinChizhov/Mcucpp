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

#include <stddef.h>
#include <atomic.h>
#include <debug.h>
#include <select_size.h>

// TODO: STL consistent iterators begin(), end(), rbegin(), rend() and etc.

namespace Mcucpp
{
namespace Containers
{
	template<size_t SIZE, class T, class Atomic = VoidAtomic>
	class FixedArray
	{
	public:
		typedef typename SelectSizeForLength<SIZE>::Result size_type;
		typedef T value_type;
		typedef T& reference;
		typedef const T& const_reference;
	private:
		value_type _data[SIZE];
		volatile size_type _bottom;
	public:
		FixedArray();

		size_type size()const;
		size_type max_size()const {return SIZE;}
		size_type capacity()const {return SIZE;}
		bool empty()const;
		bool full()const;
		reference front();
		const_reference front()const;
		reference back();
		const_reference back()const;
		bool push_back(const T& x);
		bool pop_back();
		void clear();
		inline reference operator[] (size_type i);
		inline const_reference operator[] (size_type i)const;
	};

	template<size_t SIZE, class T, class Atomic>
	FixedArray<SIZE, T, Atomic>::FixedArray()
		:_bottom(0)
	{}

	template<size_t SIZE, class T, class Atomic>
	bool FixedArray<SIZE, T, Atomic>::push_back(const T& value)
	{
		if(full())
			return 0;
		_data[Atomic::FetchAndAdd(&_bottom, 1)] = value;
		return true;
	}

	template<size_t SIZE, class T, class Atomic>
	bool FixedArray<SIZE, T, Atomic>::pop_back()
	{
		if(empty())
			return false;
		Atomic::SubAndFetch(&_bottom, 1);
		return true;
	}

	template<size_t SIZE, class T, class Atomic>
	const T& FixedArray<SIZE, T, Atomic>::front()const
	{
		MCUCPP_ASSERT(!empty());
		return _data[0];
	}

	template<size_t SIZE, class T, class Atomic>
	const T& FixedArray<SIZE, T, Atomic>::back()const
	{
		MCUCPP_ASSERT(!empty());
		return _data[Atomic::Fetch(&_bottom) - 1];
	}

	template<size_t SIZE, class T, class Atomic>
	T& FixedArray<SIZE, T, Atomic>::front()
	{
		MCUCPP_ASSERT(!empty());
		return _data[0];
	}

	template<size_t SIZE, class T, class Atomic>
	T& FixedArray<SIZE, T, Atomic>::back()
	{
		MCUCPP_ASSERT(!empty());
		return _data[Atomic::Fetch(&_bottom) - 1];
	}

	template<size_t SIZE, class T, class Atomic>
	T& FixedArray<SIZE, T, Atomic>::operator[] (size_type i)
	{
		MCUCPP_ASSERT(i < SIZE);
		return _data[i];
	}

	template<size_t SIZE, class T, class Atomic>
	const T& FixedArray<SIZE, T, Atomic>::operator[] (size_type i)const
	{
		MCUCPP_ASSERT(i < SIZE);
		return _data[i];
	}

	template<size_t SIZE, class T, class Atomic>
	bool FixedArray<SIZE, T, Atomic>::empty()const
	{
		return Atomic::Fetch(&_bottom) == 0;
	}

	template<size_t SIZE, class T, class Atomic>
	bool FixedArray<SIZE, T, Atomic>::full()const
	{
		return Atomic::Fetch(&_bottom) == SIZE;
	}

	template<size_t SIZE, class T, class Atomic>
	typename FixedArray<SIZE, T, Atomic>::size_type FixedArray<SIZE,T, Atomic>::size()const
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
	public:
		typedef typename SelectSizeForLength<SIZE>::Result size_type;
		typedef bool value_type;
		class reference
		{
		public:
			reference(unsigned char* byte, unsigned char mask)
			:_byte(byte), _mask(mask)
			{}
			operator bool() const
			{
				return (*_byte & _mask) != 0;
			}
			reference &operator=(bool value)
			{
				if(value)
					*_byte |= _mask;
				else
					*_byte &= ~_mask;
				return *this;
			}
		private:
			unsigned char* _byte;
			unsigned char _mask;
		};

		typedef const bool const_reference;
	private:
		unsigned char _data[SIZE / 8 + 1];
		volatile size_type _bottom;
	public:
		FixedArray();

		size_type size()const;
		size_type max_size()const {return SIZE;}
		size_type capacity()const {return SIZE;}
		bool empty()const;
		bool full()const;
		reference front();
		const_reference front()const;
		reference back();
		const_reference back()const;
		bool push_back(bool x);
		bool pop_back();
		void clear();
		inline reference operator[] (size_type i);
		inline const_reference operator[] (size_type i)const;
	};

	template<size_t SIZE, class Atomic>
	FixedArray<SIZE, bool, Atomic>::FixedArray()
		:_bottom(0)
	{}

	template<size_t SIZE, class Atomic>
	bool FixedArray<SIZE, bool, Atomic>::push_back(const bool value)
	{
		if(full())
			return false;
		size_type bottom = Atomic::FetchAndAdd(&_bottom, 1);
		unsigned char mask = 1 << (bottom & 7);
		size_type index = bottom >> 3;
		if(value)
			_data[index] |= mask;
		else
			_data[index] &= ~mask;
		return true;
	}

	template<size_t SIZE, class Atomic>
	bool FixedArray<SIZE, bool, Atomic>::pop_back()
	{
		if(empty())
			return false;
		Atomic::SubAndFetch(&_bottom, 1);
		return true;
	}

	template<size_t SIZE, class Atomic>
	typename FixedArray<SIZE, bool, Atomic>::const_reference FixedArray<SIZE, bool, Atomic>::front()const
	{
		MCUCPP_ASSERT(!empty());
		return reference(&_data[0], 1);
	}

	template<size_t SIZE, class Atomic>
	typename FixedArray<SIZE, bool, Atomic>::const_reference FixedArray<SIZE, bool, Atomic>::back()const
	{
		MCUCPP_ASSERT(!empty());
		size_type back = Atomic::Fetch(&_bottom) - 1;
		return reference(&_data[back >> 3], 1 << (back & 7));
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
		size_type back = Atomic::Fetch(&_bottom) - 1;
		return reference(&_data[back >> 3], 1 << (back & 7));
	}

	template<size_t SIZE, class Atomic>
	typename FixedArray<SIZE, bool, Atomic>::reference FixedArray<SIZE, bool, Atomic>::operator[] (size_type i)
	{
		MCUCPP_ASSERT(i < SIZE);
		return reference(&_data[i >> 3], 1 << (i & 7));
	}

	template<size_t SIZE, class Atomic>
	typename FixedArray<SIZE, bool, Atomic>::const_reference FixedArray<SIZE, bool, Atomic>::operator[] (size_type i)const
	{
		MCUCPP_ASSERT(i < SIZE);
		return reference(&_data[i >> 3], 1 << (i & 7));
	}

	template<size_t SIZE, class Atomic>
	bool FixedArray<SIZE, bool, Atomic>::empty()const
	{
		return Atomic::Fetch(&_bottom) == 0;
	}

	template<size_t SIZE, class Atomic>
	bool FixedArray<SIZE, bool, Atomic>::full()const
	{
		return Atomic::Fetch(&_bottom) == SIZE;
	}

	template<size_t SIZE, class Atomic>
	typename FixedArray<SIZE, bool, Atomic>::size_type FixedArray<SIZE,bool, Atomic>::size()const
	{
		return Atomic::Fetch(&_bottom);
	}

	template<size_t SIZE, class Atomic>
	void FixedArray<SIZE, bool, Atomic>::clear()
	{
		_bottom = 0;
	}
}
}

#endif
