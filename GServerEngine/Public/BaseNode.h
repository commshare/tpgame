/*
*	File  :  BaseNode.h
*   Brief :  һ�����
*   Author:  Expter 
*	Creat Date:  [2009/11/2]
*/

#pragma  once

template < typename T >

class  CBaseNode
{
public:

	T * next;
	T * prev;

	CBaseNode()
	{
		next = prev = NULL;
	} 

};