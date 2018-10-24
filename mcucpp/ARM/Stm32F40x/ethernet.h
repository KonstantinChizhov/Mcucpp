
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
#include <net/net_addr.h>
#include <net/NetInterface.h>
#include <mac_descriptors.h>
#include <EtherRxTxQueue.h>
#include <MacEnums.h>

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
	
	class EthernetMac :public Net::NetInterface
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
		
		void EnableClocks();
		void ConfigurePins();
		void AdjustClockRange();
		void CheckPhyCapability();
		void SetPhyParameters();
		void ReadLinkParameters();
		void SetMacParameters();
		void SetDmaParameters();
		void StartAutonegotiation();
		bool ReadPhyRegister(uint16_t phyReg, uint16_t *regValue);
		bool WritePhyRegister(uint16_t phyReg, uint16_t regValue);
		bool ModifyPhyRegister(uint16_t phyReg, uint16_t setMask, uint16_t clearMask);
		void ResetPhy();
		void Reset();
		void InterruptHandler();
		void InvokeCallbacks();
		
		ETH_TypeDef *_eth;
		EthMediaInterface _mediaInterface;
		uint8_t _phyAddr;
		bool _autonegotiation;
		EthSpeed _speed;
		EthDuplexMode _duplexMode;
		bool _linked;
		EthMacState _state;
		Net::EtherRxTxQueue _rxTxQueue;
		
		Net::MacAddr _macaddr;
		uint32_t _framesSend;
		uint32_t _framesRecived;
		uint32_t _sendErrors;
		uint32_t _reciveErrors;
		uint32_t _framesAppMissed;
		uint32_t _framesMacMissed;
		uint32_t _status;
		Net::TransferId _txSequence;
		
	public:
		EthernetMac(ETH_TypeDef *eth);
		void Init();
		
		void EnableAutonegotiation(bool autonegotiation);
		bool AutonegotiationEnabled();
		void SetSpeed(EthSpeed speed);
		EthSpeed GetSpeed();
		void SetDuplexMode(EthDuplexMode duplex);
		EthDuplexMode GetDuplexMode();
		void SetMediaInterface(EthMediaInterface mediaInterface);
		void SetPhyAddress(uint8_t phyAddr);
		bool Initialized(){return _state != EthNotInitialized;}

	public: // NetInterface members
		virtual bool SetMacAddress(unsigned addrNumber, const Net::MacAddr &macaddr);
		virtual unsigned MaxAddresses(){return 4;}
		virtual const Net::MacAddr& GetMacAddress(unsigned addrNumber);
		virtual Net::TransferId Transmit(const Net::MacAddr &destAddr, uint16_t protocoId, DataBuffer &buffer);
		virtual bool IsLinked();
		virtual void PauseCommand(uint16_t time);
		virtual void Poll();
		virtual uint32_t GetParameter(Net::NetInterfaceParameter parameterId);
		virtual bool TxCompleteFor(Net::TransferId txId);
	};
	//==================================================================
	extern EthernetMac ethernet;
	//==================================================================

}
