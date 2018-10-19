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
#include <stddef.h>
#include <template_utils.h>

namespace Mcucpp
{
	
	template<class T, size_t Size, class IndexT=uint16_t>
	class MedianFilter
	{
		struct Node
		{
			T value;
			IndexT next;
		};
		Node _data[Size];
		IndexT _head, _tail;
		IndexT InsertPoint();
	public:
		MedianFilter();
		void Reset(const T &initialValue);
		void Append(const T &value);
		T GetValue() const;
	};
	
	template<class T, size_t Size, class IndexT=uint16_t>
	MedianFilter<T, Size, IndexT>::MedianFilter()
	{
		Reset(0);
	}
	
	IndexT InsertPoint(const T &value)
	{
		IndexT i, step, result = 0;
		IndexT count = Size;
		while(count > 0)
		{
			i = result;
			step = count / 2;
			i += step;
			if (_data[i].value < value)
			{
				result = i + 1;
				count -= step + 1;
			}
			else
			{
				count = step;
			}
		}
		return result;
	}
	
	template<class T, size_t Size, class IndexT=uint16_t>
	void MedianFilter<T, Size, IndexT>::Reset(const T &initialValue)
	{
		IndexT i = 0;
		_head = 0;
		_tail = Size - 1;
		for(auto &point : _data)
		{
			point.value = initialValue;
			point.next = ++i;
		}
	}
	
	template<class T, size_t Size, class IndexT=uint16_t>
	void MedianFilter<T, Size, IndexT>::Append(const T &value)
	{
		IndexT i = InsertPoint(value);
		swap();
	}
	
	template<class T, size_t Size, class IndexT=uint16_t>
	T MedianFilter<T, Size, IndexT>::GetValue() const
	{
		
	}
}
