/*
*	File  :
*   Brief :	 һ�������������࣬����iocp�������װ�ӿ�
			 һ�������࣬�ṩһ��������Ϣ����ӿ�run
			 �ṩ����������������ӵĹ���........
*   Author:  Expter 
*	Creat Date:  [2009/12/4]
*/

#pragma once

#include "PacketManager.h"
#include "AcceptManager.h"
#include "Connection.h"
#include "iocp.h"
#include "MsgQueue.h"

class CServer :
	public CNetwork ,
	public CThread 
{
public:
	CServer(void)	{};
	~CServer(void)	{};

public:
	/*
	 *			ip����
	 */
	void		GetServerNameAndIP(  char * host , char *ip );

	/*
	 *			��ʼ��������������ɶ˿�
	 */
	void		Read( int MaxConnection		 = MAX_CONNECTION,	 SOCK_TYPE Type = GENERAL_TYPE ,
					  int SockSendBufferSize = SEND_BUFFER_SIZE, bool bUseKeepAliveOption = TRUE,
					  bool bUseDosDefenceOption = true);

	/*
	 *			�����������˿�ӳ�䣬�����˿ڰ�
	 */
	void		Start( char * ServerName , int ServerPort );

	/*
	 *			��������
	 */
	void		Stop();

	/*
	 *			�������ӽӿڣ����سɹ�ʧ��
	 */
	bool		Connect(CConnection* pCon,char* ip, int port, long uid = CONNECTOR_UID);

	/*
	 *			�ر�����
	 */
	bool		DisConnect( CConnection *pCon );


	PACKETBUFFER_INTERFACE	* GetBuffer();

	/*
	 *			�ͷ���Դ
	 */
	void		ReleaseBuffer( PACKETBUFFER_INTERFACE * pPacketBuffer );

	bool		ReleaseConnection( CConnection * pCon);
	
	bool		ReleaseClient(CConnection* pCon) ;

	/*
	 *			����ÿ�����ӵĻ��������߳�
	 */
	unsigned	long  _thread_func();

	/*
	 *			����ÿ�����ӵ�δ��������
	 */
	void		SendProcess(  long	NowTime );

	/*
	 *			�Ƿ����ddos��������Ҫ��ͳ��ÿ�����ӷ�������
	 */
	bool		DefenceDosAttack( CConnection *pCon);

	void		PacketProcess(PACKETBUFFER_INTERFACE* pPacketBuffer);


	/*
	 *			��Ϣ������
	 */
	virtual		void	AccedeProcess( CConnection *pCon , CMessage *pPacket)=0;

	/*
	 *			��Ϣ����ѭ������ȡ����������Ϣ
	 */

	virtual		void run() = 0;

protected:
	CIOCP				 m_Iocp;
	CAcceptManager		 m_AcceptManager;
	CMsgQueue			 m_MsgQueue;				// ���յ������ݰ�����
	sync::csectionlock   m_Section;
};
