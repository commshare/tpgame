

#pragma  once 

#include <list>
#include <vector>
#include <map>
#include "../Public/log.h"

static CLog logFail("log\\Fail.txt");
static CLog logSuce("log\\suce.txt");

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

			logFail <<	Buf ;
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

public:

	/// ��ϵID 
	size_t		    m_SendID;
	/// �׽���
	CSocket			m_Socket;
	/// ������ϵ��
	std::list<std::string>  m_list;
};