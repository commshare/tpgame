/** 
 *  file   list.h
 *  brief  ���öѷ���ķ�װ��,����ѷ����ͬ��С�ڴ����HeapNode����
 *         ÿһ��HeapNode������2��list,ÿһ��list�����ж����㣬
		   ÿһ���ڵ��ptrָ��̬������ڴ��С��ptrָ���ǰ8���ֽ�Ϊÿ���ڵ�ĵ�ֵַ��
		   �����ͷŵ�ʱ��ֱ�Ӵ����ַ���Եõ��ڵ�ָ��.
		   ���Ӷ�:�ͷţ����� O(1)
 *  author Expter
 *  date   2010/01/12
 */
#pragma  once 

#include "BaseNode.h"
#include "List.h"
#include "locks.h"
#include <list>
#include <map>	  
#include <iostream>
using namespace std;

class  HeapPool
{

	struct  listNode : CNode<listNode>
	{
		long  size;
		/// ָ����ڴ�
		char* ptr;
	};
	struct  HeapNode
	{	
		/// memory request rate
		__int64			  m_Count;
		/// all free memory list
		TlinkedList<listNode> m_FreeList;
		/// all used memory list
		TlinkedList<listNode> m_Used;
		HeapNode( )
		{
			m_Count = 0 ;
			m_FreeList.ReleaseList();
			m_Used.ReleaseList();
		}
	};

	typedef std::map<unsigned long,HeapNode* >	MHEAPLIST;
	typedef MHEAPLIST::iterator					MHIter;

private:
	MHEAPLIST				m_HeapTable;

public:

	HeapPool();
	~HeapPool();

	///  �����ڴ����
	char* Alloc( unsigned long size );

	///   �ͷ��ڴ�
	bool  Free( void*  MemAddr);

	///   �ͷ��ڴ棬����С��
	bool  Free( void*  MemAddr , unsigned long size );

	///   ���������Դ 
	void  ReleaseAll();

private:
	
	/// �ڴ����
	char*  MemorySize( unsigned long size )
	{
		MHIter it = m_HeapTable.find( size );
		/// �ҵ�ӳ���
		if ( it != m_HeapTable.end() )
		{
			listNode* node = new listNode;
			node->size = size;
			node->ptr  = new char[ size + 8];
			it->second->m_Used.PushNode( node );
			it->second->m_Count ++ ;
			memcpy(node->ptr ,&node , 8 );  // ǰ8����ַָ��Ĺ��c��ַ
			return node->ptr+8;
		}
		else
		{
			HeapNode* pHeapNode = new HeapNode;
			listNode* node = new listNode;
			node->size = size;
			node->ptr  = new char[ size + 8];
			memcpy(node->ptr ,&node , 8 );
			pHeapNode->m_Used.PushNode( node );
			pHeapNode->m_Count ++ ;
			m_HeapTable.insert( std::pair<long,HeapNode*>(size,pHeapNode) );
			return node->ptr+8;
		}
		return NULL;
	}

	/// �������
	void   Clear( HeapNode* Heap )
	{
		long  FreeSize = Heap->m_FreeList.GetSize();
		long  UsedSize = Heap->m_Used.GetSize();

		if (  UsedSize <= FreeSize || FreeSize>= 100 )
		{
			for ( long i = 0 ; i < FreeSize/2 ; i++ )
			{
				listNode* node = Heap->m_FreeList.GetHead();
				Heap->m_FreeList.RemoveNode( node );
				delete node->ptr;
				delete node;
			}
		}
	}
};