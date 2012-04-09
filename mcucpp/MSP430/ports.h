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

#if defined(__MSP430_HAS_PORT0__) ||  defined(__MSP430_HAS_PORT0_R__)
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


namespace Mcucpp
{
	namespace IO
	{

	//Port definitions for MSP430 families.
	class NativePortBase :public GpioBase
	{
		public:
		typedef uint8_t DataT;
			enum{DirBit = 1, AltSelBit = 2, ResEnBit = 4, AltSel2Bit = 8};
			typedef NativePortBase Base;
			enum{Width=sizeof(DataT)*8};
		public:
			enum Configuration
			{
				AnalogIn = AltSelBit,
				AltIn = AltSelBit,
				AltIn2 = AltSelBit | AltSel2Bit,
				In = 0,
				PullUpOrDownIn = ResEnBit,
				Out = DirBit,
				AltOut = DirBit | AltSelBit,
				AltOut2 = DirBit | AltSelBit | AltSel2Bit
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
		template<class In, class Out, class Dir, class Ifg, class Ies, class Ie, class Sel, class Sel2, class Ren, int ID>
		class PortImplementation :public NativePortBase{
		public:
			static void Write(DataT value)
			{
				Out::Set(value);
			}
			static void ClearAndSet(DataT clearMask, DataT value)
			{
				Out::And(~clearMask);
				Out::Or(value);
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
				Out::AndOr(DataT(~clearMask), value);
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
				Out::And(DataT(~value));
			}

			// Configuration interface

			template<unsigned pin>
			static void SetPinConfiguration(Configuration configuration)
			{
				BOOST_STATIC_ASSERT(pin < Width);
				if((unsigned)configuration & (unsigned)DirBit)
					Dir::Or(1 << pin);
				else
					Dir::And(DataT(~(1 << pin)));
				if((unsigned)configuration & (unsigned)AltSelBit)
					Sel::Or(1 << pin);
				else
					Sel::And(DataT(~(1 << pin)));
				if((unsigned)configuration & (unsigned)AltSel2Bit)
					Sel2::Or(1 << pin);
				else
					Sel2::And(DataT(~(1 << pin)));
				if((unsigned)configuration & (unsigned)ResEnBit)
					Ren::Or(1 << pin);
				else
					Ren::And(DataT(~(1 << pin)));
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
				if((unsigned)configuration & (unsigned)AltSel2Bit)
					Sel2::Or(mask);
				else
					Sel2::And(~mask);
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
					Dir::And(DataT(~mask));
				if((unsigned)configuration & (unsigned)AltSelBit)
					Sel::Or(mask);
				else
					Sel::And(DataT(~mask));
				if((unsigned)configuration & (unsigned)AltSel2Bit)
					Sel2::Or(mask);
				else
					Sel2::And(DataT(~mask));
				if((unsigned)configuration & (unsigned)ResEnBit)
					Ren::Or(mask);
				else
					Ren::And(DataT(~mask));
			}

			enum{Id = ID};
		};
	}

	#define MAKE_BASIC_PORT_NO_SEL(PORT_PREFIX, className, ID) \
	  namespace Private{\
		I_REG_WRAPPER(PORT_PREFIX ## IN, PORT_PREFIX ## IN ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## OUT, PORT_PREFIX ## OUT ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## DIR, PORT_PREFIX ## DIR ## _t, uint8_t);\
		}\
		  typedef Private::PortImplementation<\
				Private::PORT_PREFIX ## IN ## _t, \
				Private::PORT_PREFIX ## OUT ## _t, \
				Private::PORT_PREFIX ## DIR ## _t, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				ID> className;

	#define MAKE_BASIC_PORT(PORT_PREFIX, className, ID) \
	  namespace Private{\
		I_REG_WRAPPER(PORT_PREFIX ## IN, PORT_PREFIX ## IN ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## OUT, PORT_PREFIX ## OUT ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## DIR, PORT_PREFIX ## DIR ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## SEL, PORT_PREFIX ## SEL ## _t, uint8_t);\
		}\
		  typedef Private::PortImplementation<\
				Private::PORT_PREFIX ## IN ## _t, \
				Private::PORT_PREFIX ## OUT ## _t, \
				Private::PORT_PREFIX ## DIR ## _t, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				Private::PORT_PREFIX ## SEL ## _t, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				ID> className;


	#define MAKE_PORT_IE(PORT_PREFIX, className, ID) \
	  namespace Private{\
		I_REG_WRAPPER(PORT_PREFIX ## IN, PORT_PREFIX ## IN ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## OUT, PORT_PREFIX ## OUT ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## DIR, PORT_PREFIX ## DIR ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## SEL, PORT_PREFIX ## SEL ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IFG, PORT_PREFIX ## IFG ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IES, PORT_PREFIX ## IES ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IE, PORT_PREFIX ## IE ## _t, uint8_t);\
		}\
		  typedef Private::PortImplementation<\
				Private::PORT_PREFIX ## IN ## _t, \
				Private::PORT_PREFIX ## OUT ## _t, \
				Private::PORT_PREFIX ## DIR ## _t, \
				Private::PORT_PREFIX ## IFG ## _t, \
				Private::PORT_PREFIX ## IES ## _t, \
				Private::PORT_PREFIX ## IE ## _t, \
				Private::PORT_PREFIX ## SEL ## _t, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				ID> className;

	#define MAKE_PORT_IE_REN(PORT_PREFIX, className, ID) \
	  namespace Private{\
		I_REG_WRAPPER(PORT_PREFIX ## IN, PORT_PREFIX ## IN ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## OUT, PORT_PREFIX ## OUT ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## DIR, PORT_PREFIX ## DIR ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## SEL, PORT_PREFIX ## SEL ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IFG, PORT_PREFIX ## IFG ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IES, PORT_PREFIX ## IES ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IE, PORT_PREFIX ## IE ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## REN, PORT_PREFIX ## REN ## _t, uint8_t);\
		}\
		  typedef Private::PortImplementation<\
				Private::PORT_PREFIX ## IN ## _t, \
				Private::PORT_PREFIX ## OUT ## _t, \
				Private::PORT_PREFIX ## DIR ## _t, \
				Private::PORT_PREFIX ## IFG ## _t, \
				Private::PORT_PREFIX ## IES ## _t, \
				Private::PORT_PREFIX ## IE ## _t, \
				Private::PORT_PREFIX ## SEL ## _t, \
				NullReg<uint8_t>, \
				Private::PORT_PREFIX ## REN ## _t, \
				ID> className;

	#define MAKE_PORT_REN(PORT_PREFIX, className, ID) \
	  namespace Private{\
		I_REG_WRAPPER(PORT_PREFIX ## IN, PORT_PREFIX ## IN ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## OUT, PORT_PREFIX ## OUT ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## DIR, PORT_PREFIX ## DIR ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## SEL, PORT_PREFIX ## SEL ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## REN, PORT_PREFIX ## REN ## _t, uint8_t);\
		}\
		  typedef Private::PortImplementation<\
				Private::PORT_PREFIX ## IN ## _t, \
				Private::PORT_PREFIX ## OUT ## _t, \
				Private::PORT_PREFIX ## DIR ## _t, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				Private::PORT_PREFIX ## SEL ## _t, \
				NullReg<uint8_t>, \
				Private::PORT_PREFIX ## REN ## _t, \
				ID> className;

	#define MAKE_PORT_IE_REN_SEL2(PORT_PREFIX, className, ID) \
	  namespace Private{\
		I_REG_WRAPPER(PORT_PREFIX ## IN, PORT_PREFIX ## IN ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## OUT, PORT_PREFIX ## OUT ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## DIR, PORT_PREFIX ## DIR ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IFG, PORT_PREFIX ## IFG ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IES, PORT_PREFIX ## IES ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IE, PORT_PREFIX ## IE ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## SEL, PORT_PREFIX ## SEL ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## SEL2, PORT_PREFIX ## SEL2 ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## REN, PORT_PREFIX ## REN ## _t, uint8_t);\
		}\
		  typedef Private::PortImplementation<\
				Private::PORT_PREFIX ## IN ## _t, \
				Private::PORT_PREFIX ## OUT ## _t, \
				Private::PORT_PREFIX ## DIR ## _t, \
				Private::PORT_PREFIX ## IFG ## _t, \
				Private::PORT_PREFIX ## IES ## _t, \
				Private::PORT_PREFIX ## IE ## _t, \
				Private::PORT_PREFIX ## SEL ## _t, \
				Private::PORT_PREFIX ## SEL2 ## _t, \
				Private::PORT_PREFIX ## REN ## _t, \
				ID> className;

	#define MAKE_PORT_REN_SEL2(PORT_PREFIX, className, ID) \
	  namespace Private{\
		I_REG_WRAPPER(PORT_PREFIX ## IN, PORT_PREFIX ## IN ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## OUT, PORT_PREFIX ## OUT ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## DIR, PORT_PREFIX ## DIR ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## SEL, PORT_PREFIX ## SEL ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## SEL2, PORT_PREFIX ## SEL2 ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## REN, PORT_PREFIX ## REN ## _t, uint8_t);\
		}\
		  typedef Private::PortImplementation<\
				Private::PORT_PREFIX ## IN ## _t, \
				Private::PORT_PREFIX ## OUT ## _t, \
				Private::PORT_PREFIX ## DIR ## _t, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				Private::PORT_PREFIX ## SEL ## _t, \
				Private::PORT_PREFIX ## SEL2 ## _t, \
				Private::PORT_PREFIX ## REN ## _t, \
				ID> className;

	#define MAKE_PORT_IE_SEL2(PORT_PREFIX, className, ID) \
	  namespace Private{\
		I_REG_WRAPPER(PORT_PREFIX ## IN, PORT_PREFIX ## IN ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## OUT, PORT_PREFIX ## OUT ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## DIR, PORT_PREFIX ## DIR ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IFG, PORT_PREFIX ## IFG ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IES, PORT_PREFIX ## IES ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IE, PORT_PREFIX ## IE ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## SEL, PORT_PREFIX ## SEL ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## SEL2, PORT_PREFIX ## SEL2 ## _t, uint8_t);\
		}\
		  typedef Private::PortImplementation<\
				Private::PORT_PREFIX ## IN ## _t, \
				Private::PORT_PREFIX ## OUT ## _t, \
				Private::PORT_PREFIX ## DIR ## _t, \
				Private::PORT_PREFIX ## IFG ## _t, \
				Private::PORT_PREFIX ## IES ## _t, \
				Private::PORT_PREFIX ## IE ## _t, \
				Private::PORT_PREFIX ## SEL ## _t, \
				Private::PORT_PREFIX ## SEL2 ## _t, \
				NullReg<uint8_t>, \
				ID> className;

	#define MAKE_PORT_IE_REN_NO_SEL(PORT_PREFIX, className, ID) \
	  namespace Private{\
		I_REG_WRAPPER(PORT_PREFIX ## IN, PORT_PREFIX ## IN ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## OUT, PORT_PREFIX ## OUT ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## DIR, PORT_PREFIX ## DIR ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IFG, PORT_PREFIX ## IFG ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IES, PORT_PREFIX ## IES ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IE, PORT_PREFIX ## IE ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## REN, PORT_PREFIX ## REN ## _t, uint8_t);\
		}\
		  typedef Private::PortImplementation<\
				Private::PORT_PREFIX ## IN ## _t, \
				Private::PORT_PREFIX ## OUT ## _t, \
				Private::PORT_PREFIX ## DIR ## _t, \
				Private::PORT_PREFIX ## IFG ## _t, \
				Private::PORT_PREFIX ## IES ## _t, \
				Private::PORT_PREFIX ## IE ## _t, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				Private::PORT_PREFIX ## REN ## _t, \
				ID> className;

	#define MAKE_PORT_IE_NO_SEL(PORT_PREFIX, className, ID) \
	  namespace Private{\
		I_REG_WRAPPER(PORT_PREFIX ## IN, PORT_PREFIX ## IN ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## OUT, PORT_PREFIX ## OUT ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## DIR, PORT_PREFIX ## DIR ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IFG, PORT_PREFIX ## IFG ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IES, PORT_PREFIX ## IES ## _t, uint8_t);\
		IO_REG_WRAPPER(PORT_PREFIX ## IE, PORT_PREFIX ## IE ## _t, uint8_t);\
		}\
		  typedef Private::PortImplementation<\
				Private::PORT_PREFIX ## IN ## _t, \
				Private::PORT_PREFIX ## OUT ## _t, \
				Private::PORT_PREFIX ## DIR ## _t, \
				Private::PORT_PREFIX ## IFG ## _t, \
				Private::PORT_PREFIX ## IES ## _t, \
				Private::PORT_PREFIX ## IE ## _t, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				NullReg<uint8_t>, \
				ID> className;
#if defined(__ICCMSP__) || defined(__ICC430__)
	#include "port_def_mspiar.h"
#else
	#include "port_def_mspgcc.h"
#endif
	}
}
#endif
