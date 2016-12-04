//*****************************************************************************
//
// Author		: Konstantin Chizhov
// Date			: 2016
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification, 
// are permitted provided that the following conditions are met:
// Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.

// Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation and/or 
// other materials provided with the distribution.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY 
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//*****************************************************************************
#pragma once

namespace Mcucpp
{
namespace Net
{
	enum EtherType
	{
		IPv4 = 0x0800, ///	Internet Protocol version 4 (IPv4)
		ARP = 0x0806, ///	Address Resolution Protocol (ARP)
		WakeOnLan= 0x0842, ///	Wake-on-LAN[4]
		TRILL = 0x22F3, ///	IETF TRILL Protocol
		DECnet = 0x6003, ///	DECnet Phase IV
		ReverseArp = 0x8035, ///	Reverse Address Resolution Protocol
		Ethertalk = 0x809B, ///	AppleTalk (Ethertalk)
		AARP = 0x80F3, ///	AppleTalk Address Resolution Protocol (AARP)
		VLAN_Tagged = 0x8100, ///	VLAN-tagged frame (IEEE 802.1Q) and Shortest Path Bridging IEEE 802.1aq[5]
		IPX = 0x8137, ///	IPX
		Qnet = 0x8204, ///	QNX Qnet
		IPv6 = 0x86DD, ///	Internet Protocol Version 6 (IPv6)
		EtherFlowControl = 0x8808, ///	Ethernet flow control
		CobraNet = 0x8819, ///	CobraNet
		MPLS_Unicast = 0x8847, ///	MPLS unicast
		MPLS_multicast = 0x8848, ///	MPLS multicast
		PPPoE_Discovery = 0x8863, ///	PPPoE Discovery Stage
		PPPoE_Session = 0x8864, ///	PPPoE Session Stage
		HomePlug_1_0 = 0x887B, ///	HomePlug 1.0 MME
		EAP_OverLAN = 0x888E, ///	EAP over LAN (IEEE 802.1X)
		PROFINET = 0x8892, ///	PROFINET Protocol
		HyperSCSI = 0x889A, ///	HyperSCSI (SCSI over Ethernet)
		ATAOverEthernet = 0x88A2, ///	ATA over Ethernet
		EtherCAT = 0x88A4, ///	EtherCAT Protocol
		ProviderBridging= 0x88A8, ///	Provider Bridging (IEEE 802.1ad) & Shortest Path Bridging IEEE 802.1aq[5]
		EthernetPowerlink = 0x88AB, ///	Ethernet Powerlink[citation needed]
		GOOSE = 0x88B8, ///	GOOSE (Generic Object Oriented Substation event)
		GSE = 0x88B9, ///	GSE (Generic Substation Events) Management Services
		LLDP = 0x88CC, ///	Link Layer Discovery Protocol (LLDP)
		SERCOS_III = 0x88CD, ///	SERCOS III
		HomePlugAV = 0x88E1, ///	HomePlug AV MME[citation needed]
		MRP = 0x88E3, ///	Media Redundancy Protocol (IEC62439-2)
		MAC_Security = 0x88E5, ///	MAC security (IEEE 802.1AE)
		PBB = 0x88E7, ///	Provider Backbone Bridges (PBB) (IEEE 802.1ah)
		PTP = 0x88F7, ///	Precision Time Protocol (PTP) over Ethernet (IEEE 1588)
		PRP = 0x88FB, ///	Parallel Redundancy Protocol (PRP)
		CFM = 0x8902, ///	IEEE 802.1ag Connectivity Fault Management (CFM) Protocol / ITU-T Recommendation Y.1731 (OAM)
		FCoE = 0x8906, ///	Fibre Channel over Ethernet (FCoE)
		FCoEInit = 0x8914, ///	FCoE Initialization Protocol
		RoCE = 0x8915, ///	RDMA over Converged Ethernet (RoCE)
		TTE = 0x891D, ///	TTEthernet Protocol Control Frame (TTE)
		HSR = 0x892F, ///	High-availability Seamless Redundancy (HSR)
		ECTP = 0x9000, ///	Ethernet Configuration Testing Protocol[6]
		VLAN_DoubleTagged = 0x9100, ///	VLAN-tagged (IEEE 802.1Q) frame with double tagging
		IEEE802_1_Public1 = 0x88b5, /// IEEE Std 802 - Local Experimental Ethertype 2.  
		                            /// This Ethertype value is available for public use and for prototype and 
		                            /// vendor-specific protocol development, as defined in Amendment 802a to IEEE Std 802.
		IEEE802_1_Public2 = 0x88b6, 
		IEEE802_1_Public3 = 0x88b7
	};
}}