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
