#include <stdlib.h>

namespace Mcucpp
{

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>::FixedPoint()
	:_data(SignedDataT(0))
	{

	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>::FixedPoint(int integer)
	:_data(SignedDataT(integer) << FractionalBits)
	{

	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>::FixedPoint(float value)
	:_data(value * (1 << FractionalBits))
	{
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>::FixedPoint(double value)
	:_data(value * (1 << FractionalBits))
	{
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>::FixedPoint(unsigned integer)
	:_data(SignedDataT(integer) << FractionalBits)
	{

	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>::FixedPoint(int integer, unsigned fract)
	:_data((SignedDataT(integer) << FractionalBits) + (fract >> (sizeof(fract)*8 - FractionalBits)))
	{

	}
//------------------------------------------------------------------------------------------------------------------------------
	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator+(int rhs)const
	{
		FixedPoint result;
		result._data = _data + (SignedDataT(rhs) << FractionalBits);
		return result;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator+(const FixedPoint& rhs)const
	{
		FixedPoint result;
		result._data = _data + rhs._data;
		return result;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>& FixedPoint<IntegerBits, FractionalBits>::operator+=(int rhs)
	{
		_data += (SignedDataT(rhs) << FractionalBits);
		return *this;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>& FixedPoint<IntegerBits, FractionalBits>::operator+=(const FixedPoint& rhs)
	{
		_data += rhs._data;
		return *this;
	}
//------------------------------------------------------------------------------------------------------------------------------
	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator-(int rhs)const
	{
		FixedPoint result;
		result._data = _data - (SignedDataT(rhs) << FractionalBits);
		return result;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator-(const FixedPoint& rhs)const
	{
		FixedPoint result;
		result._data = _data - rhs._data;
		return result;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>& FixedPoint<IntegerBits, FractionalBits>::operator-=(int rhs)
	{
		_data -= (SignedDataT(rhs) << FractionalBits);
		return *this;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>& FixedPoint<IntegerBits, FractionalBits>::operator-=(const FixedPoint& rhs)
	{
		_data -= rhs._data;
		return *this;
	}
//------------------------------------------------------------------------------------------------------------------------------
	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator*(int rhs)const
	{
		FixedPoint result;
		result._data = _data * SignedDataT(rhs);
		return result;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator*(const FixedPoint& rhs)const
	{
		FixedPoint result;
		const SignedDataT inta = IntegerPart();
		const SignedDataT intb = rhs.IntegerPart();
		const SignedDataT fracta = FractionalPart();
		const SignedDataT fractb = rhs.FractionalPart();
		result._data = (inta * intb << FractionalBits) + (inta * fractb + fracta * intb) + (fracta * fractb >> FractionalBits);
		return result;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>& FixedPoint<IntegerBits, FractionalBits>::operator*=(int rhs)
	{
		_data *= SignedDataT(rhs);
		return *this;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>& FixedPoint<IntegerBits, FractionalBits>::operator*=(const FixedPoint& rhs)
	{
		const SignedDataT inta = IntegerPart();
		const SignedDataT intb = rhs.IntegerPart();
		const SignedDataT fracta = FractionalPart();
		const SignedDataT fractb = rhs.FractionalPart();
		_data = (inta * intb << FractionalBits) + (inta * fractb + fracta * intb) + (fracta * fractb >> FractionalBits);
		return *this;
	}

//------------------------------------------------------------------------------------------------------------------------------
	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator/(int rhs)const
	{
		FixedPoint result;
		result._data = _data / SignedDataT(rhs);
		return result;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits> FixedPoint<IntegerBits, FractionalBits>::operator/(const FixedPoint& rhs)const
	{
		FixedPoint result(*this);
		return result /= rhs;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>& FixedPoint<IntegerBits, FractionalBits>::operator/=(int rhs)
	{
		_data /= SignedDataT(rhs);
		return *this;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits>& FixedPoint<IntegerBits, FractionalBits>::operator/=(const FixedPoint& rhs)
	{
		// TODO: implement
  		return *this;
	}
//------------------------------------------------------------------------------------------------------------------------------

	template<size_t IntegerBits, size_t FractionalBits>
	int FixedPoint<IntegerBits, FractionalBits>::ToInt()const
	{
		return (_data & IntegerPartMask) >> FractionalBits;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	long FixedPoint<IntegerBits, FractionalBits>::ToLong()const
	{
		return (_data & IntegerPartMask) >> FractionalBits;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	long long FixedPoint<IntegerBits, FractionalBits>::ToLongLong()const
	{
		return (_data & IntegerPartMask) >> FractionalBits;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	float FixedPoint<IntegerBits, FractionalBits>::ToFloat()const
	{
		const float rawoner = 1.f / RawOne;
		return float(_data) * rawoner;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	double FixedPoint<IntegerBits, FractionalBits>::ToDouble()const
	{
		const double rawoner = 1.0 / RawOne;
		return double(_data) * rawoner;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	typename FixedPoint<IntegerBits, FractionalBits>::SignedDataT FixedPoint<IntegerBits, FractionalBits>::IntegerPart()const
	{
		return SignedDataT(_data & IntegerPartMask) >> FractionalBits;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	typename FixedPoint<IntegerBits, FractionalBits>::DataT FixedPoint<IntegerBits, FractionalBits>::FractionalPart()const
	{
		return _data & FractionalPartMask;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits> operator+(int lhs, const FixedPoint<IntegerBits, FractionalBits>& rhs)
	{
		return rhs + lhs;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits> operator-(int lhs, const FixedPoint<IntegerBits, FractionalBits>& rhs)
	{
		return FixedPoint<IntegerBits, FractionalBits>(lhs) - rhs;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits> operator*(int lhs, const FixedPoint<IntegerBits, FractionalBits>& rhs)
	{
		return rhs * lhs;
	}

	template<size_t IntegerBits, size_t FractionalBits>
	FixedPoint<IntegerBits, FractionalBits> operator/(int lhs, const FixedPoint<IntegerBits, FractionalBits>& rhs)
	{
		return FixedPoint<IntegerBits, FractionalBits>(lhs) / rhs;
	}
}
