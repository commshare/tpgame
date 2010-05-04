// WordFilter.cpp : �������̨Ӧ�ó������ڵ㡣
//

/*
   File :  WordFilter.cpp 
   brief:  �ؼ��ֹ��˳��򣬸��Ӷ�ΪO(n)������
   Author: Expter
   Data  : 2009/06/30

   �Ժ��ֻ����ַ����й�ϣ�㷨����ת��Ϊunicode���룬Ȼ������hashֵ��

   ��Ҫ�㷨Ϊ:
   1.����2�����˹ؼ�������:����1��Ϊ�����ַ�  ����2��Ϊ2�����߶���ַ�
   2.�������1��2��hashֵ������2��hashֵֻ���ǰ2���ַ���hashֵ���ɡ�
   3.ɨ��������ı���Ȼ��ÿ��ȡ2���ַ�����������2�Ƿ���ƥ�䣬���û�����������1��������  ����ΪO(1)


   ����:
   ���ܺܺõķִʡ����˲��Ǻ�׼ȷ��ÿ��ֻ��1��2���ʵĹ��ˡ�
*/
#include "stdafx.h"
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <windows.h>
#include <wchar.h>  
#include <iosfwd>
using namespace std;

wchar_t des1 [5][2] = { L"��",L"��",L"��",L"��",L"��"};
wchar_t des2 [3][5] = { L"�ú�", L"��ɶ" ,L"���԰�"};
wchar_t src[]  = { L"���ԭ���Ǵ����ɶ�Ӷ����ú���ֻ��һ���ǲ��ǲ���"};


/// ���ֵĹ�ϣֵ
long HashFun(wchar_t  word)
{
	BYTE l = LOBYTE(word);
	int  h = HIBYTE(word);

	long num = h << 8 ;
	num +=l;
	return num;
}

long HashFun(wchar_t * word)
{
	return HashFun(word[0]) + HashFun(word[1]);
}


void  ParamVer(map<long,int> hashmp , wchar_t *src , int i)
{
	long val = HashFun(src[i+1]);
	if(hashmp[val] == 1)
	{
		src[i+1] = L'*';
	}
}
void  VmAlorgthm(map<long,int> hashmp,wchar_t *src)
{
	long val = 0;
	int  m = wcslen(src) ;
	// O(n);
	for(int i = 0 ; i < m-1  ; i ++)
	{
		if( HashFun(src[i]) != L'*')
		{
			val = HashFun(src[i]) + HashFun(src[i+1]);
			if( hashmp[val] == 1)
			{
				src[i] = L'*';
				src[i+1] =L'*';
			}
			else
			{
				val = HashFun(src[i]);
				if(hashmp[val] == 1)
				{
					src[i] = L'*';
				}
				else
				{
					ParamVer(hashmp,src,i);
				}
			}
		}
		else
		{
			ParamVer(hashmp,src,i+1);
		}
	}
	ParamVer(hashmp,src,m-1);
}

template<class T1, class T2>
void ReadBufFile( T2 t2,const char * filename)
{
	T1 t1(filename);
	if (t1.fail())
	{
		return ;
	}
	string str;
	while( !t1.eof() )
	{
		t1 >> str;
		t1.push_back(str);
	}
	t1.close();
}



void Alorithm2( )
{

}


int _tmain(int argc, _TCHAR* argv[])
{
	wcout.imbue(locale("chs"));     
	typedef map<long,int> HASHMAP;

	cout <<" ��Ҫ�����ı�: ";
	wcout<< src <<endl;
	cout <<" ���˹ؼ��� : " ;
	for(int i = 0 ;i < 5; i++)
		wcout << des1[i][0] <<" ";
	wcout <<endl;
	cout <<" ���˹ؼ��� : " ;
	for(int i = 0 ;i < 3; i++)
		wcout << des2[i] <<" ";
	wcout <<endl;
   
    vector<string>  vword;
	//ReadBufFile<ifstream,vector<string> >(vword,"1.txt"); 
	long  val = 0;

	HASHMAP hash_map;
	/// �� hash
	for(int i = 0 ; i < 5 ; i++)
	{
		val = HashFun(des1[i][0]);
		hash_map[val] = 1;
	}
	/// �� hash
	for(int i =0 ; i < 3 ; i++)
	{
		val = HashFun(des2[i]);
		hash_map[val] = 1;
	}
	
	VmAlorgthm(hash_map,src);
	
	cout <<"\n-------------------------------------------------------------\n"
		<<" ���˺���ı�: ";
	wcout<< src <<endl;

	return 0;
}


