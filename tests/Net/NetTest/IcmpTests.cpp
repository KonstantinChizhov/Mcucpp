
#include <gtest/gtest.h>
#include "NetTestBundle.h"

using namespace Mcucpp;
using namespace Mcucpp::Net;
using namespace Mcucpp::Test;

static uint32_t currentTicks = 0;

static uint32_t GetTicks(){ return currentTicks;}

TEST(Arp, Resolve)
{
	NetTestBundle net;
	currentTicks = 100;
	net.dispatcher.SetTimerFunc(GetTicks);

}