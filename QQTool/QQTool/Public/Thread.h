/**
 * Brief :  ����һ���̻߳���
 * Author:  Expter  
 * Date  :  [2009-11-1]                           
 */

#pragma  once

#include <windows.h>
#include <process.h>

class CThread
{

protected:
	bool     bThreadCreate;
	HANDLE   hThread;

public:
	/*
	 *   �߳̿�ʼ	
	 */
	virtual void _start_thread(void)
	{
		bThreadCreate = true;

		hThread = (HANDLE)_beginthreadex( NULL , 0 , _thread_proc , this  , 0 , 0 );

	}

	/*
	 *	��ֹ�߳�	
	 */
	virtual void _stop_thread(void)
	{
		unsigned long cDestroy;
		bThreadCreate = false; 
		for ( ;; )
		{	 
			GetExitCodeThread( hThread , &cDestroy );
			if ( cDestroy != STILL_ACTIVE )   break;
		}

		CloseHandle(  hThread );

		_endthreadex( NULL );
	}

	/*
	 *	����߳�����״̬	
	 */
	virtual bool _check_thread_live(void)	{	return bThreadCreate; }

	/*
	 *	�̵߳��ú���
	 */
	static  unsigned int __stdcall WINAPI  _thread_proc( LPVOID pVoid)
	{
		CThread *pThis = (CThread*)pVoid;

		return  pThis->_thread_func() ;
	}

	/*
	 *	�̴߳�����
	 */
	virtual unsigned long  _thread_func() = 0 ;

};
