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

// TODO: STL consistent iterators begin(), end(), rbegin(), rend() and etc.

namespace Mcucpp
{
namespace Containers
{
	template<size_t SIZE, class T, class Atomic = VoidAtomic>
	class FixedArray
	{
	public:
		typedef size_t size_type;
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
		return _data[0];
	}

	template<size_t SIZE, class T, class Atomic>
	const T& FixedArray<SIZE, T, Atomic>::back()const
	{
		return _data[Atomic::Fetch(&_bottom) - 1];
	}

	template<size_t SIZE, class T, class Atomic>
	T& FixedArray<SIZE, T, Atomic>::front()
	{
		return _data[0];
	}

	template<size_t SIZE, class T, class Atomic>
	T& FixedArray<SIZE, T, Atomic>::back()
	{
		return _data[Atomic::Fetch(&_bottom) - 1];
	}

	template<size_t SIZE, class T, class Atomic>
	T& FixedArray<SIZE, T, Atomic>::operator[] (size_type i)
	{
		return _data[i];
	}

	template<size_t SIZE, class T, class Atomic>
	const T& FixedArray<SIZE, T, Atomic>::operator[] (size_type i)const
	{
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
}
}

#endif
