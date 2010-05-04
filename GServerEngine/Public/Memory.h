/*
*	File  :	Memory.h
*   Brief :  һ����Զ����ڴ�صĹ�����Ҫ��ͨ�������������ڴ档
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/

#pragma once

#include "List.h"
#include "locks.h"

template < class obj , long  ObjSize>
class CMemory  : public TLinkedList<obj>
{
public:

	///
	///  Brief:���캯����ʼ�� 
	///
	CMemory(void)	{	MemoryArray = NULL ; AllocSize = 0 ; }	

	~CMemory(void)	{	}

	///
	///  Brief: ����������
	/// 
	int		GetAllocMemorySize()	{	return AllocSize ; } 

	int		GetFreeMemoryNodeSize() {	return GetSize();  }

	obj *	GetMemoryNode(int index){	return MemoryArray + index; }

	///  Brief: �ڴ����
	bool	MemoryAlloc( int size ) 
	{
		AllocSize  = size;

		MemoryArray= (obj*)VirtualAlloc( NULL , sizeof(obj)*size , MEM_COMMIT , PAGE_READWRITE );
		if(	MemoryArray == NULL )	return false;

		for ( int i = 0 ; i < size ; i++ )
		{
			AddNode( MemoryArray + i );
		}
		return true;
	}

	void	MemoryRelease()
	{
		sync::scope_guard  sguid( m_cection );

		if ( MemoryArray)
		{
			ReleaseList();
			VirtualFree( MemoryArray , 0 , MEM_RELEASE );
		}

		MemoryArray = NULL;
		AllocSize	= 0;
	}

	obj * GetNewMemoryNode()
	{	
		sync::scope_guard  sguid( m_cection );

		obj * node = GetHead();
		if ( !node)
		{
			return NULL;
		}

		RemoveNode( node );

		return node ;
	}

	bool  AllocNodeMemory( obj * node)
	{
		if ( !node )	return false;

		sync::scope_guard  sguid( m_cection );
	
		RemoveNode( node );
		
		return true;
	}

	bool  ReleaseNodeMemory( obj * node)
	{
		if ( !node )	return false;

		sync::scope_guard  sguid( m_cection );

		AddNode( node );

		return true;
	}

private:
	obj * MemoryArray;
	int	  AllocSize;
	sync::csectionlock  m_cection;

};
