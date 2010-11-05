#include "stdafx.h"
#include "SendMailApp.h"
#include "SendToMail.h"
#include "SockSendMail.h"
#include "Thread.h"
#include "../Public/BaseDef.h"
#include "../Public/Singleton.h"
#include "../Public/STLFun.h"
#include "../Public/GameSetup.h"


SendMailApp::SendMailApp()
{
	m_RasDial = new MyRasDial("test","test","�������");

	GetInstObj(CGameSetup).Load();
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
	if ( 1 /*m_RasDial && m_RasDial->Listen()*/ )
	{
		m_TPool = new tp_ipc_peer_namespace::ctpool( GetInstObj( tagGlobalSetup).ThreadNum );
		
		std::list<std::string>& RsSendList = GetInstObj(DestList).m_SendList;

		/// ���͵�ַ����
		long  DestCount = RsSendList.size();
		/// �����ʺ���Ϣ
		long  AccountCount = GetInstObj( MailLoginInfo ).m_Vec.size();
		/// ÿ�η��͵���ϵ������
		const long  MTime = GetInstObj( tagGlobalSetup).SendNum ;

		size_t  SendNum = (DestCount / MTime) + (DestCount % MTime > 0 ? 1 : 0) ;
		
		/// ��ϵ������
		long  LinkNum = 0 ;
	
		std::list<std::string>::iterator BeginItr = RsSendList.begin();

		for (size_t i = 0 ; i < SendNum ; i ++  )
		{
			//SendToMail*  mail = new SendToMail( LinkNum % AccountCount );
			SocketSendToMail*  mail = new SocketSendToMail( LinkNum % AccountCount );

			std::list<std::string>  tmpList;

			ListCopy( BeginItr, RsSendList.end(),tmpList,MTime );

			///ÿ���ʺŷ�������
			mail->AppendUser( tmpList );

			//m_TPool->push<SendToMail>( mail );
			m_TPool->push<SocketSendToMail> ( mail );

			LinkNum ++ ;
		}
		
		m_TPool->Start();

		/// �����ʼ�
		for ( ;  ; )
		{
			bool Exit = m_TPool->IsExit();
			if ( Exit )
			{
				break;
			}
		}
	}

//	m_RasDial->Stop();
}

