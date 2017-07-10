
#include <ethernet.h>
#include <compiler.h>
#include <iopins.h>
#include <MemPool.h>
#include <ioreg.h>
#include <enum.h>

const unsigned MaxEthHeaders = 6;
const unsigned MaxEthFrames = 4;

using namespace Mcucpp;
using namespace Mcucpp::Net;


namespace Mcucpp
{
	EthernetMac ethernet(ETH);
	
	EthernetMac::EthernetMac(ETH_TypeDef *eth)
		:_eth(eth),
		_mediaInterface(EthRmii),
		_phyAddr(0x01),
		_autonegotiation(true),
		_speed(EthSpeed100Mbit),
		_duplexMode(EthFullDuplex),
		_state(EthNotInitialized),
		_framesSend(0),
		_framesRecived(0),
		_sendErrors(0),
		_reciveErrors(0),
		_txSequence(0)
	{

	}
	
	const Net::MacAddr& EthernetMac::GetMacAddress(unsigned addrNumber)
	{
		(void)addrNumber;
		return _macaddr;
	}
	
	uint32_t EthernetMac::GetParameter(Net::NetInterfaceParameter parameterId)
	{
		switch(parameterId)
		{
			case NetIfFramesSend:        return _framesSend; 
			case NetIfFramesRecived:     return _framesRecived;
			case NetIfFramesAppMissed:   return _framesAppMissed;
			case NetIfFramesMacMissed:   return _framesMacMissed;
			case NetIfSendErrors:        return _sendErrors;
			case NetIfReciveErrors:      return _reciveErrors;
			case NetIfLinked:            return _linked;
			case NetIfLinkSpeedKbps:     return _speed == EthSpeed100Mbit ? 100000 : 10000;
			case NetIfHwState:           return _state;
			case NetIfSupportHwCheckSum: return 1;
		}
		return 0;
	}
	
	bool EthernetMac::TxCompleteFor(Net::TransferId txId)
	{
		// for(unsigned i = 0; i < Net::EthTxPool::DescriptorCount; i++)
		// {
			// Net::EthTxPool::TxDescriptorWithBufferPointer &descr = _txPool.Descriptors[i];
			// if(txId == descr.seqNumber)
				// return false;
		// }
		return true;
	}
	
	Net::TransferId EthernetMac::Transmit(const Net::MacAddr &destAddr, uint16_t protocoId, Net::NetBuffer &buffer)
	{
		if(!IsLinked())
		{
			_state = EthNotConnected;
			return 0;
		}
		const size_t EthernetHeaderSize = 6+6+2;
		
		if(!buffer.InsertFront(EthernetHeaderSize))
		{
			_state = EthOutOfMem;
			return 0;
		}
		buffer.Seek(0);
		
		buffer.WriteMac(destAddr);
		buffer.WriteMac(_macaddr);
		
		buffer.WriteU16Be(protocoId);
	
		if(++_txSequence == 0)
			_txSequence = 0;
		
		bool res = _rxTxQueue.EnqueueBuffer(buffer, _txSequence);
		_eth->DMATPDR = 1;
		if(!res)
		{
			DataBuffer::Release(buffer.DetachFront());
			_state = EthTxQueueFull;
			_sendErrors++;
			return 0;
		}
		_state = EthOk;
		return _txSequence;
	}
	
	bool EthernetMac::SetMacAddress(unsigned addrNumber, const Net::MacAddr &macaddr)
	{
		if(addrNumber >= MaxAddresses())
		{
			return 0;
		}
		uint32_t addrHi = ((macaddr[5] << 8) | macaddr[4]);
		*(&_eth->MACA0HR + addrNumber * 2) = addrHi;

		uint32_t addrLo = (macaddr[3] << 24) | (macaddr[2] << 16) | (macaddr[1] << 8) | macaddr[0];
		*(&_eth->MACA0LR + addrNumber * 2) = addrLo;
		if(addrNumber == 0)
			_macaddr = macaddr;
		return true;
	}
	
	
	
	void EthernetMac::InterruptHandler()
	{
		uint32_t dmasr = _eth->DMASR;
		_eth->DMASR |= dmasr & (ETH_DMASR_TS | ETH_DMASR_RS | ETH_DMASR_AIS || ETH_DMASR_NIS);
		_status = dmasr;
		
		uint32_t missedFramesReg = _eth->DMAMFBOCR;
		_framesAppMissed += (missedFramesReg >> 17) & 0x07ff;
		_framesMacMissed += (missedFramesReg >> 0)  & 0x7fff;
		
		EthMacState state = _rxTxQueue.ProcessInterrupt();
		if(state != EthOk)
			_state = state;
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
			}
		}
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
		_eth->MACFFR |= ETH_MACFFR_RA;
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
		//SetDmaParameters();
		//SetMacParameters();
	}
	
	void EthernetMac::SetDmaParameters()
	{
		const unsigned descriptorExtraDWords = _rxTxQueue.ExtraDescriptorDWords();
		
		_eth->DMAOMR |= ETH_DMAOMR_FTF; // Flush Tx FIFO
		
		_eth->DMABMR = (_eth->DMABMR & ~ETH_DMABMR_DSL) | (descriptorExtraDWords << 2); // skip N words after descriptor
		
		_eth->DMAIER = ETH_DMAIER_TIE | ETH_DMAIER_TPSIE | ETH_DMAIER_TBUIE | ETH_DMAIER_TJTIE |
					ETH_DMAIER_ROIE | ETH_DMAIER_TUIE | ETH_DMAIER_RIE | ETH_DMAIER_RBUIE |
					ETH_DMAIER_RPSIE | ETH_DMAIER_RWTIE | ETH_DMAIER_ETIE | ETH_DMAIER_FBEIE |
					ETH_DMAIER_ERIE  | ETH_DMAIER_NISE | ETH_DMAIER_AISE;
		
		EthMacState state = _rxTxQueue.PrepareQueues();
		if(state != EthOk)
		{
			_state = state;
			return;
		}
		
		uint32_t timeout = 10000;
		while((_eth->DMAOMR & ETH_DMAOMR_FTF) && --timeout)
		{
			
		}
		
		if(!timeout)
		{
			_state = EthDriverError; 
			return;
		}
		
		_eth->DMATDLAR = _rxTxQueue.TxDescriptorListStartAddr();
		_eth->DMARDLAR = _rxTxQueue.RxDescriptorListStartAddr();
		_eth->DMAOMR |= ETH_DMAOMR_ST | ETH_DMAOMR_SR;
		NVIC_EnableIRQ(ETH_IRQn);
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
	
	void EthernetMac::ReadLinkParameters()
	{
		
	}
	
	void EthernetMac::Poll()
	{
		uint16_t statusReg = 0;
		ReadPhyRegister(PhyBasicStatusRegister, &statusReg);
		if(!_linked && (statusReg & PhyLinkedStatus))
		{
			_linked = true;
			ReadLinkParameters();
			SetDmaParameters();
			SetMacParameters();
		}
		if(_linked && !(statusReg & PhyLinkedStatus))
		{
			_linked = false;
			
		}
		if(_linked)
		{
			_rxTxQueue.InvokeCallbacks(_dispatch);
			_eth->DMATPDR = 1;
			_eth->DMARPDR = 1;
		}
	}
	
}


extern "C" MCUCPP_INTERRUPT(ETH_IRQHandler)
{
	ethernet.InterruptHandler();
	IO::Pd13::Toggle();
}

extern "C" MCUCPP_INTERRUPT(ETH_WKUP_IRQHandler)
{
	IO::Pd13::Toggle();
}

