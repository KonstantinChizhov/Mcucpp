

//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2020
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

#include <iopin.h>
#include <select_size.h>
#include <ioports.h>
#include <utility>
#include <type_traits>

namespace Mcucpp
{
namespace IO
{
template <class... Ts>
struct type_pack
{
};
using empty_pack = type_pack<>;

template <class T, class Tp>
struct Contains;

template <class T, class... Args>
struct Contains<T, type_pack<Args...>>
{
    static constexpr bool value = (... || std::is_same<T, Args>::value);
};

template <class IS>
struct get_impl;

template <size_t... Is>
struct get_impl<std::index_sequence<Is...>>
{
    template <class T>
    static constexpr T dummy(decltype(Is, (void *)0)..., T *, ...);
};

template <size_t I, class Tp>
struct get;

template <size_t I, class... Ts>
struct get<I, type_pack<Ts...>>
{
    using type = decltype(get_impl<std::make_index_sequence<I>>::dummy((Ts *)(0)...));
};

static_assert(std::is_same<char, get<1, type_pack<int, char, float>>::type>::value, "get type by index failed");

template <class T, class Tp>
struct TpAppend;

template <class T, class... Args>
struct TpAppend<T, type_pack<Args...>>
{
    using type = type_pack<T, Args...>;
};

template <class Tp, class T>
struct Erase;

template <class T>
struct Erase<type_pack<>, T>
{
    using type = type_pack<>;
};

template <class T, class... Tail>
struct Erase<type_pack<T, Tail...>, T>
{
    using type = type_pack<Tail...>;
};

template <class Head, class... Tail, class T>
struct Erase<type_pack<Head, Tail...>, T>
{
    using type = typename TpAppend<Head, typename Erase<type_pack<Tail...>, T>::type>::type;
};

template <class TList>
struct NoDuplicates;

template <>
struct NoDuplicates<type_pack<>>
{
    using type = type_pack<>;
};

template <class Head, class... Tail>
struct NoDuplicates<type_pack<Head, Tail...>>
{
private:
    using L1 = typename NoDuplicates<type_pack<Tail...>>::type;
    using L2 = typename Erase<L1, Head>::type;

public:
    using type = typename TpAppend<Head, L2>::type;
};

template <class TPIN, size_t POSITION>
struct PinPositionHolder
{
    using Pin = TPIN;
    static constexpr size_t Position = POSITION;
};

template <class... Ports>
void EnablePorts(type_pack<Ports...>)
{
    (Ports::Enable(), ...);
}

template <class... Ports>
void DisablePorts(type_pack<Ports...>)
{
    (Ports::Disable(), ...);
}

template <class... PinsWithIndex>
constexpr size_t GetLastBitPosition(type_pack<PinsWithIndex...>)
{
    return (PinsWithIndex::Position, ...);
}

template <class... PinsWithIndex, class Port>
constexpr auto GetInversionMask(type_pack<PinsWithIndex...>, Port)
{
    return ((PinsWithIndex::Pin::Inverted && std::is_same<typename PinsWithIndex::Pin::Port, Port>::value
                 ? (typename Port::DataT{1u} << PinsWithIndex::Pin::Number)
                 : 0) |
            ...);
}

template <class... PinsWithIndex, class Port>
constexpr auto GetPortClearMask(type_pack<PinsWithIndex...>, Port)
{
    return (typename Port::DataT)((std::is_same<typename PinsWithIndex::Pin::Port, Port>::value
                                       ? (typename Port::DataT{1u} << PinsWithIndex::Pin::Number)
                                       : 0) |
                                  ...);
}

template <int shift, class... PinsWithIndex, class Port, class DataT>
constexpr auto GetMappedPinsMask(type_pack<PinsWithIndex...>, Port, DataT value)
{
    constexpr typename Port::DataT result = ((std::is_same<typename PinsWithIndex::Pin::Port, Port>::value && ((int)(PinsWithIndex::Pin::Number - PinsWithIndex::Position) == shift)
                                                  ? (typename Port::DataT{1u} << PinsWithIndex::Pin::Number)
                                                  : 0) |
                                             ...);
    typename Port::DataT andMask = shift >= 0 ? (value << shift) : (value >> -shift);
    return typename Port::DataT(result & andMask);
}

template <class Is>
struct SimmetricSequence;

template <int... Ints>
struct SimmetricSequence<std::integer_sequence<int, Ints...>>
{
    static const int half = sizeof...(Ints) / 2;
    using type = std::integer_sequence<int, (Ints - half)...>;
};

template <class... PinsWithIndex, class Port, class ValueT, int... ShiftSeq>
constexpr auto GetPortSetMask(type_pack<PinsWithIndex...> pins, std::integer_sequence<int, ShiftSeq...>, Port port, ValueT value)
{
    typename Port::DataT result = (GetMappedPinsMask<ShiftSeq>(pins, port, value) | ...);
    return result;
}

template <class... PinsWithIndex, class Port, class ValueT>
void WritePort(type_pack<PinsWithIndex...> pins, Port port, ValueT value)
{
    constexpr int len = GetLastBitPosition(pins);
    using shifts = typename SimmetricSequence<std::make_integer_sequence<int, len * 2 + 1>>::type;
    auto setMask = GetPortSetMask(pins, shifts{}, port, value);
    auto clearMask = GetPortClearMask(pins, port);
    Port::ClearAndSet(clearMask, setMask);
}

template <class... PinsWithIndex, class Port, class ValueT>
void SetPort(type_pack<PinsWithIndex...> pins, Port port, ValueT value)
{
    constexpr int len = GetLastBitPosition(pins);
    using shifts = typename SimmetricSequence<std::make_integer_sequence<int, len * 2 + 1>>::type;
    auto setMask = GetPortSetMask(pins, shifts{}, port, value);
    Port::Set(setMask);
}

template <class... PinsWithIndex, class Port, class ValueT, class PortConfigurationT>
void SetPortConfiguration(type_pack<PinsWithIndex...> pins, Port port, ValueT mask, PortConfigurationT config)
{
    constexpr int len = GetLastBitPosition(pins);
    using shifts = typename SimmetricSequence<std::make_integer_sequence<int, len * 2 + 1>>::type;
    auto portMask = GetPortSetMask(pins, shifts{}, port, mask);
    Port::SetConfiguration(portMask, config);
}

template <class... PinsWithIndex, class Port, class ValueT>
void ClearPort(type_pack<PinsWithIndex...> pins, Port port, ValueT value)
{
    constexpr int len = GetLastBitPosition(pins);
    using shifts = typename SimmetricSequence<std::make_integer_sequence<int, len * 2 + 1>>::type;
    auto mask = GetPortSetMask(pins, shifts{}, port, value);
    Port::Clear(mask);
}

template <class... Ports, class... PinsWithIndex, class ValueT>
void WritePorts(type_pack<Ports...>, type_pack<PinsWithIndex...> pins, ValueT value)
{
    (WritePort(pins, Ports{}, value), ...);
}

template <class... Ports, class... PinsWithIndex, class ValueT>
void SetPorts(type_pack<Ports...>, type_pack<PinsWithIndex...> pins, ValueT value)
{
    (SetPort(pins, Ports{}, value), ...);
}

template <class... Ports, class... PinsWithIndex, class ValueT>
void ClearPorts(type_pack<Ports...>, type_pack<PinsWithIndex...> pins, ValueT value)
{
    (ClearPort(pins, Ports{}, value), ...);
}

template <class... Ports, class... PinsWithIndex, class ValueT, class PortConfigurationT>
void SetPortsConfiguration(type_pack<Ports...>, type_pack<PinsWithIndex...> pins, ValueT mask, PortConfigurationT config)
{
    (SetPortConfiguration(pins, Ports{}, mask, config), ...);
}

template <class... PINS>
struct PinList2 : public NativePortBase
{
    using Pins = type_pack<PINS...>;

    using Speed = NativePortBase::Speed;
    using PullMode = NativePortBase::PullMode;
    using DriverType = NativePortBase::DriverType;
    using PortConfiguration = NativePortBase::Configuration;
    using PortToPins = type_pack<typename PINS::Port...>;
    using Ports = typename NoDuplicates<PortToPins>::type;

    using IndexSeq = std::make_index_sequence<sizeof...(PINS)>;
    template <class T>
    struct AddIndex;
    template <class IT, IT... Indices>
    struct AddIndex<std::integer_sequence<IT, Indices...>>
    {
        using type = type_pack<PinPositionHolder<PINS, Indices>...>;
    };

    using PinsWithIndex = typename AddIndex<IndexSeq>::type;
    static constexpr size_t LastBitPosition = GetLastBitPosition(PinsWithIndex{});
    static constexpr int Length = sizeof...(PINS);

    using DataType = typename SelectSize<Length>::Result;
    using ValueType = DataType;

    // template<uint8_t Num>
    // class Take: public PinList2<NullPin>
    // {};

    // template<uint8_t Num>
    // class Skip: public PinList2<NullPin>
    // {};

    // template<uint8_t StartIndex, uint8_t SliceSize>
    // class Slice: public PinList2<NullPin>

    // {
    // 	//STATIC_ASSERT(SliceSize == Slice::Length);
    // };

    template <size_t PIN>
    class Pin : public get<PIN, Pins>::type
    {
    };

    template <class Pin>
    struct PinIndex
    {
        static constexpr int Value = 0;
        static constexpr int value = Value;
    };

    static void Enable()
    {
        EnablePorts(Ports{});
    }
    static void Disable()
    {
        DisablePorts(Ports{});
    }

    static void Write(ValueType value)
    {
        WritePorts(Ports{}, PinsWithIndex{}, value);
    }

    static ValueType Read() {}

    static void Set(ValueType value)
    {
        SetPorts(Ports{}, PinsWithIndex{}, value);
    }

    static void Clear(ValueType value)
    {
        ClearPorts(Ports{}, PinsWithIndex{}, value);
    }

    static ValueType PinRead() {}

    // configuration with default mask
    static void SetConfiguration(PortConfiguration config) {}

    // configuration compatible with port definition
    static void SetConfiguration(ValueType mask, PortConfiguration config) 
    {
        SetPortsConfiguration(Ports{}, PinsWithIndex{}, mask, config);
    }

    static void SetSpeed(ValueType mask, Speed speed) {}
    static void SetPullUp(ValueType mask, PullMode pull) {}
    static void SetDriverType(ValueType mask, DriverType driver) {}
    static void AltFuncNumber(ValueType mask, uint8_t funcNumber) {}

    static void SetSpeed(Speed speed) { SetSpeed(ValueType(-1), speed); }
    static void SetPullUp(PullMode pull) { SetPullUp(ValueType(-1), pull); }
    static void SetDriverType(DriverType driver) { SetDriverType(ValueType(-1), driver); }
    static void AltFuncNumber(uint8_t funcNumber) { AltFuncNumber(ValueType(-1), funcNumber); }

    // constant writing interface

    template <ValueType value>
    static void Write()
    {
    }

    template <ValueType value>
    static void Set()
    {
    }

    template <ValueType value>
    static void Clear()
    {
    }

    template <ValueType mask, PortConfiguration config>
    static void SetConfiguration()
    {
    }

    template <PortConfiguration config>
    static void SetConfiguration()
    {
    }
};
} // namespace IO
} // namespace Mcucpp