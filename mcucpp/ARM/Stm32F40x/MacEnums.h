#pragma once

namespace Mcucpp
{	
	enum PhyRegs
	{
		PhyBasicControlRegister = 0x00,
		PhyBasicStatusRegister = 0x01
	};
	
	enum PhyControl
	{
		PhyReset                  = 0x8000,
		PhyLoopback               = 0x4000,
		PhyFullDuplex             = 0x0100,
		PhySpeed100m              = 0x2000,
		PhyAutonegotiation        = 0x1000,
		PhyRestartAutonegotiation = 0x0200,
		PhyPowerdown              = 0x0800,
		PhyIsolate                = 0x0400
	};
	
	enum PhyStatus
	{
		PhyExtendedCapabilities = 0x0001,
		PhyJabberDetection      = 0x0002,
		PhyLinkedStatus         = 0x0004,
		PhyAutoNegotiateAbility = 0x0008,
		PhyRemoteFault          = 0x0010,
		PhyAutonegoComplete     = 0x0020,
		
		PhyExtendedStatus       = 0x0100,
		PhyHalfDuplex100BaseT2  = 0x0200,
		PhyFullDuplex100BaseT2  = 0x0400,
		PhyHalfDuplex10BaseT    = 0x0800,
		PhyFullDuplex10BaseT    = 0x1000,
		PhyHalfDuplex100BaseTx  = 0x2000,
		PhyFullDuplex100BaseTx  = 0x4000,
		Phy100BaseT4            = 0x8000
	};
	
	enum EthMacState
	{
		EthOk             = 0,
		EthNotInitialized = 1,
		EthMacError       = 2,
		EthPhyError       = 3,
		EthDriverError    = 4,
		EthNotConnected   = 5,
		EthOutOfMem       = 6,
		EthTxQueueFull    = 7,
	};

}