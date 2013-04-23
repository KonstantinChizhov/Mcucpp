
#include <usart.h>
#include <buffered_output.h>

#include <gtest.h>

using namespace Mcucpp;

TEST(Usart, Buffered)
{
	typedef BufferedOutput<TestUsart, 10, TimeoutCounter<unsigned> > MyUsart;
	MyUsart myUsart;
	myUsart.Init(115200, MyUsart::Default);
	myUsart.EnableInterrupts(MyUsart::AllInterrupts);
	myUsart.data = 0;
	myUsart.txReady = true;
	myUsart.Write(1);
	EXPECT_EQ(1, myUsart.data);
	myUsart.data = 0;

	myUsart.txReady = false;
	myUsart.Write(2);
	EXPECT_EQ(0, myUsart.data);
	myUsart.Write(3);
	EXPECT_EQ(0, myUsart.data);
	myUsart.Write(4);
	EXPECT_EQ(0, myUsart.data);

	myUsart.txReady = true;

	myUsart.TxInterruptHandler();
	EXPECT_EQ(2, MyUsart::data);
	myUsart.TxInterruptHandler();
	EXPECT_EQ(3, MyUsart::data);
	myUsart.TxInterruptHandler();
	EXPECT_EQ(4, MyUsart::data);
	myUsart.TxInterruptHandler();
}