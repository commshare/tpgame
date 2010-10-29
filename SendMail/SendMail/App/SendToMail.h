

#pragma  once 

#include <string>
#include <list>
#include <vector>

using namespace std;

struct  tagSend
{
	std::string  login;		/// �˺�
	std::string  name;		/// ����
	std::string  user;		/// ��¼user
	std::string  pword;		/// ��¼����
	std::string  smtp;		/// smtp

	tagSend( string login_,string name_,string user_,string pword_,string smtp_)
	{
		login = login_;
		name  = name_;
		user  = user_;
		pword = pword_;
		smtp  = smtp_;
	}
};

struct  tagSendInfo
{
	byte		 Formt;		/// ��ʽ���Ƿ��ǣȣԣͣ�
	std::string  subject;   /// ����
	std::string  Context;   /// ��������
};

struct  tagMailBase
{							   	
	/// ��ǰ�����˺���Ϣ
	tagSend			m_Send;
};

struct   MailLoginInfo
{
public:
	std::vector<tagSend>  m_Vec;
};

struct  DestList
{
	std::list<std::string>    m_SendList;
};

class  SendToMail
{

public:
	SendToMail(size_t size )
	{
		m_list.clear();
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

public:

	/// ��ϵID 
	size_t		    m_SendID;
	/// ������ϵ��
	std::list<std::string>  m_list;

};