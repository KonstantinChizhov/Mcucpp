
#pragma once

#include <saturated_impl.h>
#include <template_utils.h>

namespace Mcucpp
{
	template<class T>
	class Saturated
	{
		T _value;
	public:
		Saturated()
		:_value(0){}
		
		template<class T2>
		explicit Saturated(T2 initValue)
			:_value(saturate<T, T2>(initValue))
		{
		}
		
		template<class T2>
		Saturated &operator= (T2 initValue)
		{
			_value = saturate<T, T2>(initValue);
			return *this;
		}
		
		Saturated operator+(const Saturated &rhs) { return Saturated(addst(_value, rhs._value)); }
		Saturated operator-(const Saturated &rhs) { return Saturated(subst(_value, rhs._value)); }
		Saturated operator*(const Saturated &rhs) { return Saturated(mulst(_value, rhs._value)); }
		Saturated operator/(const Saturated &rhs) { return Saturated(_value / rhs); }
		Saturated operator>>(unsigned rhs)        { return Saturated(_value >> rhs); }
		Saturated operator<<(unsigned rhs)        { return Saturated( shlst(_value, rhs)); }
		
		Saturated& operator+=(const Saturated &rhs) { _value = addst(_value, rhs._value); return *this; }
		Saturated& operator-=(const Saturated &rhs) { _value = subst(_value, rhs._value); return *this; }
		Saturated& operator*=(const Saturated &rhs) { _value = mulst(_value, rhs._value); return *this; }
		Saturated& operator/=(const Saturated &rhs) { _value /= rhs._value; return *this; }
		Saturated& operator>>=(unsigned rhs)        { _value >>= rhs; return *this; }
		Saturated& operator<<=(unsigned rhs)        { _value = shlst(_value, rhs); return *this; }
		
		template<class T2>
		Saturated operator+(T2 rhs) { return Saturated(addst(_value, rhs)); }
		template<class T2>
		Saturated operator-(T2 rhs) { return Saturated(subst(_value, rhs)); }
		template<class T2>
		Saturated operator*(T2 rhs) { return Saturated(mulst(_value, rhs)); }
		template<class T2>
		Saturated operator/(T2 rhs) { return Saturated(_value / rhs); }

		template<class T2>
		Saturated& operator+=(T2 rhs) { _value = addst(_value, rhs); return *this; }
		template<class T2>
		Saturated& operator-=(T2 rhs) { _value = subst(_value, rhs); return *this; }
		template<class T2>
		Saturated& operator*=(T2 rhs) { _value = mulst(_value, rhs); return *this; }
		template<class T2>
		Saturated& operator/=(T2 rhs) { _value /= rhs; return *this; }
		
		operator T () { return _value;}
	};
}
