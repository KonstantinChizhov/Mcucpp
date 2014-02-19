#pragma once

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif

#ifndef STM32_PORTS_H
#define STM32_PORTS_H

#include "ioreg.h"
#include "stm32f10x.h"
#include "clock.h"

#include <static_assert.h>
#define USE_SPLIT_PORT_CONFIGURATION 8

#define USE_PORTA
#define USE_PORTB
#define USE_PORTC
#define USE_PORTD
//#define USE_PORTE
//#define USE_PORTF
//#define USE_PORTG

namespace Mcucpp
{
	namespace IO
	{
		class NativePortBase
		{
		public:
			enum{Width=16};
			typedef uint16_t DataT;
			enum Configuration
			{
				Analog =  0,
				In =      0x04,
				Out =     0x03,
				AltFunc = 0x0B
			};
			
			enum PullMode
			{
				NoPullUp = 0,
				PullUp   = 0x08,
				PullDown = 0x18
			};
			
			enum DriverType
			{
				PushPull  = 0,
				OpenDrain = 4
			};
			
			enum Speed
			{
				Slow    = 1,
				Fast    = 2,
				Fastest = 3
			};
			
			static inline unsigned UnpackConfig(unsigned mask, unsigned value, unsigned configuration, unsigned configMask)
			{
				mask = (mask & 0xf0) << 12 | (mask & 0x0f);
				mask = (mask & 0x000C000C) << 6 | (mask & 0x00030003);
				mask = (mask & 0x02020202) << 3 | (mask & 0x01010101);
				return (value & ~(mask*configMask)) | mask * configuration;
			}
			
		};
		
		namespace Private
		{
			template<unsigned mask>
			class ConfigurationMask
			{
				static const unsigned mask1 = (mask & 0xf0) << 12 | (mask & 0x0f);
				static const unsigned mask2 = (mask1 & 0x000C000C) << 6 | (mask1 & 0x00030003);
				static const unsigned mask3 = (mask2 & 0x02020202) << 3 | (mask2 & 0x01010101);
			public:
				static const unsigned value = mask3;
			};

			template<class Regs, class ClkEnReg, int ID>
			class PortImplementation :public NativePortBase
			{
			public:
				static DataT Read()
				{
				  return Regs()->ODR;
				}
				static void Write(DataT value)
				{
					Regs()->ODR = value;
				}
				static void ClearAndSet(DataT clearMask, DataT value)
				{
					Regs()->BSRR = (value | (uint32_t)clearMask << 16);
				}
				static void Set(DataT value)
				{
					Regs()->BSRR = value;
				}
				static void Clear(DataT value)
				{
					Regs()->BSRR = ((uint32_t)value << 16);
				}
				static void Toggle(DataT value)
				{
					Regs()->ODR ^= value;
				}
				static DataT PinRead()
				{
					return Regs()->IDR;
				}

				// constant interface

				template<DataT clearMask, DataT value>
				static void ClearAndSet()
				{
					Regs()->BSRR = (value | (uint32_t)clearMask << 16);
				}

				template<DataT value>
				static void Toggle()
				{
					Regs()->ODR ^= value;
				}

				template<DataT value>
				static void Set()
				{
					Regs()->BSRR = value;
				}

				template<DataT value>
				static void Clear()
				{
					Regs()->BSRR = ((uint32_t)value << 16);
				}

				template<unsigned pin>
				static void SetPinConfiguration(Configuration configuration)
				{
					STATIC_ASSERT(pin < Width);
					if(pin < 8)
					{
						Regs()->CRL = (Regs()->CRL & ~(0x0fu << pin*4)) | ((unsigned)configuration << pin*4);
					}
					else
					{
						Regs()->CRH = (Regs()->CRH & ~(0x0fu << (pin-8)*4)) | ((unsigned)configuration << (pin-8)*4);
					}
				}
				static void SetConfiguration(DataT mask, Configuration configuration)
				{
					Regs()->CRL = NativePortBase::UnpackConfig(mask, Regs()->CRL, configuration, 0x0f);
					Regs()->CRH = NativePortBase::UnpackConfig((mask>>8), Regs()->CRH, configuration, 0x0f);
				}
				
				template<DataT mask, Configuration configuration>
				static void SetConfiguration()
				{
					unsigned configMask = ConfigurationMask<mask>::value;
					Regs()->CRL = (Regs()->CRL & ~(configMask*0x0f)) | configMask * configuration;
					
					configMask = ConfigurationMask<(mask>>8)>::value;
					Regs()->CRH = (Regs()->CRH & ~(configMask*0x0f)) | configMask * configuration;
				}
				
				static void SetSpeed(DataT mask, Speed speed)
				{
					Regs()->CRL = NativePortBase::UnpackConfig(mask, Regs()->CRL, speed, 0x03);
					Regs()->CRH = NativePortBase::UnpackConfig((mask>>8), Regs()->CRH, speed, 0x03);
				}
				
				static void SetPullUp(DataT mask, PullMode pull)
				{
					Regs()->CRL = NativePortBase::UnpackConfig(mask, Regs()->CRL, pull & 0x08, 0x0f);
					Regs()->CRH = NativePortBase::UnpackConfig((mask>>8), Regs()->CRH, pull & 0x08, 0x0f);
					if(pull & 0x10) // pullup
					{
						Set(mask); 
					}
					else //pulldown
					{
						Clear(mask); 
					}
				}
				
				static void SetDriverType(DataT mask, DriverType driver)
				{
					Regs()->CRL = NativePortBase::UnpackConfig(mask, Regs()->CRL, driver, 0x04);
					Regs()->CRH = NativePortBase::UnpackConfig((mask>>8), Regs()->CRH, driver, 0x04);
				}
				
				static void AltFuncNumber(DataT mask, uint8_t number)
				{
					// nothing to do
				}

				static void Enable()
				{
					ClkEnReg::Enable();
				}

				static void Disable()
				{
					ClkEnReg::Disable();
				}
				enum{Id = ID};
			};

			/*Lower part of port. Need for effective configuration writing*/
			template<class Regs, class ClkEnReg, int ID>
			class PortImplementationL :public PortImplementation<Regs, ClkEnReg, ID>
			{
				typedef PortImplementation<Regs, ClkEnReg, ID> Base;
				public:
					typedef typename Base::Configuration Configuration;
					typedef typename Base::DataT DataT;

					template<unsigned pin>
					static void SetPinConfiguration(Configuration configuration)
					{
						STATIC_ASSERT(pin < 8);
						Regs()->CRL = (Regs()->CRL & ~(0x0fu << pin*4)) | ((unsigned)configuration << pin*4);
					}
					static void SetConfiguration(DataT mask, Configuration configuration)
					{
						Regs()->CRL = NativePortBase::UnpackConfig(mask, Regs()->CRL, configuration, 0x0f);
					}
					template<DataT mask, Configuration configuration>
					static void SetConfiguration()
					{
						unsigned configMask = ConfigurationMask<mask>::value;
						Regs()->CRL = (Regs()->CRL & ~(configMask*0x0f)) | configMask * configuration;
					}
			};

			 /*Hight part of port. Need for effective configuration writing*/
			template<class Regs, class ClkEnReg, int ID>
			class PortImplementationH :public PortImplementation<Regs, ClkEnReg, ID>
			{
				typedef PortImplementation<Regs, ClkEnReg, ID> Base;
				public:
					typedef typename Base::Configuration Configuration;
					typedef typename Base::DataT DataT;

				template<unsigned pin>
				static void SetPinConfiguration(Configuration configuration)
				{
					STATIC_ASSERT(pin >= 8 && pin < 16);
					Regs()->CRH = (Regs()->CRH & ~(0x0fu << (pin-8)*4)) | ((unsigned)configuration << (pin-8)*4);
				}
				static void SetConfiguration(DataT mask, Configuration configuration)
				{
					Regs()->CRH = NativePortBase::UnpackConfig((mask>>8), Regs()->CRH, configuration, 0x0f);
				}
				template<DataT mask, Configuration configuration>
				static void SetConfiguration()
				{
					unsigned configMask = ConfigurationMask<(mask>>8)>::value;
					Regs()->CRH = (Regs()->CRH & ~(configMask*0x0f)) | configMask * configuration;
				}
			};
		}

	#define MAKE_PORT(REGS, ClkEnReg, className, ID) \
	   namespace Private{\
			IO_STRUCT_WRAPPER(REGS, className ## Regs, GPIO_TypeDef);\
		}\
		  typedef Private::PortImplementation<\
				Private::className ## Regs, \
				ClkEnReg,\
				ID> className; \
			typedef Private::PortImplementationL<\
				Private::className ## Regs, \
				ClkEnReg,\
				ID> className ## L; \
			typedef Private::PortImplementationH<\
				Private::className ## Regs, \
				ClkEnReg,\
				ID> className ## H; \

	#ifdef USE_PORTA
	MAKE_PORT(GPIOA, Clock::PortaClock, Porta, 'A')
	#define MCUCPP_HAS_PORTA
	#endif

	#ifdef USE_PORTB
	MAKE_PORT(GPIOB, Clock::PortbClock, Portb, 'B')
	#define MCUCPP_HAS_PORTB
	#endif

	#ifdef USE_PORTC
	MAKE_PORT(GPIOC, Clock::PortcClock, Portc, 'C')
	#define MCUCPP_HAS_PORTC
	#endif

	#ifdef USE_PORTD
	MAKE_PORT(GPIOD, Clock::PortdClock, Portd, 'D')
	#define MCUCPP_HAS_PORTD
	#endif

	#ifdef USE_PORTE
	MAKE_PORT(GPIOE, Clock::PorteClock, Porte, 'E')
	#define MCUCPP_HAS_PORTE
	#endif

	#ifdef USE_PORTF
	MAKE_PORT(GPIOF, Clock::PortfClock, Portf, 'F')
	#define MCUCPP_HAS_PORTF
	#endif

	#ifdef USE_PORTG
	MAKE_PORT(GPIOG, Clock::PortgClock, Portg, 'G')
	#define MCUCPP_HAS_PORTG
	#endif

	//==================================================================================================
	}//namespace IO
}
#endif
