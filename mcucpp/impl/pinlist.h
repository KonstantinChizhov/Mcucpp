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

#include <static_if.h>
#include <select_size.h>

namespace Mcucpp
{
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
			enum{value = (Head::Pin::Inverted ? (1ul << Head::Pin::Number) : 0) | GetInversionMask<Tail>::value};
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
			enum{value = (1ul << Head::Pin::Number) | GetPortMask<Tail>::value};
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
			enum{value = (1ul << Head::Position) | GetValueMask<Tail>::value};
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
				BitPosition == I::BitPosition - 1) ?
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
			template<class DataType>
			static inline DataType AppendValue(DataType, DataType result)
			{
				return result;
			}

			template<class DataType>
			static inline DataType AppendReadValue(DataType, DataType result)
			{
				return result;
			}
		};

		template <class Head, class Tail>
		struct PinWriteIterator< Typelist<Head, Tail> >
		{
			template<class DataType>
			static inline DataType AppendValue(DataType value, DataType result)
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

					return PinWriteIterator<NotTransparentPins>::AppendValue(value, result);
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

					return PinWriteIterator<RestList>::AppendValue(value, result);
				}

				if(Head::Pin::Inverted == false)
				{
					if(value & (1ul << Head::Position))
						result |= (1ul << Head::Pin::Number);
				}
				else
				{
					if(!(value & (1ul << Head::Position)))
						result |= (1ul << Head::Pin::Number);
				}

				return PinWriteIterator<Tail>::AppendValue(value, result);
			}

			template<class DataType>
			static inline DataType AppendReadValue(DataType portValue, DataType result)
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

					return PinWriteIterator<NotTransparentPins>::AppendReadValue(portValue, result);
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
					return PinWriteIterator<RestList>::AppendReadValue(portValue, result);
				}

				if(Head::Pin::Inverted)
				{
					if(!(portValue & (1ul << Head::Pin::Number)))
						result |= (1ul << Head::Position);

				}else
				{
					if(portValue & (1ul << Head::Pin::Number))
						result |= (1ul << Head::Position);
				}


				return PinWriteIterator<Tail>::AppendReadValue(portValue, result);
			}
		};
////////////////////////////////////////////////////////////////////////////////
// PinConstWriteIterator
////////////////////////////////////////////////////////////////////////////////
		template <class TList, class DataType, DataType value> struct PinConstWriteIterator;

		template <class DataType, DataType value>
		struct PinConstWriteIterator<NullType, DataType, value>
		{
			static const DataType PortValue = 0;
		};

		template <class Head, class Tail, class DataType, DataType value>
		struct PinConstWriteIterator< Typelist<Head, Tail>, DataType, value>
		{
			static const DataType PortValue = (value & (1ul << Head::Position) ?
					(1ul << Head::Pin::Number) : 0) |
					PinConstWriteIterator<Tail, DataType, value>::PortValue;
		};
////////////////////////////////////////////////////////////////////////////////
// class template PortWriteIterator
// Iterates througth port list and write value to them
// Assume that PinList is type list of PinPositionHolder types.
// And PortList is type list of port types.
////////////////////////////////////////////////////////////////////////////////

		template <class PortList, class PinList, class ValueType> struct PortWriteIterator;

		template <class PinList, class ValueType> struct PortWriteIterator<NullType, PinList, ValueType>
		{
			typedef ValueType DataType;
			
			static void Enable()
			{   }

			static void Write(DataType)
			{   }

			static void Set(DataType)
			{   }

			static void Clear(DataType)
			{   }

			static DataType PinRead()
			{
				return 0;
			}

			template<class Configuration>
			static void SetConfiguration(Configuration, DataType)
			{	}
			
			template<class Speed>
			static void SetSpeed(Speed, DataType)
			{	}
			
			template<class PullUp>
			static void SetPullUp(PullUp, DataType)
			{	}
			
			template<class DriverType>
			static void SetDriverType(DriverType, DataType)
			{	}
			
			static void AltFuncNumber(uint8_t, DataType)
			{	}

			static DataType OutRead()
			{
				return 0;
			}

			// constant writing interface

			template<DataType value>
			static void Write()
			{	}

			template<DataType value>
			static void Set()
			{	}

			template<DataType value>
			static void Clear()
			{	}

			template<class Configuration, Configuration config, DataType mask>
			static void SetConfiguration()
			{	}
			
			
        };

        template <class Head, class Tail, class PinList, class ValueType>
        struct PortWriteIterator< Typelist<Head, Tail>, PinList, ValueType>
        {
			//pins on current port
			typedef typename SelectPins<PinList, PinsWithPort<Head>::template Result>::Result Pins;

			enum{Mask = GetPortMask<Pins>::value};

			typedef Head Port; //Head points to current port in the list.
			typedef typename Port::DataT PortType;

			typedef typename StaticIf
			  		<
					  sizeof(PortType) >= sizeof(ValueType),
					  PortType,
					  ValueType
					 >::Result DataType;

			static void Enable()
			{
				Port::Enable();
				PortWriteIterator<Tail, PinList, ValueType>::Enable();
			}
			
			static void Write(DataType value)
			{
				DataType result = PinWriteIterator<Pins>::AppendValue(value, DataType(0));

				if((int)Length<Pins>::value == (int)Port::Width)// whole port
					Port::Write(result);
				else
				{
					Port::ClearAndSet(Mask, result);
				}

				PortWriteIterator<Tail, PinList, ValueType>::Write(value);
			}

			static void Set(DataType value)
			{
				DataType result = PinWriteIterator<Pins>::AppendValue(value, DataType(0));
				Port::Set(result);
				PortWriteIterator<Tail, PinList, ValueType>::Set(value);
			}

			static void Clear(DataType value)
			{
				DataType result = PinWriteIterator<Pins>::AppendValue(value, DataType(0));
				Port::Clear(result);
				PortWriteIterator<Tail, PinList, ValueType>::Clear(value);
			}

			template<class Configuration>
			static void SetConfiguration(Configuration config, DataType mask)
			{
				DataType portMask = PinWriteIterator<Pins>::AppendValue(mask, DataType(0));
				Port::SetConfiguration(portMask, config);
				PortWriteIterator<Tail, PinList, ValueType>::SetConfiguration(config, mask);
			}
			
			template<class Speed>
			static void SetSpeed(Speed speed, DataType mask)
			{
				DataType portMask = PinWriteIterator<Pins>::AppendValue(mask, DataType(0));
				Port::SetSpeed(portMask, speed);
				PortWriteIterator<Tail, PinList, ValueType>::SetSpeed(speed, mask);
			}
			
			template<class PullUp>
			static void SetPullUp(PullUp pull, DataType mask)
			{
				DataType portMask = PinWriteIterator<Pins>::AppendValue(mask, DataType(0));
				Port::SetPullUp(portMask, pull);
				PortWriteIterator<Tail, PinList, ValueType>::SetPullUp(pull, mask);
			}
			
			template<class DriverType>
			static void SetDriverType(DriverType driver, DataType mask)
			{
				DataType portMask = PinWriteIterator<Pins>::AppendValue(mask, DataType(0));
				Port::SetDriverType(portMask, driver);
				PortWriteIterator<Tail, PinList, ValueType>::SetDriverType(driver, mask);
			}

			static void AltFuncNumber(uint8_t number, DataType mask)
			{
				DataType portMask = PinWriteIterator<Pins>::AppendValue(mask, DataType(0));
				Port::AltFuncNumber(portMask, number);
				PortWriteIterator<Tail, PinList, ValueType>::AltFuncNumber(number, mask);
			}

			static DataType PinRead()
			{
				DataType portValue = Port::PinRead();
				return PinWriteIterator<Pins>::template AppendReadValue<DataType>(
							portValue,
							PortWriteIterator<Tail, PinList, ValueType>::PinRead());
			}

			static DataType OutRead()
			{
				DataType portValue = Port::Read();
				return PinWriteIterator<Pins>::template AppendReadValue<DataType>(
							portValue,
							PortWriteIterator<Tail, PinList, ValueType>::OutRead());
			}

			// constant writing interface

			template<DataType value>
			static void Write()
			{
				const DataType portValue =
					PinConstWriteIterator<Pins, DataType, value>::PortValue ^
					GetInversionMask<Pins>::value;

				Port::template ClearAndSet<Mask, portValue>();
				PortWriteIterator<Tail, PinList, ValueType>::template Write<value>();
			}

			template<DataType value>
			static void Set()
			{
				const DataType portValue =
					PinConstWriteIterator<Pins, DataType, value>::PortValue ^
					GetInversionMask<Pins>::value;

				Port::template Set<portValue>();
				PortWriteIterator<Tail, PinList, ValueType>::template Set<value>();
			}

			template<DataType value>
			static void Clear()
			{
				const DataType portValue =
					PinConstWriteIterator<Pins, DataType, value>::PortValue^
					GetInversionMask<Pins>::value;

				Port::template Clear<portValue>();
				PortWriteIterator<Tail, PinList, ValueType>::template Clear<value>();
			}
        };
////////////////////////////////////////////////////////////////////////////////
// PortConfigurationIterator
////////////////////////////////////////////////////////////////////////////////

		template <class PortList, class PinList, class Configuration, Configuration config, class ValueType>
		struct PortConfigurationIterator;

		template <class PinList, class Configuration, Configuration config, class ValueType>
		struct PortConfigurationIterator<NullType, PinList, Configuration, config, ValueType>
		{
			template<ValueType mask>
			static void SetConfiguration()
			{	}
		};

		template <class Head, class Tail, class PinList, class Configuration, Configuration config, class ValueType>
		struct PortConfigurationIterator< Typelist<Head, Tail>, PinList, Configuration, config, ValueType>
		{
			//pins on current port
			typedef typename SelectPins<PinList, PinsWithPort<Head>::template Result>::Result Pins;
			typedef Head Port; //Head points to current port i the list.
			typedef typename Port::DataT PortType;

			typedef typename StaticIf
			  		<
					  sizeof(PortType) >= sizeof(ValueType),
					  PortType,
					  ValueType
					 >::Result DataType;

			template<DataType mask>
			static void SetConfiguration()
			{
				const DataType portValue = PinConstWriteIterator<Pins, DataType, mask>::PortValue;
				Port::template SetConfiguration<portValue, config>();
				PortConfigurationIterator<Tail, PinList, Configuration, config, ValueType>::template SetConfiguration<mask>();
			}
		};
////////////////////////////////////////////////////////////////////////////////
// PortConfigurationIterator
////////////////////////////////////////////////////////////////////////////////
		
		template<class PinList, class Pin> struct PinIndexSelector;
		
		template<class Pin> struct PinIndexSelector<NullType, Pin>
		{
			static const int PinIndex = -1; // Pin not found
		};
		
		template<class Head, class Tail, class Pin> struct PinIndexSelector<Typelist<Head, Tail>, Pin>
		{
			static const int PinIndex = (Pin::Number == Head::Pin::Number && (int)Pin::Port::Id == (int)Head::Pin::Port::Id) ? 
				Head::Position : 
				PinIndexSelector<Tail, Pin>::PinIndex;
		};
	}
}
}
