#include "stdafx.h"
#include "SendToMail.h"
#include "../Public/Singleton.h"
#include "../Public/STLFun.h"

#import "../jmail.dll"

bool  SendToMail::Send( size_t index )
{

	size_t tsize = GetInstObj(MailLoginInfo).m_Vec.size();
	if ( tsize <= m_SendID )
	{
		return false ;
	}				  
	
	tagSend		m_Send     = GetInstObj(MailLoginInfo).m_Vec[ m_SendID ];

	tagSendInfo& m_SendInfo = GetInstObj(tagSendInfo);

	CoInitialize(NULL); 	// COM�ĳ�ʼ��

	try{

		jmail::IMessagePtr pMessage("JMail.Message");

		// ����������
		pMessage->From = m_Send.login.c_str();

		// ����������
		pMessage->FromName = m_Send.name.c_str() ;

		// ����ռ���1, ���ռ���������PGP KEY

		for ( std::list<std::string>::iterator itr = m_list.begin() ; itr != m_list.end() ; itr ++ )
		{
			 pMessage->AddRecipient((*itr).c_str(),"friends","");	
		}
		
		pMessage->MailServerUserName=m_Send.user.c_str();

		pMessage->MailServerPassWord=m_Send.pword.c_str();

		//pMessage->PutServerPort(465);

		// ���ȼ�����,1-5��ν���, 3Ϊ�м�
		pMessage->Priority = 3;

		pMessage->Charset = "GB2312";

		pMessage->Subject = "this istest!";//m_SendInfo.subject.c_str();

		pMessage->ContentType =  m_SendInfo.Formt ? "text/html " : "text/plain ";

		// ����
		pMessage->Body = m_SendInfo.Context.c_str();	

		pMessage->Send(m_Send.smtp.c_str(),VARIANT_FALSE);
		//pMessage->Send("smtp.qq.com",VARIANT_FALSE);
	}
	catch(_com_error e)
	{
		CString strerr;
		strerr.Format("�����:%0x ������Ϣ:%s����������%s,���͵�ַ:%s",e.Error(),(LPCTSTR)e.ErrorMessage(), (LPCTSTR)e.Description(),m_Send.login.c_str());
		WriteFile("error.txt",strerr.GetBuffer(strerr.GetLength()));

		//Sleep( 10 );
	    return false;
	}
	//CoUninitialize();	
	//Sleep( 1 );
	return true;
}

/*
//pMessage->AddNativeHeader("Date", "2010-05-10");

//pMessage->AddNativeHeader("From", "tangpeng@126.com");

//pMessage->AddHeader("Mailer", "Expter 1.0");		  	// �ʼ�����������
//pMessage->AddNativeHeader("Disposition-Notification-To", "52311940@qq.com");

// ��Ӹ���
//pMessage->AddAttachment("C:\\CONFIG.SYS", VARIANT_FALSE, "application/octet-stream");
//pMessage->AddAttachment("C:\\test.gif", VARIANT_TRUE, "image/gif");

*/