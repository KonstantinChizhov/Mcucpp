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

namespace Mcucpp
{

template<class DataT, class Pins1, class Pins2>
class Encoder
{
	STATIC_ASSERT(Pins1::Length == Pins2::Length);
public:
	typedef typename Pins1::DataType PortType;
	static const unsigned Channels = Pins1::Length;

	static void Init()
	{
		Pins1::template SetConfiguration<Pins1::PullUpOrDownIn>();
		Pins2::template SetConfiguration<Pins1::PullUpOrDownIn>();
		Pins1::template Write<typename Pins1::DataT(-1)>();
		Pins2::template Write<typename Pins2::DataT(-1)>();
	}

	static inline  void CaptureHandler()
	{
		PortType y1 = Pins1::PinRead();
		PortType y2 = Pins2::PinRead();

		PortType fwd  = Detect(_x1, _x2, y1, y2);
		PortType back = Detect(_x2, _x1, y2, y1);

		_x1 = y1;
		_x2 = y2;

		volatile DataT * ptr = _value;
		for(uint8_t i = Channels; i; --i)
		{
			//if(fwd & 1)
			//	 (*ptr) ++;
			//else 
			//if(back & 1)
			//	(*ptr) --;
			*ptr = *ptr + (fwd & 1) - (back & 1);
			ptr++;
			fwd >>= 1;
			back >>= 1;
		}
	}

	static DataT Value(unsigned  index)
	{
		return _value[index];
	}
private:
	static inline PortType Detect(PortType x1, PortType x2, PortType y1, PortType y2) 
	{
		//return ~(x1 | y2) & (x2 ^ y1) | x1 & y2 & ~(x2 & y1);
		return (x2 ^ y1) & ~(x1 ^ y2);
	}

	static volatile DataT _value[Pins1::Length];
	static PortType _x1, _x2;
};

	template<class DataT, class Pins1, class Pins2>
	volatile DataT Encoder<DataT, Pins1, Pins2>::_value[Pins1::Length];

	template<class DataT, class Pins1, class Pins2>
	typename Encoder<DataT, Pins1, Pins2>::PortType Encoder<DataT, Pins1, Pins2>::_x1;

	template<class DataT, class Pins1, class Pins2>
	typename Encoder<DataT, Pins1, Pins2>::PortType Encoder<DataT, Pins1, Pins2>::_x2;
}
