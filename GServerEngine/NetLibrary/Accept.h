/*
*	File  :  Accept.h
*   Brief :	 һ��accept�࣬���̹߳���
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/
#pragma once

#include "Network.h"
#include "..\Public\Thread.h"

class CAccept :
	public CNetwork ,
	public CThread
{

public:
	CAccept(void) {};

	~CAccept(void){};

public:
	/*
	 *	brief:���׽���
	 */
	bool			_socket_bind( int port );

	/*
	*   brief:��������������
	*/
	unsigned long	_thread_func();

	/*
	 *	brief:�ر�����
	 */
	void			_destroy_accept();
	
private:
	/*
	 *   brief:��Ա����
	 */
	SOCKET			m_Sockaccept;		/// �󶨵��׽���
	SOCKADDR_IN		m_SockAddr;			/// �˿���Ϣ
	HANDLE			m_hThread;			/// �����߳̾��
};
