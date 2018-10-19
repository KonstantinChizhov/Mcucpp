
#pragma once
#include <net/ArpProtocol.h>
#include <net/IcmpProtocol.h>
#include <net/NetDispatch.h>
#include <net/ether_type.h>

#include <memory_reader.h>
#include <binary_stream.h>
#include <TestNetworkInterface.h>

using namespace Mcucpp;
using namespace Mcucpp::Net;
using namespace Mcucpp::Test;


class NetTestBundle
{
public:
	TaskItem tasks[20];
	TimerData timers[20];
	Dispatcher dispatcher;
	TestNetworkInterface iface;
	NetDispatch netDispatch;
	ArpProtocol arp;
	IP4Protocol ip;
	
	NetTestBundle()
	: dispatcher(tasks, 20, timers, 20),
		netDispatch(dispatcher),
		arp(dispatcher, netDispatch, iface),
		ip(dispatcher, netDispatch, iface)
		
	{
		iface.SetMacAddress(0, MacAddr(0x1c, 0x6f, 0x65, 0x8a, 0x07, 0x01));
		netDispatch.AddInterface(&iface);
		netDispatch.AddProtocol(EtherARP, &arp);
		netDispatch.AddProtocol(EtherIPv4, &ip);
		ip.SetAddressResolver(&arp);
		arp.SetIp4Protocol(&ip);
		ip.SetIpAddr(IpAddr(0xc0, 0xa8, 0x01, 0x02));
		ip.SetNetMask(IpAddr(255,255,255,0));
	}
	
};