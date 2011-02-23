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
	Base( type tp ) 
	{ m_type = tp;}

	class Handle
	{
	public:
		Handle()
		{
			Base<type>::m_handlelist.push_back( this );
		}
		virtual void todo( Base<type> * base)
		{
			std::cout <<" Handle base " << std::endl;
		}
	};

public:
	virtual void   todo()
	{
		for( std::list<Handle*>::iterator itr = m_handlelist.begin() ; itr != m_handlelist.end() ; itr ++ )
		{
			(*itr)->todo(this);
		}  
	}


protected:
	static  type            		m_type;
	static  std::list<Handle*>  	m_handlelist;
};

template < typename type > 
class BaseObj1 : public Base< type >
{
public:
	BaseObj1( type tp ) : Base ( tp )
	{		}

	void print()
	{
		std::cout << " BaseObj1 print() " << std::endl;
		todo();
	}	
};

template < typename type >
class BaseObj2 : public Base < type >
{
public:
	BaseObj2( type tp ) : Base ( tp )
	{		}

	void display()
	{
		//todo();
	}
};

template < typename type >
class Print : public Base< type >::Handle
{
public:
	virtual	void todo( Base<type> * base )
	{
		std::cout << " 	baseobj1  m_type address " << std::endl;
	}
};

template < typename type >
class display : public Base < type >::Handle 
{
public: 
	virtual void todo( BaseObj2<type> * base )
	{
		std::cout <<" baseobj2 m_type address " << std::endl;
	}
};	





template < typename type >
std::list<typename Base<type>::Handle*>		 Base<type>::m_handlelist;

template < typename type >
type Base<type>::m_type;

// template < typename type >
// std::list<typename BaseObj1<type>::Handle*>  BaseObj1<type>::m_handlelist;
// template < typename type >
// std::list<typename BaseObj2<type>::Handle*>  BaseObj2<type>::m_handlelist;

int _tmain(int argc, _TCHAR* argv[])
{
	BaseObj1<int>  b1(1);
	Print<int>     p1;

	b1.todo();

	return 0;
}

