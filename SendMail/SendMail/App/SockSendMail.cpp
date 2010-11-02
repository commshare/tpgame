#include "stdafx.h"
#include "SockSendMail.h"
#include "SendToMail.h"
#include "../Public/Singleton.h"
#include "../Public/STLFun.h"
#include "../Public/Base.h"

bool  SocketSendToMail::Send( size_t index )
{
	size_t tsize = GetInstObj(MailLoginInfo).m_Vec.size();
	if ( tsize <= m_SendID )
	{
		return false ;
	}				  

	tagSend		m_Send     = GetInstObj(MailLoginInfo).m_Vec[ m_SendID ];

	tagSendInfo& m_SendInfo = GetInstObj(tagSendInfo);

	if( !CreateSocket() )
		return false;

	if( !m_Socket.Connect(m_Send.smtp.c_str(), 25 ) )
	{
		m_Socket.Close();
		return FALSE;
	}

	if( !CheckResponse( "220" ) )
	{
		m_Socket.Close();
		return false;
	}

	if (!CheckAccount(m_Send.smtp,m_Send.user,m_Send.pword) )
	{
		ReleaseSocket();
		return false;
	}

	//// ��֤ͨ����������

	if(m_list.size()<=0) return false;

	string strTmp;

	//����MAIL FROM:<abc@xyz.com>
	strTmp="MAIL FROM:<"+m_Send.login+">\r\n";
	if(m_Socket.Send(strTmp.c_str(),strTmp.length()) == SOCKET_ERROR)
	{
		int elen = WSAGetLastError();
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("250")) return false;

	//����RCPT To:<abc@xyz.com>
	for ( std::list<std::string>::iterator itr = m_list.begin() ; itr != m_list.end() ; itr ++ )
	{
		strTmp="RCPT To:friends<"+(*itr)+">\r\n";
		if(m_Socket.Send(strTmp.c_str(),strTmp.length()) == SOCKET_ERROR)
		{
			int elen = WSAGetLastError();
			ReleaseSocket();
			return false;
		}
		if(!CheckResponse("250")) return false;
	}

	//����"DATA\r\n"
	if(m_Socket.Send("DATA\r\n",strlen("DATA\r\n")) == SOCKET_ERROR)
	{
		int elen = WSAGetLastError();
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("354")) return false;

	//"Mail From:SenderName<xxx@mail.com>\r\n"
	strTmp="From:<"+m_Send.login+">\r\n";//"+m_Send.name+"

	strTmp+="Sender:"+m_Send.name+"\r\n";

	//"Subject: �ʼ�����\r\n"
	strTmp+="Subject:"+m_SendInfo.subject+"\r\n";

	//"MIME_Version:1.0\r\n"
	strTmp+="MIME_Version:1.0\r\n";

	//	//"X-Mailer:Smtp Client By xxx"//��Ȩ��Ϣ
	strTmp+="X-Mailer: Microsoft Outlook Express\r\n\r\n";

	if(m_Socket.Send(strTmp.c_str(),strTmp.length() ) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;	
	}

	//�ʼ�����
	strTmp="--";
	strTmp+="boundary";
	strTmp+="\r\n";
	strTmp+="Content-type:text/html;Charset=gb2312\r\n";
	strTmp+="Content-Transfer-Encoding:8bit\r\n\r\n";

	//�ʼ�����
	strTmp+=m_SendInfo.Context+"\r\n\r\n";

	//���ʼ����ݷ��ͳ�ȥ
	if(m_Socket.Send(strTmp.c_str(),strTmp.length() ) == SOCKET_ERROR)
	{
		ReleaseSocket();

		return false;	
	}

	//��β
	strTmp="--";
	strTmp+="boundary";
	strTmp+="--\r\n.\r\n";

	//���ʼ����ݷ��ͳ�ȥ
	if(m_Socket.Send(strTmp.c_str(),strTmp.length() ) == SOCKET_ERROR)
	{
		ReleaseSocket();

		return false;	
	}
	if(!CheckResponse("250")) return false;

	if(m_Socket.Send("QUIT\r\n",strlen("QUIT\r\n") ) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}

	if(!CheckResponse("221")) return false;

	ReleaseSocket();
	return true;
}

bool  SocketSendToMail::CheckAccount(std::string ip,std::string name,std::string pass)
{
	CBase base64;

	//�����������"HELO "+��������
	string strTmp="HELO Smtp\r\n";
	if(m_Socket.Send( strTmp.c_str(),strTmp.length() )  == SOCKET_ERROR)	
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("250")) return false;

	if(m_Socket.Send("AUTH LOGIN\r\n",strlen("AUTH LOGIN\r\n")) == SOCKET_ERROR)
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("334")) return false;

	//���;�base64������û���
	string strUserName=base64.Encode((unsigned char *)name.c_str(),name.length())+"\r\n";

	if( m_Socket.Send(strUserName.c_str(),strUserName.length() ) ==  SOCKET_ERROR) 
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("334")) return false;

	//���;�base64���������
	string strPassword=base64.Encode((unsigned char *)pass.c_str(),pass.length())+"\r\n";

	if( m_Socket.Send(strPassword.c_str(),strPassword.length() ) ==  SOCKET_ERROR) 
	{
		ReleaseSocket();
		return false;
	}
	if(!CheckResponse("235")) return false;
	return true;
}

