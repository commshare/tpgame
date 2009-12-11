#pragma once
#include "Region.h"


class  CGUID;
class CObj
{
	/// ��������
	enum  OBJ_TYPE
	{
		OBJ_TYPE_INVALID,			// ��Ч
		OBJ_TYPE_PLAYER,			// ���
		OBJ_NPC,					// NPC
		OBJ_TYPE_MONSTER,			// ����ս���Ĺ���
		OBJ_TYPE_PET,				// ����		
		/// �������µ����ͼ�������
	};

	/// ״̬
	enum  STATE
	{	
		STATE_PEACE,				// ��ƽ״̬
		STATE_FIGHT,				// ս��״̬
		STATE_DIED,					// ����״̬
		STATE_FEIGNDEATH,			// ����״̬
		STATE_HANGUP,				// ����״̬
		STATE_DELETE,				// ɾ��״̬
		/// �����������
	};

public:

	CObj(void);

	~CObj(void);

	/*
	 *	�����Ա����
	 */
public:

	virtual  void		Init()	;
	virtual	 void		Clear() ;
	virtual	 void		SetObjType( OBJ_TYPE  obj ) {	m_obj = obj ;		}
	virtual	 OBJ_TYPE	GetObjType() const			{	return   m_obj;		}
	virtual	 void		SetState( STATE  state )	{	m_state = state ;	}
	virtual	 STATE		GetState() const			{	return   m_state;	}	
	virtual	 CGUID&		GetGUID() const				{	return	 m_GUID;	}
	virtual	 bool		StarAi( int	uTimer =0 );
	

private:
	AI_SYSTEM::CRegion		m_Pos;
	CGUID					m_GUID;
	OBJ_TYPE				m_obj;
	STATE					m_state;

};
