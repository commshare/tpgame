/** 
 * @brief: һ��˫������
 *
 * @Author:Expter
 * @date:  03/01/2010
 */

#pragma once

#include "TypeDef.h"
#include "tools.h"

struct ListNode
{
	ListNode *next,*prev;
};

struct timernode
{
	ListNode  tlist;
	/// ѭ������ʱ��
	ulong    expires;
	/// ��������ʱ��
	ulong    etime;
	/// ��������
	void     *pFun;
};

class CLinkList
{

public:

	CLinkList(void);

	CLinkList( int size );
	
	~CLinkList(void);
	
	/// ��ʼ��
	void  init();

	/// ��ָ��ָ���Լ�
	void  init_list_self( int  index);

	/// ��news���뵽prev,next֮��
	void  insert_listnode(ListNode *news , ListNode* prev , ListNode* next);

	/// ���뵽����ͷ
	void  insert_head( ListNode* news , ListNode* head );

	/// ���뵽����β
	void  insert_tail( ListNode* news , ListNode* head );

	/// ɾ���ڵ�
	void  list_del( ListNode* list);

	int        GetIndex( ) const { return m_index ;}

	void       SetIndex(int idx) { m_index = idx  ;}

	ListNode*  GetNode(int index) const;

private:

	ListNode *m_List;  /// ����

	int		  m_index; /// ����

	int       m_Size;

};
