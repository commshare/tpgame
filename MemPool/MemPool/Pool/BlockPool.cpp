#include "stdafx.h"
#include "BlockPool.h"
#include <algorithm>

BlockPool::BlockPool()
{
	//m_MemPool.clear();
}

BlockPool::~BlockPool()
{
	ReleaseAll() ;
}


void  BlockPool::InitMemoryAlloc()
{
	for ( int idx = 0 ; idx < eBT_END ; idx ++ )
	{
		Init( idx , (eBuff_Type) idx );
	}
}

void*  BlockPool::Alloc( unsigned long size )
{
	Buffer* pBuf = NULL;
	long   idx = GetIndex( size );
	if ( idx >=0 )
	{
		 pBuf = m_MemPool[idx].GetHead();
		 if( pBuf != NULL )
		 {
			 m_MemPool[idx].RemoveNode(pBuf);
			 return ((Buffer_Face*)pBuf)->buf;
		 }
		 /// �ڴ�������������
		 else
		 {
			std::cout <<" �������ڴ���� " ;
			Init( idx , (eBuff_Type) idx );   
		 }
	}

	return NULL;
}

bool BlockPool::Free(void *Mem)
{
	if( !Mem ) return false;

	/// ���ƫ�Ƶ�ַ
	Buffer* buf = (Buffer_Face*)( (char*)Mem - offsetof(Buffer_Face,buf) );
	
	if ( !buf) return false;
	
	m_MemPool[ (long)buf->type ].PushNode(buf);
	  
	return true;
}

void   BlockPool::ReleaseAll()
{
	for ( int i = 0 ; i < eBT_END ; i++ )
	{
		  m_MemPool[i].ReleaseList();
		  std::cout <<" ��Դ�ͷ� " << typeid(m_listPool[i]).name();
		  for ( std::list<Buffer*>::iterator it = m_listPool[i].begin(); it != m_listPool[i].end(); it ++ )
		  {
			   /// std::cout <<" ��Դ�ͷ�it " << typeid(*it).name() << std::endl;
			   VirtualFree(*it,0,MEM_RELEASE);
		  }  
	}
}