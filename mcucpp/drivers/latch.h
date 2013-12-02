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
#ifndef LATCH_H
#define LATCH_H

#include "ioports.h"
namespace Mcucpp
{
	
	template<class ClockPin, class DataPin, class LatchPin, unsigned ID, class T = uint8_t>
	class ThreePinLatch :public IO::NativePortBase
	{
		public:
		
		typedef T DataT;
		enum{Id = ID};
		enum{Width=sizeof(DataT)*8};

		static void Write(T value)
		{	
			_currentValue = value;
			for(uint8_t i=0; i < Width; ++i)
			{
				DataPin::Set(value & 1);
				ClockPin::Set();
				value >>= 1;
				ClockPin::Clear();
			}
			LatchPin::Set();
			LatchPin::Clear();
		}
		static DataT Read()
		{
				return _currentValue;
		}
		static void ClearAndSet(DataT clearMask, DataT value)
		{
			Write(_currentValue = (_currentValue & ~clearMask) | value);
		}
	
		static void Set(DataT value)
		{
			Write(_currentValue |= value);
		}
		static void Clear(DataT value)
		{
			Write(_currentValue &= ~value);
		}
		static void Toggle(DataT value)
		{
			Write(_currentValue ^= value);
		}

		static DataT DirRead()
		{
			return 0xff;
		}

		template<DataT clearMask, DataT value>
		static void ClearAndSet()
		{ClearAndSet(clearMask, value);}

		template<DataT value>
		static void Toggle()
		{Toggle(value);}

		template<DataT value>
		static void Set()
		{Set(value);}

		template<DataT value>
		static void Clear()
		{Clear(value);}

		template<unsigned pin, class Configuration>
		static void SetPinConfiguration(Configuration configuration)
		{}
		template<class Configuration>
		static void SetConfiguration(DataT mask, Configuration configuration)
		{}
		
		template<DataT mask, Configuration configuration>
		static void SetConfiguration()
		{}
		
		template<Configuration configuration>
		static void SetConfiguration()
		{}
		protected:
		static DataT _currentValue;
	};

	template<class ClockPin, class DataPin, class LatchPin, unsigned ID, class T>
	T ThreePinLatch<ClockPin, DataPin, LatchPin, ID, T>::_currentValue = 0;
}
#endif
