

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
#include <stm32f4xx.h>
#include <clock.h>
#include <pinlist.h>
#include <iopins.h>
#include <ioreg.h>
#include <enum.h>
#include <net/net_addr.h>
#include <mac_descriptors.h>
#include <data_transfer.h>

namespace Mcucpp
{
	enum EthSpeed
	{
		EthSpeed10Mbit = 0,
		EthSpeed100Mbit = ETH_MACCR_FES
	};
	
	enum EthDuplexMode
	{
		EthHalfDuplex = 0,
		EthFullDuplex = ETH_MACCR_DM
	};
	
	enum EthMediaInterface
	{
		EthMii = 0,
		EthRmii = SYSCFG_PMC_MII_RMII_SEL
	};
	
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
		EthDriverError    = 4
	};
	
		
	class EthernetMac
	{
	public:
		typedef IO::PinList<
			IO::Pa1, // ETH_RMII_REF_CLK
			IO::Pa2, // ETH_MDIO
			IO::Pa7, // ETH_RMII_CRS_DV
			IO::Pc1, // ETH_MDC
			IO::Pc4, // ETH_RMII_RXD0
			IO::Pc5, // ETH_RMII_RXD1
			IO::Pb11, // ETH_RMII_TX_EN
			IO::Pb12, // ETH_RMII_TXD0
			IO::Pb13 // ETH_RMII_TXD1
			> EthRmiiPins;
		
		inline void EnableClocks();
		inline void ConfigurePins();
		inline void AdjustClockRange();
		inline void CheckPhyCapability();
		inline void SetPhyParameters();
		inline void ReadLinkParameters();
		inline void SetMacParameters();
		inline void SetDmaParameters();
		inline void StartAutonegotiation();
		inline bool ReadPhyRegister(uint16_t phyReg, uint16_t *regValue);
		inline bool WritePhyRegister(uint16_t phyReg, uint16_t regValue);
		inline bool ModifyPhyRegister(uint16_t phyReg, uint16_t setMask, uint16_t clearMask);
		inline void ResetPhy();
		inline void Reset();
		inline void InterruptHandler();
		
		ETH_TypeDef *_eth;
		EthMediaInterface _mediaInterface;
		uint8_t _phyAddr;
		bool _autonegotiation;
		EthSpeed _speed;
		EthDuplexMode _duplexMode;
		bool _linked;
		EthMacState _state;
		Net::EthTxPool _txPool;
		
		Net::MacRxDescriptor _rx;
		uint32_t _status;
		uint8_t _rxBuffer[1800];
		TaggedTransferCallback _callback;
		void *_tag;
	public:
		inline EthernetMac(ETH_TypeDef *eth);
		inline void Init();
		
		inline bool SetMacAddress(unsigned addrNumber, Net::NetAddress<6> macaddr);
		inline unsigned MaxAddresses(){return 4;}
		
		inline void EnableAutonegotiation(bool autonegotiation);
		inline bool AutonegotiationEnabled();
		inline void SetSpeed(EthSpeed speed);
		inline EthSpeed GetSpeed();
		inline void SetDuplexMode(EthDuplexMode duplex);
		inline EthDuplexMode GetDuplexMode();
		inline void SetMediaInterface(EthMediaInterface mediaInterface);
		inline void SetPhyAddress(uint8_t phyAddr);
		inline bool Initialized(){return _state != EthNotInitialized;}
		inline bool IsLinked();
		inline void PauseCommand(uint16_t time);
		inline void Poll();
		
		inline bool Transmit(const Net::MacAddr &dest, uint16_t etherType, void *data, size_t size);
		
	};
	//==================================================================
	extern EthernetMac ethernet;
	//==================================================================
	
	EthernetMac::EthernetMac(ETH_TypeDef *eth)
		:_eth(eth),
		_mediaInterface(EthRmii),
		_phyAddr(0x01),
		_autonegotiation(true),
		_speed(EthSpeed100Mbit),
		_duplexMode(EthFullDuplex),
		_state(EthNotInitialized),
		_callback(0)
	{
	
	}
	
	bool EthernetMac::Transmit(const Net::MacAddr &dest, uint16_t etherType, void *data, size_t size)
	{
		bool res = _txPool.EnqueueBuffer(dest, etherType, data, size);
		ETH->DMATPDR = 1;
		return res;
	}
	
	bool EthernetMac::SetMacAddress(unsigned addrNumber, Net::NetAddress<6> macaddr)
	{
		if(addrNumber >= MaxAddresses())
		{
			return false;
		}
		uint32_t addrHi = ((macaddr[5] << 8) | macaddr[4]);
		*(&_eth->MACA0HR + addrNumber * 2) = addrHi;

		uint32_t addrLo = (macaddr[3] << 24) | (macaddr[2] << 16) | (macaddr[1] << 8) | macaddr[0];
		*(&_eth->MACA0LR + addrNumber * 2) = addrLo;
		if(addrNumber == 0)
			_txPool.SetSrcAddr(macaddr);
		return true;
	}
	
	void EthernetMac::InterruptHandler()
	{
		uint32_t dmasr = _eth->DMASR;
		_eth->DMASR |= dmasr & (ETH_DMASR_TS | ETH_DMASR_RS | ETH_DMASR_AIS || ETH_DMASR_NIS);
		_status = dmasr;
		
		_rx.SetReady();
	}
	
	void EthernetMac::SetSpeed(EthSpeed speed)
	{
		_speed = speed;
		CheckPhyCapability();
	}
	
	EthSpeed EthernetMac::GetSpeed()
	{
		return _speed;
	}
	
	void EthernetMac::SetDuplexMode(EthDuplexMode duplex)
	{
		_duplexMode = duplex;
	}
	
	EthDuplexMode EthernetMac::GetDuplexMode()
	{
		return _duplexMode;
	}
	
	void EthernetMac::EnableAutonegotiation(bool autonegotiation)
	{
		_autonegotiation = autonegotiation;
	}
	
	bool EthernetMac::AutonegotiationEnabled()
	{
		return _autonegotiation;
	}
	
	bool EthernetMac::IsLinked()
	{
		return _linked;
	}

	void EthernetMac::SetPhyAddress(uint8_t phyAddr)
	{
		_phyAddr = phyAddr;
	}
	
	void EthernetMac::AdjustClockRange()
	{
		uint32_t clockRange = 0;
		uint32_t hclk = Clock::AhbClock::ClockFreq();

		if(hclk >= 150000000)
		{
			clockRange = ETH_MACMIIAR_CR_Div102; 
		}else if(hclk >= 100000000)
		{
			clockRange = ETH_MACMIIAR_CR_Div62;
		}else if(hclk >= 60000000)
		{
			clockRange = ETH_MACMIIAR_CR_Div42;
		}else if(hclk >= 35000000)
		{
			clockRange = ETH_MACMIIAR_CR_Div26;
		}else
		{
			clockRange = ETH_MACMIIAR_CR_Div16;
		}
		_eth->MACMIIAR = (_eth->MACMIIAR & ~ETH_MACMIIAR_CR) | clockRange;
	}
	
	void EthernetMac::Reset()
	{
		_eth->DMABMR |= ETH_DMABMR_SR;
		while ((_eth->DMABMR & ETH_DMABMR_SR) != 0)
		{
		}
	}
	
	
	void EthernetMac::EnableClocks()
	{
		using namespace Clock;
		using namespace IO;
		
		EthRmiiPins::Enable();
		EthMacClock::Enable();
		EthMacTxClock::Enable();
		EthMacRxClock::Enable();
		EthMacPtpClock::Enable();
		SyscfgClock::Enable();
	}
	
	void EthernetMac::ConfigurePins()
	{
		using namespace IO;
		EthRmiiPins::SetConfiguration(NativePortBase::AltFunc);
		EthRmiiPins::SetSpeed(NativePortBase::Fastest);
		EthRmiiPins::SetPullUp(NativePortBase::NoPullUp);
		EthRmiiPins::SetDriverType(NativePortBase::PushPull);
		EthRmiiPins::AltFuncNumber(11);
	}
	
	void EthernetMac::SetMediaInterface(EthMediaInterface mediaInterface)
	{
		_mediaInterface = mediaInterface;
		Clock::SyscfgClock::Enable(); 
		SYSCFG->PMC = (SYSCFG->PMC & ~(SYSCFG_PMC_MII_RMII_SEL)) | (uint32_t)_mediaInterface;
	}
	
	void EthernetMac::ResetPhy()
	{
		if(!WritePhyRegister(PhyBasicControlRegister, PhyReset))
		{
			
		}
	}
	
	void EthernetMac::CheckPhyCapability()
	{
		if(Initialized())
		{
			uint16_t statusReg;
			ReadPhyRegister(PhyBasicStatusRegister, &statusReg);
			if(!(statusReg & PhyAutoNegotiateAbility))
			{
				_autonegotiation = false;
			}
			
			const uint16_t phy100MbitModes = 
				PhyHalfDuplex100BaseT2 |
				PhyFullDuplex100BaseT2 |
				PhyHalfDuplex100BaseTx |
				PhyFullDuplex100BaseTx |
				Phy100BaseT4;
			
			const uint16_t phyFullDuplex100Modes = 
				PhyFullDuplex100BaseT2 |
				PhyFullDuplex100BaseTx;
				
			if(!(statusReg & phy100MbitModes))
			{
				_speed = EthSpeed10Mbit;
			}
			
			if(_speed == EthSpeed100Mbit)
			{
				if(!(statusReg & phyFullDuplex100Modes))
				{
					_duplexMode = EthHalfDuplex;
				}
			}else
			{
				if(!(statusReg & PhyFullDuplex10BaseT))
				{
					_duplexMode = EthHalfDuplex;
				}
			}		}
	}
	
	void EthernetMac::PauseCommand(uint16_t time)
	{
		while(_eth->MACFCR & ETH_MACFCR_FCBBPA)
		{}
		_eth->MACFCR = ETH_MACFCR_PLT_Minus28 | ETH_MACFCR_RFCE | ETH_MACFCR_TFCE | ETH_MACFCR_FCBBPA | (uint32_t)(time << 16);
	}
	
	void EthernetMac::SetPhyParameters()
	{
		uint16_t phyControlReg = 0;
		if(_speed == EthSpeed100Mbit)
		{
			phyControlReg |= PhySpeed100m;
		}
		if(_duplexMode == EthFullDuplex)
		{
			phyControlReg |= PhyFullDuplex;
		}
		if(_autonegotiation)
		{
			phyControlReg |= PhyAutonegotiation;
		}
		WritePhyRegister(PhyBasicControlRegister, phyControlReg);
	}
	
	void EthernetMac::StartAutonegotiation()
	{
		if(_autonegotiation)
		{
			ModifyPhyRegister(PhyBasicControlRegister, PhyRestartAutonegotiation, 0);
		}
	}
	
	void EthernetMac::SetMacParameters()
	{
		uint32_t clearMask = ETH_MACCR_WD | ETH_MACCR_JD | ETH_MACCR_IFG | ETH_MACCR_CSD |
							ETH_MACCR_CSD | ETH_MACCR_FES | ETH_MACCR_ROD | ETH_MACCR_LM |
							ETH_MACCR_DM | ETH_MACCR_IPCO | ETH_MACCR_RD | ETH_MACCR_APCS |
							ETH_MACCR_BL | ETH_MACCR_DC | ETH_MACCR_TE | ETH_MACCR_RE;
							
		uint32_t crValue = ETH_MACCR_IFG_64Bit | _speed | _duplexMode | ETH_MACCR_TE | ETH_MACCR_RE;
		_eth->MACCR = (_eth->MACCR & ~clearMask) | crValue;
	}

	void EthernetMac::Init()
	{
		EnableClocks();
		ConfigurePins();
		SetMediaInterface(_mediaInterface);
		Reset();
		AdjustClockRange();
		ResetPhy();
		_state = EthOk;
		CheckPhyCapability();
		SetPhyParameters();
		StartAutonegotiation();
		SetDmaParameters();
		SetMacParameters();
	}
	
	void EthernetMac::SetDmaParameters()
	{
		//_eth->DMABMR = ETH_DMABMR_DA | 
		_eth->DMAIER = ETH_DMAIER_TIE | ETH_DMAIER_TPSIE | ETH_DMAIER_TBUIE | ETH_DMAIER_TJTIE |
					ETH_DMAIER_ROIE | ETH_DMAIER_TUIE | ETH_DMAIER_RIE | ETH_DMAIER_RBUIE |
					ETH_DMAIER_RPSIE | ETH_DMAIER_RWTIE | ETH_DMAIER_ETIE | ETH_DMAIER_FBEIE |
					ETH_DMAIER_ERIE  | ETH_DMAIER_NISE | ETH_DMAIER_AISE;
		NVIC_EnableIRQ(ETH_IRQn);
		_rx.Reset();
		_rx.SetBuffer(_rxBuffer, sizeof(_rxBuffer));
		_rx.ChainNext(&_rx);
		_rx.SetReady();
		
		_eth->DMATDLAR = (uint32_t)_txPool.StartOfList();
		_eth->DMARDLAR = (uint32_t)&_rx;
		_eth->DMAOMR |= ETH_DMAOMR_ST | ETH_DMAOMR_SR;
	}
	
	bool EthernetMac::WritePhyRegister(uint16_t phyReg, uint16_t regValue)
	{
		while(_eth->MACMIIAR & ETH_MACMIIAR_MB)
		{ }
		
		_eth->MACMIIDR = regValue;
		_eth->MACMIIAR = (_eth->MACMIIAR & ETH_MACMIIAR_CR) |
			ETH_MACMIIAR_MW |
			ETH_MACMIIAR_MB |
			(((uint32_t)phyReg & 0x1f) << 6) |
			(((uint32_t)_phyAddr & 0x1f) << 11);

		while(_eth->MACMIIAR & ETH_MACMIIAR_MB)
		{ }
		return true; 
	}
	
	bool EthernetMac::ReadPhyRegister(uint16_t phyReg, uint16_t *regValue)
	{
		_eth->MACMIIAR = (_eth->MACMIIAR & ETH_MACMIIAR_CR) |
			ETH_MACMIIAR_MB |
			(((uint32_t)phyReg & 0x1f) << 6) |
			(((uint32_t)_phyAddr & 0x1f) << 11);
		
		while(_eth->MACMIIAR & ETH_MACMIIAR_MB)
		{ }

		*regValue = (uint16_t)(_eth->MACMIIDR);
		return true;
	}
	
	bool EthernetMac::ModifyPhyRegister(uint16_t phyReg, uint16_t setMask, uint16_t clearMask)
	{
		uint16_t value;
		if(!ReadPhyRegister(phyReg, &value))
			return false;
		if(!WritePhyRegister(phyReg, (value & ~clearMask) | setMask ))
			return false;
		return true;
	}
	
	void EthernetMac::Poll()
	{
		uint16_t statusReg = 0;
		ReadPhyRegister(PhyBasicStatusRegister, &statusReg);
		if(!_linked && (statusReg & PhyLinkedStatus))
		{
			_linked = true;
			//ReadLinkParameters();
			SetMacParameters();
		}
		if(_linked && !(statusReg & PhyLinkedStatus))
		{
			_linked = false;
		}
		if(_linked)
		{
			//InvokeCallbacks();
			_txPool.ClenUp();
			ETH->DMATPDR = 1;
		}
	}

}
