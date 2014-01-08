
#pragma once

#include <template_utils.h>


namespace Mcucpp
{
	
	template<class T1, class T2>
	T1 saturate(T2 value)
	{
		if(Util::IsSigned<T2>::value && Util::MinValue<T1>::value > value)
			return Util::MinValue<T1>::value;
		
		if(!Util::IsSigned<T1>::value && (T2)Util::MinValue<T1>::value > value)
			return Util::MinValue<T1>::value;
			
		if(Util::MaxValue<T1>::value < value)
			return Util::MaxValue<T1>::value;
		
		return T1(value);
	}
	
	template<class T1, class T2>
	T1 addst(T1 lhs, T2 rhs)
	{
		if(Util::MinValue<T1>::value - rhs > lhs)
			return Util::MinValue<T1>::value;
		
		if(Util::MaxValue<T1>::value - rhs < lhs)
			return Util::MaxValue<T1>::value;
		
		return lhs + rhs;
	}
	
	template<class T1, class T2>
	T1 subst(T1 lhs, T2 rhs)
	{
		if(Util::MinValue<T1>::value + rhs > lhs)
			return Util::MinValue<T1>::value;
		
		if(Util::MaxValue<T1>::value + rhs < lhs)
			return Util::MaxValue<T1>::value;
		
		return lhs - rhs;
	}
	
	template<class T1, class T2>
	T1 mulst(T1 lhs, T2 rhs)
	{
		return lhs * rhs;
	}
	
	template<class T1, class T2>
	T1 shlst(T1 lhs, T2 rhs)
	{
		return lhs << rhs;
	}
}
