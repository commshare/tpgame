

#pragma once 

#include "RasDial.h"
#include "SendToMail.h"


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

	/// ����
	MyRasDial*						m_RasDial;
	/// �̳߳�
	tp_ipc_peer_namespace::ctpool*  m_TPool;
};