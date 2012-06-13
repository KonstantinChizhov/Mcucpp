#include <iostream>
#include <string>
#include "iopins.h"
#include "pinlist.h"
#include <gtest.h>

using namespace std;
using namespace Mcucpp;
using namespace IO;
using namespace IO::Test;

typedef TestPort<unsigned, 'A'> Porta;
typedef TestPort<unsigned, 'B'> Portb;

DECLARE_PORT_PINS(Porta, Pa)

DECLARE_PORT_PINS(Portb, Pb)

template<class Pin>
void PinTest()
{
	typedef typename Pin::Port Port;
	typedef typename Pin::ConfigPort ConfigPort;
	typedef typename Port::DataT DataT;
	const DataT mask = 1 << Pin::Number;

	cout << __FUNCTION__ << "\tPort: " << (char)Port::Id << "\tPinNumber: " << Pin::Number;

	Port::Write(0);
	Port::template SetConfiguration<DataT(-1), Port::In>();
	EXPECT_EQ(Port::OutReg, 0);
	Pin::Set();
	EXPECT_EQ(Port::OutReg, mask);
	Pin::Clear();
	EXPECT_EQ(Port::OutReg, 0);
	Pin::Set(1);
	EXPECT_EQ(Port::OutReg, mask);
	Pin::Set(0);
	EXPECT_EQ(Port::OutReg, 0);
	Pin::Toggle();
	EXPECT_EQ(Port::OutReg, mask);
	Pin::Toggle();
	EXPECT_EQ(Port::OutReg, 0);

	Pin::SetDirRead();
	EXPECT_EQ(Port::DirReg, 0);
	Pin::SetDirWrite();
	EXPECT_EQ(Port::DirReg, mask);

	Pin::SetDir(0);
	EXPECT_EQ(Port::DirReg, 0);
	Pin::SetDir(1);
	EXPECT_EQ(Port::DirReg, mask);

	Pin::SetConfiguration(Port::In);
	EXPECT_EQ(Port::DirReg, 0);
	Pin::SetConfiguration(Port::Out);
	EXPECT_EQ(Port::DirReg, mask);

	Pin::template SetConfiguration<Port::In>();
	EXPECT_EQ(Port::DirReg, 0);
	Pin:: template SetConfiguration<Port::Out>();
	EXPECT_EQ(Port::DirReg, mask);

	Port::InReg = 0;
	EXPECT_FALSE(Pin::IsSet());

	Port::InReg = mask;
	EXPECT_TRUE(Pin::IsSet());

	cout << "\tOK" << endl;
}

template<class Pins>
struct PrintPinList
{
    template<class List, int index>
    struct Iterator
    {
        static void Print()
        {
            Iterator<List, index-1>::Print();
            typedef typename List:: template Pin<index-1> CurrentPin;
            if(index == List::Length)
                std::cout << (char)CurrentPin::Port::Id << CurrentPin::Number;
            else
                std::cout << (char)CurrentPin::Port::Id << CurrentPin::Number << ", ";
        }
    };

    template<class List>
    struct Iterator<List, 0>
    {
        static void Print()
        {}
    };

    static void Print()
    {
        std::cout << "PinList<";
        Iterator<Pins, Pins::Length>::Print();
        std::cout << ">";
    }
};


template<class Pins>
void TestOnePortPinList(unsigned listValue, unsigned portValue)
{
    typedef typename Pins::template Pin<0>::Port Port;
    typename Pins::DataType val;
    cout << __FUNCTION__ << "\t";
    PrintPinList<Pins>::Print();

    Port::Write(0);

    Pins::Write(listValue);
    EXPECT_EQ(Port::OutReg,  portValue);
    val = Pins::Read();
    EXPECT_EQ(val, listValue);

    Port::DirReg = 0;
    Pins::SetConfiguration(listValue, Pins::Out);
    EXPECT_EQ(Port::DirReg, portValue);

    Port::Write(0);
    Port::DirReg = 0;

    Port::InReg = portValue;
    val = Pins::PinRead();
    EXPECT_EQ(val, listValue);

    Port::InReg = 0;
    val = Pins::PinRead();
    EXPECT_EQ(val, 0);

    Pins::Write(0);
    EXPECT_EQ(Port::OutReg, 0);

    Pins::Set(listValue);
    EXPECT_EQ(Port::OutReg, portValue);

    Pins::Clear(listValue);
    EXPECT_EQ(Port::OutReg, 0);

    Pins::SetConfiguration(Pins::In);
    EXPECT_EQ(Port::DirReg, 0);

    cout << "\tOK" << endl;
}

template<class Pins, unsigned listValue, unsigned portValue>
void TestOnePortConstIface()
{
    typedef typename Pins::template Pin<0>::Port Port;
    typename Pins::DataType val;
    cout << __FUNCTION__ << "\t";
    PrintPinList<Pins>::Print();

    Port::template Write<0>();

    Pins::template Write<listValue>();
    EXPECT_EQ(Port::OutReg,  portValue);
    val = Pins::Read();
    EXPECT_EQ(val, listValue);

    Port::DirReg = 0;
    Pins::template SetConfiguration<listValue, Pins::Out>();
    EXPECT_EQ(Port::DirReg, portValue);

    Port::template Write<0>();
    Port::DirReg = 0;

    Port::InReg = portValue;
    val = Pins::PinRead();
    EXPECT_EQ(val, listValue);

    Port::InReg = 0;
    val = Pins::PinRead();
    EXPECT_EQ(val, 0);

    Port::template Write<0>();
    EXPECT_EQ(Port::OutReg, 0);

    Pins::template Set<listValue>();
    EXPECT_EQ(Port::OutReg, portValue);

    Pins::template Clear<listValue>();
    EXPECT_EQ(Port::OutReg, 0);

    Pins::template SetConfiguration<0xff, Pins::In>();
    EXPECT_EQ(Port::DirReg, 0);

    cout << "\tOK" << endl;
}

template<class Pins, class Port1, class Port2>
void Test2PortConfiguration(unsigned listValue, unsigned portValue, unsigned portValue2)
{
    typename Pins::DataType val;
    cout << __FUNCTION__ << "\t";
    PrintPinList<Pins>::Print();
    Pins::Write(listValue);
    EXPECT_EQ(Port1::OutReg, portValue);
    EXPECT_EQ(Port2::OutReg, portValue2);
    val = Pins::Read();
    EXPECT_EQ(val, listValue);

    Port1::OutReg = 0;
    Pins::Set(listValue);
    EXPECT_EQ(Port1::OutReg, portValue);
    EXPECT_EQ(Port2::OutReg, portValue2);
    val = Pins::Read();
    EXPECT_EQ(val, listValue);

    Pins::Clear(listValue);
    EXPECT_EQ(Port1::OutReg, 0);
    EXPECT_EQ(Port2::OutReg, 0);
    val = Pins::Read();
    EXPECT_EQ(val, 0);

    Pins::SetConfiguration(listValue, Pins::Out);
    EXPECT_EQ(Port1::DirReg, portValue);
    EXPECT_EQ(Port2::DirReg, portValue2);
    cout << "\tOK" << endl;
}

TEST(GPIO, PinsTests)
{
	PinTest<Pa0>();
	PinTest<Pa1>();
	PinTest<Pa2>();
	PinTest<Pa3>();
	PinTest<Pa4>();
	PinTest<Pa5>();
	PinTest<Pa6>();
	PinTest<Pa7>();
	PinTest<Pa8>();
	PinTest<Pa9>();
	PinTest<Pa10>();
	PinTest<Pa11>();
	PinTest<Pa12>();
	PinTest<Pa13>();
	PinTest<Pa14>();
	PinTest<Pa15>();
	PinTest<Pa16>();
	PinTest<Pa17>();
	PinTest<Pa18>();
	PinTest<Pa19>();
	PinTest<Pa20>();
	PinTest<Pa21>();
	PinTest<Pa22>();
	PinTest<Pa23>();
	PinTest<Pa24>();
	PinTest<Pa25>();
	PinTest<Pa26>();
	PinTest<Pa27>();
	PinTest<Pa28>();
	PinTest<Pa29>();
	PinTest<Pa30>();
	PinTest<Pa31>();
}

TEST(GPIO, PinListTest)
{
    for(int i=0; i< 16; i++)
    {
        cout << "Writing value: " << i << endl;
        TestOnePortPinList<PinList<Pa0, Pa1, Pa2, Pa3> >(i, i);
        TestOnePortPinList<PinList<Pa1, Pa2, Pa3, Pa4> >(i, i << 1);
        TestOnePortPinList<PinList<Pa2, Pa3, Pa4, Pa5> >(i, i << 2);
    }

    TestOnePortPinList<PinList<Pa1, Pa3, Pa2, Pa0> >(0x0f, 0x0f);
    TestOnePortPinList<PinList<Pa0, Pa2, Pa1, Pa3> >(0x0f, 0x0f);
    TestOnePortPinList<PinList<Pa2, Pa1, Pa3, Pa4, Pa6> >(0x1f, 0x5e);

    TestOnePortPinList<PinList<Pa5, Pa6, Pa7, Pa0, Pa1, Pa2, Pa3, Pa4> >(0xff, 0xff);

    TestOnePortPinList<PinList<Pa2, Pa1, Pa3, Pa4, Pa6, Pa8, Pa7, Pa0, Pa5> >(0x1ff, 0x1ff);

    TestOnePortPinList<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pa8>::Slice<5, 4> >(0x1e0, 0x1e0);
    cout << "Length = \t" <<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pa8>::Slice<5, 4>::Length << std::endl;
    TestOnePortPinList<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pa8>::Slice<0, 4> >(0x0f, 0x0f);

    TestOnePortPinList<PinList<Pa4, Pa1, Pa6, Pa3, Pa7, Pa5, Pa0 > >(0x7f, 0xfb);
    TestOnePortPinList<PinList<Pa4, Pa1, Pa6, Pa3, Pa2, Pa5, Pa0, Pa7 > >(0xaa, 0xaa);
    TestOnePortPinList<PinList<Pa4, Pa1, Pa6, Pa3, Pa2, Pa5, Pa0, Pa7 > >(0x55, 0x55);

    TestOnePortPinList<PinList<Pa1, Pa2, Pa14, Pa15> >(0x0f, (0x3 << 1) | (0x3 << 14));

    typedef PinList<Pa2, Pa1, Pa3, Pa4, Pa6> Pins1;
    typedef PinList<Pins1::Pin<0>, Pins1::Pin<1>, Pins1::Pin<2>, Pins1::Pin<3>, Pins1::Pin<4> > Pins1Clone;

    TestOnePortPinList<Pins1Clone >(0x1f, 0x5e);

    TestOnePortConstIface<PinList<Pa1, Pa3, Pa2, Pa0>, 0x0f, 0x0f>();
    TestOnePortConstIface<PinList<Pa0, Pa2, Pa1, Pa3>, 0x0f, 0x0f>();
    TestOnePortConstIface<PinList<Pa2, Pa1, Pa3, Pa4, Pa6>, 0x1f, 0x5e>();
    TestOnePortConstIface<PinList<Pa5, Pa6, Pa7, Pa0, Pa1, Pa2, Pa3, Pa4>, 0xff, 0xff>();
    TestOnePortConstIface<PinList<Pa2, Pa1, Pa3, Pa4, Pa6, Pa8, Pa7, Pa0, Pa5>, 0x1ff, 0x1ff>();
    TestOnePortConstIface<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pa8>::Slice<5, 4>, 0x1e0, 0x1e0>();
    TestOnePortConstIface<PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7, Pa8>::Slice<0, 4>, 0x0f, 0x0f>();
    TestOnePortConstIface<Pins1Clone, 0x1f, 0x5e>();

    Test2PortConfiguration<PinList<Pa1, Pa3, Pa2, Pa0, Pb1, Pb3, Pb2, Pb0>, Porta, Portb>(0xff, 0x0f, 0x0f);
    Test2PortConfiguration<PinList<Pa1, Pa2, Pa3, Pa0, Pb0, Pb1, Pb2, Pb3>, Porta, Portb>(0xff, 0x0f, 0x0f);
}
