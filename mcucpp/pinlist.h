
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

#include "iopin.h"
#include "loki/Typelist.h"
#include "gpiobase.h"
#include <static_if.h>
#include <select_size.h>

namespace IO
{
	using namespace Loki;
	using namespace Loki::TL;

	namespace IoPrivate
	{

////////////////////////////////////////////////////////////////////////////////
// class template CheckSameConfig
// Checks if all ports has the same configuration enum
// PlaceHolderType matchs any type in TList
////////////////////////////////////////////////////////////////////////////////

		template <class TList, class CurrentConfig = typename TList::Head::Configuration>
		struct CheckSameConfigImp;

		template<class TList, class CurrentConfig, class NextConfig>
		struct CheckSameConfigHelper
		{
			static const bool value = false;
		};

		template<class Head, class Tail, class CurrentConfig>
		struct CheckSameConfigHelper<Typelist<Head, Tail>, CurrentConfig, CurrentConfig>
		{
			static const bool value = CheckSameConfigImp<Tail, CurrentConfig>::value;
		};

        template <class CurrentConfig> struct CheckSameConfigImp<NullType, CurrentConfig>
        {
            static const bool value = true;
        };

        template <class Head, class Tail, class CurrentConfig>
        struct CheckSameConfigImp<Typelist<Head, Tail>, CurrentConfig>
        {
			static const bool value = CheckSameConfigHelper<Typelist<Head, Tail>, CurrentConfig, typename Head::Configuration>::value;
        };

		template <class TList>
		struct CheckSameConfig
		{
			static const bool value = CheckSameConfigImp<TList, typename TList::Head::Configuration>::value;
		};

		template <>
		struct CheckSameConfig<NullType>
		{
				static const bool value = true;
		};


////////////////////////////////////////////////////////////////////////////////
		template <class TList, class T, class ConfigT>
		struct ErisePortsHelper;

		template <class TList, class T>
		struct ErasePortsWithConfig
		{
			typedef typename ErisePortsHelper<TList, T, typename TList::Head::Configuration>::Result Result;
		};

		template < class T>
		struct ErasePortsWithConfig<NullType, T>
		{
			typedef NullType Result;
		};

        template <class T, class ConfigT>
        struct ErisePortsHelper<NullType, T, ConfigT>
        {
            typedef NullType Result;
        };

        template <class Head, class Tail, class T>
        struct ErisePortsHelper<Typelist<Head, Tail>, T, T>
        {
            // Go all the way down the list removing the type
            typedef typename ErasePortsWithConfig<Tail, T>::Result Result;
        };

        template <class Head, class Tail, class T, class ConfigT>
        struct ErisePortsHelper<Typelist<Head, Tail>, T, ConfigT>
        {
            // Go all the way down the list removing the type
            typedef Typelist<Head,
                    typename ErasePortsWithConfig<Tail, T>::Result>
                Result;
        };

////////////////////////////////////////////////////////////////////////////////

		template<unsigned BitsToShift>
		struct ShiftLeft
		{
			template<class T>
			static T Shift(T value)
			{
				return value << BitsToShift;
			}
		};

		template<unsigned BitsToShift>
		struct ShiftRight
		{
			template<class T>
			static T Shift(T value)
			{
				return value >> BitsToShift;
			}
		};

		enum MapDirection{ValueToPort = 1 , PortToValue = 0};

		template<unsigned First, unsigned Second, MapDirection mapDirection>
		class Shifter
		{
			typedef ShiftRight<First - Second> RightShifter1;
			typedef ShiftRight<Second - First> RightShifter2;

			typedef ShiftLeft<First - Second> LeftShifter1;
			typedef ShiftLeft<Second - First> LeftShifter2;

            static const bool ShiftDirection = First > Second;
			typedef typename StaticIf<ShiftDirection, LeftShifter1, RightShifter2>::Result FirstShifter;
			typedef typename StaticIf<ShiftDirection, RightShifter1, LeftShifter2>::Result SecondShifter;

			typedef typename StaticIf<mapDirection, FirstShifter, SecondShifter>::Result ActualShifter;
		public:
			template<class T>
			static T Shift(T value)
			{
				return ActualShifter::Shift(value);
			}
		};

////////////////////////////////////////////////////////////////////////////////
		template<class PINS> struct GetLastBitPosition;

		template<>
		struct GetLastBitPosition<NullType>
		{
		    static const unsigned value = 0;
		};

		template<class Head>
		struct GetLastBitPosition<Typelist<Head, NullType> >
		{
		    static const unsigned value = Head::Position;
		};

		template<class Head, class Tail>
		struct GetLastBitPosition<Typelist<Head, Tail> >
		{
		    static const unsigned value = GetLastBitPosition<Tail>::value;
		};

////////////////////////////////////////////////////////////////////////////////
// Holds a Pin class and its bit position in value to read/write
////////////////////////////////////////////////////////////////////////////////

		template<class TPIN, uint8_t POSITION>
		struct PinPositionHolder
		{
			typedef TPIN Pin;
			static const unsigned Position = POSITION;
		};

////////////////////////////////////////////////////////////////////////////////
// class template GetPorts
// Converts list of Pin types to Port types
////////////////////////////////////////////////////////////////////////////////

 		template <class TList> struct GetPorts;

        template <> struct GetPorts<NullType>
        {
            typedef NullType Result;
        };

        template <class Head, class Tail>
        struct GetPorts< Typelist<Head, Tail> >
        {
        private:
			typedef typename Head::Pin::Port Port;
            typedef typename GetPorts<Tail>::Result L1;
        public:
            typedef Typelist<Port, L1> Result;
        };

////////////////////////////////////////////////////////////////////////////////
		template <class TList> struct GetConfigPins;

        template <> struct GetConfigPins<NullType>
        {
            typedef NullType Result;
        };

        template <class Tail,
				uint8_t ValueBitPosition,
				class PinClass>
        struct GetConfigPins< Typelist<PinPositionHolder<PinClass, ValueBitPosition>, Tail> >
        {
		  typedef typename PinClass::ConfigPort ConfigPort;
        private:
			typedef PinPositionHolder<
			  TPin<ConfigPort, PinClass::Number, ConfigPort>,
						ValueBitPosition> Pin;
            typedef typename GetConfigPins<Tail>::Result L1;
        public:
            typedef Typelist<Pin, L1> Result;
        };

////////////////////////////////////////////////////////////////////////////////
// class template SelectPins
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

 		template <class TList, template <class> class Predicate>
		struct SelectPins;

        template <template <class> class Predicate>
        struct SelectPins<NullType, Predicate>
        {
            typedef NullType Result;
        };

        template <class Head, class Tail, template <class> class Predicate>
        class SelectPins<Typelist<Head, Tail>, Predicate>
        {
			 typedef typename SelectPins<Tail, Predicate>::Result NotMatch;
			 typedef Typelist<Head,
                    typename SelectPins<Tail, Predicate>::Result>
                Match;
			 static const bool IsMatch = Predicate<Head>::value;
		public:
			 typedef typename Select<IsMatch,
			 	Match,
			 	NotMatch>::Result Result;
        };

////////////////////////////////////////////////////////////////////////////////
//
// 			Select Predicates
//
////////////////////////////////////////////////////////////////////////////////
// class template TransparentMappedPins
// Selects pins types form pin list with port bit position equals to value bit position
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

	template<class Item>
	struct TransparentMappedPins
	{
		static const bool value = Item::Position == Item::Pin::Number;
	};

 	template<class Item>
	struct NotTransparentMappedPins
	{
		static const bool value = Item::Position != Item::Pin::Number;
	};

////////////////////////////////////////////////////////////////////////////////
// class template PinsWithPort
// Selects with specified port
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////
	template<class Port>
	struct PinsWithPort
	{
		template<class Item>
		struct Result
		{
			static const bool value = IsSameType<Port, typename Item::Pin::Port>::value;
		};
	};


////////////////////////////////////////////////////////////////////////////////
//	Mask for inverted pins
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct GetInversionMask;

        template <> struct GetInversionMask<NullType>
        {
            enum{value = 0};
        };

        template <class Head, class Tail>
        struct GetInversionMask< Typelist<Head, Tail> >
        {
			enum{value = (Head::Pin::Inverted ? (1 << Head::Pin::Number) : 0) | GetInversionMask<Tail>::value};
        };
////////////////////////////////////////////////////////////////////////////////
// class template GetPortMask
// Computes port bit mask for pin list
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct GetPortMask;

        template <> struct GetPortMask<NullType>
        {
            enum{value = 0};
        };

        template <class Head, class Tail>
        struct GetPortMask< Typelist<Head, Tail> >
        {
			enum{value = (1 << Head::Pin::Number) | GetPortMask<Tail>::value};
        };

////////////////////////////////////////////////////////////////////////////////
// class template GetValueMask
// Computes value bit mask for pin list
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct GetValueMask;

        template <> struct GetValueMask<NullType>
        {
            enum{value = 0};
        };

        template <class Head, class Tail>
        struct GetValueMask< Typelist<Head, Tail> >
        {
			enum{value = (1 << Head::Position) | GetValueMask<Tail>::value};
        };
////////////////////////////////////////////////////////////////////////////////
// class template GetSerialCount
// Computes number of seqental pins in list
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct GetSerialCount;

        template <> struct GetSerialCount<NullType>
        {
            static const int value = 0;
			static const int PinNumber = -1;
			static const int BitPosition = -1;
        };
        template <class Head, class Tail>

        struct GetSerialCount< Typelist<Head, Tail> >
        {
			typedef GetSerialCount<Tail> I;
			static const int PinNumber = Head::Pin::Number;
			static const int BitPosition = Head::Position;
			static const int value =
                ((PinNumber == I::PinNumber - 1 &&
                BitPosition == I::BitPosition) ?
                I::value + 1 : 1);
        };
////////////////////////////////////////////////////////////////////////////////
// Returns first Num elements from Typelist
////////////////////////////////////////////////////////////////////////////////
		template <class TList, uint8_t Num> struct TakeFirst;

		template <>
		struct TakeFirst<NullType, 0>
		{
			typedef NullType Result;
		};

		template <class Head, class Tail>
		struct TakeFirst<Typelist<Head, Tail>, 0>
		{
			typedef NullType Result;
		};

		template <class Head, class Tail, uint8_t Num>
		struct TakeFirst<Typelist<Head, Tail>, Num>
		{
			typedef Typelist<Head,
						typename TakeFirst<Tail, Num-1>::Result
						>Result;
		};
////////////////////////////////////////////////////////////////////////////////
// Skip Num first elements from tipelist
////////////////////////////////////////////////////////////////////////////////
		template <class TList, uint8_t Num> struct SkipFirst;

		template <>
		struct SkipFirst<NullType, 0>
		{
			typedef NullType Result;
		};

		template <class Head, class Tail>
		struct SkipFirst<Typelist<Head, Tail>, 0>
		{
			typedef Typelist<Head, Tail> Result;
		};

		template <class Head, class Tail, uint8_t Num>
		struct SkipFirst<Typelist<Head, Tail>, Num>
		{
			typedef typename SkipFirst<Tail, Num-1>::Result Result;
		};

////////////////////////////////////////////////////////////////////////////////
// class template PinWriteIterator
// Iterates througth pin list to compute value to write to their port
// Assume that TList is type list of PinPositionHolder types.
////////////////////////////////////////////////////////////////////////////////

		template <class TList> struct PinWriteIterator;

        template <> struct PinWriteIterator<NullType>
        {
			template<class DataType, class PortDataType>
			static inline PortDataType UppendValue(DataType value, PortDataType result)
			{
				return result;
			}

			template<class DataType, class PortDataType>
			static inline DataType UppendReadValue(PortDataType portValue, DataType result)
			{
				return result;
			}
        };

        template <class Head, class Tail>
        struct PinWriteIterator< Typelist<Head, Tail> >
        {
			//typedef typename Head::Pin::Port::DataT PortDataType;

			template<class DataType, class PortDataType>
			static inline PortDataType UppendValue(DataType value, PortDataType result)
			{
				typedef Typelist<Head, Tail> CurrentList;
				typedef typename SelectPins<CurrentList, TransparentMappedPins>::Result TransparentPins;
				typedef typename SelectPins<CurrentList, NotTransparentMappedPins>::Result NotTransparentPins;
				const int TransparentCount = Length<TransparentPins>::value;

				if(TransparentCount > 1)
				{
					result |= (value &
							GetPortMask<TransparentPins>::value) ^
							GetInversionMask<TransparentPins>::value;

					return PinWriteIterator<NotTransparentPins>::UppendValue(value, result);
				}

				enum{SerialLength = GetSerialCount<CurrentList>::value};

				if(SerialLength >= 2)
				{
					typedef typename TakeFirst<CurrentList, SerialLength>::Result SerialList;
					typedef typename SkipFirst<CurrentList, SerialLength>::Result RestList;

					result |= (Shifter<
							Head::Pin::Number, 	//bit position in port
							Head::Position, 	//bit position in value
							ValueToPort>::Shift(value) &
							GetPortMask<SerialList>::value) ^
							GetInversionMask<SerialList>::value;

					return PinWriteIterator<RestList>::UppendValue(value, result);
				}

				if(Head::Pin::Inverted == false)
				{
					if(value & (1 << Head::Position))
						result |= (1 << Head::Pin::Number);
				}
				else
				{
					if(!(value & (1 << Head::Position)))
						result |= (1 << Head::Pin::Number);
				}

				return PinWriteIterator<Tail>::UppendValue(value, result);
			}

			template<class DataType, class PortDataType>
			static inline DataType UppendReadValue(PortDataType portValue, DataType result)
			{
				typedef Typelist<Head, Tail> CurrentList;
				typedef typename SelectPins<CurrentList, TransparentMappedPins>::Result TransparentPins;
				typedef typename SelectPins<CurrentList, NotTransparentMappedPins>::Result NotTransparentPins;
				const int TransparentCount = Length<TransparentPins>::value;

				if(TransparentCount > 1)
				{
					result |= (portValue &
							GetValueMask<TransparentPins>::value) ^
							GetInversionMask<TransparentPins>::value;

					return PinWriteIterator<NotTransparentPins>::UppendReadValue(portValue, result);
				}
				enum{SerialLength = GetSerialCount<CurrentList>::value};

				if(SerialLength >= 2)
				{
					typedef typename TakeFirst<CurrentList, SerialLength>::Result SerialList;
					typedef typename SkipFirst<CurrentList, SerialLength>::Result RestList;

                    typedef Shifter<
							Head::Pin::Number, 	//bit position in port
							Head::Position, 	//bit position in value
							PortToValue> AtctualShifter;

					result |= (AtctualShifter::Shift(portValue) &
					GetValueMask<SerialList>::value) ^
					GetInversionMask<SerialList>::value;
					return PinWriteIterator<RestList>::UppendReadValue(portValue, result);
				}

				if(Head::Pin::Inverted)
				{
				    if(!(portValue & (1 << Head::Pin::Number)))
						result |= (1 << Head::Position);

				}else
				{
                    if(portValue & (1 << Head::Pin::Number))
						result |= (1 << Head::Position);
				}


				return PinWriteIterator<Tail>::UppendReadValue(portValue, result);
			}
        };
////////////////////////////////////////////////////////////////////////////////
// PinConstWriteIterator
////////////////////////////////////////////////////////////////////////////////
		template <class TList, class DataType, class PortDataType, DataType value> struct PinConstWriteIterator;

        template <class DataType, class PortDataType, DataType value>
		struct PinConstWriteIterator<NullType, DataType, PortDataType, value>
        {
			static const PortDataType PortValue = 0;
        };

        template <class Head, class Tail, class DataType, class PortDataType, DataType value>
        struct PinConstWriteIterator< Typelist<Head, Tail>, DataType, PortDataType, value>
        {
			static const PortDataType PortValue = (value & (1 << Head::Position) ?
					(1 << Head::Pin::Number) : 0) |
					PinConstWriteIterator<Tail, DataType, PortDataType, value>::PortValue;
        };
////////////////////////////////////////////////////////////////////////////////
// class template PortWriteIterator
// Iterates througth port list and write value to them
// Assume that PinList is type list of PinPositionHolder types.
// And PortList is type list of port types.
////////////////////////////////////////////////////////////////////////////////

		template <class PortList, class PinList> struct PortWriteIterator;

        template <class PinList> struct PortWriteIterator<NullType, PinList>
        {
			template<class DataType>
			static void Write(DataType value)
			{   }

			template<class DataType>
			static void Set(DataType value)
			{   }

			template<class DataType>
			static void Clear(DataType value)
			{   }

			template<class DataType>
			static DataType PinRead()
			{
				return 0;
			}

			template<class Configuration, class DataType>
			static void SetConfiguration(Configuration config, DataType mask)
			{	}

			template<class DataType>
			static DataType OutRead()
			{
				return 0;
			}

			// constant writing interface

			template<class DataType, DataType value>
			static void Write()
			{	}

			template<class DataType, DataType value>
			static void Set()
			{	}

			template<class DataType, DataType value>
			static void Clear()
			{	}

			template<class Configuration, class DataType, Configuration config, DataType mask>
			static void SetConfiguration()
			{	}

			template<class DataType, GpioBase::GenericConfiguration config, DataType mask>
			static void SetConfiguration()
			{	}
        };

        template <class Head, class Tail, class PinList>
        struct PortWriteIterator< Typelist<Head, Tail>, PinList>
        {
			//pins on current port
			typedef typename SelectPins<PinList, PinsWithPort<Head>::template Result>::Result Pins;

			enum{Mask = GetPortMask<Pins>::value};

			typedef Head Port; //Head points to current port i the list.

			template<class DataType>
			static void Write(DataType value)
			{
				DataType result = PinWriteIterator<Pins>::UppendValue(value, DataType(0));

				if((int)Length<Pins>::value == (int)Port::Width)// whole port
					Port::Write(result);
				else
				{
					Port::ClearAndSet(Mask, result);
				}

				PortWriteIterator<Tail, PinList>::Write(value);
			}

			template<class DataType>
			static void Set(DataType value)
			{
				DataType result = PinWriteIterator<Pins>::UppendValue(value, DataType(0));
				Port::Set(result);

				PortWriteIterator<Tail, PinList>::Set(value);
			}

			template<class DataType>
			static void Clear(DataType value)
			{
				DataType result = PinWriteIterator<Pins>::UppendValue(value, DataType(0));
				Port::Clear(result);

				PortWriteIterator<Tail, PinList>::Clear(value);
			}

			template<class Configuration, class DataType>
			static void SetConfiguration(Configuration config, DataType mask)
			{
				DataType portMask = PinWriteIterator<Pins>::UppendValue(mask, DataType(0));
				Port::SetConfiguration(portMask, config);
				PortWriteIterator<Tail, PinList>::SetConfiguration(config, mask);
			}

			template<class DataType>
			static void SetConfiguration(GpioBase::GenericConfiguration config, DataType mask)
			{
				DataType portMask = PinWriteIterator<Pins>::UppendValue(mask, DataType(0));
				Port::SetConfiguration(portMask, Port::MapConfiguration(config) );
				PortWriteIterator<Tail, PinList>::SetConfiguration(config, mask);
			}

			template<class DataType>
			static DataType PinRead()
			{
				typename Port::DataT portValue = Port::PinRead();
				return PinWriteIterator<Pins>::UppendReadValue(
							portValue,
							PortWriteIterator<Tail, PinList>::template PinRead<DataType>());
			}

			template<class DataType>
			static DataType OutRead()
			{
				typename Port::DataT portValue = Port::Read();
				return PinWriteIterator<Pins>::UppendReadValue(
							portValue,
							PortWriteIterator<Tail, PinList>::template OutRead<DataType>());
			}

			// constant writing interface

			template<class DataType, DataType value>
			static void Write()
			{
				const typename Port::DataT portValue =
					PinConstWriteIterator<Pins, DataType, typename Port::DataT, value>::PortValue ^
					GetInversionMask<Pins>::value;

				Port::template ClearAndSet<Mask, portValue>();
				PortWriteIterator<Tail, PinList>::template Write<DataType, value>();
			}

			template<class DataType, DataType value>
			static void Set()
			{
				const typename Port::DataT portValue =
					PinConstWriteIterator<Pins, DataType, typename Port::DataT, value>::PortValue ^
					GetInversionMask<Pins>::value;

				Port::template Set<portValue>();
				PortWriteIterator<Tail, PinList>::template Set<DataType, value>();
			}

			template<class DataType, DataType value>
			static void Clear()
			{
				const typename Port::DataT portValue =
					PinConstWriteIterator<Pins, DataType, typename Port::DataT, value>::PortValue^
					GetInversionMask<Pins>::value;

				Port::template Clear<portValue>();
				PortWriteIterator<Tail, PinList>::template Clear<DataType, value>();
			}
        };
////////////////////////////////////////////////////////////////////////////////
// PortConfigurationIterator
////////////////////////////////////////////////////////////////////////////////

		template <class PortList, class PinList, class Configuration, Configuration config>
		struct PortConfigurationIterator;

        template <class PinList, class Configuration, Configuration config>
		struct PortConfigurationIterator<NullType, PinList, Configuration, config>
        {
			template<class DataType, DataType mask>
			static void SetConfiguration()
			{	}
        };

        template <class Head, class Tail, class PinList, class Configuration, Configuration config>
        struct PortConfigurationIterator< Typelist<Head, Tail>, PinList, Configuration, config>
        {
			//pins on current port
			typedef typename SelectPins<PinList, PinsWithPort<Head>::template Result>::Result Pins;
			typedef Head Port; //Head points to current port i the list.

			template<class DataType, DataType mask>
			static void SetConfiguration()
			{
				const typename Port::DataT portValue = PinConstWriteIterator<Pins, DataType, typename Port::DataT, mask>::PortValue;
				Port::template SetConfiguration<portValue, config>();
				PortConfigurationIterator<Tail, PinList, Configuration, config>::template SetConfiguration<DataType, mask>();
			}
        };

		template <class Head, class Tail, class PinList, GpioBase::GenericConfiguration config>
        struct PortConfigurationIterator< Typelist<Head, Tail>, PinList, GpioBase::GenericConfiguration, config>
        {
			//pins on current port
			typedef typename SelectPins<PinList, PinsWithPort<Head>::template Result>::Result Pins;
			typedef Head Port; //Head points to current port i the list.

			template<class DataType, DataType mask>
			static void SetConfiguration()
			{
				const typename Port::DataT portValue =
					PinConstWriteIterator<Pins, DataType, typename Port::DataT, mask>::PortValue;

				const typename Port::Configuration portConfig =
					Port::template MapConfigurationConst<config>::value;

				Port::template SetConfiguration<portValue, portConfig>();

				PortConfigurationIterator<Tail, PinList, GpioBase::GenericConfiguration, config>::
					template SetConfiguration<DataType, mask>();
			}
        };
	}
////////////////////////////////////////////////////////////////////////////////
// class template PinSet
// Holds implimentation of pin list manipulations.
// Pins from list are grouped by their port and group read/write operation is
// performed on each port.
////////////////////////////////////////////////////////////////////////////////
		template<class PINS>
		class PinListProperties
		{
			typedef typename IoPrivate::GetPorts<PINS>::Result PinsToPorts;

			enum{LengthEnum = Length<PINS>::value};
			enum{LastBitPositionEnum = IoPrivate::GetLastBitPosition<PINS>::value};
			typedef PINS PinTypeList;
		public:
		  	typedef typename IoPrivate::GetConfigPins<PINS>::Result ConfigPins;
			typedef typename IoPrivate::GetPorts<ConfigPins>::Result PinsToConfigPorts;

			typedef typename Loki::TL::NoDuplicates<PinsToPorts>::Result Ports;
			typedef typename Loki::TL::NoDuplicates<PinsToConfigPorts>::Result AllConfigPorts;

			typedef typename IoPrivate::ErasePortsWithConfig<
								AllConfigPorts,
								GpioBase::DontCareConfiguration
							>::Result ConfigPorts;

			static const unsigned Length = LengthEnum;
			static const unsigned LastBitPosition = LastBitPositionEnum;

			enum {PortsHasSameConfig =
			  IoPrivate::CheckSameConfig<ConfigPorts>::value};

			typedef typename StaticIf
			  		<
					  PortsHasSameConfig,
					  typename Ports::Head::Base,
					  GpioBase
					 >::Result BasePortType;

			typedef typename SelectSize<LastBitPosition+1>::Result DataType;
		};

		template<class PINS>
		class PinSet :public PinListProperties<PINS>, public PinListProperties<PINS>::BasePortType
		{
		  typedef PinListProperties<PINS> Config;
		public:
			typedef typename Config::DataType DataType;
			typedef typename Config::Ports Ports;
			typedef typename Config::ConfigPorts ConfigPorts;
			typedef typename Config::ConfigPins ConfigPins;
			typedef typename Config::BasePortType::Configuration PortConfiguration;

			using Config::Length;

			template<uint8_t Num>
			class Take: public PinSet< typename IoPrivate::TakeFirst<PINS, Num>::Result >
			{};

			template<uint8_t Num>
			class Skip: public PinSet< typename IoPrivate::SkipFirst<PINS, Num>::Result >
			{};

			template<uint8_t StartIndex, uint8_t SliceSize>
			class Slice: public PinSet
					<
						typename IoPrivate::SkipFirst<
							typename IoPrivate::TakeFirst<PINS, StartIndex + SliceSize>::Result,
							StartIndex>::Result
					>
			{
                BOOST_STATIC_ASSERT(SliceSize == Slice::Length);
			};

			template<uint8_t PIN>
			class Pin :public TypeAt<PINS, PIN>::Result::Pin
			{};

			static void Write(DataType value)
			{
				IoPrivate::PortWriteIterator<Ports, PINS>::Write(value);
			}

			static DataType Read()
			{
				typedef IoPrivate::PortWriteIterator<Ports, PINS> iter;
				DataType result = iter::template OutRead<DataType>();
				return result;
			}
			static void Set(DataType value)
			{
				IoPrivate::PortWriteIterator<Ports, PINS>::Set(value);
			}

			static void Clear(DataType value)
			{
				IoPrivate::PortWriteIterator<Ports, PINS>::Clear(value);
			}

			static DataType PinRead()
			{
				typedef IoPrivate::PortWriteIterator<Ports, PINS> iter;
				DataType result = iter::template PinRead<DataType>();
				return result;
			}

			template<class ConfigurationT>
			static void SetConfiguration(ConfigurationT config, DataType mask = DataType(-1))
			{
				IoPrivate::PortWriteIterator<ConfigPorts, ConfigPins>::SetConfiguration(config, mask);
			}

			// constant writing interface

			template<DataType value>
			static void Write()
			{
				IoPrivate::PortWriteIterator<Ports, PINS>:: template Write<DataType, value>();
			}

			template<DataType value>
			static void Set()
			{
				IoPrivate::PortWriteIterator<Ports, PINS>:: template Set<DataType, value>();
			}

			template<DataType value>
			static void Clear()
			{
				IoPrivate::PortWriteIterator<Ports, PINS>:: template Clear<DataType, value>();
			}

			template<PortConfiguration config, DataType mask>
			static void SetConfiguration()
			{
				IoPrivate::PortConfigurationIterator<ConfigPorts, ConfigPins, PortConfiguration, config>::
					template SetConfiguration<DataType, mask>();
			}
		};

////////////////////////////////////////////////////////////////////////////////
// class template MakePinList
// This class is used to generate PinList and associate each pin in the list with
// its bit position in value to be Write to and Read from pin list.
////////////////////////////////////////////////////////////////////////////////

		template
        <
			int Position,
            typename T1  = NullType, typename T2  = NullType, typename T3  = NullType,
            typename T4  = NullType, typename T5  = NullType, typename T6  = NullType,
            typename T7  = NullType, typename T8  = NullType, typename T9  = NullType,
            typename T10 = NullType, typename T11 = NullType, typename T12 = NullType,
            typename T13 = NullType, typename T14 = NullType, typename T15 = NullType,
            typename T16 = NullType, typename T17 = NullType, typename T18 = NullType,
			typename T19 = NullType, typename T20 = NullType, typename T21 = NullType,
			typename T22 = NullType, typename T23 = NullType, typename T24 = NullType,
			typename T25 = NullType, typename T26 = NullType, typename T27 = NullType,
			typename T28 = NullType, typename T29 = NullType, typename T30 = NullType,
			typename T31 = NullType, typename T32 = NullType, typename T33 = NullType
        >
        struct MakePinList
        {
        private:
            typedef typename MakePinList
            <
				Position + 1,
                T2 , T3 , T4 ,
                T5 , T6 , T7 ,
                T8 , T9 , T10,
                T11, T12, T13,
                T14, T15, T16,
                T17, T18, T19,
				T20, T21, T22,
				T23, T24, T25,
				T26, T27, T28,
				T29, T30, T31,
				T32, T33
            >
            ::Result TailResult;
			enum{PositionInList = Position};
        public:
            typedef Typelist< IoPrivate::PinPositionHolder<T1, PositionInList>, TailResult> Result;

        };

        template<int Position>
        struct MakePinList<Position>
        {
            typedef NullType Result;
        };

////////////////////////////////////////////////////////////////////////////////
// class template PinList
// Represents generic set of IO pins that could be used like a virtual port.
// It can be composed from any number of pins from 1 to 16 from any IO port present in selected device
// (the last T17 type in PinList is a end of list marker).
// It can be used like this:
//		typedef PinList<Pa0, Pa1, Pa2, Pa3, Pb5, Pb4, Pb2> pins;
//		pins::Write(someValue);

////////////////////////////////////////////////////////////////////////////////

		template
        <
            typename T1  = NullType, typename T2  = NullType, typename T3  = NullType,
            typename T4  = NullType, typename T5  = NullType, typename T6  = NullType,
            typename T7  = NullType, typename T8  = NullType, typename T9  = NullType,
            typename T10 = NullType, typename T11 = NullType, typename T12 = NullType,
            typename T13 = NullType, typename T14 = NullType, typename T15 = NullType,
            typename T16 = NullType, typename T17 = NullType, typename T18 = NullType,
			typename T19 = NullType, typename T20 = NullType, typename T21 = NullType,
			typename T22 = NullType, typename T23 = NullType, typename T24 = NullType,
			typename T25 = NullType, typename T26 = NullType, typename T27 = NullType,
			typename T28 = NullType, typename T29 = NullType, typename T30 = NullType,
			typename T31 = NullType, typename T32 = NullType, typename T33 = NullType
        >
        struct PinList: public PinSet
			<
				typename MakePinList
				<	0,	T1,
					T2 , T3 , T4 ,
                	T5 , T6 , T7 ,
                	T8 , T9 , T10,
                	T11, T12, T13,
                	T14, T15, T16,
                	T17, T18, T19,
					T20, T21, T22,
					T23, T24, T25,
					T26, T27, T28,
					T29, T30, T31,
					T32, T33
				>::Result
			>
        {	};

}
