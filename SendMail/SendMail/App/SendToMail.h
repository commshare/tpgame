

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
	std::string  stmp;		/// smtp
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
	tagSendInfo		m_SendInfo;
};

struct   MailLoginInfo
{
public:
	std::vector<tagMailBase>  m_Vec;
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