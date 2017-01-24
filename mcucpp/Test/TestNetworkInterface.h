
#pragma once

#include <net/NetInterface.h>
#include <vector>

namespace Mcucpp
{
namespace Test
{
	class TestNetworkInterface :public Net::NetInterface
	{
	public:
		Net::MacAddr _macaddr;
		Net::NetBuffer LastTransmittedFrame;
		unsigned txSequence;
		
		TestNetworkInterface()
			:txSequence(0)
		{}
		
		virtual bool SetMacAddress(unsigned addrNumber, const Net::MacAddr &macaddr)
		{
			if(addrNumber == 0)
			{
				_macaddr = macaddr;
				return true;
			}
			return false;
		}
		
		
		virtual unsigned MaxAddresses(){ return 1;}
		
		virtual const Net::MacAddr& GetMacAddress(unsigned addrNumber)
		{	
			return _macaddr;
		}
		
		
		virtual bool IsLinked() {return true;}
		
		virtual void PauseCommand(uint16_t time){(void)time;}
		virtual void Poll()
		{
			
		}
		
		virtual uint32_t GetParameter(Net::NetInterfaceParameter parameterId)
		{
			(void)parameterId;
			return 0;
		}

		virtual Net::TransferId Transmit(const Net::MacAddr &destAddr, uint16_t protocoId, Net::NetBuffer &buffer)
		{
			LastTransmittedFrame = buffer;
			LastTransmittedFrame.InsertFront(6+6+2);
			LastTransmittedFrame.Seek(0);
			LastTransmittedFrame.WriteMac(destAddr);
			LastTransmittedFrame.WriteMac(_macaddr);
			LastTransmittedFrame.WriteU16Be(protocoId);
			LastTransmittedFrame.Seek(0);
			
			return ++txSequence;
		}
		
		virtual bool TxCompleteFor(Net::TransferId txId)
		{
			return true;
		}
		
	};
	
	
}}