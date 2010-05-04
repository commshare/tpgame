/*============================================
*
* file:   DataBlockAllocator.h
*
* brief:  �̶���С�ڴ���ٷ�������װ�ֻ࣬�ṩ��ʼ�� �� Add , Get�ӿ�
*
* Authtor:Expter
*
* Data:   20090818
*
* Mend:   //20090818
===========================================*/
#pragma  once 

#include "stdafx.h"
#include "..\Datablock\DataBlockAllocator.h"
#include "..\Datablock\DataBlockPool.h"
#include "..\Datablock\DataBlockSetWriteRead.h"

namespace _tp_message_load
{
	class CBassMessage
	{
		struct  stMsg 
		{
			unsigned long lSize;		   /// ��Ϣ��С
			unsigned long eType;		   /// ��Ϣ����
			unsigned long lVerifyCode;     /// У����
		};

	public:
		///
		/// ���캯��
		///
		CBassMessage(){
			pool = new CDataBlockPool(2048); 
		}
		~CBassMessage(){}

		///
		/// �������� 
		///

		void writeinit()
		{
			DBWriteSet(SetWriteDB);
			/// DBReadSet(SetReadDB);
		}

		void readinit()
		{
			DBReadSet(SetReadDB);
		}
	public:
		bool DBWriteSet( CDataBlockSetWrite & DBWrite)
		{
			DBWrite.DBWriteSet(pWritePam,pool,pDataSet);
			return true;
		}
		bool DBReadSet (DataBlockSetRead & DataBlocks)
		{
			DataBlocks.DBReadSet(pWritePam,pool,pDataSet);
			return true;
		}

		void print()
		{
			SetWriteDB.PutAllInfo();
		}
	public:
		/// �������
		void AddtoStrLenByteArray(void * pSource, long n = 0)
		{
			SetWriteDB.AddtoByteArray(pSource,n);
		}
		void AddtoStrByteArray(const char * pStr)
		{
			SetWriteDB.AddtoByteArray(pStr);
		}
		template<typename obj>
		void AddtoByteArray(obj   l)
		{
			SetWriteDB.AddtoByteArray(l);
		}


		/// ȡ����
		void *GetBufferFromByteArray(void *Buf,long lLen)
		{
			return SetReadDB.GetBufferFromByteArray(Buf,lLen);
		}

		char *GetBufferFromByteArray(char *Buf,long lLen)
		{
			return SetReadDB.GetBufferFromByteArray(Buf,lLen);
		}

		template<typename type>
		inline type GetDataFromByteArray()
		{
			return SetReadDB.GetObjDataFromByteArray<type>();
		}

		/// �ڴ����Ҫ������
	private:
		CDataBlockPool						  *pool ; 
		vector<CDataBlockPool*>			      pDataSet;
		tagDBOpParam						  pWritePam;


		/// ��д���� ���ݿ�
	private:

		CDataBlockSetWrite SetWriteDB;     /// д���ݿ� 
		DataBlockSetRead   SetReadDB;	   /// �����ݿ�
	};
}


/************************************************************************
_tp_message_load::CBassMessage  base;
base.writeinit();

/// д����
char  str[50] = "this is only test ! ";
char  tmp[101];

for(int  i = 0 ; i < sizeof(tmp) ;  i++)
tmp[i] = '1';
tmp[100] = '\0';
base.AddtoByteArray(10);
base.AddtoStrByteArray(str);
base.AddtoStrByteArray(tmp);
base.AddtoByteArray(30);

//	base.print();

int  lNum = 0;
base.readinit();
lNum = base.GetDataFromByteArray<long>();
char * p1 = base.GetBufferFromByteArray(str,256);
char * p2 = base.GetBufferFromByteArray(tmp,10010);
int  lNum1 = base.GetDataFromByteArray<long>();

printf("lNum = %d\n str = %s\n tmp = %s\n  lNum1 = %d",lNum,p1,p2,lNum1);
***********************************************************************/