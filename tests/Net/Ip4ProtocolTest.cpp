
#include <gtest/gtest.h>
#include "NetTestBundle.h"


static uint32_t currentTicks = 0;
static uint32_t GetTicks(){ return currentTicks;}

	class TestSubIpProtocol :public IIpSubProtocol
	{
		public:
		TestSubIpProtocol()
		:_hitCount(0)
		{
			
		}
		
		Net::IpAddr _srcAddr;
		Net::IpAddr _destAddr;
		Net::NetBuffer _buffer;
		int _hitCount;
		
		void ProcessMessage(const Net::IpAddr &srcAddr, const Net::IpAddr &destAddr, Net::NetBuffer &buffer);
		bool SendMessage(const Net::IpAddr &destAddr, IpProtocolId protocolId, Net::NetBuffer &buffer)
		{
			return false;
		}
	};
	
	void TestSubIpProtocol::ProcessMessage(const Net::IpAddr &srcAddr, const Net::IpAddr &destAddr, Net::NetBuffer &buffer)
	{
		_srcAddr = srcAddr;
		_destAddr = destAddr;
		_buffer = buffer;
		_hitCount++;
	}


TEST(Ip4, Checksum)
{
	uint8_t testHeader[] = 
		{0x45, 0x00, 0x00, 0x95, 0x66, 0x75, 0x00, 0x00, 0x80, 0x11, 0xf2, 0x2d, 0xac, 0x12, 0x44 , 0x9a,
		 0xac, 0x12, 0x44, 0xf6};
		 
	uint16_t result = IP4Protocol::HeaderChecksum(testHeader, sizeof(testHeader));

	EXPECT_EQ(0, result);
}


TEST(Ip4, Ip4Checksum)
{
	uint8_t testHeader[] = 
		{0x45, 0x00, 0x00, 0x95, 0x66, 0x75, 0x00, 0x00, 0x80, 0x11, 0xf2, 0x2d, 0xac, 0x12, 0x44 , 0x9a,
		 0xac, 0x12, 0x44, 0xf6};
		 
	Ip4Checksum checksum;
	BinaryStream<BufferReader> reader(testHeader, sizeof(testHeader));
	
	while(!reader.EndOfStream())
		checksum.Feed(reader.ReadU16Be());
	
	EXPECT_EQ(0, checksum.Result());
}

TEST(Ip4, SendPacket)
{
	NetTestBundle net;
	currentTicks = 100;
	net.dispatcher.SetTimerFunc(GetTicks);
	
	Net::NetBuffer buffer;
	buffer.InsertFront(42);
	
	EXPECT_TRUE(net.ip.SendMessage(IpAddr(192,168,1,5), UDP, buffer));
	int frameSize = net.iface.LastTransmittedFrame.Size();
	EXPECT_EQ(42, frameSize); // ARP request
	
	net.ip.AddressResolved(IpAddr(192,168,1,5), MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02));
	
	EXPECT_EQ(14 + 20 + 42, net.iface.LastTransmittedFrame.Size());
	
	EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02), net.iface.LastTransmittedFrame.ReadMac());
	EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01), net.iface.LastTransmittedFrame.ReadMac());
	
	EXPECT_EQ(EtherIPv4, net.iface.LastTransmittedFrame.ReadU16Be());
	
	EXPECT_EQ(0x4500, net.iface.LastTransmittedFrame.ReadU16Be()); // version, header size and DSCP
	EXPECT_EQ(42+20, net.iface.LastTransmittedFrame.ReadU16Be()); // packet size
	EXPECT_NE(0, net.iface.LastTransmittedFrame.ReadU16Be()); // fragment id
	EXPECT_EQ(0, net.iface.LastTransmittedFrame.ReadU16Be()); // flags, fragment offset
	EXPECT_NE(0, net.iface.LastTransmittedFrame.ReadU8());
	EXPECT_EQ(UDP, net.iface.LastTransmittedFrame.ReadU8());
	EXPECT_NE(0, net.iface.LastTransmittedFrame.ReadU16Be());
	EXPECT_EQ(IpAddr(192,168,1,2), net.iface.LastTransmittedFrame.ReadIp());
	EXPECT_EQ(IpAddr(192,168,1,5), net.iface.LastTransmittedFrame.ReadIp());
	
	Net::NetBuffer buffer2;
	buffer2.InsertFront(33);
	
	net.arp.AddAddress(IpAddr(192,168,1,5), MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02));
	
	EXPECT_TRUE(net.ip.SendMessage(IpAddr(192,168,1,5), UDP, buffer2));
	
	EXPECT_EQ(14 + 20 + 33, net.iface.LastTransmittedFrame.Size());
	
	EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02), net.iface.LastTransmittedFrame.ReadMac());
	EXPECT_EQ(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01), net.iface.LastTransmittedFrame.ReadMac());
	
	EXPECT_EQ(EtherIPv4, net.iface.LastTransmittedFrame.ReadU16Be());
	
	EXPECT_EQ(0x4500, net.iface.LastTransmittedFrame.ReadU16Be()); // version, header size and DSCP
	EXPECT_EQ(33+20, net.iface.LastTransmittedFrame.ReadU16Be()); // packet size
	EXPECT_NE(0, net.iface.LastTransmittedFrame.ReadU16Be()); // fragment id
	EXPECT_EQ(0, net.iface.LastTransmittedFrame.ReadU16Be()); // flags, fragment offset
	EXPECT_NE(0, net.iface.LastTransmittedFrame.ReadU8());
	EXPECT_EQ(UDP, net.iface.LastTransmittedFrame.ReadU8());
	EXPECT_NE(0, net.iface.LastTransmittedFrame.ReadU16Be());
	EXPECT_EQ(IpAddr(192,168,1,2), net.iface.LastTransmittedFrame.ReadIp());
	EXPECT_EQ(IpAddr(192,168,1,5), net.iface.LastTransmittedFrame.ReadIp());
}


TEST(Ip4, RecivePacket)
{
	NetTestBundle net;
	currentTicks = 100;
	net.dispatcher.SetTimerFunc(GetTicks);
	
	uint8_t ipPacket[] = 
	{
		0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01, // Ethernet dest MAC
		0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02, // Ethernet src MAC
		0x08, 0x00, // ether type IP
		0x45, // version, header size
		0x00, //DSCP
		0x00, 20 + 24, //Total Length
		0x12, 0x34, // ID
		0x00, 0x00, // flags and fragment offset
		0x64, // TTL
		0x11, // protocol
		0x83, 0x73, // checksum
		192,168,1,5, // src IP
		192,168,1,2, // dest IP
		
		0x11, 0x22, // UDP header
		0x33, 0x44,
		0x00, 24,
		0x00, 0x00,
		
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00
	};
	TestSubIpProtocol protocol;
	net.ip.AddProtocol(UDP, &protocol);

	NetBuffer buffer;
	buffer.AttachFront(DataBuffer::GetNew(ipPacket, sizeof(ipPacket)));
	buffer.Seek(14);
	
	net.netDispatch.RxComplete(MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x02), MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01), 0x0800, buffer);
	
	EXPECT_EQ(1, protocol._hitCount);
	EXPECT_EQ(IpAddr(192,168,1,5), protocol._srcAddr);
	EXPECT_EQ(IpAddr(192,168,1,2), protocol._destAddr);
	EXPECT_EQ(0x1122, protocol._buffer.ReadU16Be());
}