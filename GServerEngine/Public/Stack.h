/*
*	File  :  Stack.h
*   Brief :  һ��֧���ٽ����Ķ�ջ��
*   Author:  Expter 
*	Creat Date:  [2009/11/2]
*/

#include "locks.h"
#include "CList.h"


template < class T >

class CStack
{

public:
	///
	///  Brief:  ջ�Ĺ��캯��
	/// 
	CStack()	
	{
		m_hEvent =  CreateEvent( NULL  , 0  , 0 , NULL );
	}

	~CStack() 
	{
		CloseHandle( m_hEvent );
	}

	void  SetStackEvent()
	{
		SetEvent( m_hEvent );
	}

	HANDLE GetStackEvent ()	{	return m_hEvent ; }

	int	   GetStackSize  ()	{	return m_SList.GetSize(); }

	void   Push( T * data) 
	{
		sync::scope_guard  guard( m_section );

		m_SList.AddNode( data );

	}

	T * Pop()
	{
		sync::scope_guard  guard( m_section );

		if ( m_SList.GetSize ==  0 )	return NULL;

		T * outdata = m_QList.GetTail();
		m_SList.RemoveNode( outdata );


		return outdata;
	}

private:

	TLinkedList<T>		 m_SList;
	HANDLE				 m_hEvent;
	sync::csectionlock   m_section;

};