

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
	
	MacAddr macAddr;
	bool result = net.arp.AddressResolve(IpAddr(0xc0, 0xa8, 0x01, 0x05), macAddr);
	EXPECT_FALSE(result);
	int frameSize = net.iface.LastTransmittedFrame.Size();
	EXPECT_EQ(42, frameSize);
	
	MacAddr destMacAddr = net.iface.LastTransmittedFrame.ReadMac();
	EXPECT_EQ(MacAddr::Broadcast(), destMacAddr);
	
	MacAddr srcMacAddr = net.iface.LastTransmittedFrame.ReadMac();
	EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01), srcMacAddr);
	
	EXPECT_EQ(EtherARP, net.iface.LastTransmittedFrame.ReadU16Be());
	EXPECT_EQ(0x0001, net.iface.LastTransmittedFrame.ReadU16Be());
	EXPECT_EQ(0x0800, net.iface.LastTransmittedFrame.ReadU16Be());
	
	EXPECT_EQ(0x06, net.iface.LastTransmittedFrame.ReadU8());
	EXPECT_EQ(0x04, net.iface.LastTransmittedFrame.ReadU8());
	
	EXPECT_EQ(0x0001, net.iface.LastTransmittedFrame.ReadU16Be());
	
	EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01), net.iface.LastTransmittedFrame.ReadMac());
	EXPECT_EQ(IpAddr(0xc0, 0xa8, 0x01, 0x02), net.iface.LastTransmittedFrame.ReadIp());
	
	EXPECT_EQ(MacAddr(0, 0, 0, 0, 0, 0), net.iface.LastTransmittedFrame.ReadMac());
	EXPECT_EQ(IpAddr(0xc0, 0xa8, 0x01, 0x05), net.iface.LastTransmittedFrame.ReadIp());
	
	uint8_t arpResponce[] = 
	{
		0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01, // Ethernet dest MAC
		0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02, // Ethernet src MAC
		0x08, 0x06, // ether type
		0x00, 0x01, // ARP hardware type
		0x08, 0x00, // ARP address protocol
		0x06,       // ARP hardware addr size
		0x04,       // ARP protocol addr size
		0x00, 0x02, // ARP operation code
		0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02, // ARP src MAC
		0xc0, 0xa8, 0x01, 0x05,             // ARP src IP
		0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01, // ARP dest MAC
		0xc0, 0xa8, 0x01, 0x02              // ARP dest IP
	};
	
	DataBuffer responceBuffer;
	responceBuffer.AttachFront(DataBuffer::GetNew(arpResponce, sizeof(arpResponce)));
	responceBuffer.Seek(6+6+2);
	
	net.netDispatch.RxComplete(Net::MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02), Net::MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01), EtherARP, responceBuffer);
	
	result = net.arp.AddressResolve(IpAddr(0xc0, 0xa8, 0x01, 0x05), macAddr);
	EXPECT_TRUE(result);
	EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02), macAddr);
	
	
}


TEST(Arp, Respond)
{
	NetTestBundle net;
	currentTicks = 100;
	net.dispatcher.SetTimerFunc(GetTicks);
	
	uint8_t arpRequest[] = 
	{
		0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // Ethernet dest MAC
		0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02, // Ethernet src MAC
		0x08, 0x06, // ether type
		0x00, 0x01, // ARP hardware type
		0x08, 0x00, // ARP address protocol
		0x06,       // ARP hardware addr size
		0x04,       // ARP protocol addr size
		0x00, 0x01, // ARP operation code
		0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02, // ARP src MAC
		0xc0, 0xa8, 0x01, 0x05,             // ARP src IP
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // ARP dest MAC
		0xc0, 0xa8, 0x01, 0x02              // ARP dest IP
	};
	
		
	DataBuffer requestBuffer;
	requestBuffer.AttachFront(DataBuffer::GetNew(arpRequest, sizeof(arpRequest)));
	requestBuffer.Seek(6+6+2);
	
	net.netDispatch.RxComplete(Net::MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02), Net::MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01), EtherARP, requestBuffer);
	
	int frameSize = net.iface.LastTransmittedFrame.Size();
	EXPECT_EQ(42, frameSize);

	MacAddr destMacAddr = net.iface.LastTransmittedFrame.ReadMac();
	EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02), destMacAddr);
	
	MacAddr srcMacAddr = net.iface.LastTransmittedFrame.ReadMac();
	EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01), srcMacAddr);
	
	EXPECT_EQ(EtherARP, net.iface.LastTransmittedFrame.ReadU16Be());
	EXPECT_EQ(0x0001, net.iface.LastTransmittedFrame.ReadU16Be());
	EXPECT_EQ(0x0800, net.iface.LastTransmittedFrame.ReadU16Be());
	
	EXPECT_EQ(0x06, net.iface.LastTransmittedFrame.ReadU8());
	EXPECT_EQ(0x04, net.iface.LastTransmittedFrame.ReadU8());
	
	EXPECT_EQ(0x0002, net.iface.LastTransmittedFrame.ReadU16Be());
	
	EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01), net.iface.LastTransmittedFrame.ReadMac());
	EXPECT_EQ(IpAddr(0xc0, 0xa8, 0x01, 0x02), net.iface.LastTransmittedFrame.ReadIp());
	
	EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02), net.iface.LastTransmittedFrame.ReadMac());
	EXPECT_EQ(IpAddr(0xc0, 0xa8, 0x01, 0x05), net.iface.LastTransmittedFrame.ReadIp());
}


TEST(Arp, ResolveTimeout)
{
	NetTestBundle net;
	currentTicks = 100;
	net.dispatcher.SetTimerFunc(GetTicks);
	
	MacAddr macAddr;
	bool result = net.arp.AddressResolve(IpAddr(0xc0, 0xa8, 0x01, 0x05), macAddr);
	EXPECT_FALSE(result);
	net.dispatcher.Poll();
	
	for(int i = 0; i < 4; i++)
	{
		if(i == 3)
		{
			EXPECT_EQ(0, net.iface.LastTransmittedFrame.Size());
			break;
		}
		int frameSize = net.iface.LastTransmittedFrame.Size();
		EXPECT_EQ(42, frameSize);

		MacAddr destMacAddr = net.iface.LastTransmittedFrame.ReadMac();
		EXPECT_EQ(MacAddr::Broadcast(), destMacAddr);
		
		MacAddr srcMacAddr = net.iface.LastTransmittedFrame.ReadMac();
		EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01), srcMacAddr);
		
		EXPECT_EQ(EtherARP, net.iface.LastTransmittedFrame.ReadU16Be());
		EXPECT_EQ(0x0001, net.iface.LastTransmittedFrame.ReadU16Be());
		EXPECT_EQ(0x0800, net.iface.LastTransmittedFrame.ReadU16Be());
		
		EXPECT_EQ(0x06, net.iface.LastTransmittedFrame.ReadU8());
		EXPECT_EQ(0x04, net.iface.LastTransmittedFrame.ReadU8());
		
		EXPECT_EQ(0x0001, net.iface.LastTransmittedFrame.ReadU16Be());
		
		EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01), net.iface.LastTransmittedFrame.ReadMac());
		EXPECT_EQ(IpAddr(0xc0, 0xa8, 0x01, 0x02), net.iface.LastTransmittedFrame.ReadIp());
		
		EXPECT_EQ(MacAddr(0, 0, 0, 0, 0, 0), net.iface.LastTransmittedFrame.ReadMac());
		EXPECT_EQ(IpAddr(0xc0, 0xa8, 0x01, 0x05), net.iface.LastTransmittedFrame.ReadIp());
		
		net.iface.LastTransmittedFrame.Clear();
		currentTicks += 150;
		net.dispatcher.Poll();
	}
}
