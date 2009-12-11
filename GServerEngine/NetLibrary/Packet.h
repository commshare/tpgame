/*
*	File  :
*   Brief :
*   Author:  Expter 
*	Creat Date:  [2009/11/2]
*/

#pragma  once
#include "..\Public\TypeDef.h"
#include "..\Public\BaseNode.h"
#include <string>
#include <cstdlib>
using  namespace std;

#define PACK_HEADER_LEN				sizeof( CPacket::PackHead )

/*
 *		һ�����ݰ��İ�ͷ
 *		�� eVerifyCode / PacketSize / KeepLive / buf��
 *		��У���롿 �����ݰ���С(����ͷ�Ĵ�С)��	�����ݰ�Э�顿
 */

class  CPacket 
{
public:
	struct  PackHead
	{
		long	Size;
		long	Mark;
		long	ptcl;
	};
protected:
	PackHead	pakheader;
	char		buf[ BUFFER_SIZE - PACK_HEADER_LEN ];

public:

	long	GetMark()				{	return  (pakheader.Mark & STARTMARK) ;	}

	long	GetSequenceNum()		{	return  (pakheader.Mark^STARTMARK)>>5;	}/// ȡMark�ĵ�8λ

	long	GetSize()				{	return	 pakheader.Size;				}

	long	GetPtcl()				{	return	 pakheader.ptcl;				}

	void	SetMsgPtcl(long pptcl)	{	pakheader.ptcl = pptcl;					}
};

class CMessage :
	public CPacket
{
public:
	CMessage()
	{
		index = PACK_HEADER_LEN ;
		memset( &pakheader , 0 , sizeof(pakheader) );
		memset( buf , 0 , BUFFER_SIZE );
	}

	/*
	 *	�������ݰ�
	 */ 
	CMessage( void * data)
	{
		index = 0 ;
		memcpy( &pakheader , data , sizeof(pakheader) );
		memcpy( buf , ((char*)data + sizeof(pakheader)) , pakheader.Size - sizeof(pakheader) );
	}
	CMessage(long ptcl )
	{
		index = PACK_HEADER_LEN;
		memset( buf , 0 , sizeof(buf) );
		pakheader.Mark = STARTMARK;
		pakheader.Size = 0;
		pakheader.ptcl = ptcl;
	}

	void  SetIndex( int  in = 0)
	{
		index = in ;
	}

	/*
	 *	д����
	 */
	void AddtocharArray(void * pSource, long n)
	{
		AddBuf((char*)pSource,n);
	}
	void AddtocharArray(const char * pStr)
	{
		long size = strlen(pStr);
		AddBuf((char*)&size,sizeof(long));
		AddBuf((char*)pStr,strlen(pStr));
	}
	template<typename type>
	void AddtocharArray(type   l)
	{
		AddBuf((char*)&l,sizeof(type));
	}

	/*
	 *	������
	 */
	char *GetBufferFromcharArray(char *Buf,long lLen)
	{
		if(lLen <= 1) return NULL;
		long len = GetObjDataFromcharArray<long>();
		if(len < 0 )  len =0;

		len = min(len,lLen-1);
		GetBuf((char*)Buf,len);
		Buf[len] ='\0';
		return Buf;
	}

	template<typename type>
	inline type GetObjDataFromcharArray()
	{
		type t;
		GetBuf((char*)&t,sizeof(type));
		return t;
	}


	/*
	 *	�������ݰ�
	 */
	void  BuildPacketHeader()
	{
		int  nPro = PACK_HEADER_LEN;
		pakheader.Size = index;
		memcpy( buf	,(char*)&pakheader , nPro );
	}

	/*
	 *	ת�����ݰ�
	 */

	void  ReSetPacket( long  sType )
	{
		pakheader.Size = index + PACK_HEADER_LEN;
		pakheader.ptcl = sType;
		char rTemp [ BUFFER_SIZE ];
		memcpy( rTemp ,  buf , index );
		memcpy( buf ,(char*)&pakheader , PACK_HEADER_LEN );
		memcpy( buf + PACK_HEADER_LEN,rTemp , index );
		index += PACK_HEADER_LEN;
	}

	/*
	 *	�õ����ݰ�
	 */
	const char * GetPacket()  const
	{
		return  buf;
	}

	/*
	 *	һЩ������
	 */
	long	GetLength()				{	return index;							}

// 	CMessage & ReMoveHead(){
// 		pakheader = GetObjDataFromcharArray<CPacket>();
// 		return *this;
// 	}
// 	bool	SendPlayer( CConnection *pCon , bool iUseNegle)
// 	{
// 		pCon->SendData( buf ,  Size , iUseNegle );
//  
// 		return true;
// 	}

	template< class _ConObj >
	bool  SendPlayer( _ConObj *pCon , bool iUseNegle)
	{
		pCon->SendData( buf , index , iUseNegle );

		return true;
	}

private:

	void   AddBuf(char * pBuf,long size)
	{
		if( size > 0 && index >= 0 )
		{
			int  nmin = Min( (BUFFER_SIZE - index) , size );

			memcpy( buf + index ,pBuf , nmin );	
		}
		else
			return ;
		index += size;
	}

	void  GetBuf( char *pBuf , long size )
	{
		if( size > 0 && index >= 0 )
		{
			int  nmin = Min( (BUFFER_SIZE - index) , size );

			memcpy( pBuf  , buf + index , nmin );	
		}
		else
		{
			pBuf = NULL;
			return ;
		}

		index += size;
	}

private:
	long		index;
};