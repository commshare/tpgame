// ClientTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "ClientMsg.h"
#include <iostream>
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{

	int port ;
	
	std::cin >> port ;
	CClientMsg client;
	client.Init();
	client.Read( 2000 , GENERAL_TYPE , BUFFER_SIZE , TRUE , TRUE );

	client.ConnectCacheServer( port , "10.34.64.57", 5100 ) ;

	client.run()  ;

	client.Stop() ;
	return 0;
}

