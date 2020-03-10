#include <iostream>
#include <string>
#include <iopins.h>
#include <pinlist2.h>
#include <gtest/gtest.h>
#include <iterator>
#include <sstream>

using namespace std;
using namespace Mcucpp;
using namespace IO;

template <class... Pins>
using PinList = Mcucpp::IO::PinList2<Pins...>;

template <class PinsList>
struct PrintPinList;

template <class T, std::size_t N>
constexpr std::size_t size(const T (&array)[N]) noexcept
{
    return N;
}

template <class Pin>
constexpr std::string PrintPin()
{
    std::stringstream result;
    result << (char)Pin::Port::Id << (int)Pin::Number;
    return result.str();
}

template <class... Pins>
struct PrintPinList<PinList2<Pins...>>
{
    static void Print()
    {
        std::cout << "PinList<";
        std::string pinNames[]{PrintPin<Pins>()...};
        size_t sz = std::size(pinNames);
        for (size_t i = 0; i < sz; i++)
        {
            std::cout << pinNames[i];
            if (i != sz - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << ">";
    }
};

template <class Pins>
void TestOnePortPinList(unsigned listValue, unsigned portValue)
{
    typedef typename Pins::template Pin<0>::Port Port;
    typename Pins::DataType val;
    cout << __FUNCTION__ << "\t";
    PrintPinList<Pins>::Print();

    Port::Write(0);

    Pins::Write(listValue);
    EXPECT_EQ(Port::OutReg, portValue);
    val = Pins::Read();
    EXPECT_EQ(listValue, val);

    Port::DirReg = 0;
    Pins::SetConfiguration(listValue, Pins::Out);
    EXPECT_EQ(portValue, Port::DirReg);

    Port::Write(0);
    Port::DirReg = 0;

    Port::InReg = portValue;
    val = Pins::PinRead();
    EXPECT_EQ(listValue, val);

    Port::InReg = 0;
    val = Pins::PinRead();
    EXPECT_EQ(0, val);

    Pins::Write(0);
    EXPECT_EQ(0u, Port::OutReg);

    Pins::Set(listValue);
    EXPECT_EQ(portValue, Port::OutReg);

    Pins::Clear(listValue);
    EXPECT_EQ(0u, Port::OutReg);

    Pins::SetConfiguration(Pins::In);
    EXPECT_EQ(0u, Port::DirReg);

    cout << "\tOK" << endl;
}

template <class Pins, unsigned listValue, unsigned portValue>
void TestOnePortConstIface()
{
    typedef typename Pins::template Pin<0>::Port Port;
    typename Pins::DataType val;
    cout << __FUNCTION__ << "\t";
    PrintPinList<Pins>::Print();

    Port::template Write<0>();

    Pins::template Write<listValue>();
    EXPECT_EQ(Port::OutReg, portValue);
    val = Pins::Read();
    EXPECT_EQ(listValue, val);

    Port::DirReg = 0;
    Pins::template SetConfiguration<listValue, Pins::Out>();
    EXPECT_EQ(portValue, Port::DirReg);

    Port::template Write<0>();
    Port::DirReg = 0;

    Port::InReg = portValue;
    val = Pins::PinRead();
    EXPECT_EQ(listValue, val);

    Port::InReg = 0;
    val = Pins::PinRead();
    EXPECT_EQ(0, val);

    Port::template Write<0>();
    EXPECT_EQ(0u, Port::OutReg);

    Pins::template Set<listValue>();
    EXPECT_EQ(portValue, Port::OutReg);

    Pins::template Clear<listValue>();
    EXPECT_EQ(0u, Port::OutReg);

    Pins::template SetConfiguration<0xff, Pins::In>();
    EXPECT_EQ(0u, Port::DirReg);

    cout << "\tOK" << endl;
}

template <class Pins, class Port1, class Port2>
void Test2PortConfiguration(unsigned listValue, unsigned portValue, unsigned portValue2)
{
    typename Pins::DataType val;
    cout << __FUNCTION__ << "\t";

    Port1::OutReg = 0;
    Port2::OutReg = 0;
    Port1::DirReg = 0;
    Port2::DirReg = 0;

    PrintPinList<Pins>::Print();
    Pins::Write(listValue);
    EXPECT_EQ(portValue, Port1::OutReg);
    EXPECT_EQ(portValue2, Port2::OutReg);
    val = Pins::Read();
    EXPECT_EQ(listValue, val);

    Port1::OutReg = 0;
    Pins::Set(listValue);
    EXPECT_EQ(portValue, Port1::OutReg);
    EXPECT_EQ(portValue2, Port2::OutReg);
    val = Pins::Read();
    EXPECT_EQ(listValue, val);

    Pins::Clear(listValue);
    EXPECT_EQ(0u, Port1::OutReg);
    EXPECT_EQ(0u, Port2::OutReg);
    val = Pins::Read();
    EXPECT_EQ(0u, val);

    Pins::SetConfiguration(listValue, Pins::Out);
    EXPECT_EQ(portValue, Port1::DirReg);
    EXPECT_EQ(portValue2, Port2::DirReg);
    cout << "\tOK" << endl;
}

TEST(GPIO, PinList2EnableTest)
{
    Porta::enabled = false;
    Portb::enabled = false;
    using pins = PinList<Pb2, Pa0, Pa1, Pb3>;
    pins::Enable();
    EXPECT_TRUE(Porta::enabled);
    EXPECT_TRUE(Portb::enabled);
    pins::Disable();
    EXPECT_FALSE(Porta::enabled);
    EXPECT_FALSE(Portb::enabled);
}

TEST(GPIO, PinList2WriteTest)
{
    Porta::OutReg = 0;
    using pins = PinList<Pb2, Pa0, Pa1, Pb3>;
    pins::Write(1);
    EXPECT_EQ(Porta::OutReg, 0);
    EXPECT_EQ(Portb::OutReg, 4);

    pins::Write(2);
    EXPECT_EQ(Porta::OutReg, 1);
    EXPECT_EQ(Portb::OutReg, 0);
}

TEST(GPIO, PinList2Test)
{
    for (int i = 0; i < 16; i++)
    {
        cout << "Writing value: " << i << endl;
        TestOnePortPinList<PinList<Pa0, Pa1, Pa2, Pa3>>(i, i);
        TestOnePortPinList<PinList<Pa1, Pa2, Pa3, Pa4>>(i, i << 1);
        TestOnePortPinList<PinList<Pa2, Pa3, Pa4, Pa5>>(i, i << 2);
    }

    // TestOnePortPinList<PinList<Pa1, Pa3, Pa2, Pa0> >(0x0f, 0x0f);
    // TestOnePortPinList<PinList<Pa0, Pa2, Pa1, Pa3> >(0x0f, 0x0f);
    // TestOnePortPinList<PinList<Pa2, Pa1, Pa3, Pa4, Pa6> >(0x1f, 0x5e);

    // TestOnePortPinList<PinList<Pa5, Pa6, Pa7, Pa0, Pa1, Pa2, Pa3, Pa4> >(0xff, 0xff);

    // TestOnePortPinList<PinList<Pa2, Pa1, Pa3, Pa4, Pa6, Pa8, Pa7, Pa0, Pa5> >(0x1ff, 0x1ff);

    // TestOnePortPinList<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pa8>::Slice<5, 4> >(0x1e0, 0x1e0);
    // cout << "Length = \t" <<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pa8>::Slice<5, 4>::Length << std::endl;
    // TestOnePortPinList<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pa8>::Slice<0, 4> >(0x0f, 0x0f);

    // TestOnePortPinList<PinList<Pa4, Pa1, Pa6, Pa3, Pa7, Pa5, Pa0 > >(0x7f, 0xfb);
    // TestOnePortPinList<PinList<Pa4, Pa1, Pa6, Pa3, Pa2, Pa5, Pa0, Pa7 > >(0xaa, 0xaa);
    // TestOnePortPinList<PinList<Pa4, Pa1, Pa6, Pa3, Pa2, Pa5, Pa0, Pa7 > >(0x55, 0x55);

    // TestOnePortPinList<PinList<Pa1, Pa2, Pa14, Pa15> >(0x0f, (0x3 << 1) | (0x3 << 14));

    // typedef PinList<Pa2, Pa1, Pa3, Pa4, Pa6> Pins1;
    // typedef PinList<Pins1::Pin<0>, Pins1::Pin<1>, Pins1::Pin<2>, Pins1::Pin<3>, Pins1::Pin<4> > Pins1Clone;

    // TestOnePortPinList<Pins1Clone >(0x1f, 0x5e);

    // TestOnePortPinList<PinList<Pa1, Pa3, NullPin, Pa0> >(0x0b, 0x0b);

    // TestOnePortConstIface<PinList<Pa1, Pa3, Pa2, Pa0>, 0x0f, 0x0f>();
    // TestOnePortConstIface<PinList<Pa0, Pa2, Pa1, Pa3>, 0x0f, 0x0f>();
    // TestOnePortConstIface<PinList<Pa2, Pa1, Pa3, Pa4, Pa6>, 0x1f, 0x5e>();
    // TestOnePortConstIface<PinList<Pa5, Pa6, Pa7, Pa0, Pa1, Pa2, Pa3, Pa4>, 0xff, 0xff>();
    // TestOnePortConstIface<PinList<Pa2, Pa1, Pa3, Pa4, Pa6, Pa8, Pa7, Pa0, Pa5>, 0x1ff, 0x1ff>();
    // TestOnePortConstIface<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pa8>::Slice<5, 4>, 0x1e0, 0x1e0>();
    // TestOnePortConstIface<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pa8>::Slice<0, 4>, 0x0f, 0x0f>();
    // TestOnePortConstIface<Pins1Clone, 0x1f, 0x5e>();

    // Test2PortConfiguration<PinList<Pa1, Pa3, Pa2, Pa0, Pb1, Pb3, Pb2, Pb0>, Porta, Portb>(0xff, 0x0f, 0x0f);
    // Test2PortConfiguration<PinList<Pa1, Pa2, Pa3, Pa0, Pb0, Pb1, Pb2, Pb3>, Porta, Portb>(0xff, 0x0f, 0x0f);
}
