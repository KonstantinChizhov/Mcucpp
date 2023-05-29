//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2023
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
#include <stdint.h>
#include <stddef.h>
#include <tuple>

namespace Mcucpp
{
	template<typename ...T, size_t... I>
	auto makeReferencesHelper(std::tuple<T...>& t, std::index_sequence<I...>)
	{
		return std::tie(*std::get<I>(t)...);
	}

	template<typename ...T>
	auto makeReferences(std::tuple<T...>& t) {
		return makeReferencesHelper<T...>(t, std::make_index_sequence<sizeof...(T)>{});
	}

	template<class T, size_t ...Is>
	bool TupleAnyEqInt(const T& a, const T& b, std::index_sequence<Is...>)
	{
		return (... || (std::get<Is>(a) == std::get<Is>(b)));
	}

	template<class T>
	bool TupleAnyEq(const T& a, const T& b)
	{
		return TupleAnyEqInt(a, b, std::make_index_sequence<std::tuple_size<T>::value>{});
	}

	template<class ...ForwardIterators>
	class ZipIterator
	{
	public:

		typedef typename ptrdiff_t   difference_type;
	public:
		ZipIterator() : _iterators() {}
		explicit   ZipIterator(ForwardIterators... s)
			:_iterators(s...)
		{}

		inline bool				operator== (const ZipIterator& iter) const { return TupleAnyEq(_iterators, iter._iterators); }
		inline bool				operator!= (const ZipIterator& iter) const { return !TupleAnyEq(_iterators, iter._iterators); }

		inline auto operator* () {
			return makeReferences(_iterators);
		}

		inline ZipIterator& operator++ () { std::apply(&Increment, _iterators); return (*this); }
	protected:

		static void Increment(ForwardIterators&... s)
		{
			(++s, ...);
		}

		std::tuple<ForwardIterators...> _iterators;
	};

	template<class ...T>
	class ZipRange
	{
		ZipIterator<T...> _begin, _end;
	public:

		template<class ...TT>
		ZipRange(TT &&...a)
			:_begin(std::begin(a)...),
			_end(std::end(a)...)
		{

		}

		auto begin() { return _begin; }
		auto end() { return _end; }
	};

	template<class ...T>
	auto Zip(T &&...a)
	{
		return ZipRange<decltype(std::begin(a))...>(std::forward<T>(a)...);
	}
}