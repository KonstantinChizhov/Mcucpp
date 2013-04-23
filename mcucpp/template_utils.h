#pragma once
#include <stdint.h>

namespace Mcucpp
{
	namespace Util
	{
		template<class T> struct IsSigned
		{
			enum{value = 1};
		};

		template<> struct IsSigned<unsigned int>{ enum{value = 0};	};
		template<> struct IsSigned<unsigned char>{ enum{value = 0};	};
		template<> struct IsSigned<unsigned short>{ enum{value = 0};	};
		template<> struct IsSigned<unsigned long>{ enum{value = 0};	};
		template<> struct IsSigned<unsigned long long>{ enum{value = 0};	};


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

		template<class T>
		struct Signed
		{
			typedef T Result;
		};

		template<> struct Unsigned<int> {typedef unsigned int Result;};
		template<> struct Unsigned<char> {typedef unsigned char Result;};
		template<> struct Unsigned<signed char> {typedef unsigned char Result;};
		template<> struct Unsigned<long> {typedef unsigned long Result;};
		template<> struct Unsigned<short> {typedef unsigned short Result;};
		template<> struct Unsigned<long long> {typedef unsigned long long Result;};

		template<> struct Signed<unsigned int> {typedef int Result;};
		template<> struct Signed<unsigned char> {typedef char Result;};
		template<> struct Signed<unsigned long> {typedef long Result;};
		template<> struct Signed<unsigned short> {typedef short Result;};
		template<> struct Signed<unsigned long long> {typedef long long Result;};

		template<class T> T (max)(T a, T b) {return a > b ? a : b;}
		template<class T> T (min)(T a, T b) {return a > b ? b : a;}
		template<class T> T (sqr)(T a) {return a*a;}

		template<bool Signed> struct AbsHelper
		{
			template<class T> static T Abs(T value){return value >= T(0) ? value : -value;}
		};
		template<> struct AbsHelper<false>
		{
			template<class T> static T Abs(T value){return value;}
		};

		template<class T> T (abs)(T a) {return AbsHelper< IsSigned<T>::value >::Abs(a);}


		template<class T, T arg>
		struct ReverseBits;

		template<uint32_t arg>
		struct ReverseBits<uint32_t, arg>
		{
		private:
			static const uint32_t t1 = ((arg >> 1) & 0x55555555) | ((arg & 0x55555555) << 1);
			static const uint32_t t2 = ((t1 >> 2) & 0x33333333) | ((t1 & 0x33333333) << 2);
			static const uint32_t t3 = ((t2 >> 4) & 0x0F0F0F0F) | ((t2 & 0x0F0F0F0F) << 4);
			static const uint32_t t4 = ((t3 >> 8) & 0x00FF00FF) | ((t3 & 0x00FF00FF) << 8);
		public:
			static const uint32_t value = (t4 >> 16) | ( t4 << 16);
		};

		template<uint16_t arg>
		struct ReverseBits<uint16_t, arg>
		{
		private:
			static const uint16_t t1 = ((arg >> 1) & 0x5555) | ((arg & 0x5555) << 1);
			static const uint16_t t2 = ((t1 >> 2) & 0x3333) | ((t1 & 0x3333) << 2);
			static const uint16_t t3 = ((t2 >> 4) & 0x0F0F) | ((t2 & 0x0F0F) << 4);
		public:
			static const uint16_t value = (uint16_t)(t3 >> 8) | (uint16_t)(t3 << 8);
		};

		template<uint8_t arg>
		struct ReverseBits<uint8_t, arg>
		{
		private:
			static const uint8_t t1 = ((arg >> 1) & 0x55) | ((arg & 0x55) << 1);
			static const uint8_t t2 = ((t1 >> 2) & 0x33) | ((t1 & 0x33) << 2);
		public:
			static const uint8_t value = uint8_t(t2 >> 4) | uint8_t(t2 << 4);
		};


		template<class T>
		struct HiResType;

		template<> struct HiResType <uint8_t> { typedef uint16_t Result; };
		template<> struct HiResType  <int8_t> { typedef  int16_t Result; };
		template<> struct HiResType<uint16_t> { typedef uint32_t Result; };
		template<> struct HiResType <int16_t> { typedef  int32_t Result; };
		template<> struct HiResType<uint32_t> { typedef uint64_t Result; };
		template<> struct HiResType <int32_t> { typedef uint64_t Result; };
	}
}
