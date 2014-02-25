
#include <usart.h>
#include <buffered_output.h>

#include <gtest.h>

using namespace Mcucpp;

TEST(Usart, Buffered)
{
	typedef BufferedOutput<Usart1, 10, TimeoutCounter<unsigned> > MyUsart;
	MyUsart myUsart;
	myUsart.Init(115200, MyUsart::Default);
	myUsart.EnableInterrupt(MyUsart::AllInterrupts);
	myUsart.usartData.data = 0;
	myUsart.usartData.WriteReady = true;
	myUsart.Write(1);
	EXPECT_EQ(1, myUsart.usartData.data);
	myUsart.usartData.data = 0;

	myUsart.usartData.WriteReady = false;
	myUsart.Write(2);
	EXPECT_EQ(0, myUsart.usartData.data);
	myUsart.Write(3);
	EXPECT_EQ(0, myUsart.usartData.data);
	myUsart.Write(4);
	EXPECT_EQ(0, myUsart.usartData.data);

	myUsart.usartData.WriteReady = true;

	myUsart.TxInterruptHandler();
	EXPECT_EQ(2, MyUsart::usartData.data);
	myUsart.TxInterruptHandler();
	EXPECT_EQ(3, MyUsart::usartData.data);
	myUsart.TxInterruptHandler();
	EXPECT_EQ(4, MyUsart::usartData.data);
	myUsart.TxInterruptHandler();
}