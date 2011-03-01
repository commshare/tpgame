// Design.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

/*
����:

virtual  Base * getBase() ;

/// ����objָ��,��þ������������Ϣ������֮�����в���
void  function(Base  *obj)
{
/// ����objֻ��Base����ָ��

BaseObj1 * obj1;
BaseObj2 * obj2;

if ( obj1 = dynamic_cast<BaseObj1*>( obj )  )
{
/// to do
}
else if ( obj2 = dynamic_cast<BaseObj2*>( obj ) )
{
/// to do
}
else 
{
/// ... you get the idea
}
}

����������������Base������ʱ�������if else������Խ��Խ����

��������������ͨ�����¼��ַ�ʽ�����:

1. һ������¿��Ƕ�̬��objֻ��Ҫ�����麯�����ɡ�����
void function( Base  *obj )
{
obj->todo();
}

�������function������ĳ��������free objʱ��ͨ��todo���²����ʡ�


2. ���ǵ��������⣬��������visitorģʽ��
��ôͬʱ���ṩ����Ĳ����ӿ�:
virtual void visit( BaseObj1 * obj1 );
virtual void visit( BaseObj1 * obj2 );

/// ����ж�������͵Ļ�����Ҫ��дvisit����ʱfuction
void function( Base  *obj )
{
	visit( obj );
}

3. һ���½����ʽ.
*/

#include <list>
#include <iostream>

using namespace  std;

template < typename type>
class Base
{
public:
	class Handle
	{
	public:
		Handle()
		{
			Base<type>::m_handlelist.push_back( this );
		}
		virtual void todo( const type& tp)   = 0;
	};

public:
	static void   todo(Base<type> * base)
	{
		for( std::list<Handle*>::iterator itr = m_handlelist.begin() ; 
			 itr != m_handlelist.end() ; itr ++ )
		{
			(*itr)->todo( *(type*)base );
		}  
	}  
	void  todo()
	{
		type::todo( this );
	}

protected:
	typedef std::list<Handle*>		listtype;
	static  listtype			 	m_handlelist;
};

class BaseObj1 : public Base< BaseObj1 >
{
public:
	void print() const
	{
		std::cout << " BaseObj1 print() " << std::endl;
	}	
};

class BaseObj2 : public Base < BaseObj2 >
{
public:
	void display() const
	{
		std::cout << " BaseObj2 display() " << std::endl;
	}
};

class DoHandle : public BaseObj1::Handle ,
				 public BaseObj2::Handle				 
{
public:
	virtual	void todo(const BaseObj1 & obj1)
	{
		std::cout <<" baseobj1  m_type address " << std::endl;
		obj1.print();
	}
	virtual void todo(const BaseObj2 & obj2)
	{
		std::cout <<" baseobj2 m_type address " << std::endl;
		obj2.display();
	}
};






// template < typename type >
// std::list<typename Base<type>::Handle*>		 Base<type>::m_handlelist;
// 
// template < typename type >
// type Base<type>::m_type;

// template < typename type >
// std::list<typename BaseObj1<type>::Handle*>  BaseObj1<type>::m_handlelist;
// template < typename type >
// std::list<typename BaseObj2<type>::Handle*>  BaseObj2<type>::m_handlelist;

Base<BaseObj1>::listtype Base<BaseObj1>::m_handlelist ;
Base<BaseObj2>::listtype Base<BaseObj2>::m_handlelist ;

/*
void  fun( BaseObj1 * obj1)
{
	obj1->print();
}

void  fun( BaseObj2 * obj2)
{
	obj2->display();
}
*/

template<typename type>
void  fun( Base<type> * base)
{
	base->todo();
}

int _tmain(int argc, _TCHAR* argv[])
{
	//BaseObj1<int>  b1(1);
	//Print<int>     p1;

	//b1.todo();

	BaseObj1  obj1;
	BaseObj2  obj2;

	DoHandle  handle;

	//obj1.todo();
	//obj2.todo();

	fun( & obj1 );
	fun( & obj2 );

	return 0;
}

