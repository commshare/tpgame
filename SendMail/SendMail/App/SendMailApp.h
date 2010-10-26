

#pragma once 

#include "RasDial.h"
#include "Thread.h"
#include "SendToMail.h"


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
	/// һ���ٽ�����
	sync::csectionlock			    m_Sendlock_;
	/// һ���ٽ�����
	sync::csectionlock			    m_SendTolock_;
	

};