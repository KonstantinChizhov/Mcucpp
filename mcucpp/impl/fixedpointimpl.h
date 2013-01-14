#include <stdlib.h>

namespace Mcucpp
{

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>::FixedPoint()
	:_data(DataT(0))
	{

	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>::FixedPoint(int integer)
	:_data(DataT(integer) << FractionalBits)
	{

	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>::FixedPoint(float value)
	:_data(value * RawOne)
	{
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>::FixedPoint(double value)
	:_data(value * RawOne)
	{
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>::FixedPoint(unsigned integer)
	:_data(DataT(integer) << FractionalBits)
	{

	}

	//template<class T, size_t IntegerBits>
	//FixedPoint<T, IntegerBits>::FixedPoint(int integer, unsigned fract)
	//:_data((DataT(integer) << FractionalBits) + (fract >> (sizeof(fract)*8 - FractionalBits)))
	//{
	//
	//}
//------------------------------------------------------------------------------------------------------------------------------
	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> FixedPoint<T, IntegerBits>::operator+(int rhs)const
	{
		FixedPoint result;
		result._data = _data + (DataT(rhs) << FractionalBits);
		return result;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> FixedPoint<T, IntegerBits>::operator+(const FixedPoint& rhs)const
	{
		FixedPoint result;
		result._data = _data + rhs._data;
		return result;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>& FixedPoint<T, IntegerBits>::operator+=(int rhs)
	{
		_data += (DataT(rhs) << FractionalBits);
		return *this;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>& FixedPoint<T, IntegerBits>::operator+=(const FixedPoint& rhs)
	{
		_data += rhs._data;
		return *this;
	}
//------------------------------------------------------------------------------------------------------------------------------
	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> FixedPoint<T, IntegerBits>::operator-()const
	{
		FixedPoint result;
		result._data = 0 - _data;
		return result;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> FixedPoint<T, IntegerBits>::operator-(int rhs)const
	{
		FixedPoint result;
		result._data = _data - (DataT(rhs) << FractionalBits);
		return result;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> FixedPoint<T, IntegerBits>::operator-(const FixedPoint& rhs)const
	{
		FixedPoint result;
		result._data = _data - rhs._data;
		return result;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>& FixedPoint<T, IntegerBits>::operator-=(int rhs)
	{
		_data -= (DataT(rhs) << FractionalBits);
		return *this;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>& FixedPoint<T, IntegerBits>::operator-=(const FixedPoint& rhs)
	{
		_data -= rhs._data;
		return *this;
	}
//------------------------------------------------------------------------------------------------------------------------------
	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> FixedPoint<T, IntegerBits>::operator*(int rhs)const
	{
		FixedPoint result;
		result._data = _data * DataT(rhs);
		return result;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> FixedPoint<T, IntegerBits>::operator*(const FixedPoint& rhs)const
	{
		FixedPoint result;
		if(0)
		{
			const DataT inta = IntegerPart();
			const DataT intb = rhs.IntegerPart();
			const DataT fracta = FractionalPart();
			const DataT fractb = rhs.FractionalPart();
			result._data = (inta * intb << FractionalBits) + (inta * fractb + fracta * intb) + (fracta * fractb >> FractionalBits);
		}
		else
		{
			typedef typename Util::HiResType<T>::Result hi_t;
			hi_t product = hi_t(_data) * rhs._data;
			result._data = (T)(product >> FractionalBits);
			result._data += ((T)product & 0x8000) >> 15;
		}
		return result;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>& FixedPoint<T, IntegerBits>::operator*=(int rhs)
	{
		_data *= DataT(rhs);
		return *this;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>& FixedPoint<T, IntegerBits>::operator*=(const FixedPoint& rhs)
	{
		const DataT inta = IntegerPart();
		const DataT intb = rhs.IntegerPart();
		const DataT fracta = FractionalPart();
		const DataT fractb = rhs.FractionalPart();
		_data = (inta * intb << FractionalBits) + (inta * fractb + fracta * intb) + (fracta * fractb >> FractionalBits);
		return *this;
	}

//------------------------------------------------------------------------------------------------------------------------------
	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> FixedPoint<T, IntegerBits>::operator/(int rhs)const
	{
		FixedPoint result;
		result._data = _data / DataT(rhs);
		return result;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> FixedPoint<T, IntegerBits>::operator/(const FixedPoint& rhs)const
	{
		FixedPoint result(*this);
		return result /= rhs;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>& FixedPoint<T, IntegerBits>::operator/=(int rhs)
	{
		_data /= DataT(rhs);
		return *this;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits>& FixedPoint<T, IntegerBits>::operator/=(const FixedPoint& rhs)
	{
		uint_fast8_t sign = 0;
		if(_data < 0)
		{
			_data = -_data;
			sign = ~sign;
		}
		uint32_t r = _data >> IntegerBits, q = _data << FractionalBits, d = rhs._data;
		if(d < 0)
		{
			d = -d;
			sign = ~sign;
		}
		uint_fast8_t carry = 0;
		for (uint_fast8_t i = 0; i < 32; i++)
		{
			if(r & 0x80000000)
				carry = 1;
			else
				carry = 0;
			r <<= 1;
			if (q & 0x80000000)
				r |= 1;
			q <<= 1;
			if (r >= d || carry)
			{
				q = q | 1;
				r -= d;
			}
		}
		if(sign)
			q = -q;
		_data = q;
  		return *this;
	}
//------------------------------------------------------------------------------------------------------------------------------

	template<class T, size_t IntegerBits>
	int FixedPoint<T, IntegerBits>::ToInt()const
	{
		return (_data & IntegerPartMask) >> FractionalBits;
	}

	template<class T, size_t IntegerBits>
	long FixedPoint<T, IntegerBits>::ToLong()const
	{
		return (_data & IntegerPartMask) >> FractionalBits;
	}

	template<class T, size_t IntegerBits>
	long long FixedPoint<T, IntegerBits>::ToLongLong()const
	{
		return (_data & IntegerPartMask) >> FractionalBits;
	}

	template<class T, size_t IntegerBits>
	float FixedPoint<T, IntegerBits>::ToFloat()const
	{
		const float rawoner = 1.f / RawOne;
		return float(_data) * rawoner;
	}

	template<class T, size_t IntegerBits>
	double FixedPoint<T, IntegerBits>::ToDouble()const
	{
		const double rawoner = 1.0 / RawOne;
		return double(_data) * rawoner;
	}

	template<class T, size_t IntegerBits>
	T FixedPoint<T, IntegerBits>::IntegerPart()const
	{
		return DataT(_data & IntegerPartMask) >> FractionalBits;
	}

	template<class T, size_t IntegerBits>
	typename FixedPoint<T, IntegerBits>::UnsignedT FixedPoint<T, IntegerBits>::FractionalPart()const
	{
		return UnsignedT(_data) & FractionalPartMask;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> operator+(int lhs, const FixedPoint<T, IntegerBits>& rhs)
	{
		return rhs + lhs;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> operator-(int lhs, const FixedPoint<T, IntegerBits>& rhs)
	{
		return FixedPoint<T, IntegerBits>(lhs) - rhs;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> operator*(int lhs, const FixedPoint<T, IntegerBits>& rhs)
	{
		return rhs * lhs;
	}

	template<class T, size_t IntegerBits>
	FixedPoint<T, IntegerBits> operator/(int lhs, const FixedPoint<T, IntegerBits>& rhs)
	{
		return FixedPoint<T, IntegerBits>(lhs) / rhs;
	}

	template<class T, size_t IntegerBits>
	bool FixedPoint<T, IntegerBits>::operator==(const FixedPoint& rhs)const
	{
		return _data == rhs._data;
	}

	template<class T, size_t IntegerBits>
	bool FixedPoint<T, IntegerBits>::operator>(const FixedPoint& rhs)const
	{
		return _data > rhs._data;
	}

	template<class T, size_t IntegerBits>
	bool FixedPoint<T, IntegerBits>::operator>=(const FixedPoint& rhs)const
	{
		return _data >= rhs._data;
	}

	template<class T, size_t IntegerBits>
	bool FixedPoint<T, IntegerBits>::operator<(const FixedPoint& rhs)const
	{
		return _data < rhs._data;
	}

	template<class T, size_t IntegerBits>
	bool FixedPoint<T, IntegerBits>::operator<=(const FixedPoint& rhs)const
	{
		return _data <= rhs._data;
	}

	template<class T, size_t IntegerBits>
	bool FixedPoint<T, IntegerBits>::operator!=(const FixedPoint& rhs)const
	{
		return _data != rhs._data;
	}

	template<class T, size_t IntegerBits>
	inline FixedPoint<T, IntegerBits> FixedPoint<T, IntegerBits>::operator<<(int shift)const
	{
		FixedPoint result;
		result._data = _data << shift;
		return result;
	}

	template<class T, size_t IntegerBits>
	inline FixedPoint<T, IntegerBits> FixedPoint<T, IntegerBits>::operator>>(int shift)const
	{
		FixedPoint result;
		result._data = _data >> shift;
		return result;
	}

	template<class T, size_t IntegerBits>
	inline FixedPoint<T, IntegerBits>& FixedPoint<T, IntegerBits>::operator<<=(int shift)
	{
		_data <<= shift;
		return *this;
	}

	template<class T, size_t IntegerBits>
	inline FixedPoint<T, IntegerBits>& FixedPoint<T, IntegerBits>::operator>>=(int shift)
	{
		_data >>= shift;
		return *this;
	}
}
