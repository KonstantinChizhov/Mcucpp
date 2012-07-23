#include <fixedpoint.h>
#include <drivers/7segments.h>
#include <iopins.h>
#include <pinlist.h>
#include <gtest.h>
#include <iostream>
#include <util.h>

using namespace Mcucpp;
using namespace IO;
using namespace IO::Test;

typedef TestPort<uint8_t, 'A'> Porta;
typedef TestPort<uint8_t, 'B'> Portb;

DECLARE_PORT_PINS(Porta, Pa)
DECLARE_PORT_PINS(Portb, Pb)

typedef PinList<Pa0, Pa1, Pa2, Pa3, Pa4, Pa5, Pa6, Pa7> Segments;
typedef PinList<Pb0, Pb1, Pb2, Pb3, Pb4, Pb5, Pb6, Pb7> Commons;

class TestMap
{
public:
	static uint8_t Map(uint8_t bcd)
	{
		return bcd <= 9 ? bcd + '0' : bcd + 'A' - 10;
	}

	static uint8_t Minus(){return '-';}
	static uint8_t Empty(){return '_';}
	static uint8_t Exp(){return 'E';}
};

template<class Disp>
char *GetDisplayStr()
{
	static char buffer[Disp::Digits+1];
	for(int i=0; i<Disp::Digits; i++)
	{
		Disp::Update();
		uint8_t index = ilog2(Portb::OutReg);
		EXPECT_LT(index, Disp::Digits);
		buffer[index] = char(Porta::OutReg);
	}
	buffer[sizeof(buffer)-1] = '\0';
	return buffer;
}

TEST(SevenSegments, TestDec)
{
	typedef Display7Segment<Segments, Commons, Base7Segment::Seg7All, TestMap> display;
	display::Dec();
	display::RightAlign();
	display::Write(1234);
	EXPECT_STREQ("____1234", GetDisplayStr<display>());
	display::LeftAlign();
	display::Write(1234);
	EXPECT_STREQ("1234____", GetDisplayStr<display>());

	display::RightAlign();
	display::Write(-1234);
	EXPECT_STREQ("___-1234", GetDisplayStr<display>());
	display::LeftAlign();
	display::Write(-1234);
	EXPECT_STREQ("-1234___", GetDisplayStr<display>());

	display::RightAlign();
	display::Write(1234567890);
	EXPECT_STREQ("123456E3", GetDisplayStr<display>());
	display::LeftAlign();
	display::Write(-1234567890);
	EXPECT_STREQ("-12345E4", GetDisplayStr<display>());

}


TEST(SevenSegments, TestHex)
{
	typedef Display7Segment<Segments, Commons, Base7Segment::Seg7All, TestMap> display;
	display::Hex();
	display::RightAlign();
	display::Write(0xABCD);
	EXPECT_STREQ("____ABCD", GetDisplayStr<display>());
	display::LeftAlign();
	display::Write(0xABCD);
	EXPECT_STREQ("ABCD____", GetDisplayStr<display>());

	display::RightAlign();
	display::Write(0x1234567890ll);
	EXPECT_STREQ("123456E3", GetDisplayStr<display>());
	display::LeftAlign();
	display::Write(0x1234567890ll);
	EXPECT_STREQ("-12345E4", GetDisplayStr<display>());

}

