#include "stdafx.h"
#include "SendMailApp.h"
#include "../Public/Singleton.h"
#include "../Public/STLFun.h"

#define  SAFE_DELETE( P ) { if(P!=NULL) { delete P; P=NULL;}}

SendMailApp::SendMailApp()
{
	m_RasDial = new MyRasDial("test","test","�������");
}

SendMailApp::~SendMailApp()
{
	SAFE_DELETE(m_RasDial);
	SAFE_DELETE(m_TPool);
}

bool  SendMailApp::IsSendMail()
{
	return m_TPool->IsExit();
}


void  SendMailApp::Start()
{
	if ( m_RasDial->Listen() )
	{
		m_TPool = new tp_ipc_peer_namespace::ctpool( 1 );
		
		std::list<std::string>& RsSendList = GetInstObj(DestList).m_SendList;

		/// ���͵�ַ����
		long  DestCount = RsSendList.size();
		/// �����ʺ���Ϣ
		long  AccountCount = GetInstObj( MailLoginInfo ).m_Vec.size();
		/// ÿ�η��͵���ϵ������
		const long  MTime = 5;
		size_t  SendNum = DestCount / MTime + DestCount%MTime > 0 ? 1 : 0;
		/// ��ϵ������
		long  LinkNum = 0 ;

		
		for (size_t i = 0 ; i < SendNum ; i ++  )
		{
			SendToMail*  mail = new SendToMail( LinkNum % AccountCount );

			std::list<std::string>  tmpList;

			ListCopy( RsSendList.begin(),RsSendList.end(),tmpList.begin(),MTime );

			///ÿ���ʺŷ�������
			mail->AppendUser( tmpList );

			//tp_ipc_peer_namespace::task<SendToMail> *task =  new tp_ipc_peer_namespace::task<SendToMail>( mail );

			m_TPool->push<SendToMail>( mail );

			AccountCount ++ ;
		}
		
		m_TPool->Start();

		/// �����ʼ�
		for ( ; !m_TPool->IsExit() ; )
		{
			
		}
	}

	m_RasDial->Stop();
}

