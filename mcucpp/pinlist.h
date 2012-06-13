
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

#include "impl/pinlist.h"

namespace Mcucpp
{
namespace IO
{
	using namespace IoPrivate;

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

////////////////////////////////////////////////////////////////////////////////
// class template PinSet
// Holds implimentation of pin list manipulations.
// Pins from list are grouped by their port and group read/write operation is
// performed on each port.
////////////////////////////////////////////////////////////////////////////////

    template<class PINS>
    class PinSet :public PinListProperties<PINS>, public PinListProperties<PINS>::BasePortType
    {
      typedef PinListProperties<PINS> Config;
    public:
        typedef typename Config::DataType ValueType;
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

        static void Write(ValueType value)
        {
            IoPrivate::PortWriteIterator<Ports, PINS, ValueType>::Write(value);
        }

        static ValueType Read()
        {
            typedef IoPrivate::PortWriteIterator<Ports, PINS, ValueType> iter;
            ValueType result = iter::OutRead();
            return result;
        }
        static void Set(ValueType value)
        {
            IoPrivate::PortWriteIterator<Ports, PINS, ValueType>::Set(value);
        }

        static void Clear(ValueType value)
        {
            IoPrivate::PortWriteIterator<Ports, PINS, ValueType>::Clear(value);
        }

        static ValueType PinRead()
        {
            typedef IoPrivate::PortWriteIterator<Ports, PINS, ValueType> iter;
            ValueType result = iter::PinRead();
            return result;
        }

		// configuration with default mask 
        template<class ConfigurationT>
        static void SetConfiguration(ConfigurationT config, ValueType mask = ValueType(-1))
        {
            IoPrivate::PortWriteIterator<ConfigPorts, ConfigPins, ValueType>::SetConfiguration(config, mask);
        }

		// configuration compatible with port definition
		template<class ConfigurationT>
        static void SetConfiguration(ValueType mask, ConfigurationT config)
        {
            IoPrivate::PortWriteIterator<ConfigPorts, ConfigPins, ValueType>::SetConfiguration(config, mask);
        }

        // constant writing interface

        template<ValueType value>
        static void Write()
        {
            IoPrivate::PortWriteIterator<Ports, PINS, ValueType>:: template Write<value>();
        }

        template<ValueType value>
        static void Set()
        {
            IoPrivate::PortWriteIterator<Ports, PINS, ValueType>:: template Set<value>();
        }

        template<ValueType value>
        static void Clear()
        {
            IoPrivate::PortWriteIterator<Ports, PINS, ValueType>:: template Clear<value>();
        }

        template<ValueType mask, PortConfiguration config>
        static void SetConfiguration()
        {
            IoPrivate::PortConfigurationIterator<ConfigPorts, ConfigPins, PortConfiguration, config, ValueType>::
                template SetConfiguration<mask>();
        }

        template<PortConfiguration config>
        static void SetConfiguration()
        {
            IoPrivate::PortConfigurationIterator<ConfigPorts, ConfigPins, PortConfiguration, config, ValueType>::
                template SetConfiguration<ValueType(-1)>();
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
}
