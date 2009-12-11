/*
*	File  :
*   Brief :	 �����ڴ�صķ�ʽ����ÿһ��������׽���
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/

#pragma once

#include "Connection.h"
#include "..\Public\KeepAlive.h"
#include "..\Public\Memory.h"

class CConnectionManager	
{
public:
	CConnectionManager(void);

	~CConnectionManager(void);

public:

	/*
	 *	 �������Ӷ���ռ�
	 */
	bool	InitConnectionManager(int Size,SOCK_TYPE Type,int SendBufferSize,bool bUseKeepAlive,long KeepAliveTimer);

	/*
	 *	 �õ����������
	 */
	int		GetMaxConnectionCount()		{ return m_MaxCount;					}
	
	/*
	 *	 �õ�������
	 */
	int		GetActiveConnectionCount()	{ return m_ActiveConnectCount;			}

	/*
	 *	 �õ�����������صĴ�С
	 */
	int		GetConnectionPoolCount()	{ return m_ConnectionPool.GetSize();	}

	/*
	 *	 ����index�Ķ���
	 */
	CConnection*	GetConnection(int index); 

	/*
	 *	 ����һ�����Ӷ���
	 */
	CConnection*	GetNewConnection(); 

	/*
	 *	 �ͷ�ĳ������
	 */
	bool			ReleaseConnection(CConnection* pConnection); 

private:

	sync::csectionlock		m_Section;

	int						m_ActiveConnectCount ;

	int						m_MaxCount;

	CMemory<CConnection>	m_ConnectionPool;

	BYTE					*m_ConnectionAccount;

};
