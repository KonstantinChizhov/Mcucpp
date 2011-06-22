#pragma once

namespace Util
{
	template<class T, T IntegralTypeCheck=T(0)>
	struct IsSigned
	{
		enum{value = T(-1) < T(0)};
	};

	template <uint64_t Arg, unsigned Base>
	struct Log
	{
		static const int  value = Log<Arg/Base, Base>::value + 1;
	};

	template <unsigned Base>
	struct Log<1, Base>
	{
		static const int value = 0;
	};

	template <unsigned Base>
	struct Log<0, Base>
	{
		static const int value = -1;
	};

	template<uint64_t num, unsigned pow>
	struct Pow
	{
		static const uint64_t value = num * Pow<num, pow-1>::value;
	};

	template<uint64_t num>
	struct Pow<num, 0>
	{
		static const uint64_t value = 1u;
	};

	template<class T>
	struct MaxValue
	{
		static const T value = (Pow<2, sizeof(T)*8 - IsSigned<T>::value >::value - 1u);
	};

	template<class T, unsigned Base>
	struct MaxPowerOf
	{
		static const T value = Pow<Base, Log<MaxValue<T>::value, Base>::value>::value;
	};
	
	template<class T>
	struct Unsigned
	{
		typedef T Result;
	};

	template<> struct Unsigned<int> {typedef unsigned int Result;};
	template<> struct Unsigned<char> {typedef unsigned char Result;};
	template<> struct Unsigned<long> {typedef unsigned long Result;};
	template<> struct Unsigned<short> {typedef unsigned short Result;};
	template<> struct Unsigned<long long> {typedef unsigned long long Result;};

}