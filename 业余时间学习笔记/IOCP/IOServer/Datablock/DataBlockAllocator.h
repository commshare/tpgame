
/*============================================
*
* file:   DataBlockAllocator.h
*
* brief:  �̶���С�ڴ���ٷ�����
*
* Authtor:Expter
*
* Data:   20090616
*
* Mend:   //20090918
===========================================*/



#pragma once

#include <list>
#include <map>
#include <windows.h>
using namespace std;

class CDataBlockPool;


/// һ�����ݿ����ϸ��Ϣ
struct tagDBOpParam
{
	tagDBOpParam()
	{
		memset(this,0,sizeof(tagDBOpParam));
	}
	/// ��ǰ��д���ݿ���
	long  nCurNum;
	/// ��ǰ��д���ݿ�Ĵ�С
	long  nCurDBSize;
	/// ��ǰ��д���ݵ�λ��
	long  nCurPos;
	/// ��ǰ��д���ݿ��ָ��
	BYTE* pDBPtr;

};

class CDataBlockAllocator
{
public:
	CDataBlockAllocator(void);
	CDataBlockAllocator(int nDBNum,int nDBSize,int bIniShareDB = false );

public:
	~CDataBlockAllocator(void);


private:
	typedef list<CDataBlockPool*>  FreeDataBlocks;
	typedef list<CDataBlockPool*>::iterator itFreeDB;

	FreeDataBlocks				   m_FreeDataBlocks;
	CRITICAL_SECTION			   m_CSFreeDB;
	int							   m_nMaxDBNum;
	int							   m_nDBSize;

	/// �����߼�����ʱʹ�õ����ݿ�
	/// �������࣬ʹ�ýϴ�����ݿ���������ִ�С������

	typedef multimap<long,BYTE*>  MMapShareDB;
	typedef pair <long,BYTE*>     SHareDBPair;
	typedef multimap<long,BYTE*>::iterator itShareDB;
	MMapShareDB					  m_ShareDataBlocks;

	typedef map<BYTE*,long>       mapAllocedRecord;
	typedef map<BYTE*,long>::iterator itAllocR;
	mapAllocedRecord              m_AllocedShareDB;


	/// �������������ݿ����
	map<long,long> m_MapTest;

private:
	void Initial(int bIniShareDB = false);
	void Release();

public:
	CDataBlockPool* AllocDB(long lTestFlag = 0);
	void FreeDB(CDataBlockPool *);

	BYTE * AllockShareDb(long lSize);
	void FreeShareDB(BYTE* pData);

	void PutAllocInfo(const char * pszFileName);

};
