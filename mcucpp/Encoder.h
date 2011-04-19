#pragma once
#include <static_assert.h>

template<class DataT, class Pins1, class Pins2>
class Encoder
{
	BOOST_STATIC_ASSERT(Pins1::Length == Pins2::Length);
public:
	typedef typename Pins1::DataType PortType;
	static const unsigned Channels = Pins1::Length;

	static void CaptureHandler()
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
			if(fwd & 1)
				 (*ptr) ++;
			else 
			if(back & 1)
				(*ptr) --;
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
