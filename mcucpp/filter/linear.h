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
#include <algorithm>
#include <numeric>
#include <iterator>

namespace Mcucpp
{
	template <class DataT, class AFactorsT, class BFactorsT>
	class LinearFilter
	{
		typedef typename Util::HiResType<DataT>::Result AccumT;
		AFactorsT _a;
		BFactorsT _b;
		AccumT _scale;
		
	public:
		template<class T>
		LinearFilter(std::initializer_list<T> a, std::initializer_list<T> b)
			:_a(a),
			_b(b)
		{
			_scale = std::accumulate(std::begin(_a), std::end(_a), AccumT(0));
			_scale += std::accumulate(std::begin(_b), std::end(_b), AccumT(0));
		}
		
		template<class InputIterator, class OutputIterator>
		void Filter(InputIterator first, InputIterator last, OutputIterator dest)
		{
			InputIterator current = first;
			OutputIterator out = dest;
			auto asize = std::distance(std::begin(_a), std::end(_a));
			auto bsize = std::distance(std::begin(_b), std::end(_b));
			
			while (current != last)
			{
				AccumT aAccum = std::inner_product(std::max(first, current - asize + 1), current + 1, std::begin(_a), AccumT(0));
				AccumT bAccum = std::inner_product(std::max(dest, out - bsize), out, std::begin(_b), AccumT(0));
				
				*out = static_cast<DataT>( (bAccum + aAccum) / _scale);
				std::cout << std::dec << *current << "\t" <<  bAccum << "\t" << aAccum << "\t" << *out << std::endl;
				
				++current;
				++out;
			}
		}
	};
}
