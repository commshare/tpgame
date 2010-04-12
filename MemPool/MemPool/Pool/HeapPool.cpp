#include "stdafx.h"
#include "HeapPool.h"


HeapPool::HeapPool()
{
	ReleaseAll();
}

HeapPool::~HeapPool()
{
	ReleaseAll();
}

void  HeapPool::ReleaseAll()
{
//#ifdef  _DEBUG
	std::cout <<"\n\n�ѷ�����Ϣ: "<<std::endl;
//#endif
	MHIter it ( m_HeapTable.begin() );

	for ( ; it != m_HeapTable.end() ; it++ )
	{

		HeapNode* pNode = it->second;
		if( !pNode ) continue;

//#ifdef  _DEBUG
		std::cout <<" Count = "<<pNode->m_Count <<" Size " << it->first <<" M_FreeList size:"<<pNode->m_FreeList.GetSize() <<" m_Used Size:"<<pNode->m_Used.GetSize() <<std::endl;
//#endif
		/// �ͷſ��е�
		while ( pNode->m_FreeList.GetSize() > 0)
		{
			listNode* node = pNode->m_FreeList.GetHead();
			pNode->m_FreeList.RemoveNode( node );
			delete [] node->ptr ;
			delete node;
		}
		/// �ͷ�ʹ���е�
		while ( pNode->m_Used.GetSize() > 0)
		{
			listNode* node = pNode->m_Used.GetHead();
			pNode->m_Used.RemoveNode( node );
			delete [] node->ptr ;
			delete node;
		}
		delete pNode;
	}
}

char*  HeapPool::Alloc(unsigned long size)
{
	MHIter it = m_HeapTable.find( size );
	/// �ҵ�
	if ( it != m_HeapTable.end() )
	{
		HeapNode* pNode = it->second;
		/// �ҵ����п��е�
		while ( pNode->m_FreeList.GetSize() > 0)
		{
			listNode* node = pNode->m_FreeList.GetHead();
			pNode->m_FreeList.RemoveNode( node );
			pNode->m_Used.PushNode( node );
			pNode->m_Count ++;
			return  node->ptr+8;
		}
		//std::cout <<" ���㣬�M���ٴη��� ";
	}
	/// δ�ҵ���Ҫ����
	return MemorySize( size );
}

bool    HeapPool::Free(void* MemAddr , unsigned long size )
{
	/*offsetof(listNode,ptr)*/
	__int64   pAddr = *(__int64*)((char*)MemAddr-8);
	listNode* pNode =  (listNode*)pAddr;

	if( !pNode && pNode->size != size) return false;

	MHIter it = m_HeapTable.find( size );
	/// �ҵ�
	if ( it != m_HeapTable.end() )
	{
		it->second->m_Used.RemoveNode( pNode );
		it->second->m_FreeList.PushNode( pNode );

		Clear( it->second );
	}
	else
	{
#ifdef  _DEBUG
		std::cout <<" HeapPool Free size "<< size <<" is error " << std::endl;
#endif
		return false;
	}
	return true;
}

bool   HeapPool::Free(void* MemAddr)
{
	__int64   pAddr = *(__int64*)((char*)MemAddr-8);
	listNode* pNode =  (listNode*)pAddr;
	if( !pNode ) return false;
	MHIter it = m_HeapTable.find( pNode->size );
	/// �ҵ�
	if ( it != m_HeapTable.end() )
	{
		it->second->m_Used.RemoveNode( pNode );
		it->second->m_FreeList.PushNode( pNode );

		Clear( it->second );
	}
	else
	{
#ifdef  _DEBUG
		std::cout <<" HeapPool Free size "<< pNode->size <<" is error " << std::endl;
#endif
		return false;
	}
	return true;
}