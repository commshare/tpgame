

#pragma once 

#include "RasDial.h"

namespace tp_ipc_peer_namespace
{
	class ctpool;
}
//class  tp_ipc_peer_namespace::ctpool;						   

class  SendMailApp
{

public:
	SendMailApp();

	~SendMailApp();

	void  Start();

	bool  IsSendMail();

private:

	void  InitPool(tp_ipc_peer_namespace::ctpool* pool,std::list<std::string>& RsSendList);

private:

	/// ����
	MyRasDial*						m_RasDial;
	/// �̳߳�
	tp_ipc_peer_namespace::ctpool*  m_TPool1;
	tp_ipc_peer_namespace::ctpool*  m_TPool2;
};