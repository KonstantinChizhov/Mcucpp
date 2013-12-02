#pragma once

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

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif

#ifndef XMEGA_PORTS_H
#define XMEGA_PORTS_H
//Port definitions for XMega A families.

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

namespace IO
{

	class NativePortBase
	{
		public:
			typedef uint8_t DataT;
			enum{Width=sizeof(DataT)*8};
		public:
			/*TODO: Implement other configurations*/
			enum Configuration
			{
				AnalogIn = 0,
				In = 0x00,
				PullUpOrDownIn = 0x00,
				Out = 0x01,
				AltOut = 0x01,
			};
	};

	template<class Port>
	class PortImplimentation: public NativePortBase{
	public:
		static void Write(DataT value)
		{
			Port::Port().OUT = value;
		}

		static void ClearAndSet(DataT clearMask, DataT value)
		{
			Clear(clearMask);
			Set(value);
		}

		static DataT Read()
		{
			return Port::Port().OUT;
		}

		static void Set(DataT value)
		{	
			Port::Port().OUTSET = value;
		}

		static void Clear(DataT value)
		{
			Port::Port().OUTCLR = value;
		}

		static void Toggle(DataT value)
		{
			Port::Port().OUTTGL = value;
		}

		static DataT PinRead()
		{
			return Port::Port().IN;
		}

		template<unsigned pin>
		static void SetPinConfiguration(Configuration configuration)
		{
			STATIC_ASSERT(pin < Width);
			if(configuration)
				Port::Port().DIRSET = 1 << pin;
			else
				Port::Port().DIRCLR = 1 << pin;
		}

		static void SetConfiguration(DataT mask, Configuration configuration)
		{
			if(configuration)
				Port::Port().DIRSET = mask;
			else
				Port::Port().DIRCLR = mask;
		}
		
		template<DataT mask, Configuration configuration>
		static void SetConfiguration()
		{
			if(configuration)
				Port::Port().DIRSET = mask;
			else
				Port::Port().DIRCLR = mask;
		}

		template<DataT clearMask, DataT value>
		static void ClearAndSet()
		{
			Port::Port().OUTCLR = clearMask;
			Port::Port().OUTSET = value;
		}

		template<DataT value>
		static void Toggle()
		{
			Port::Port().OUTTGL = value;
		}

		template<DataT value>
		static void Set()
		{
			Port::Port().OUTSET = value;
		}

		template<DataT value>
		static void Clear()
		{
			Port::Port().OUTCLR = value;
		}
	};

#define MAKE_PORT(portName, className, ID) \
			class className :public PortImplimentation<className>{\
				static PORT_t &Port(){return portName;}\
				friend class PortImplimentation<className>;\
				public:\
				enum{Id = ID};\
			};\

#ifdef USE_PORTA
MAKE_PORT(PORTA, Porta, 'A')
#endif


#ifdef USE_PORTB
MAKE_PORT(PORTB, Portb, 'B')
#endif


#ifdef USE_PORTC
MAKE_PORT(PORTC, Portc, 'C')
#endif


#ifdef USE_PORTD
MAKE_PORT(PORTD, Portd, 'D')
#endif


#ifdef USE_PORTE
MAKE_PORT(PORTE, Porte, 'E')
#endif


#ifdef USE_PORTF
MAKE_PORT(PORTF, Portf, 'F')
#endif

#ifdef USE_PORTG
MAKE_PORT(PORTG, Portg, 'G')
#endif

#ifdef USE_PORTH
MAKE_PORT(PORTH, Porth, 'H')
#endif

#ifdef USE_PORTJ
MAKE_PORT(PORTJ, Portj, 'J')
#endif

#ifdef USE_PORTK
MAKE_PORT(PORTK, Portk, 'K')
#endif

#ifdef USE_PORTQ
MAKE_PORT(PORTQ, Portq, 'Q')
#endif

#ifdef USE_PORTR
MAKE_PORT(PORTR, Portr, 'R')
#endif


#define MAKE_VIRTUAL_PORT(portName, className, VPCTRL, VPMAP_enum, ID) \
	class className{\
	public:\
		typedef uint8_t DataT;\
	public:\
		enum PinConfiguration\
		{\
			AnalogIn = 0,\
			In = 0x00,\
			PullUpOrDownIn = 0x00,\
			Out = 0x01,\
			AltOut = 0x01,\
		};\
		static void Write(DataT value)\
		{\
			portName.OUT = value;\
		}\
		static DataT Read()\
		{\
			return portName.OUT;\
		}\
		static void DirWrite(DataT value)\
		{\
			portName.DIR = value;\
		}\
		static DataT DirRead()\
		{\
			return portName.DIR;\
		}\
		static void Set(DataT value)\
		{\
			portName.OUT |= value;\
		}\
		static void Clear(DataT value)\
		{\
			portName.OUT &= ~value;\
		}\
		static void Toggle(DataT value)\
		{\
			portName.OUT ^= value;\
		}\
		static void DirSet(DataT value)\
		{\
			portName.DIR |= value;\
		}\
		static void DirClear(DataT value)\
		{\
			portName.DIR &= ~value;\
		}\
		static DataT PinRead()\
		{\
			return portName.IN;\
		}\
		static void DirToggle(DataT value)\
		{\
			portName.DIR ^= value;\
		}\
		enum{Id = ID};\
		enum{Width=8};\
		static void Map(VPMAP_enum value)\
		{\
			VPCTRL |= value;\
		}\
	};

#ifndef __ICCAVR__//PORTCFG_VP0MAP_t is undefined for some strange reason
#ifdef USE_VPORT0
	MAKE_VIRTUAL_PORT(VPORT0, VPort0, PORTCFG.VPCTRLA, PORTCFG_VP0MAP_t, 'V0')
#endif

#ifdef USE_VPORT1
	MAKE_VIRTUAL_PORT(VPORT1, VPort1, PORTCFG.VPCTRLA, PORTCFG_VP1MAP_t, 'V1')
#endif

#ifdef USE_VPORT2
	MAKE_VIRTUAL_PORT(VPORT2, VPort2, PORTCFG.VPCTRLB, PORTCFG_VP2MAP_t, 'V2')
#endif

#ifdef USE_VPORT3
	MAKE_VIRTUAL_PORT(VPORT3, VPort3, PORTCFG.VPCTRLB, PORTCFG_VP3MAP_t, 'V3')
#endif
#endif     
}//namespace IO

#endif
