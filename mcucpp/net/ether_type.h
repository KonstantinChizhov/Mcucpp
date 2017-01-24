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
		EtherIPv4 = 0x0800, ///	Internet Protocol version 4 (IPv4)
		EtherARP = 0x0806, ///	Address Resolution Protocol (ARP)
		EtherWakeOnLan= 0x0842, ///	Wake-on-LAN[4]
		EtherTRILL = 0x22F3, ///	IETF TRILL Protocol
		EtherDECnet = 0x6003, ///	DECnet Phase IV
		EtherReverseArp = 0x8035, ///	Reverse Address Resolution Protocol
		EtherEthertalk = 0x809B, ///	AppleTalk (Ethertalk)
		EtherAARP = 0x80F3, ///	AppleTalk Address Resolution Protocol (AARP)
		EtherVLAN_Tagged = 0x8100, ///	VLAN-tagged frame (IEEE 802.1Q) and Shortest Path Bridging IEEE 802.1aq[5]
		EtherIPX = 0x8137, ///	IPX
		EtherQnet = 0x8204, ///	QNX Qnet
		EtherIPv6 = 0x86DD, ///	Internet Protocol Version 6 (IPv6)
		EtherEtherFlowControl = 0x8808, ///	Ethernet flow control
		EtherCobraNet = 0x8819, ///	CobraNet
		EtherMPLS_Unicast = 0x8847, ///	MPLS unicast
		EtherMPLS_multicast = 0x8848, ///	MPLS multicast
		EtherPPPoE_Discovery = 0x8863, ///	PPPoE Discovery Stage
		EtherPPPoE_Session = 0x8864, ///	PPPoE Session Stage
		EtherHomePlug_1_0 = 0x887B, ///	HomePlug 1.0 MME
		EtherEAP_OverLAN = 0x888E, ///	EAP over LAN (IEEE 802.1X)
		EtherPROFINET = 0x8892, ///	PROFINET Protocol
		EtherHyperSCSI = 0x889A, ///	HyperSCSI (SCSI over Ethernet)
		EtherATAOverEthernet = 0x88A2, ///	ATA over Ethernet
		EtherEtherCAT = 0x88A4, ///	EtherCAT Protocol
		EtherProviderBridging= 0x88A8, ///	Provider Bridging (IEEE 802.1ad) & Shortest Path Bridging IEEE 802.1aq[5]
		EtherEthernetPowerlink = 0x88AB, ///	Ethernet Powerlink[citation needed]
		EtherGOOSE = 0x88B8, ///	GOOSE (Generic Object Oriented Substation event)
		EtherGSE = 0x88B9, ///	GSE (Generic Substation Events) Management Services
		EtherLLDP = 0x88CC, ///	Link Layer Discovery Protocol (LLDP)
		EtherSERCOS_III = 0x88CD, ///	SERCOS III
		EtherHomePlugAV = 0x88E1, ///	HomePlug AV MME[citation needed]
		EtherMRP = 0x88E3, ///	Media Redundancy Protocol (IEC62439-2)
		EtherMAC_Security = 0x88E5, ///	MAC security (IEEE 802.1AE)
		EtherPBB = 0x88E7, ///	Provider Backbone Bridges (PBB) (IEEE 802.1ah)
		EtherPTP = 0x88F7, ///	Precision Time Protocol (PTP) over Ethernet (IEEE 1588)
		EtherPRP = 0x88FB, ///	Parallel Redundancy Protocol (PRP)
		EtherCFM = 0x8902, ///	IEEE 802.1ag Connectivity Fault Management (CFM) Protocol / ITU-T Recommendation Y.1731 (OAM)
		EtherFCoE = 0x8906, ///	Fibre Channel over Ethernet (FCoE)
		EtherFCoEInit = 0x8914, ///	FCoE Initialization Protocol
		EtherRoCE = 0x8915, ///	RDMA over Converged Ethernet (RoCE)
		EtherTTE = 0x891D, ///	TTEthernet Protocol Control Frame (TTE)
		EtherHSR = 0x892F, ///	High-availability Seamless Redundancy (HSR)
		EtherECTP = 0x9000, ///	Ethernet Configuration Testing Protocol[6]
		EtherVLAN_DoubleTagged = 0x9100, ///	VLAN-tagged (IEEE 802.1Q) frame with double tagging
		EtherIEEE802_1_Public1 = 0x88b5, /// IEEE Std 802 - Local Experimental Ethertype 2.  
		                            /// This Ethertype value is available for public use and for prototype and 
		                            /// vendor-specific protocol development, as defined in Amendment 802a to IEEE Std 802.
		EtherIEEE802_1_Public2 = 0x88b6, 
		EtherIEEE802_1_Public3 = 0x88b7
	};
}}