// view.cpp : �������̨Ӧ�ó������ڵ㡣
//

/// ����
/// 1.���
/// 2.fun���Ǿ�̬��������Ĭ�ϵ�this����
#include "stdafx.h"
#include <iostream>
using namespace std;

int  (* fun)(int * p );

struct  tagStr 
{
	int  spfun( int  size,void (*fun)(int* p) , int* p)
	{
		return spfun( size, fun , p );
	}
	
	void  fun(int *p)
	{
		static  int arr[10000];
		int     arr2[10000];
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	tagStr   tag;
	tag.spfun( 20000, tag.fun, NULL) ;
	return 0;
}

