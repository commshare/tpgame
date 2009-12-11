/*
*	File  :  PacketManager.h
*   Brief :	 ͨ��Queue���������е�����  (һ���ڴ����ʽ)
			 һ���ڴ�ص�ʵ�֣��ο�loki��С�������ʽ���
*   Author:  Expter 
*	Creat Date:  [2009/12/9]
*/
#pragma once

#include "Packet.h"
#include "Connection.h"
#include "..\Public\Queue.h"

/*
 *	 ���ݰ�
 */
struct PACKETBUFFER : CBaseNode<PACKETBUFFER>
{
	CConnection*	pCon;
	BUFFER_TYPE		Type;
};

/*
 *	 ��ͬ��С�����ݰ�����
 */
struct PACKETBUFFER_32		: PACKETBUFFER	{	char Buffer[32];	};
struct PACKETBUFFER_64		: PACKETBUFFER	{	char Buffer[64];	};
struct PACKETBUFFER_128		: PACKETBUFFER	{	char Buffer[128];	};
struct PACKETBUFFER_256		: PACKETBUFFER	{	char Buffer[256];	};
struct PACKETBUFFER_512		: PACKETBUFFER	{	char Buffer[512];	};
struct PACKETBUFFER_1024	: PACKETBUFFER	{	char Buffer[1024];	};
struct PACKETBUFFER_5120	: PACKETBUFFER	{	char Buffer[5120];	};
struct PACKETBUFFER_INTERFACE: PACKETBUFFER	{	char Buffer[5120];	};


class CPacketManager
{
public:
	/*
	 *	  ���캯������ʼ��
	 */
	CPacketManager();

	/*
	 *	  ��������
	 */
	~CPacketManager();

	/*
	 *	  �Ƿ�����Dos����
	 */
	void	SetDosDefenceOption( bool bUse)	{ m_bUseDosDefence = bUse ;}

	/*
	 *	  �����ڴ�
	 */
	void	PacketBufferAlloc();

	/*
	 *	  ���ݰ�����
	 */
	void	SetPacketEvent()	{	m_Event_Q.SetQEvent();	}

	HANDLE	GetPacketEvent()	{	return m_Event_Q.GetEvent() ; }

	bool    WritePacket(const  char* pPacket,long Size,CConnection* pCon);

	PACKETBUFFER * ReadPacket( );

	void	ReadAllPacket( CQueue<PACKETBUFFER> &rTemp);

	/*
	 *		�ͷſռ�
	 */
	void    ReleasePacket( PACKETBUFFER *pBuffer);

private:
	/*
	 *	   ������С�����ݰ�
	 */
	PACKETBUFFER_32*			PacketBufferArray_32;
	PACKETBUFFER_64*			PacketBufferArray_64;
	PACKETBUFFER_128*			PacketBufferArray_128;
	PACKETBUFFER_256*			PacketBufferArray_256;
	PACKETBUFFER_512*			PacketBufferArray_512;
	PACKETBUFFER_1024*			PacketBufferArray_1024;
	PACKETBUFFER_5120*			PacketBufferArray_5120;

	/*
	 *	ͨ������ķ�ʽ������Щ���е��ڴ����ݰ�
	 */
	TLinkedList<PACKETBUFFER>	PacketBufferPool_32;
	TLinkedList<PACKETBUFFER>	PacketBufferPool_64;
	TLinkedList<PACKETBUFFER>	PacketBufferPool_128;
	TLinkedList<PACKETBUFFER>	PacketBufferPool_256;
	TLinkedList<PACKETBUFFER>	PacketBufferPool_512;
	TLinkedList<PACKETBUFFER>	PacketBufferPool_1024;
	TLinkedList<PACKETBUFFER>	PacketBufferPool_5120;

	/*
	 *	���յ������ݰ�
	 */
	CQueue<PACKETBUFFER>		m_Event_Q;

	bool						m_bUseDosDefence;

	sync::csectionlock			m_Section;
};