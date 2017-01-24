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
	enum IpProtocolId
	{
		HOPOPT		= 0x00, // IPv6 Hop-by-Hop Option RFC 2460
		ICMP		= 0x01, // Internet Control Message Protocol RFC 792
		IGMP		= 0x02, // Internet Group Management Protocol RFC 1112
		GGP			= 0x03, // Gateway-to-Gateway Protocol RFC 823
		IPinIP		= 0x04, // IP in IP (encapsulation) RFC 2003
		ST			= 0x05, // Internet Stream Protocol, RFC 1190, RFC 1819
		TCP			= 0x06, // Transmission Control Protocol, RFC 793
		CBT			= 0x07, // Core-based trees, RFC 2189
		EGP			= 0x08, // Exterior Gateway Protocol, RFC 888
		IGP			= 0x09, // Interior Gateway Protocol (any private interior gateway (used by Cisco for their IGRP))
		BBN_RCC_MON = 0x0A, // BBN RCC Monitoring
		NVP_II		= 0x0B, // Network Voice Protocol, RFC 741
		PUP			= 0x0C, // Xerox PUP
		ARGUS		= 0x0D, // ARGUS
		EMCON		= 0x0E, // EMCON
		XNET		= 0x0F, // Cross Net Debugger, IEN 158
		CHAOS		= 0x10, // Chaos
		UDP			= 0x11, // User Datagram Protocol, RFC 768
		MUX			= 0x12, // Multiplexing, IEN 90
		DCN_MEAS	= 0x13, // DCN Measurement Subsystems
		HMP			= 0x14, // Host Monitoring Protocol, RFC 869
		PRM			= 0x15, // Packet Radio Measurement
		XNS_IDP		= 0x16, // XEROX NS IDP
		TRUNK_1		= 0x17, // Trunk-1
		TRUNK_2		= 0x18, // Trunk-2
		LEAF_1		= 0x19, // Leaf-1
		LEAF_2		= 0x1A, // Leaf-2
		RDP			= 0x1B, // Reliable Datagram Protocol, RFC 908
		IRTP		= 0x1C, // Internet Reliable Transaction Protocol, RFC 938
		ISO_TP4		= 0x1D, // ISO Transport Protocol Class 4, RFC 905
		NETBLT		= 0x1E, // Bulk Data Transfer Protocol, RFC 998
		MFE_NSP		= 0x1F, // MFE Network Services Protocol
		MERIT_INP	= 0x20, // MERIT Internodal Protocol
		DCCP		= 0x21, // Datagram Congestion Control Protocol, RFC 4340
		ThirdParty	= 0x22, // Third Party Connect Protocol
		IDPR		= 0x23, // Inter-Domain Policy Routing Protocol, RFC 1479
		XTP			= 0x24, // Xpress Transport Protocol
		DDP			= 0x25, // Datagram Delivery Protocol
		IDPR_CMTP	= 0x26, // IDPR Control Message Transport Protocol
		TP_plus_plus= 0x27, // TP++ Transport Protocol
		IL			= 0x28, // IL Transport Protocol
		IPv6		= 0x29, // IPv6 Encapsulation, RFC 2473
		SDRP		= 0x2A, // Source Demand Routing Protocol, RFC 1940
		IPv6_Route	= 0x2B, // Routing Header for IPv6, RFC 2460
		IPv6_Frag	= 0x2C, // Fragment Header for IPv6, RFC 2460
		IDRP		= 0x2D, // Inter-Domain Routing Protocol
		RSVP		= 0x2E, // Resource Reservation Protocol, RFC 2205
		GRE			= 0x2F, // Generic Routing Encapsulation, RFC 2784, RFC 2890
		DSR			= 0x30, // Dynamic Source Routing Protocol, RFC 4728
		BNA			= 0x31, // Burroughs Network Architecture
		ESP			= 0x32, // Encapsulating Security Payload, RFC 4303
		AH			= 0x33, // Authentication Header, RFC 4302
		I_NLSP		= 0x34, // Integrated Net Layer Security Protocol		// TUBA
		SWIPE		= 0x35, // SwIPe, IP with Encryption
		NARP		= 0x36, // NBMA Address Resolution Protocol, RFC 1735
		MOBILE		= 0x37, // IP Mobility (Min Encap), RFC 2004
		TLSP		= 0x38, // Transport Layer Security Protocol (using Kryptonet key management)
		SKIP		= 0x39, // Simple Key-Management for Internet Protocol, RFC 2356
		IPv6_ICMP	= 0x3A, // ICMP for IPv6, RFC 4443, RFC 4884
		IPv6_NoNxt	= 0x3B, // No Next Header for IPv6, RFC 2460
		IPv6_Opts	= 0x3C, // Destination Options for IPv6, RFC 2460
		CFTP		= 0x3E, // CFTP
		SAT_EXPAK	= 0x40, // SATNET and Backroom EXPAK
		KRYPTOLAN	= 0x41, // Kryptolan
		RVD			= 0x42, // MIT Remote Virtual Disk Protocol
		IPPC		= 0x43, // Internet Pluribus Packet Core
		AnyDistribFs= 0x44, // Any distributed file system
		SAT_MON		= 0x45, // SATNET Monitoring
		VISA		= 0x46, // VISA Protocol
		IPCU		= 0x47, // Internet Packet Core Utility
		CPNX		= 0x48, // Computer Protocol Network Executive
		CPHB		= 0x49, // Computer Protocol Heart Beat
		WSN			= 0x4A, // Wang Span Network
		PVP			= 0x4B, // Packet Video Protocol
		BR_SAT_MON	= 0x4C, // Backroom SATNET Monitoring
		SUN_ND		= 0x4D, // SUN ND PROTOCOL-Temporary
		WB_MON		= 0x4E, // WIDEBAND Monitoring
		WB_EXPAK	= 0x4F, // WIDEBAND EXPAK
		ISO_IP		= 0x50, // International Organization for Standardization Internet Protocol
		VMTP		= 0x51, // Versatile Message Transaction Protocol, RFC 1045
		SECURE_VMTP	= 0x52, // Secure Versatile Message Transaction Protocol, RFC 1045
		VINES		= 0x53, // VINES
		TTP			= 0x54, // TTP
		IPTM		= 0x54, // Internet Protocol Traffic Manager
		NSFNET_IGP	= 0x55, // NSFNET-IGP
		DGP			= 0x56, // Dissimilar Gateway Protocol
		TCF			= 0x57, // TCF
		EIGRP		= 0x58, // EIGRP
		OSPF		= 0x59, // Open Shortest Path First, RFC 1583
		Sprite_RPC	= 0x5A, // Sprite RPC Protocol
		LARP		= 0x5B, // Locus Address Resolution Protocol
		MTP			= 0x5C, // Multicast Transport Protocol
		AX_25		= 0x5D, // AX.25
		OS			= 0x5E, // KA9Q NOS compatible IP over IP tunneling
		MICP		= 0x5F, // Mobile Internetworking Control Protocol
		SCC_SP		= 0x60, // Semaphore Communications Sec. Pro
		ETHERIP		= 0x61, // Ethernet-within-IP Encapsulation, RFC 3378
		ENCAP		= 0x62, // Encapsulation Header, RFC 1241
		AnyIpPrivateEncryption = 0x63, 
		GMTP		= 0x64, // GMTP
		IFMP		= 0x65, // Ipsilon Flow Management Protocol
		PNNI		= 0x66, // PNNI over IP
		PIM			= 0x67, // Protocol Independent Multicast
		ARIS		= 0x68, // IBM's ARIS (Aggregate Route IP Switching) Protocol
		SCPS		= 0x69, // SCPS (Space Communications Protocol Standards)		// SCPS-TP[2]
		QNX			= 0x6A, // QNX
		ANActiveNetworks = 0x6B,
		IPComp		= 0x6C, // IP Payload Compression Protocol, RFC 3173
		SNP			= 0x6D, // Sitara Networks Protocol
		Compaq_Peer	= 0x6E, // Compaq Peer Protocol
		IPX_in_IP	= 0x6F, // IPX in IP
		VRRP		= 0x70, // Virtual Router Redundancy Protocol, Common Address Redundancy Protocol (not IANA assigned)		// VRRP:RFC 3768
		PGM			= 0x71, // PGM Reliable Transport Protocol, RFC 3208
		L2TP		= 0x73, // Layer Two Tunneling Protocol Version 3, RFC 3931
		DDX			= 0x74, // D-II Data Exchange (DDX)
		IATP		= 0x75, // Interactive Agent Transfer Protocol
		STP			= 0x76, // Schedule Transfer Protocol
		SRP			= 0x77, // SpectraLink Radio Protocol
		UTI			= 0x78, // Universal Transport Interface Protocol
		SMP			= 0x79, // Simple Message Protocol
		SM			= 0x7A, // Simple Multicast Protocol		// draft-perlman-simple-multicast-03
		PTP			= 0x7B, // Performance Transparency Protocol
		IS_IS_over_IPv4 = 0x7C, // Intermediate System to Intermediate System (IS-IS) Protocol over IPv4, RFC 1142 and RFC 1195
		FIRE		= 0x7D, // Flexible Intra-AS Routing Environment
		CRTP		= 0x7E, // Combat Radio Transport Protocol
		CRUDP		= 0x7F, // Combat Radio User Datagram
		SSCOPMCE	= 0x80, // Service-Specific Connection-Oriented Protocol in a Multilink and Connectionless Environment		// ITU-T Q.2111 (1999)
		IPLT		= 0x81, // 
		SPS			= 0x82, // Secure Packet Shield
		PIPE		= 0x83, // Private IP Encapsulation within IP		// Expired I-D draft-petri-mobileip-pipe-00.txt
		SCTP		= 0x84, // Stream Control Transmission Protocol
		FC			= 0x85, // Fibre Channel
		RSVP_E2E_IGNORE = 0x86, // Reservation Protocol (RSVP) End-to-End Ignore, RFC 3175
		Mobility_Header = 0x87, // Mobility Extension Header for IPv6, RFC 6275
		UDPLite		= 0x88, // Lightweight User Datagram Protocol, RFC 3828
		MPLS_in_IP	= 0x89, // Multiprotocol Label Switching Encapsulated in IP, RFC 4023
		Manet		= 0x8A, // MANET Protocols, RFC 5498
		HIP			= 0x8B, // Host Identity Protocol, RFC 5201
		Shim6		= 0x8C, // Site Multihoming by IPv6 Intermediation, RFC 5533
		WESP		= 0x8D, // Wrapped Encapsulating Security Payload, RFC 5840
		ROHC		= 0x8E, // Robust Header Compression, RFC 5856
		IpExperimantal = 0xFD,
		IpExperimantal2= 0xFE
	};
}}