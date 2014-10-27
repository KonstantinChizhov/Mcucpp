//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2013
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
#include <stdint.h>
#include <stddef.h>
#include <static_assert.h>

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

		template<bool isSigned> struct NegativeHelper
		{
			template<class T> static bool IsNegative(T v) {return v < 0;}
		};
		
		template<> struct NegativeHelper<false>
		{
			 template<class T> static bool IsNegative(T) {return false;}
		};
		
		template<class T> static inline bool (IsNegative)(T v)
		{
			return NegativeHelper<IsSigned<T>::value>::IsNegative(v);
		}

		template<bool Signed> struct AbsHelper
		{
			template<class T> static T Abs(T value){return value >= T(0) ? value : -value;}
		};
		template<> struct AbsHelper<false>
		{
			template<class T> static T Abs(T value){return value;}
		};

		template<class T> T (abs)(T a) {return AbsHelper< IsSigned<T>::value >::Abs(a);}

		static inline float abs(float a)
		{
			union FloatToInt
			{
				float f;
				uint32_t i;
			} fi;
			fi.f = a;
			fi.i &= 0x7fffffff;
			return fi.f;
		}

		template<class T>
		inline uint8_t ilog2 (T x)
		{
			int8_t res = -1;
			do
			{
				res++;
			}while(x >>= 1);
			return res;
		}

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
			static const uint16_t value = uint16_t(t3 >> 8) | uint16_t((t3 << 8) & 0xff00);
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
		
		template<unsigned long x>
		class PopulatedBits
		{
			static const unsigned long x1 = (x & 0x55555555) + ((x >> 1) & 0x55555555);
			static const unsigned long x2 = (x1 & 0x33333333) + ((x1 >> 2) & 0x33333333);
			static const unsigned long x3 = (x2 & 0x0f0f0f0f) + ((x2 >> 4) & 0x0f0f0f0f);
			static const unsigned long x4 = (x3 & 0x00ff00ff) + ((x3 >> 8) & 0x00ff00ff);
		public:
			static const unsigned long value = (x4 & 0x0000ffff) + ((x4 >> 16) & 0x0000ffff);
		};
		
		template <class ForwardIterator, class T>
		void fill (ForwardIterator first, ForwardIterator last, const T& val)
		{
			while (first != last)
			{
				*first = val;
				++first;
			}
		}
		
		template <class ForwardIterator, class T>
		void fill_n (ForwardIterator first, size_t count, const T& val)
		{
			while (count--)
			{
				*first = val;
				++first;
			}
		}
		
		
		template <class InputIterator, class OutputIterator>
		void copy (InputIterator source, OutputIterator target, size_t count)
		{
			for(size_t i = 0; i < count; i++)
			{
				*target = *source;
				target++;
				source++;
			}
		}
		
		template<class InputIterator, class OutputIterator>
		OutputIterator copy (InputIterator first, InputIterator last, OutputIterator target)
		{
			while (first!=last)
			{
				*target = *first;
				++target; 
				++first;
			}
			return target;
		}
		
		template<class T> void swap(T &a, T &b)
		{
			T tmp = a;
			a = b;
			b = tmp;
		}

		static inline uint32_t sqrt(uint32_t value)
		{
			uint32_t result = 0;
			uint32_t add = 0x8000;   
			for(int i = 0; i < 16; i++)
			{
				uint32_t rootGuess = result | add;
				uint32_t guess = rootGuess * rootGuess;
				if (value >= guess)
				{
					result = rootGuess;           
				}
				add >>= 1;
			}
			return result;
		}

		static inline int32_t sqrt(int32_t value)
		{
			return sqrt((uint32_t)value);
		}

		static inline float sqrt(float value)
		{
			STATIC_ASSERT(sizeof(float) == 4);
			const uint32_t InitialGuessConstant = (1ul << 29) - (1ul << 22);
			union FloatToInt
			{
				float f;
				uint32_t i;
			}fToInt;
			
			fToInt.f = value;
			fToInt.i &= 0x7ffffffful;
			fToInt.i = InitialGuessConstant + (fToInt.i >> 1);

			float guess = fToInt.f;
			guess = 0.5f * (guess + value / guess);
            guess = 0.5f * (guess + value / guess);

			return guess;
		}
	}
}
