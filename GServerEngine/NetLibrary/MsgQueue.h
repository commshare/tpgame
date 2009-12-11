/*
*	File  :	 
*   Brief :  ��Ϣ����
*   Author:  Expter 
*	Creat Date:  [2009/11/11]
*/

#pragma once

#include "PacketManager.h"

class CMsgQueue
{
	typedef  CQueue<PACKETBUFFER>  msgQueue;

public:
	/*
	 *  Brief: ���캯������ʼ������
	 */
	CMsgQueue(void);

	~CMsgQueue(void);

	/*
	 *   Push����
	 */
	bool	PushMessage(	PACKETBUFFER  *pMsg );

	/*
	 *   Pop����
	 */
	PACKETBUFFER	*PopMessage(	);

	/*
	 *  ȡ����Ϣ�ж�size������
	 */
	void	GetSizeMessage(  msgQueue& pTempQMsg , int size );

	/*
	 *  ȡ����������
	 */
	void	GetAllMessage(	 msgQueue& pTempQMsg);

	/*
	 *	������Ϣ�ж����ݴ�С
	 */
	int		GetSize();

	/*
	 *  �������
	 */
	void	Clear();

private:
	msgQueue			m_msgQueue;			/// ��Ϣ����
	sync::csectionlock	m_Section;			/// ��

};
