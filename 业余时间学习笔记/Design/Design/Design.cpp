// Design.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

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




int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

