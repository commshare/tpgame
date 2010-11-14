

#pragma  once 

#include <list>
#include <vector>
#include <map>
#include "../Public/log.h"
#include "Thread.h"

static CLog logFail("log\\Fail.txt");
static CLog logSuce("log\\suce.txt");

extern tp_ipc_peer_namespace::ctpool*  g_CurrThread;
class  SocketSendToMail
{

public:
	SocketSendToMail(size_t size )
	{
		m_SendID = size;
	}

	void  AppendUser(std::list<std::string>& List)
	{
		m_list = List;
	}
	 
	bool  operator() ( )
	{
		return Send( m_SendID );
	}

private:
	bool  Send( size_t index );


private:
	bool  CreateSocket()
	{
		try
		{
			return m_Socket.Create() > 0 ? true : false ;
		}
		catch (...)
		{
			long eid = GetLastError();
			logFail <<" Create Fail Errid= " << eid  <<"\r\n";
			return false;
		}
		
	}

	void  ReleaseSocket()
	{
		m_Socket.Close();
	}

	bool  CheckResponse(std::string RecvCode,char* Buf )
	{
		try
		{
			m_Socket.Receive( Buf, 1024 );

			if ( RecvCode[0] == Buf[0] &&
				RecvCode[1] == Buf[1] &&
				RecvCode[2] == Buf[2] )
			{
				return true;
			}

			static long num = 0;
			logFail <<	Buf ;
			if ( g_CurrThread && strstr( Buf,"554") )
				num ++;
			else if ( g_CurrThread && strstr( Buf,"553") )
				num ++;

			if ( num > 20 )
			{
				num = 0;
				g_CurrThread->RasStates();
				logFail <<" ���²�������!!\r\n";
			}
			return false;
		}
		catch(...)
		{
			return false;
		}
	}

	bool  CheckAccount(std::string ip,std::string name,std::string pass);

	/// ���͵��Ż�
	bool  SendYahoo(std::string& tmp,std::string & context);

	bool  SendOther(std::string& tmp,std::string & context);

	/// tom
	bool  SendTom(std::string& tmp,std::string & context);

public:

	/// ��ϵID 
	size_t		    m_SendID;
	/// �׽���
	CSocket			m_Socket;
	/// ������ϵ��
	std::list<std::string>  m_list;
};