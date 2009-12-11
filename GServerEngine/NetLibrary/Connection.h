/*
*	File  :	 Connection.h
*   Brief :	 �̳�Socket,��ʾÿһ�������ӣ���Ҫ����ÿ���׽������ݰ����߼��ϵĲ����������������
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/
#pragma once

#include "ssocket.h"
#include "..\Public\locks.h"
#include "..\Public\CList.h"
#include "..\Public\BaseNode.h"

class CConnection :
	public CSocket,
	public CBaseNode<CConnection> 
{	

public:
	/*
	 *	 ��ʼ�������캯��
	 */
	CConnection(void):CSocket(){	}

	CConnection(SOCK_TYPE Type , int BufSize) : CSocket( Type , BufSize )	{	}

	void	Init( SOCKADDR_IN * addr , SOCKET sock );

	/*
	 *		Get���� 
	 */
	char*	GetIP() { return m_ip; }

	LPVOID	GetClientInfo() { return m_ClientInfo;}

	long	GetPacketRecvTime() { return m_RecvTime; }

	/*
	 *		����һ�������������ݰ�
	 */
	char*   GetSamplePacket(long RestBytes,long* PacketSize,long* ErrorCode);

	/*
	 *		���ò��� 
	 */
	void	SetClientInfo(LPVOID Info) { m_ClientInfo = Info;}

	void	SetUID(long ID)			   { m_uid = ID;	   	 }

	long	GetUID()				   { return m_uid;		 }

	void	SetWait()				   { m_State = false;	 }
 
	void	SetActive()				   { m_State = true;	 }

	void	SetPacketRecvTime(long time){ m_RecvTime = time;}

	/*
	 *		���Ĳ��� 
	 */
	bool	QesActive()				   { return		m_State; }


	/*
	 *		���ͺͽ��ղ���
	 */
	void	SendPacket(CMessage* packet,bool bUseNegleAlgorithm = false);

	void	RecvPacket();

	/*
	 *		�ͷŲ���
	 */
	virtual	~CConnection(void);

	void	Release();

private:
	char	m_ip[16];				/// ��ǰ�׽��ֵ�ip	
	long	m_uid;					/// uid
	bool	m_State;				/// ״̬
	BYTE	m_DeleyCount;	
	LPVOID	m_ClientInfo;
	long	m_RecvTime;
};