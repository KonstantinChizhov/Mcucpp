//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
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

#include <stdint.h>
#include <stddef.h>

namespace Mcucpp
{
	struct input_iterator_tag{};
	struct output_iterator_tag{};
	struct forward_iterator_tag{};
	struct bidirectional_iterator_tag{};
	struct random_access_iterator_tag{};

	template <typename Iterator>
	struct iterator_traits
	{
		typedef typename Iterator::difference_type   difference_type;
		typedef typename Iterator::value_type        value_type;
		typedef typename Iterator::pointer           pointer;
		typedef typename Iterator::reference         reference;
		typedef typename Iterator::iterator_category iterator_category;
	};


	template <typename T>
	struct iterator_traits<T*>
	{
		typedef ptrdiff_t difference_type;
		typedef T         value_type;
		typedef T*        pointer;
		typedef T&        reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template <typename T>
	struct iterator_traits<const T*>
	{
		typedef ptrdiff_t difference_type;
		typedef T         value_type;
		typedef const T*  pointer;
		typedef const T&  reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template <>
	struct iterator_traits<void*>
	{
		typedef ptrdiff_t         difference_type;
		typedef uint8_t           value_type;
		typedef void*             pointer;
		typedef value_type&       reference;
		typedef random_access_iterator_tag iterator_category;
	};

	template <>
	struct iterator_traits<const void*>
	{
		typedef ptrdiff_t         difference_type;
		typedef uint8_t           value_type;
		typedef const void*       pointer;
		typedef const value_type& reference;
		typedef random_access_iterator_tag iterator_category;
	};


	template <class Iterator>
	class reverse_iterator
	{
	public:
		typedef Iterator iterator_type;
		typedef typename iterator_traits<Iterator>::iterator_category iterator_category;
		typedef typename iterator_traits<Iterator>::value_type        value_type;
		typedef typename iterator_traits<Iterator>::difference_type   difference_type;
		typedef typename iterator_traits<Iterator>::pointer           pointer;
		typedef typename iterator_traits<Iterator>::reference         reference;
	public:
		reverse_iterator () : _base() {}
		explicit reverse_iterator (iterator_type iter) : _base (iter) {}
		inline iterator_type base () const { return _base; }
		inline bool              operator== (const reverse_iterator& iter) const { return (_base == iter._base); }
		inline bool              operator!= (const reverse_iterator& iter) const { return (_base != iter._base); }
		inline bool              operator< (const reverse_iterator& iter) const { return (iter._base < _base); }
		inline reference         operator* () const { iterator_type prev (_base); --prev; return (*prev); }
		inline pointer           operator-> () const { return &(operator*()); }
		inline reverse_iterator& operator++ () { -- _base; return (*this); }
		inline reverse_iterator& operator-- () { ++ _base; return (*this); }
		inline reverse_iterator  operator++ (int) { reverse_iterator prev (*this); -- _base; return prev; }
		inline reverse_iterator  operator-- (int) { reverse_iterator prev (*this); ++ _base; return prev; }
		inline reverse_iterator& operator+= (size_t n) { _base -= n; return (*this); }
		inline reverse_iterator& operator-= (size_t n) { _base += n; return (*this); }
		inline reverse_iterator  operator+ (size_t n) const { return reverse_iterator (_base - n); }
		inline reverse_iterator  operator- (size_t n) const { return reverse_iterator (_base + n); }
		inline reference         operator[] (ptrdiff_t n) const { return (*(*this + n)); }
		inline difference_type   operator- (const reverse_iterator& iter) const { return _base - iter._base; }
		inline                   operator bool () const { return (bool)_base; }
	protected:
		iterator_type _base;
	};

}