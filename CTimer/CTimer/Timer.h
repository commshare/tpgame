/** 
 *
 *
 *
 *
 */

#pragma once

#include "LinkList.h"

#define  ListProp(g, tv,n)\
	g[n-1] = tv##n

class CTimer
{
	///  ���캯������
public:
	CTimer(void);

	CTimer( int second =0);

	~CTimer(void);

public:

	void  Init(int Second = 0);

	void  add_timer(timernode *times );

	bool  check_timer(timernode* times);

	bool  delete_timer(CLinkList* list, timernode *times);

	void  init_timer(timernode* timers);

	void  cascade_timer(CLinkList* timers);

	void  Expires( ulong  jeffies);

	void  Cancel(timernode* timers);

	void  Mod_timer(timernode* timers);

private:

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
