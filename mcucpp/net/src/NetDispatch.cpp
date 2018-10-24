
#include <net/NetDispatch.h>

using namespace Mcucpp;
using namespace Mcucpp::Net;


void NetDispatch::TxComplete(TransferId txId, bool success)
{
	(void)txId;
	(void)success;
}

void NetDispatch::RxComplete(const Net::MacAddr &srcAddr, const Net::MacAddr &destAddr, uint16_t protocoId, DataBuffer &buffer)
{	
	for(unsigned i = 0; i < _protocols.size(); i++)
	{
		if(_protocols[i].id == protocoId)
		{
			_protocols[i].protocol->ProcessMessage(srcAddr, destAddr, buffer);
			break;
		}
	}
}

bool NetDispatch::SendMesage(const Net::MacAddr &destAddr, uint16_t protocoId, DataBuffer &buffer)
{
	for(unsigned i = 0; i < _interfaces.size(); i++)
	{
		_interfaces[i]->Transmit(destAddr, protocoId, buffer);
	}
	// TODO: handle
	return true;
}

NetDispatch::NetDispatch(Dispatcher &dispatcher)
	:_dispatcher(dispatcher)
{
	
}

void NetDispatch::AddInterface(NetInterface *interface)
{
	_interfaces.push_back(interface);
}

void NetDispatch::AddProtocol(uint16_t protocoId, INetProtocol *protocol)
{
	_protocols.push_back(ProtocolIdPair(protocol, protocoId));
}

void NetDispatch::Poll()
{
	for(unsigned i = 0; i < _interfaces.size(); i++)
	{
		_interfaces[i]->Poll();
	}
}