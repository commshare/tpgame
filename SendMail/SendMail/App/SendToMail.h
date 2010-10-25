

#pragma  once 

#include <string>
#include <list>

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

class  SendToMail
{

public:
	SendToMail(tagSend msend,tagSendInfo msendinfo):m_Send(msend),m_SendInfo(msendinfo)
	{
		m_list.clear();
	}

	void  AppendUser(std::list<std::string>& List)
	{
		m_list = List;
	}

	bool  Send();


public:

	/// ��ǰ�����˺���Ϣ
	tagSend			m_Send;
	tagSendInfo		m_SendInfo;

	/// ������ϵ��
	std::list<std::string>  m_list;

};