//*****************************************************************************
//
// Title		: C++ IO for ATMEL AVR
// Author		: Konstantin Chizhov
// Date			: 2010
// Target MCU	: TI Msp430
//
//       This code is distributed under the GNU Public License
//       which can be found at http://www.gnu.org/licenses/gpl.txt
//
// As a special exception, you may use this file as part of a free software
// library without restriction. Specifically, if other files instantiate
// templates or use macros or inline functions from this file, or you compile
// this file and link it with other files to produce an executable, this
// file does not by itself cause the resulting executable to be covered by
// the GNU General Public License. This exception does not however
// invalidate any other reasons why the executable file might be covered by
// the GNU General Public License.
//*****************************************************************************

#pragma once

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif
#include <ioreg.h>

#ifndef MSP430_PORTS_H
#define MSP430_PORTS_H

#if defined(__MSP430_HAS_PORT0__)
#define USE_PORT0
#endif

#if defined(__MSP430_HAS_PORT1__)  ||  defined(__MSP430_HAS_PORT1_R__)
#define USE_PORT1
#endif

#if defined(__MSP430_HAS_PORT2__)  ||  defined(__MSP430_HAS_PORT2_R__)
#define USE_PORT2
#endif

#if defined(__MSP430_HAS_PORT3__)  ||  defined(__MSP430_HAS_PORT3_R__)
#define USE_PORT3
#endif

#if defined(__MSP430_HAS_PORT4__)  ||  defined(__MSP430_HAS_PORT4_R__)
#define USE_PORT4
#endif

#if defined(__MSP430_HAS_PORT5__)  ||  defined(__MSP430_HAS_PORT5_R__)
#define USE_PORT5
#endif

#if defined(__MSP430_HAS_PORT6__)  ||  defined(__MSP430_HAS_PORT6_R__)
#define USE_PORT6
#endif

#if defined(__MSP430_HAS_PORT7__)  ||  defined(__MSP430_HAS_PORT7_R__)
#define USE_PORT7
#endif

#if defined(__MSP430_HAS_PORT8__)  ||  defined(__MSP430_HAS_PORT8_R__)
#define USE_PORT8
#endif

#if defined(__MSP430_HAS_PORT9__)  ||  defined(__MSP430_HAS_PORT9_R__)
#define USE_PORT9
#endif

#if defined(__MSP430_HAS_PORT10__)  ||  defined(__MSP430_HAS_PORT10_R__)
#define USE_PORT10
#endif

namespace IO
{

	//Port definitions for MSP430 families.
	class NativePortBase :public GpioBase
	{
		public:
		typedef uint8_t DataT;
			enum{DirBit = 1, AltSelBit = 2, ResEnBit = 4};
			typedef NativePortBase Base;
			enum{Width=sizeof(DataT)*8};
		public:
			enum Configuration
			{
				AnalogIn = AltSelBit,
				In = 0,
				PullUpOrDownIn = ResEnBit,
				Out = DirBit,
				AltOut = DirBit | AltSelBit
			};
			
			static Configuration MapConfiguration(GpioBase::GenericConfiguration config)
			{
				switch(config)
				{
				case GpioBase::In: return In;
				case GpioBase::AnalogIn: return AnalogIn;
				case GpioBase::PullUpOrDownIn: return PullUpOrDownIn;
				case GpioBase::AltOut: return AltOut;
				//case GpioBase::Out: 
				default:
					return Out;
				}
			}
			
			template<GenericConfiguration config>
			struct MapConfigurationConst
			{
				static const Configuration value = Out;
			};
	};
	
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::In>{static const Configuration value = In;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::AnalogIn>{static const Configuration value = AnalogIn;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::OpenDrainOut>{static const Configuration value = Out;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::AltOut>{static const Configuration value = AltOut;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::AltOpenDrain>{static const Configuration value = AltOut;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::PullUpOrDownIn>{static const Configuration value = PullUpOrDownIn;};
	
	namespace Private
	{
		template<class Out, class Dir, class In, class Sel, class Int, class IntEdge, class Ren, int ID>
		class PortImplementation :public NativePortBase{
		public:
			static void Write(DataT value)
			{
				Out::Set(value);
			}
			static void ClearAndSet(DataT clearMask, DataT value)
			{
				Out::AndOr(~clearMask, value);
			}
			static DataT Read()
			{
				return Out::Get();
			}
			static void Set(DataT value)
			{
				Out::Or(value);
			}
			static void Clear(DataT value)
			{
				Out::And(~value);
			}
			static void Toggle(DataT value)
			{
				Out::Xor(value);
			}
			static DataT PinRead()
			{
				return In::Get();
			}
			
			// constant interface

			template<DataT clearMask, DataT value>
			static void ClearAndSet()
			{
				Out::AndOr(~clearMask, value);
			}

			template<DataT value>
			static void Toggle()
			{
				Out::Xor(value);
			}

			template<DataT value>
			static void Set()
			{
				Out::Or(value);
			}

			template<DataT value>
			static void Clear()
			{
				Out::And(~value);
			}
			
			// Configuration interface
			
			template<unsigned pin>
			static void SetPinConfiguration(Configuration configuration)
			{
				BOOST_STATIC_ASSERT(pin < Width);
				if((unsigned)configuration & (unsigned)DirBit)
					Dir::Or(1 << pin);
				else
					Dir::And(~(1 << pin));
				if((unsigned)configuration & (unsigned)AltSelBit)
					Sel::Or(1 << pin); 
				else
					Sel::And(~(1 << pin));
				if((unsigned)configuration & (unsigned)ResEnBit)
					Ren::Or(1 << pin); 
				else
					Ren::And(~(1 << pin));
			}
			static void SetConfiguration(DataT mask, Configuration configuration)
			{
				if((unsigned)configuration & (unsigned)DirBit)
					Dir::Or(mask);
				else
					Dir::And(~mask);
				if((unsigned)configuration & (unsigned)AltSelBit)
					Sel::Or(mask); 
				else
					Sel::And(~mask);
				if((unsigned)configuration & (unsigned)ResEnBit)
					Ren::Or(mask); 
				else
					Ren::And(~mask);
			}
			
			template<DataT mask, Configuration configuration>
			static void SetConfiguration()
			{
				if((unsigned)configuration & (unsigned)DirBit)
					Dir::Or(mask);
				else
					Dir::And(~mask);
				if((unsigned)configuration & (unsigned)AltSelBit)
					Sel::Or(mask); 
				else
					Sel::And(~mask);
				if((unsigned)configuration & (unsigned)ResEnBit)
					Ren::Or(mask); 
				else
					Ren::And(~mask);
			}

			enum{Id = ID};
		};
	}
	
	#define MAKE_PORT(portName, dirName, pinName, selectName, interruptName, interruptEdge, resistorEnable, className, ID) \
	  namespace Private{\
		IO_REG_WRAPPER(portName, portName ## _t, uint8_t);\
		IO_REG_WRAPPER(dirName, dirName ## _t, uint8_t);\
		IO_REG_WRAPPER(pinName, pinName ## _t, uint8_t);\
		IO_REG_WRAPPER(selectName, selectName ## _t, uint8_t);\
		IO_REG_WRAPPER(interruptName, interruptName ## _t, uint8_t);\
		IO_REG_WRAPPER(interruptEdge, interruptEdge ## _t, uint8_t);\
		IO_REG_WRAPPER(resistorEnable, resistorEnable ## _t, uint8_t);\
		}\
		  typedef Private::PortImplementation<\
				Private::portName ## _t, \
				Private::dirName ## _t, \
				Private::pinName ## _t, \
				Private::selectName ## _t, \
				Private::interruptName ## _t, \
				Private::interruptEdge ## _t, \
				Private::resistorEnable ## _t, \
				ID> className;
		

	#ifdef USE_PORT0
	MAKE_PORT(P0OUT, P0DIR, P0IN, Port0, '0')
	#endif

	#ifdef USE_PORT1
	MAKE_PORT(P1OUT, P1DIR, P1IN, P1SEL, P1IE, P1IES, P1REN, Port1, '1')
	#endif

	#ifdef USE_PORT2
	MAKE_PORT(P2OUT, P2DIR, P2IN, P2SEL, P2IE, P2IES, P2REN, Port2, '2')
	#endif

	#ifdef USE_PORT3
	MAKE_PORT(P3OUT, P3DIR, P3IN, P3SEL, P3IE, P3IES, P3REN, Port3, '3')
	#endif

	#ifdef USE_PORT4
	MAKE_PORT(P4OUT, P4DIR, P4IN, P4SEL, P4IE, P4IES, P4REN, Port4, '4')
	#endif

	#ifdef USE_PORT5
	MAKE_PORT(P5OUT, P5DIR, P5IN, P5SEL, P5IE, P5IES, P5REN, Port5, '5')
	#endif

	#ifdef USE_PORT6
	MAKE_PORT(P6OUT, P6DIR, P6IN, P6SEL, P6IE, P6IES, P6REN, Port6, '6')
	#endif

	#ifdef USE_PORT7
	MAKE_PORT(P7OUT, P7DIR, P7IN, P7SEL, P7IE, P7IES, P7REN, Port7, '7')
	#endif

	#ifdef USE_PORT8
	MAKE_PORT(P8OUT, P8DIR, P8IN, P8SEL, P8IE, P8IES, P8REN, Port8, '8')
	#endif

	#ifdef USE_PORT9
	MAKE_PORT(P9OUT, P9DIR, P9IN, P9SEL, P9IE, P9IES, P9REN, Port9, '9')
	#endif

	#ifdef USE_PORT10
	MAKE_PORT(P10OUT, P10DIR, P10IN, P10SEL, P10IE, P10IES, P10REN, Port10, '10')
	#endif

}

#endif
