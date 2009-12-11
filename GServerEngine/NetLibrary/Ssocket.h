/*
*	File  :	 Sscoket.h
*   Brief :  �Լ���װһ���첽���ص��׽�����
*   Author:  Expter 
*	Creat Date:  [2009/11/24]
*/
#pragma once

#include <winsock2.h>

#include "Packet.h"

#pragma  comment( lib ,"WS2_32.lib")

class CSocket 
{
public:
	WORD		m_SendErrorCode;		///  ��ǰ�������
	WORD		m_SaveCountoRecvPacket; ///  ���յ����ݰ�����

	int			m_ReadIndex;			///  ��ǰ��������,����һ�����ݰ�������������ʾȡ����λ��
	int			m_SaveRecvBytes;		///  ���գ����͵��ֽ���
	int			m_SaveSendBytes;


public:	
	/*
	 *			�����ʼ�� 
	 */
	CSocket	(	SOCK_TYPE Type = SPECIAL_TYPE,int Size = MAX_SEND_BUFFER_SIZE);

	void		InitSocket( SOCKADDR_IN * addr , SOCKET  sock);

	void		InitOverLapped( OVER_TYPE Type);

	/*
	 *			��ǰ�׽���״̬���� 
	 */

	void		SetOverReady()	{	m_OverState = READY ;	}

	void		SetOverBad()	{	m_OverState = BAD;		}

	/*
	 *			�����׽���״̬
	 */	

	OVER_STATE	GetOverState()	{	return m_OverState;		}

	int			GetSaveCountofRecvPacket()	{	return m_SaveCountoRecvPacket ; }

	int			GetSaveCountofSendPacket()	{	return 0;						}

	/*
	 *  ��		��ѭNagle�㷨
	 *  ��		���ͣ����棬�������ݺ���
	 */
	void		SendData(const char * packet ,  int packetSize , bool  bUseN = FALSE );

	void		SaveSendData();

	void		RecvData();

	void		SaveRecvBuffer( int SaveLen);

	/*
	 *	 ��		�õ��첽I/O����
	 */
	OVERLAPPED	*GetRecvOverLapped()	{	return  &m_RecvOver ; }

	OVERLAPPED	*GetSendOverLapped()	{	return	&m_SendOver	; }

	SOCKADDR_IN *GetSockAddr()			{	return  &m_SockAddr	; }

	SOCKET      *GetSocket()			{	return  &m_Socket	; }

	/*
	 *			����׽��ֵ�״̬
	 */
	bool		CheckActiveSocket()		{	return	m_Socket != INVALID_SOCKET ; }

	/*
	 *			�õ����ݰ�
	 */ 
	char*		GetRecvBuffer()			{	return m_RecvBuffer ;}

	long		GetPacketHead();	

	char*		GetPacket();		

	/*
	 *			�ͷ���Դ 
	 */
	virtual     ~CSocket(void);

	void		ReleaseSocket();


private:

	SOCKET		m_Socket;				///  �׽���
	SOCK_TYPE	m_SockType;				///  �׽�������
	SOCKADDR_IN	m_SockAddr;				///  ������Ϣ
	DWORD		m_DwFlag;				///  ����������
	OVERLAPPED	m_RecvOver;				///  ���ܲ������첽�ɣϲ���		
	OVERLAPPED	m_SendOver;				///  �������ݰ����첽�ɣϲ���
	OVER_STATE	m_OverState;			///  �����׽���״̬
	WSABUF		m_WsaRbuf;				///  ����buf,  ��������ָ��ͻ������ĳ���
	WSABUF		m_WsaSbuf;				///  ����buf   ��������ָ��ͻ������ĳ���
	DWORD		m_DwBytesRecv;			///  �����ֽ�
	DWORD		m_DwBytesSent;			///  ����ֱ��
	char		m_RecvBuffer[ BUFFER_SIZE ];/// ����buf
	char*		m_SendBuffer;			///  ����buf
	int			m_SendBufSize;			///  ����buf��С
};
