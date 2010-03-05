/** 
 * @brief: һ��������ʱ������˫������
 *         һ�����ӣ���۴�СΪm_size,��ʾÿһ����۶���һ��˫������
 *
 * @Author:Expter
 * @date:  03/01/2010
 */

#pragma once

#include "TypeDef.h"
#include "tools.h"


///			  
/// һ������,һ��ѭ������
/// 
/// 
class CLinkList
{

public:

	CLinkList(void);

	CLinkList( int size );
	
	~CLinkList(void);
	
	/// 
	/// ��ʼ��
	/// 
	void  init();

	/// 
	/// ��ָ��ָ���Լ�
	/// 
	void  init_list_self( int  index);

	/// 
	/// ��news���뵽prev,next֮��
	/// 
	void  insert_listnode(ListNode *news , ListNode* prev , ListNode* next);

	/// 
	/// ���뵽����ͷ
	/// 
	void  insert_head( ListNode* news , ListNode* head );

	///
	/// ���뵽����β
	/// 
	void  insert_tail( ListNode* news , ListNode* head );

	/// 
	/// ɾ���ڵ�
	/// 
	void  list_del( ListNode* list);

	/// 
	/// �õ�������ת���ڼ������
	///
	int        GetIndex( ) const { return m_index ;}

	///
	/// �������ӵĲ��
	///
	void       SetIndex(int idx) { m_index = idx  ;}

	/// 
	/// �õ����Ӳ�۵�ָ�� 
	///
	ListNode*  GetNode(int index) const;

private:
	///
	/// ���ӵĲ������
	/// 
	ListNode *m_List;  
	
	///
	/// ���ӵ�ǰת��������
	/// 
	int		  m_index; 

	///
	/// ���Ӵ�С
	///
	int       m_Size;

};
