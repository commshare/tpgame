

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
	byte		 file;
	std::string  fileaddr;  
	std::string  Context;   /// ��������
};

struct  tagMailBase
{							   	
	/// ��ǰ�����˺���Ϣ
	tagSend			m_Send;
};

struct  MailLoginInfo
{
	std::vector<tagSend>  m_Vec;
	long				  Num1;
	long				  Num2;
};

struct  DestList
{
	/// �����б�
	std::list<std::string>    m_SendListOne;
	std::list<std::string>    m_SendListTwo;
};

struct  tagGlobalSetup
{
	/// ȫ��������Ϣ
	
	/// ��������
	long         ThreadNum;
	long		 SendInter;
	long		 BoTime;
	long		 SendNum;
	long		 SendMax;
	long		 SameDomainMax;

	/// �������
	std::string   BroadName;
	std::string   Account;
	std::string   PassWord;

	/// ������Ϣ
	std::string   SendAddrList;
	std::string	  RecvAddrList;
	std::string   SendRole;
	std::string	  MailContext;
	std::string   MailSub;
	std::string   SmtpIni;
	
};

struct   MailSubNode
{
	std::string   sMailSub;
	long		  lReNum;
};

struct   tagMailSub 
{
	vector<MailSubNode>  VecMailSub;
};

struct   tagSendRole
{
	long		 NoChangeRand;	///> ά��ԭ״���ı�������
	long		 Rand1;			///> ���1���ַ�������
	long		 Rand2;			///> ���2���ַ�������
	long		 Rand3;			///> ���3���ַ�������

	vector<std::string>  sReplace;
};

struct	tagSendAddrList
{
	std::string			SendList;
	std::vector<string>	KeyValue;
};

struct	SendAddrList
{
	 tagSendAddrList	SendListOne;
	 tagSendAddrList	SendListTwo; 
};

struct	tagSmtpList 
{
	long    value;
	string  index;
	string  smtp;
};

struct	SmtpList 
{
	std::vector<tagSmtpList>	tSmtpList;
};