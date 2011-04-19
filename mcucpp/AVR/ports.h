//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2010
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

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif

#ifndef AVR_PORTS_H
#define AVR_PORTS_H

#ifdef PORTA
#define USE_PORTA
#endif

#ifdef PORTB
#define USE_PORTB
#endif


#ifdef PORTC
#define USE_PORTC
#endif

#ifdef PORTD
#define USE_PORTD
#endif

#ifdef PORTE
#define USE_PORTE
#endif

#ifdef PORTF
#define USE_PORTF
#endif

#ifdef PORTG
#define USE_PORTG
#endif

#ifdef PORTH
#define USE_PORTH
#endif

#ifdef PORTJ
#define USE_PORTJ
#endif

#ifdef PORTK
#define USE_PORTK
#endif

#ifdef PORTQ
#define USE_PORTQ
#endif

#ifdef PORTR
#define USE_PORTR
#endif

#ifdef VPORT0
#define USE_VPORT0
#endif

#ifdef VPORT1
#define USE_VPORT1
#endif

#ifdef VPORT2
#define USE_VPORT2
#endif

#ifdef VPORT3
#define USE_VPORT3
#endif

#include "atomic.h"
#include "ioreg.h"

#define PORT_REGS_WRAPPER(PORT_LETTER) \
	IO_REG_WRAPPER(PORT ## PORT_LETTER, Out ## PORT_LETTER, uint8_t);\
	IO_REG_WRAPPER(DDR ## PORT_LETTER, Dir ## PORT_LETTER, uint8_t);\
	IO_REG_WRAPPER(PIN ## PORT_LETTER, In ## PORT_LETTER, uint8_t);

namespace IO
{

	class NativePortBase :public GpioBase
	{
		public:
			typedef uint8_t DataT;
			typedef NativePortBase Base;
			enum{Width=sizeof(DataT)*8};
			static const unsigned MaxBitwiseOutput = 5;
		public:
			enum Configuration
			{
				AnalogIn = 0,
				In = 0x00,
				PullUpOrDownIn = 0x00,
				Out = 0x01,
				AltOut = 0x01
			};

			static Configuration MapConfiguration(GenericConfiguration config)
			{
				if(config & GpioBase::Out)
					return Out;
				return In;
			}

			template<GenericConfiguration config>
			struct MapConfigurationConst
			{
				static const Configuration value = In;
			};
	};


	template<> struct NativePortBase::MapConfigurationConst<GpioBase::Out>{static const Configuration value = Out;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::OpenDrainOut>{static const Configuration value = Out;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::AltOut>{static const Configuration value = Out;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::AltOpenDrain>{static const Configuration value = Out;};

	//Port definitions for AtTiny, AtMega families.

		template<class Out, class Dir, class In, int ID>
		class PortImplimentation :public NativePortBase
		{
			template<DataT value, DataT mask>
			static inline void SetBitWise()
			{
				if(mask == 0) return;
				if(value & mask)
					Out::Or(value & mask);
				SetBitWise<value, mask << 1>();
			}

			template<DataT value, DataT mask>
			static inline void ClearBitWise()
			{
				if(mask == 0) return;
				if(value & mask)
					Out::And((DataT)~(value & mask));
				ClearBitWise<value, mask << 1>();
			}
		public:
			static void Write(DataT value)
			{
				Out::Set(value);
			}

			static void ClearAndSet(DataT clearMask, DataT value)
			{
				ATOMIC Out::AndOr(~clearMask, value);
			}

			static DataT Read()
			{
				return Out::Get();
			}

			static void Set(DataT value)
			{
				ATOMIC Out::Or(value);
			}

			static void Clear(DataT value)
			{
				ATOMIC Out::And(~value);
			}

			static void Toggle(DataT value)
			{
				ATOMIC Out::Xor(value);
			}

			static DataT PinRead()
			{
				return In::Get();
			}

			// constant interface

			template<DataT clearMask, DataT value>
			static void ClearAndSet()
			{
				const DataT bitsToSet = value & clearMask;
				const DataT bitsToClear = ~value & clearMask;

				const unsigned countBitsToChange = PopulatedBits<clearMask>::value;

				if(countBitsToChange <= MaxBitwiseOutput &&
					Id < 4)
				{
					SetBitWise<bitsToSet, 1>();
					ClearBitWise<bitsToClear, 1>();
				}
				else
					ATOMIC Out::AndOr(~clearMask, value);
			}

			template<DataT value>
			static void Toggle()
			{
				ATOMIC Out::Xor(value);
			}

			template<DataT value>
			static void Set()
			{
				if(PopulatedBits<value>::value <= MaxBitwiseOutput &&
					Id < 4)
					SetBitWise<value, 1>();
				else
					ATOMIC Out::Or(value);
			}

			template<DataT value>
			static void Clear()
			{
				if(PopulatedBits<value>::value <= MaxBitwiseOutput &&
					Id < 4)
					ClearBitWise<value, 1>();
				else
					ATOMIC Out::And((DataT)~value);
			}

			// Configuration interface

			template<unsigned pin>
			static void SetPinConfiguration(Configuration configuration)
			{
				BOOST_STATIC_ASSERT(pin < Width);
				if(configuration)
					Dir::Or(1 << pin);
				else
					Dir::And((DataT)~(1 << pin));
			}

			static void SetConfiguration(DataT mask, Configuration configuration)
			{
				if(configuration)
					Dir::Or(mask);
				else
					Dir::And(~mask);
			}

			template<DataT mask, Configuration configuration>
			static void SetConfiguration()
			{
				if(configuration)
					Dir::Or(mask);
				else
					Dir::And(~mask);
			}

			enum{Id = ID};
		};

	namespace Private
	{
		#ifdef USE_PORTA
		PORT_REGS_WRAPPER(A)
		#endif

		#ifdef USE_PORTB
		PORT_REGS_WRAPPER(B)
		#endif

		#ifdef USE_PORTC
		PORT_REGS_WRAPPER(C)
		#endif

		#ifdef USE_PORTD
		PORT_REGS_WRAPPER(D)
		#endif

		#ifdef USE_PORTE
		PORT_REGS_WRAPPER(E)
		#endif

		#ifdef USE_PORTF
		PORT_REGS_WRAPPER(F)
		#endif

		#ifdef USE_PORTG
		PORT_REGS_WRAPPER(G)
		#endif
	}

#define DECLARE_PORT(SUFFIX, CLASS_NAME, ID) \
		typedef PortImplimentation<\
			Private::Out ## SUFFIX,\
			Private::Dir ## SUFFIX,\
			Private::In ## SUFFIX,\
			ID> CLASS_NAME;

	#ifdef USE_PORTA
	DECLARE_PORT(A, Porta, 0)
	#endif

	#ifdef USE_PORTB
	DECLARE_PORT(B, Portb, 1)
	#endif

	#ifdef USE_PORTC
	DECLARE_PORT(C, Portc, 2)
	#endif

	#ifdef USE_PORTD
	DECLARE_PORT(D, Portd, 3)
	#endif

	#ifdef USE_PORTE
	DECLARE_PORT(E, Porte, 4)
	#endif

	#ifdef USE_PORTF
	DECLARE_PORT(F, Portf, 5)
	#endif

	#ifdef USE_PORTG
	DECLARE_PORT(G, Portg, 6)
	#endif
}

#undef DECLARE_PORT
#undef PORT_REGS_WRAPPER

#endif
