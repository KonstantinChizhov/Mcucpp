#pragma once

#ifndef IOPORTS_HPP
#error "Do not include this file directly. Use ioports.h instead."
#endif

#ifndef MDR_1986BE9x_PORTS_H
#define MDR_1986BE9x_PORTS_H

#include "ioreg.h"
#include "MDR32Fx.h"
#include "clock.h"

#include <static_assert.h>

namespace Mcucpp
{
	namespace IO
	{
		class NativePortBase :public GpioBase
		{
		public:
			enum{Width=16};
			typedef NativePortBase Base;
			typedef uint16_t DataT;
			enum ConfigurationFields
			{
				ModeMainFunc        = 0x0001,
				ModeAltFunc         = 0x0002,
				ModeRemapFunc       = 0x0003,
				ModeMask            = 0x0003,
				OutputEnable        = 0x0004,
				AnalogField         = 0x0008,
				PullupField         = 0x0010,
				PulldownField       = 0x0020,
				SchmittTriggerField = 0x0040,
				OpenDrainField      = 0x0080,
				SpeedSlow           = 0x0100,
				SpeedFast           = 0x0200,
				SpeedFastest        = 0x0300,
				SpeedMask           = 0x0300,
				SpeedShift          = 8
			};
			
			enum Configuration
			{
				AnalogIn = 0x00,
				In = 0x00,
				PullUpIn = PullupField,
				PullDownIn = PulldownField,
				PullUpOrDownIn = PullUpIn,
				
				Out = OutFast,
				OutFast = OutputEnable | SpeedFast | AnalogField,
				OutSlow = OutputEnable | SpeedSlow | AnalogField,
				OutFastest = OutputEnable | SpeedFastest | AnalogField,
				
				OpenDrainOut = OpenDrainOutFast,
				OpenDrainOutFast = OutFast | OpenDrainField,
				OpenDrainOutSlow = OutSlow | OpenDrainField,
				OpenDrainOutFastest = OpenDrainOut | OpenDrainField,
				
				AltOut = AltOutFast,
				AltOutFast = OutFast | ModeMainFunc,
				AltOutSlow = OutSlow | ModeMainFunc,
				AltOutFastest =  OutFastest | ModeMainFunc,
				
				Alt2Out = Alt2OutFast,
				Alt2OutFast = OutFast | ModeAltFunc,
				Alt2OutSlow = OutSlow | ModeAltFunc,
				Alt2OutFastest =  OutFastest | ModeAltFunc,
				
				RemapOut = Alt2OutFast,
				RemapOutFast = OutFast | ModeRemapFunc,
				RemapOutSlow = OutSlow | ModeRemapFunc,
				RemapOutFastest =  OutFastest | ModeRemapFunc,
				
				AltOpenDrain = AltOpenDrainFast,
				AltOpenDrainFast = AltOutFast | OpenDrainField,
				AltOpenDrainSlow = AltOutSlow | OpenDrainField,
				AltOpenDrainFastest = AltOutFastest | OpenDrainField
			};
			
			static inline unsigned UnpackConfig(unsigned mask, unsigned value, unsigned configuration)
			{
				mask = (mask & 0xff00) << 8 | (mask & 0x00ff);
				mask = (mask & 0x00f000f0) << 4 | (mask & 0x000f000f);
				mask = (mask & 0x0C0C0C0C) << 2 | (mask & 0x03030303);
				mask = (mask & 0x22222222) << 1 | (mask & 0x11111111);
				return (value & ~(mask*0x03)) | mask * configuration;
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
			template<class GpioStruct, class Clock, int ID>
			class PortImplementation :public NativePortBase
			{
			public:
				static DataT Read()
				{
				  return GpioStruct()->RXTX;
				}
				static void Write(DataT value)
				{
					GpioStruct()->RXTX = value;
				}
				static void ClearAndSet(DataT clearMask, DataT value)
				{
					GpioStruct()->RXTX = (GpioStruct()->RXTX & ~clearMask) | value;
				}
				static void Set(DataT value)
				{
					GpioStruct()->RXTX |= value;
				}
				static void Clear(DataT value)
				{
					GpioStruct()->RXTX &= ~value;
				}
				static void Toggle(DataT value)
				{
					GpioStruct()->RXTX ^= value;
				}
				static DataT PinRead()
				{
					return GpioStruct()->RXTX;
				}

				// constant interface

				template<DataT clearMask, DataT value>
				static void ClearAndSet()
				{
					GpioStruct()->RXTX = (GpioStruct()->RXTX & ~clearMask) | value;
				}

				template<DataT value>
				static void Toggle()
				{
					GpioStruct()->RXTX ^= value;
				}

				template<DataT value>
				static void Set()
				{
					GpioStruct()->RXTX |= value;
				}

				template<DataT value>
				static void Clear()
				{
					GpioStruct()->RXTX &= ~value;
				}

				template<unsigned pin>
				static void SetPinConfiguration(Configuration configuration)
				{
					BOOST_STATIC_ASSERT(pin < Width);
					uint32_t mask = 1 << pin;
					SetConfiguration(mask, configuration);
				}
				static void SetConfiguration(DataT mask, Configuration configuration)
				{
					if((unsigned)configuration & (unsigned)OutputEnable)
						GpioStruct()->OE |= mask;
					else
						GpioStruct()->OE &= ~mask;
					
					GpioStruct()->FUNC = UnpackConfig(mask, GpioStruct()->FUNC, configuration & ModeMask);
					
					if((unsigned)configuration & (unsigned)AnalogField)
						GpioStruct()->ANALOG |= mask;
					else
						GpioStruct()->ANALOG &= ~mask;
					
					if((unsigned)configuration & (unsigned)PullupField)
						GpioStruct()->PULL |= mask;
					else if((unsigned)configuration & (unsigned)PulldownField)
						GpioStruct()->PULL |= mask << 16;
					else
						GpioStruct()->PULL &= ((~mask) << 16) | ((~mask) & 0x0000ffff);
					
					if((unsigned)configuration & (unsigned)(SchmittTriggerField | OpenDrainField) == 
						(unsigned)(SchmittTriggerField | OpenDrainField))
						GpioStruct()->PD |= mask | (mask << 16);
					else if((unsigned)configuration & (unsigned)(SchmittTriggerField | OpenDrainField) == 
						(unsigned)(SchmittTriggerField))
						GpioStruct()->PD |= mask;
					else if((unsigned)configuration & (unsigned)(SchmittTriggerField | OpenDrainField) == 
						(unsigned)(OpenDrainField))
						GpioStruct()->PD |= (mask << 16);
					else
						GpioStruct()->PD &= ~(mask | (mask << 16));
					
					GpioStruct()->PWR = UnpackConfig(mask, GpioStruct()->PWR, (configuration & SpeedMask) >> SpeedShift);
				}
				template<DataT mask, Configuration configuration>
				static void SetConfiguration()
				{
					if((unsigned)configuration & (unsigned)OutputEnable)
						GpioStruct()->OE |= mask;
					else
						GpioStruct()->OE &= ~mask;
					
					GpioStruct()->FUNC = UnpackConfig(mask, GpioStruct()->FUNC, configuration & ModeMask);
					
					if((unsigned)configuration & (unsigned)AnalogField)
						GpioStruct()->ANALOG |= mask;
					else
						GpioStruct()->ANALOG &= ~mask;
					
					if((unsigned)configuration & (unsigned)PullupField)
						GpioStruct()->PULL |= mask;
					else if((unsigned)configuration & (unsigned)PulldownField)
						GpioStruct()->PULL |= mask << 16;
					else
						GpioStruct()->PULL &= ((~mask) << 16) | ((~mask) & 0x0000ffff);
					
					if((unsigned)configuration & (unsigned)(SchmittTriggerField | OpenDrainField) == 
						(unsigned)(SchmittTriggerField | OpenDrainField))
						GpioStruct()->PD |= mask | (mask << 16);
					else if((unsigned)configuration & (unsigned)(SchmittTriggerField | OpenDrainField) == 
						(unsigned)(SchmittTriggerField))
						GpioStruct()->PD |= mask;
					else if((unsigned)configuration & (unsigned)(SchmittTriggerField | OpenDrainField) == 
						(unsigned)(OpenDrainField))
						GpioStruct()->PD |= (mask << 16);
					else
						GpioStruct()->PD &= ~(mask | (mask << 16));
					
					GpioStruct()->PWR = UnpackConfig(mask, GpioStruct()->PWR, (configuration & SpeedMask) >> SpeedShift);
				}

				static void Enable()
				{
					Clock::Enable();
				}

				static void Disable()
				{
					Clock::Disable();
				}
				enum{Id = ID};
			};


	#define MAKE_PORT(PortStruct, ClkEnReg, className, ID) \
	   namespace Private{\
			IO_STRUCT_WRAPPER(PortStruct, className ## Regs, MDR_PORT_TypeDef);\
		}\
		  typedef Private::PortImplementation<\
				className ## Regs, \
				ClkEnReg,\
				ID> className \

	MAKE_PORT(MDR_PORTA, Clock::PortaClock, Porta, 'A');
	MAKE_PORT(MDR_PORTB, Clock::PortbClock, Portb, 'B');
	MAKE_PORT(MDR_PORTC, Clock::PortcClock, Portc, 'C');
	MAKE_PORT(MDR_PORTD, Clock::PortdClock, Portd, 'D');
	MAKE_PORT(MDR_PORTE, Clock::PorteClock, Porte, 'E');
	MAKE_PORT(MDR_PORTF, Clock::PortfClock, Portf, 'F');

	//==================================================================================================
	}//namespace IO
}
#endif
