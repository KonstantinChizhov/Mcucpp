#pragma once

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif

#ifndef LPC17_PORTS_H
#define LPC17_PORTS_H

#include <ioreg.h>
#include <LPC17xx.h>
#include <static_assert.h>

#define USE_PORT1
#define USE_PORT2
#define USE_PORT3
#define USE_PORT4

namespace Mcucpp
{
	namespace IO
	{
		class NativePortBase
		{
		public:
			enum{Width=32};
			typedef uint32_t DataT;
			enum Configuration
			{
				In  = 0,
				Out = 1,
			};
			
			enum PullMode
			{
				NoPullUp = 2,
				PullUp   = 0,
				PullDown = 3,
				Repeater = 1,
			};
			
			enum DriverType
			{
				PushPull  = 0,
				OpenDrain = 1,
			};

			enum Speed // Speed is constant
			{
				Slow    = 0,
				Fast    = 0,
				Fastest = 0
			};
			
			static inline unsigned UnpackConfig2bits(unsigned mask, unsigned value, unsigned configuration)
			{
				mask = (mask & 0xff00)     << 8 | (mask & 0x00ff);
				mask = (mask & 0x00f000f0) << 4 | (mask & 0x000f000f);
				mask = (mask & 0x0C0C0C0C) << 2 | (mask & 0x03030303);
				mask = (mask & 0x22222222) << 1 | (mask & 0x11111111);
				return (value & ~(mask*0x03)) | mask * configuration;
			}

		};
		
		namespace Private
		{
			template<class SEL0, class SEL1, class MODE0, class MODE1, class OD, class DIR, class PIN, class SET, class CLR, int ID>
			class PortImplementation :public NativePortBase
			{
			public:
				static DataT Read()
				{
				  return SET::Get();
				}
				static void Write(DataT value)
				{
					PIN::Set(value);
				}
				static void ClearAndSet(DataT clearMask, DataT value)
				{
					CLR::Set(clearMask);
					SET::Set(value);
				}
				static void Set(DataT value)
				{
					SET::Set(value);
				}
				static void Clear(DataT value)
				{
					CLR::Set(value);
				}
				static void Toggle(DataT value)
				{
					PIN::Xor(value);
				}
				static DataT PinRead()
				{
					return PIN::Get();
				}

				// constant interface

				template<DataT clearMask, DataT value>
				static void ClearAndSet()
				{
					CLR::Set(clearMask);
					SET::Set(value);
				}

				template<DataT value>
				static void Toggle()
				{
					PIN::Xor(value);
				}

				template<DataT value>
				static void Set()
				{
					SET::Set(value);
				}

				template<DataT value>
				static void Clear()
				{
					CLR::Set(value);
				}

				template<unsigned pin>
				static void SetPinConfiguration(Configuration configuration)
				{
					STATIC_ASSERT(pin < Width);
					DIR::AndOr(~(1u << pin), static_cast<DataT>(configuration) << pin);
				}
				
				static void SetConfiguration(DataT mask, Configuration configuration)
				{
					if(configuration)
						DIR::Or(mask);
					else
						DIR::And(~mask);
				}
				
				template<DataT mask, Configuration configuration>
				static void SetConfiguration()
				{
					if(configuration)
						DIR::Or(mask);
					else
						DIR::And(~mask);
				}
				
				static void SetSpeed(DataT mask, Speed speed)
				{
					// speed is constant
				}
				
				static void SetPullUp(DataT mask, PullMode pull)
				{
					MODE0::Set( UnpackConfig2bits(mask,       MODE0::Get(), pull) );
					MODE1::Set( UnpackConfig2bits(mask >> 16, MODE1::Get(), pull) );
				}
				
				static void SetDriverType(DataT mask, DriverType driver)
				{
					if(driver)
						OD::Or(mask);
					else
						OD::And(~mask);
				}
				
				static void AltFuncNumber(DataT mask, uint8_t number)
				{
					SEL0::Set( UnpackConfig2bits(mask,       SEL0::Get(), number) );
					SEL1::Set( UnpackConfig2bits(mask >> 16, SEL1::Get(), number) );
				}

				// static void Enable()
				// {
				// 	//use Clock::GpioClock::Enable() instead
				// }

				// static void Disable()
				// {
				// 	//use Clock::GpioClock::Disable() instead
				// }
				
				enum{Id = ID};
			};
			
		}

		#define MAKE_PORT(SEL0, SEL1, MODE0, MODE1, OD, DIR, PIN, SET, CLR, className, ID) \
		   namespace Private{\
				IO_REG_WRAPPER(SEL0,  className ## Sel0,  uint32_t);\
				IO_REG_WRAPPER(SEL1,  className ## Sel1,  uint32_t);\
				IO_REG_WRAPPER(MODE0, className ## Mode0, uint32_t);\
				IO_REG_WRAPPER(MODE1, className ## Mode1, uint32_t);\
				IO_REG_WRAPPER(OD,    className ## Od,    uint32_t);\
				IO_REG_WRAPPER(DIR,   className ## Dir,   uint32_t);\
				IO_REG_WRAPPER(PIN,   className ## Pin,   uint32_t);\
				IO_REG_WRAPPER(SET,   className ## Set,   uint32_t);\
				IO_REG_WRAPPER(CLR,   className ## Clr,   uint32_t);\
			}\
			  typedef Private::PortImplementation<\
					Private::className ## Sel0, \
					Private::className ## Sel1, \
					Private::className ## Mode0, \
					Private::className ## Mode1, \
					Private::className ## Od, \
					Private::className ## Dir, \
					Private::className ## Pin, \
					Private::className ## Set, \
					Private::className ## Clr, \
					ID> className; \

		#ifdef USE_PORT0
		MAKE_PORT(LPC_PINCON->PINSEL0, LPC_PINCON->PINSEL1, LPC_PINCON->PINMODE0, LPC_PINCON->PINMODE1, LPC_PINCON->PINMODE_OD0, LPC_GPIO0->FIODIR, LPC_GPIO0->FIOPIN, LPC_GPIO0->FIOSET, LPC_GPIO0->FIOCLR, Port0, '0')
		#define MCUCPP_HAS_PORT0
		#endif

		#ifdef USE_PORT1
		MAKE_PORT(LPC_PINCON->PINSEL2, LPC_PINCON->PINSEL3, LPC_PINCON->PINMODE2, LPC_PINCON->PINMODE3, LPC_PINCON->PINMODE_OD1, LPC_GPIO1->FIODIR, LPC_GPIO1->FIOPIN, LPC_GPIO1->FIOSET, LPC_GPIO1->FIOCLR, Port1, '1')
		#define MCUCPP_HAS_PORT1
		#endif

		#ifdef USE_PORT2
		MAKE_PORT(LPC_PINCON->PINSEL4, LPC_PINCON->PINSEL5, LPC_PINCON->PINMODE4, LPC_PINCON->PINMODE5, LPC_PINCON->PINMODE_OD2, LPC_GPIO2->FIODIR, LPC_GPIO2->FIOPIN, LPC_GPIO2->FIOSET, LPC_GPIO2->FIOCLR, Port2, '2')
		#define MCUCPP_HAS_PORT2
		#endif

		#ifdef USE_PORT3
		MAKE_PORT(LPC_PINCON->PINSEL6, LPC_PINCON->PINSEL7, LPC_PINCON->PINMODE6, LPC_PINCON->PINMODE7, LPC_PINCON->PINMODE_OD3, LPC_GPIO3->FIODIR, LPC_GPIO3->FIOPIN, LPC_GPIO3->FIOSET, LPC_GPIO3->FIOCLR, Port3, '3')
		#define MCUCPP_HAS_PORT3
		#endif

		#ifdef USE_PORT4
		MAKE_PORT(LPC_PINCON->PINSEL8, LPC_PINCON->PINSEL9, LPC_PINCON->PINMODE8, LPC_PINCON->PINMODE9, LPC_PINCON->PINMODE_OD4, LPC_GPIO4->FIODIR, LPC_GPIO4->FIOPIN, LPC_GPIO4->FIOSET, LPC_GPIO4->FIOCLR, Port4, '4')
		#define MCUCPP_HAS_PORT4
		#endif
	}//namespace IO
}
#endif
