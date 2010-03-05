/** 
 *	@brief: ��ʱ��������,����ʱ�����������
 *          ʵ�ַ�ʽ�ο�linux���жϴ���ÿ�δ���ֻ������һ�����ӵĽ�㡣
 *
 *  @Author:Expter
 *	@Date : 03/02/2010
 */

#pragma once

#include "LinkList.h"

#define  ListProp(g, tv,n)\
	g[n-1] = tv##n

///
/// ��ʱ��������,����ʱ�����������
/// 
class CTimer
{
public:	
	///
	///  ���캯������
	/// 
	CTimer(void);

	CTimer( int second);

	~CTimer(void);

public:
	///
	/// ��ʼ����ʱ��������
	///
	void  Init(int Second = 0);

	///
	/// ����һ����ʱ��
	///
	void  add_timer(timernode *times );

	///
	///	��ⶨʱ���Ƿ����
	///
	/// @return  ������ڷ���true,����Ϊfalse
	/// 
	bool  check_timer(timernode* times);

	///
	///	ɾ����ʱ��
	///
	/// @return  ���ɾ���ɹ�����true,����Ϊfalse
	///
	bool  delete_timer(CLinkList* list, timernode *times);

	///
	/// ���³�ʼ��һ����ʱ��
	///
	void  init_timer(timernode* timers);

	///
	/// ��ʱ����Ǩ�ƣ�Ҳ����һ����ʱ������ԭ�������Ķ�ʱ������Ǩ�Ƶ���һ����ʱ�������С�
	///
	void  cascade_timer(CLinkList* timers);

	///
	/// ִ�е�ǰ�Ѿ����ڵĶ�ʱ��,����С��jeffies�Ķ�ʱ��
	///
	void  Expires( ulong  jeffies);

	///
	/// ���³�ʼ��һ����ʱ��
	///
	void  Cancel(timernode* timers);

	///
	/// ���¼���һ����ʱ��
	///
	void  Mod_timer(timernode* timers);

private:
	/// 5������
	CLinkList*    m_tv1;
	CLinkList*    m_tv2;
	CLinkList*    m_tv3;
	CLinkList*    m_tv4;
	CLinkList*    m_tv5;
	CLinkList**   g_vecs;

	/// ��ʱ��ȫ��tick
	ulong		  m_jeffies;
    /// �ϴ�����ʱ��
 	ulong		  m_Lasttime;
	/// ��ȷ������
	ulong		  m_mSecond;
};
