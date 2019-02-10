//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2018
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
#include <iterator.h>

namespace Mcucpp
{
	namespace Containers
	{
		constexpr ptrdiff_t dynamic_extent = -1;
		
		template<class T, ptrdiff_t Extent = dynamic_extent>
		class span
		{
		public:
			typedef T value_type;
			typedef T& reference;
			typedef const T& const_reference;
			typedef size_t size_type;
			typedef ptrdiff_t index_type;
			typedef T* iterator;
			typedef T* pointer;
			typedef Mcucpp::reverse_iterator<T*> reverse_iterator;
		private:
			pointer _data;
			size_t _size;
		public:

			span() noexcept :_data(nullptr), _size(0){}
			constexpr span(pointer ptr, index_type count) noexcept :_data(ptr), _size(count){}
			constexpr span(pointer firstElem, pointer lastElem) noexcept :_data(firstElem), _size(lastElem - firstElem){}
			template <std::size_t N>
			constexpr span(value_type (&arr)[N]) noexcept :_data(arr), _size(N){}

			template <class ContainerT>
			constexpr span(ContainerT &container) noexcept :_data(container.begin()), _size(container.size()) {}
			
			size_type size()const {return _size;}
			size_type size_bytes()const {return _size * sizeof(T);}
			size_type max_size()const {return _size;}
			bool empty()const {return _size == 0;}
			reference front(){return *_data;}
			const_reference front()const{return *_data;}
			reference back(){return *(_data + _size - 1);}
			const_reference back()const{return *(_data + _size - 1);}
			iterator begin(){return _data;}
			iterator end(){return _data + _size;}
			reverse_iterator rbegin(){return reverse_iterator(end());}
			reverse_iterator rend(){return reverse_iterator(begin());}
			
			pointer data(){ return _data; }
			pointer data()const { return _data; }
			inline reference operator[] (index_type i){return _data[i];}
			inline const_reference operator[] (index_type i)const{return _data[i];}
		};

	}
}

