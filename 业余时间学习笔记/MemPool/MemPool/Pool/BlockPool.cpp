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

char*  BlockPool::Alloc( unsigned long size )
{
	Buffer* pBuf = NULL;
	long   idx = GetIndex( size );
	if ( idx >=0 )
	{
		 pBuf = m_MemPool[idx].GetHead();
		 if( pBuf != NULL )
		 {
			 m_MemPool[idx].RemoveNode(pBuf);
			 return ((Buffer_Face*)pBuf)->buf ;
		 }
		 /// �ڴ�������������
		 else
		 {
#ifdef  _DEBUG
			std::cout << m_MemPool[idx].GetSize() <<":new Alloc" ;
#endif
			if( Init( idx , (eBuff_Type) idx ) )
				return Alloc( size );
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
//#ifdef  _DEBUG
	std::cout <<"\n\nջ������Ϣ: "<<std::endl;
//#endif

	long long  SizeSum = 0;
	for ( int i = 0 ; i < eBT_END ; i++ )
	{
		  m_MemPool[i].ReleaseList();
//#ifdef  _DEBUG
		  std::cout <<" ��Դ�ͷ� " << typeid(m_listPool[i]).name() <<" ���ڴ� " << m_listPool[i].size() <<std::endl;
//#endif		  
		  for ( std::list<Buffer*>::iterator it = m_listPool[i].begin(); it != m_listPool[i].end(); it ++ )
		  {
			   //std::cout <<" ��Դ�ͷ�it " << typeid(*it).name() << std::endl;
			   VirtualFree(*it,0,MEM_RELEASE);
			   SizeSum   += 20;
		  }  
	}

	std::cout <<"BlockPool ���ڴ� " << SizeSum << " M "<< std::endl;
}