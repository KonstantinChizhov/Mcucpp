

#include <gtest/gtest.h>
#include <ARM/Stm32F40x/EtherRxTxQueue.h>

using namespace Mcucpp;
using namespace Mcucpp::Net;


static uint32_t currentTicks = 0;

static uint32_t GetTicks(){ return currentTicks;}

TEST(EtherRxTxQueueTests, Rx)
{
	EtherRxTxQueue queue;
	ASSERT_EQ(EthOk, queue.PrepareQueues());
	ASSERT_TRUE(queue._rxPool.Descriptors[0].InUse());
	

	ASSERT_NE(queue._rxPool.Descriptors[0].buffer1, queue._rxPool.Descriptors[0].buffer2);
	ASSERT_NE(0, (size_t)queue._rxPool.Descriptors[0].buffer1);
	ASSERT_NE(0, (size_t)queue._rxPool.Descriptors[0].buffer2);

	queue._rxPool.Descriptors[0].SetFrameLength(50);
	queue._rxPool.Descriptors[0].des0 |= MacDmaRxLastDescriptor | MacDmaRxFirstDescriptor | MacDmaRxFrameType;
	queue._rxPool.Descriptors[0].Lock();
	EthMacState state = queue.ProcessInterrupt();
	ASSERT_EQ(EthOk, state);
	
	ASSERT_EQ(1, queue._rxQueue.size());

	queue.InvokeCallbacks(0);

	ASSERT_EQ(0, queue._rxQueue.size());
}