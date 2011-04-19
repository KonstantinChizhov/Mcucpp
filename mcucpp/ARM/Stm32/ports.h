#pragma once

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif

#ifndef STM32_PORTS_H
#define STM32_PORTS_H

#include "ioreg.h"
#include "stm32f10x.h"

#include <static_assert.h>
#define USE_SPLIT_PORT_CONFIGURATION 8

namespace IO
{
	class NativePortBase :public GpioBase
	{
	public:
		enum{Width=16};
		typedef NativePortBase Base;
		typedef uint16_t DataT;
		enum Configuration
		{
			AnalogIn = 0,
			In = 0x04,
			PullUpOrDownIn = 0x08,
			Out = 0x03, // default Out is Out50Mhz
			Out10Mhz = 0x01,
			Out2Mhz = 0x02,
			Out50Mhz = 0x03,
			OpenDrainOut = 0x07,
			OpenDrainOut10Mhz = 0x05,
			OpenDrainOut2Mhz = 0x06,
			OpenDrainOut50Mhz = 0x07,
			AltOut = 0x0B,
			AltOut10Mhz = 0x09,
			AltOut2Mhz = 0x0A,
			AltOut50Mhz = 0x0B,
			AltOpenDrain = 0x0f,
			AltOpenDrain10Mhz = 0x0C,
			AltOpenDrain2Mhz = 0x0E,
			AltOpenDrain50Mhz = 0x0f
		};

		static inline unsigned UnpackConfig(unsigned mask, unsigned value, Configuration configuration)
		{
			mask = (mask & 0xf0) << 12 | (mask & 0x0f);
			mask = (mask & 0x000C000C) << 6 | (mask & 0x00030003);
			mask = (mask & 0x02020202) << 3 | (mask & 0x01010101);
			return (value & ~(mask*0x0f)) | mask * configuration;
		}

		static Configuration MapConfiguration(GenericConfiguration config)
		{
			switch(config)
			{
			case GpioBase::In: return In;
			case GpioBase::AnalogIn: return AnalogIn;
			case GpioBase::PullUpOrDownIn: return PullUpOrDownIn;
			case GpioBase::OpenDrainOut: return OpenDrainOut;
			case GpioBase::AltOut: return AltOut;
			case GpioBase::AltOpenDrain: return AltOpenDrain;
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
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::OpenDrainOut>{static const Configuration value = OpenDrainOut;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::AltOut>{static const Configuration value = AltOut;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::AltOpenDrain>{static const Configuration value = AltOpenDrain;};
	template<> struct NativePortBase::MapConfigurationConst<GpioBase::PullUpOrDownIn>{static const Configuration value = PullUpOrDownIn;};

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

		template<class ConfigReg, unsigned mask, NativePortBase::Configuration config>
		struct WriteConfig
		{
			static void Write()
			{
			  const unsigned configMask = ConfigurationMask<mask>::value;
			  unsigned result = (ConfigReg::Get() & ~(configMask*0x0f)) | configMask * config;
			  ConfigReg::Set(result);
			}
		};

		template<class CRL, class CRH, class IDR, class ODR, class BSRR, class BRR, class LCKR, uint32_t ClkEnMask, int ID>
		class PortImplementation :public NativePortBase
		{
		public:
			static DataT Read()
			{
			  return ODR::Get();
			}
			static void Write(DataT value)
			{
				ODR::Set(value);
			}
			static void ClearAndSet(DataT clearMask, DataT value)
			{
				BSRR::Set(value | (uint32_t)clearMask << 16);
			}
			static void Set(DataT value)
			{
				BSRR::Set(value);
			}
			static void Clear(DataT value)
			{
				BSRR::Set((uint32_t)value << 16);
			}
			static void Toggle(DataT value)
			{
				ODR::Xor(value);
			}
			static DataT PinRead()
			{
				return IDR::Get();
			}

			// constant interface

			template<DataT clearMask, DataT value>
			static void ClearAndSet()
			{
				BSRR::Set(value | (uint32_t)clearMask << 16);
			}

			template<DataT value>
			static void Toggle()
			{
				ODR::Xor(value);
			}

			template<DataT value>
			static void Set()
			{
				BSRR::Set(value);
			}

			template<DataT value>
			static void Clear()
			{
				BSRR::Set((uint32_t)value << 16);
			}

			template<unsigned pin>
			static void SetPinConfiguration(Configuration configuration)
			{
				BOOST_STATIC_ASSERT(pin < Width);
				if(pin < 8)
				{
					CRL::AndOr(~(0x0fu << pin*4), (unsigned)configuration << pin*4);
				}
				else
				{
					CRH::AndOr(~(0x0fu << (pin-8)*4), (unsigned)configuration << (pin-8)*4);
				}
			}
			static void SetConfiguration(DataT mask, Configuration configuration)
			{
				CRL::Set(UnpackConfig(mask, CRL::Get(), configuration));
				CRH::Set(UnpackConfig((mask>>8), CRH::Get(), configuration));
			}
			template<DataT mask, Configuration configuration>
			static void SetConfiguration()
			{
			  WriteConfig<CRL, mask, configuration>::Write();
			  WriteConfig<CRH, (mask>>8), configuration>::Write();
			}
			
			static void Enable()
			{
				RCC->APB2ENR |= ClkEnMask;
			}
			
			static void Disable()
			{
				RCC->APB2ENR &= ~ClkEnMask;
			}
			enum{Id = ID};
		};

		/*Lower part of port. Need for effective configuration writing*/
		template<class CRL, class CRH, class IDR, class ODR, class BSRR, class BRR, class LCKR, uint32_t ClkEnMask, int ID>
		class PortImplementationL :public PortImplementation<CRL, CRH, IDR, ODR, BSRR, BRR, LCKR, ClkEnMask, ID>
		{
			typedef PortImplementation<CRL, CRH, IDR, ODR, BSRR, BRR, LCKR, ClkEnMask, ID> Base;
			public:
				typedef typename Base::Configuration Configuration;
				typedef typename Base::DataT DataT;

				template<unsigned pin>
				static void SetPinConfiguration(Configuration configuration)
				{
					BOOST_STATIC_ASSERT(pin < 8);
					CRL::AndOr(~(0x0fu << pin*4), (uint32_t)configuration << pin*4);
				}
				static void SetConfiguration(DataT mask, Configuration configuration)
				{
					CRL::Set(NativePortBase::UnpackConfig(mask, CRL::Get(), configuration));
				}
				template<DataT mask, Configuration configuration>
				static void SetConfiguration()
				{
					 WriteConfig<CRL, mask, configuration>::Write();
				}
		};

		 /*Hight part of port. Need for effective configuration writing*/
		template<class CRL, class CRH, class IDR, class ODR, class BSRR, class BRR, class LCKR, uint32_t ClkEnMask, int ID>
		class PortImplementationH :public PortImplementation<CRL, CRH, IDR, ODR, BSRR, BRR, LCKR, ClkEnMask, ID>
		{
			typedef PortImplementation<CRL, CRH, IDR, ODR, BSRR, BRR, LCKR, ClkEnMask, ID> Base;
			public:
				typedef typename Base::Configuration Configuration;
				typedef typename Base::DataT DataT;

			template<unsigned pin>
			static void SetPinConfiguration(Configuration configuration)
			{
				BOOST_STATIC_ASSERT(pin >= 8 && pin < 16);
				CRH::AndOr(~(0x0fu << (pin-8)*4), (uint32_t)configuration << (pin-8)*4);
			}
			static void SetConfiguration(DataT mask, Configuration configuration)
			{
			  CRH::Set(NativePortBase::UnpackConfig(mask>>8, CRH::Get(), configuration));
			}
			template<DataT mask, Configuration configuration>
			static void SetConfiguration()
			{
				WriteConfig<CRH, (mask>>8), configuration>::Write();
			}
		};
	}

#define MAKE_PORT(CRL, CRH, IDR, ODR, BSRR, BRR, LCKR, ClkEnMask, className, ID) \
   namespace Private{\
		IO_REG_WRAPPER(CRL, className ## Crl, uint32_t);\
		IO_REG_WRAPPER(CRH, className ## Crh, uint32_t);\
		IO_REG_WRAPPER(IDR, className ## Idr, uint32_t);\
		IO_REG_WRAPPER(ODR, className ## Odr, uint32_t);\
		IO_REG_WRAPPER(BSRR, className ## Bsrr, uint32_t);\
		IO_REG_WRAPPER(BRR, className ## Brr, uint32_t);\
		IO_REG_WRAPPER(LCKR, className ## Lckr, uint32_t);\
	}\
	  typedef Private::PortImplementation<\
			Private::className ## Crl, \
			Private::className ## Crh, \
			Private::className ## Idr, \
			Private::className ## Odr, \
			Private::className ## Bsrr, \
			Private::className ## Brr, \
			Private::className ## Lckr, \
			ClkEnMask,\
			ID> className; \
		typedef Private::PortImplementationL<\
			Private::className ## Crl, \
			Private::className ## Crh, \
			Private::className ## Idr, \
			Private::className ## Odr, \
			Private::className ## Bsrr, \
			Private::className ## Brr, \
			Private::className ## Lckr, \
			ClkEnMask,\
			ID> className ## L; \
		typedef Private::PortImplementationH<\
			Private::className ## Crl, \
			Private::className ## Crh, \
			Private::className ## Idr, \
			Private::className ## Odr, \
			Private::className ## Bsrr, \
			Private::className ## Brr, \
			Private::className ## Lckr, \
			ClkEnMask,\
			ID> className ## H; \

#ifdef USE_PORTA
MAKE_PORT(GPIOA->CRL, GPIOA->CRH, GPIOA->IDR, GPIOA->ODR, GPIOA->BSRR, GPIOA->BRR, GPIOA->LCKR, 1 << 2, Porta, 'A')
#endif

#ifdef USE_PORTB
MAKE_PORT(GPIOB->CRL, GPIOB->CRH, GPIOB->IDR, GPIOB->ODR, GPIOB->BSRR, GPIOB->BRR, GPIOB->LCKR, 1 << 3, Portb, 'B')
#endif

#ifdef USE_PORTC
MAKE_PORT(GPIOC->CRL, GPIOC->CRH, GPIOC->IDR, GPIOC->ODR, GPIOC->BSRR, GPIOC->BRR, GPIOC->LCKR, 1 << 4, Portc, 'C')
#endif

#ifdef USE_PORTD
MAKE_PORT(GPIOD->CRL, GPIOD->CRH, GPIOD->IDR, GPIOD->ODR, GPIOD->BSRR, GPIOD->BRR, GPIOD->LCKR, 1 << 4, Portd, 'D')
#endif

#ifdef USE_PORTE
MAKE_PORT(GPIOE->CRL, GPIOE->CRH, GPIOE->IDR, GPIOE->ODR, GPIOE->BSRR, GPIOE->BRR, GPIOE->LCKR, 1 << 5, Porte, 'E')
#endif

#ifdef USE_PORTF
MAKE_PORT(GPIOF->CRL, GPIOF->CRH, GPIOF->IDR, GPIOF->ODR, GPIOF->BSRR, GPIOF->BRR, GPIOF->LCKR, 1 << 6, Portf, 'F')
#endif

#ifdef USE_PORTG
MAKE_PORT(GPIOG->CRL, GPIOG->CRH, GPIOG->IDR, GPIOG->ODR, GPIOG->BSRR, GPIOG->BRR, GPIOG->LCKR, 1 << 7, Portg, 'G')
#endif

//==================================================================================================
}//namespace IO

#endif
