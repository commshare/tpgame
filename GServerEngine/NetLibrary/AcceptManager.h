/*
*	File  :  AcceptManager.h
*   Brief :  ��Accept����,�����û������Accept����
			 ��������OO�����˼��,����ϣ������
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/

#pragma once

#include "Accept.h"

class CAcceptManager
{
	CAccept  Accept;

public:
	CAcceptManager(void) {};

	~CAcceptManager(void){};

	/*
	 *  �󶨶˿�
	 */
	void   CreateAccept( int Port )
	{
		Accept._socket_bind( Port );
		Accept._start_thread();
	};

	/*
	 *  �رն˿�
	 */
	void   CloseAccept()	{	Accept._destroy_accept() ; }
};
