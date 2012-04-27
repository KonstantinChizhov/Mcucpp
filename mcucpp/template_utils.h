#pragma once
#include <stdint.h>

namespace Mcucpp
{
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
		struct MinValue
		{
			static const T value = IsSigned<T>::value ? ( T(1) << (sizeof(T)*8 - 1)) : 0;
		};

		template<class T>
		struct MaxValue
		{
			static const T value = ~MinValue<T>::value;
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

		template<class T> T (max)(T a, T b) {return a > b ? a : b;}
		template<class T> T (min)(T a, T b) {return a > b ? b : a;}
		template<class T> T (abs)(T a) {return a >= 0 ? a : -a;}
		template<class T> T (sqr)(T a) {return a*a;}

	}
}
