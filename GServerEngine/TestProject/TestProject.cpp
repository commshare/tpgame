// TestProject.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ServerMsg.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	CServerMsg  Server;

	Server.Read( 2000 , GENERAL_TYPE , BUFFER_SIZE , TRUE , TRUE );

	Server.Start("TestServer",5100);

	Server.run() ;

	Server.Stop();

	return 0;
}

