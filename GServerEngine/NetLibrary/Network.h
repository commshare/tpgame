/*
*	File  :	 Network.h
*   Brief :  �����ʼ����
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/

#pragma once

#include "LibObject.h"

#define  MAX_CONNECTION		1800
#define  KEEPALIVE_TIMER	10000
#define  CONNECTIONMANAGER  CNetwork::ConnectionManager

class CNetwork :
	public CLibObject
{
	
	/*
	 *   Brief:��Ա������Ϊ��ɶ˿ڵ�ȫ�־��
	 */
	static  HANDLE	 hCompletionPort;
public:
	
	/*
	 *   Brief: �����ʼ��������WSAStartup
	 */
	bool	NetworkInit();

	/*
 	 *   Brief:��������ɶ˿ڡ�
	 */
	void	CreateCompletionPortHandle();

	/*
	 *   Brief:  ������ɶ˿ھ����
	 */
	static  HANDLE	GetCompletionHandle()	{ return  hCompletionPort ;}

	/*
	 *   Brief: �����ӵ���ɶ˿���
	 */
	static  bool	SetCompletionPort( SOCKET  sock , CConnection * pCon);


 	CNetwork(void)			{	};

 	virtual ~CNetwork(void) {	};
};
