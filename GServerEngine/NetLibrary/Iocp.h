/*
*	File  :	 iocp.h
*   Brief :	 ��ɶ˿ڹ����߳�
*   Author:  Expter 
*	Creat Date:  [2009/11/24]
*/

#pragma once

#include <process.h>

#include "Network.h"
#include "..\Public\locks.h"
#include "..\Public\CList.h"
#include "..\Public\Memory.h"

#define MAX_WORKER_COUNT	8
#define WORKER_DESTROY_KEY	1 


class CIOCP: public CNetwork
{

private:

	void StartWorkerThread();
	void DestroyWorker();

public:
	CIOCP()			 {};
	
	virtual ~CIOCP() {};

	/*
	 *		��ʼiocp
	 */
	void	StartIOCP();	

	/*
	 *		ֹͣiocp
	 */
	void	StopIOCP();

	/*
	 *		���ù����¼�
	 */
	void	SetWorkerEvent();

	/*
	 *		���Ӵ���
	 */
	void	ConnectionError(CConnection* pCon);

	/*
	 *		��ɶ˿ڹ����߳�
	 */
	static unsigned int __stdcall WINAPI _work_thread_(LPVOID pVoid);

private:
	sync::csectionlock   m_Scection;
	int					 m_WorkerNum;
	int					 m_HandleIndex;
	HANDLE				 m_hEvent[10];
};