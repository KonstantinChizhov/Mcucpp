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

#ifndef MCUCPP_STACK_H
#define MCUCPP_STACK_H

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
		class FixedStack
		{
		public:
			typedef typename SelectSizeForLength<SIZE>::Result size_type;
			typedef T value_type;
			typedef T& reference;
			typedef const T& const_reference;
		private:
			unsigned _buffer[(sizeof(value_type) * (SIZE + 1) - 1) / sizeof(unsigned)];
			value_type *_data(){return reinterpret_cast<value_type*>(_buffer);}
			const value_type *_data()const{return reinterpret_cast<const value_type*>(_buffer);}
			size_type _top;
		public:

			FixedStack();

			size_type size()const;
			size_type max_size()const {return SIZE;}
			size_type capacity()const {return SIZE;}
			bool empty()const;
			bool full()const;
			reference front();
			const_reference front()const;
			reference back();
			const_reference back()const;
			bool push_front(const T& x);
			bool pop_front();
			void clear();
			inline reference operator[] (size_type i);
			inline const_reference operator[] (size_type i)const;
		};

		template<size_t SIZE, class T, class Atomic>
		FixedStack<SIZE, T, Atomic>::FixedStack()
			:_top(SIZE)
		{
		}

		template<size_t SIZE, class T, class Atomic>
		bool FixedStack<SIZE, T, Atomic>::push_front(const T& value)
		{
			if(full())
				return 0;
			_data()[Atomic::SubAndFetch(&_top, 1)] = value;
			return true;
		}

		template<size_t SIZE, class T, class Atomic>
		bool FixedStack<SIZE, T, Atomic>::pop_front()
		{
			if(empty())
				return false;
			Atomic::AddAndFetch(&_top, 1);
			return true;
		}

		template<size_t SIZE, class T, class Atomic>
		const T& FixedStack<SIZE, T, Atomic>::front()const
		{
			MCUCPP_ASSERT(!empty());
			return _data()[Atomic::Fetch(&_top)];
		}

		template<size_t SIZE, class T, class Atomic>
		const T& FixedStack<SIZE, T, Atomic>::back()const
		{
			MCUCPP_ASSERT(!empty());
			return _data()[SIZE-1];
		}

		template<size_t SIZE, class T, class Atomic>
		T& FixedStack<SIZE, T, Atomic>::front()
		{
			MCUCPP_ASSERT(!empty());
			return _data()[Atomic::Fetch(&_top)];
		}

		template<size_t SIZE, class T, class Atomic>
		T& FixedStack<SIZE, T, Atomic>::back()
		{
			MCUCPP_ASSERT(!empty());
			return _data()[SIZE-1];
		}

		template<size_t SIZE, class T, class Atomic>
		T& FixedStack<SIZE, T, Atomic>::operator[] (size_type i)
		{
			MCUCPP_ASSERT(i < SIZE);
			return _data()[Atomic::Fetch(&_top) + i];
		}

		template<size_t SIZE, class T, class Atomic>
		const T& FixedStack<SIZE, T, Atomic>::operator[] (size_type i)const
		{
			MCUCPP_ASSERT(i < SIZE);
			return _data()[Atomic::Fetch(&_top) + i];
		}

		template<size_t SIZE, class T, class Atomic>
		bool FixedStack<SIZE, T, Atomic>::empty()const
		{
			return Atomic::Fetch(&_top) == SIZE;
		}

		template<size_t SIZE, class T, class Atomic>
		bool FixedStack<SIZE, T, Atomic>::full()const
		{
			return Atomic::Fetch(&_top) == 0;
		}

		template<size_t SIZE, class T, class Atomic>
		typename FixedStack<SIZE, T, Atomic>::size_type FixedStack<SIZE,T, Atomic>::size()const
		{
			return SIZE - Atomic::Fetch(&_top);
		}

		template<size_t SIZE, class T, class Atomic>
		void FixedStack<SIZE, T, Atomic>::clear()
		{
			_top = SIZE;
		}
	}
}

#endif
