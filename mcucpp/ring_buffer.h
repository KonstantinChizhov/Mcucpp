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

#include <static_assert.h>
#include <stddef.h>
#include <atomic.h>
#include <debug.h>
#include <select_size.h>

// TODO: STL consistent iterators begin(), end(), rbegin(), rend() and etc.

namespace Mcucpp
{
	namespace Containers
	{
		// RingBufferPO2 is slightly faster than RingBuffer, but limited to sizes of power of 2
		template<size_t SIZE, class T, class Atomic = VoidAtomic>
		class RingBufferPO2
		{
		public:
			typedef typename SelectSizeForLength<SIZE>::Result size_type;
			typedef T value_type;
			typedef T& reference;
			typedef const T& const_reference;
		private:
			STATIC_ASSERT((SIZE&(SIZE-1))==0);//SIZE must be a power of 2
			unsigned _buffer[(sizeof(value_type) * (SIZE + 1) - 1) / sizeof(unsigned)];
			value_type *_data(){return reinterpret_cast<value_type*>(_buffer);}
			const value_type *_data()const{return reinterpret_cast<const value_type*>(_buffer);}
			size_type _writeCount;
			size_type _readCount;
			static const size_type _mask = SIZE - 1;
		public:

			RingBufferPO2();

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
			bool push_back();
			bool pop_front();
			void clear();
			inline reference operator[] (size_type i);
			inline const_reference operator[] (size_type i)const;
		};

		template<size_t SIZE, class T, class Atomic>
		RingBufferPO2<SIZE, T, Atomic>::RingBufferPO2()
			:_writeCount(0),  _readCount(0)
		{
		}

		template<size_t SIZE, class T, class Atomic>
		bool RingBufferPO2<SIZE, T, Atomic>::push_back(const T& value)
		{
			if(full())
				return 0;
			_data()[_writeCount++ & _mask] = value;
			return true;
		}
		
		template<size_t SIZE, class T, class Atomic>
		bool RingBufferPO2<SIZE, T, Atomic>::push_back()
		{
			if(full())
				return 0;
			new(&_data()[_writeCount++ & _mask]) T();
			return true;
		}

		template<size_t SIZE, class T, class Atomic>
		bool RingBufferPO2<SIZE, T, Atomic>::pop_front()
		{
			if(empty())
				return false;
			_readCount++;
			return true;
		}

		template<size_t SIZE, class T, class Atomic>
		const T& RingBufferPO2<SIZE, T, Atomic>::front()const
		{
			MCUCPP_ASSERT(!empty());
			return _data()[_readCount & _mask];
		}

		template<size_t SIZE, class T, class Atomic>
		const T& RingBufferPO2<SIZE, T, Atomic>::back()const
		{
			MCUCPP_ASSERT(!empty());
			return _data()[(_writeCount-1) & _mask];
		}

		template<size_t SIZE, class T, class Atomic>
		T& RingBufferPO2<SIZE, T, Atomic>::front()
		{
			MCUCPP_ASSERT(!empty());
			return _data()[_readCount & _mask];
		}

		template<size_t SIZE, class T, class Atomic>
		T& RingBufferPO2<SIZE, T, Atomic>::back()
		{
			MCUCPP_ASSERT(!empty());
			return _data()[(_writeCount-1) & _mask];
		}

		template<size_t SIZE, class T, class Atomic>
		T& RingBufferPO2<SIZE, T, Atomic>::operator[] (size_type i)
		{
			MCUCPP_ASSERT(i < SIZE);
			return _data()[(_readCount + i) & _mask];
		}

		template<size_t SIZE, class T, class Atomic>
		const T& RingBufferPO2<SIZE, T, Atomic>::operator[] (size_type i)const
		{
			MCUCPP_ASSERT(i < SIZE);
			return _data()[(_readCount + i) & _mask];
		}

		template<size_t SIZE, class T, class Atomic>
		bool RingBufferPO2<SIZE, T, Atomic>::empty()const
		{
			return Atomic::Fetch(&_writeCount) == _readCount;
		}

		template<size_t SIZE, class T, class Atomic>
		bool RingBufferPO2<SIZE, T, Atomic>::full()const
		{
			return ((_writeCount - Atomic::Fetch(&_readCount)) &
					(size_type)~(_mask)) != 0;
		}

		template<size_t SIZE, class T, class Atomic>
		typename RingBufferPO2<SIZE, T, Atomic>::size_type RingBufferPO2<SIZE,T, Atomic>::size()const
		{
			return (Atomic::Fetch(&_writeCount) - Atomic::Fetch(&_readCount));
		}

		template<size_t SIZE, class T, class Atomic>
		void RingBufferPO2<SIZE, T, Atomic>::clear()
		{
			_readCount=0;
			_writeCount=0;
		}

	//=====================================================================================

		template<size_t SIZE, class T=unsigned char, class Atomic = VoidAtomic>
		class RingBuffer
		{
		public:
			typedef typename SelectSizeForLength<SIZE>::Result size_type;
			typedef T value_type;
			typedef T& reference;
			typedef const T& const_reference;
		private:
			size_type  _count;
			size_type  _first;
			size_type  _last;
			unsigned _buffer[(sizeof(value_type) * (SIZE + 1) - 1) / sizeof(unsigned)];
			value_type *_data(){return reinterpret_cast<value_type*>(_buffer);}
			const value_type *_data()const{return reinterpret_cast<const value_type*>(_buffer);}
		public:
			RingBuffer() : _count(0), _first(0), _last(0) { }

			size_type size()const {return Atomic::Fetch(&_count);}
			size_type max_size()const {return SIZE;}
			size_type capacity()const {return SIZE;}
			bool empty()const;
			bool full()const;
			reference front();
			const_reference front()const;
			reference back();
			const_reference back()const;
			bool push_back(const T& x);
			bool push_back();
			bool pop_front();
			void clear();
			inline reference operator[] (size_type i);
			inline const_reference operator[] (size_type i)const;
		};

		template<size_t SIZE, class T, class Atomic>
		T& RingBuffer<SIZE, T, Atomic>::front()
		{
			MCUCPP_ASSERT(!empty());
			return _data()[_first];
		}

		template<size_t SIZE, class T, class Atomic>
		bool RingBuffer<SIZE, T, Atomic>::empty()const
		{
			return Atomic::Fetch(&_count) == 0;
		}

		template<size_t SIZE, class T, class Atomic>
		bool RingBuffer<SIZE, T, Atomic>::full()const
		{
			return Atomic::Fetch(&_count) == SIZE;
		}

		template<size_t SIZE, class T, class Atomic>
		void RingBuffer<SIZE, T, Atomic>::clear()
		{
			do{
				_first = _last = 0;
			}while(!Atomic::CompareExchange(&_count, _count, size_type(0)));
		}

		template<size_t SIZE, class T, class Atomic>
		const T& RingBuffer<SIZE, T, Atomic>::front()const
		{
			MCUCPP_ASSERT(!empty());
			return _data()[_first];
		}

		template<size_t SIZE, class T, class Atomic>
		T& RingBuffer<SIZE, T, Atomic>::back()
		{
			MCUCPP_ASSERT(!empty());
			return _data()[_last-1];
		}

		template<size_t SIZE, class T, class Atomic>
		const T& RingBuffer<SIZE, T, Atomic>::back()const
		{
			MCUCPP_ASSERT(!empty());
			return _data()[_last-1];
		}

		template<size_t SIZE, class T, class Atomic>
		T& RingBuffer<SIZE, T, Atomic>::operator[](size_type index)
		{
			MCUCPP_ASSERT(index < SIZE);
			size_type offset = _first + index;
			if(offset >= SIZE)
				offset -= SIZE;
			return _data()[offset];
		}

		template<size_t SIZE, class T, class Atomic>
		const T& RingBuffer<SIZE, T, Atomic>::operator[](size_type index)const
		{
			MCUCPP_ASSERT(index < SIZE);
			size_type offset = _first + index;
			if(offset >= SIZE)
				offset -= SIZE;
			return _data()[offset];
		}
		
		template<size_t SIZE, class T, class Atomic>
		bool RingBuffer<SIZE, T, Atomic>::push_back()
		{
			if(Atomic::Fetch(&_count) == SIZE)
				return false;
			new (&_data()[_last]) T();
			_last++;
			if(_last >= SIZE)
				_last = 0;
			Atomic::FetchAndAdd(&_count, 1);
			return true;
		}

		template<size_t SIZE, class T, class Atomic>
		bool RingBuffer<SIZE, T, Atomic>::push_back(const T& item)
		{
			if(Atomic::Fetch(&_count) == SIZE)
				return false;
			_data()[_last] = item;
			_last++;
			if(_last >= SIZE)
				_last = 0;
			Atomic::FetchAndAdd(&_count, 1);
			return true;
		}

		template<size_t SIZE, class T, class Atomic>
		bool RingBuffer<SIZE, T, Atomic>::pop_front()
		{
			if(Atomic::Fetch(&_count) != 0)
			{
				Atomic::FetchAndSub(&_count, 1);
				_first++;
				if(_first >= SIZE)
					_first = 0;
				return true;
			}
			else
				return false;
		}
	}
}

