 /**
  *	@brief ����ʵ��Ļ���
  *
  */
#pragma once 

#include "../../Render/Vector2D.h"
#include "../../Render/MathGeo.h"
#include "../../Render/Utils.h"
#include "../../Public/BaseDef.h"
#include "../../Public/MsgImpl.h"

class  tagMessage;
class  CBaseEntity
{

public:
	CBaseEntity( long  id);
	virtual	  ~CBaseEntity() {}

	virtual   void  Update() = 0 ;
	virtual   void  Render() = 0 ;
	virtual	  bool  OnMessage(const tagMessage& e ){ return false;}

	static    int   GetNextValidId()		{ return m_iNextValidID; }
	static    void  ResetNextValidId()		{ m_iNextValidID = 0 ;   }

	Vector2D		Pos()const			    {return m_vPosition;}
	void			SetPos(Vector2D new_pos){m_vPosition = new_pos;}

	double			GetSize()const			{return m_dSize;}
	void			SetSize(double r)		{m_dSize = r;}
	long			GetID()const			{return m_id;}
	Vector2D		Scale()const				{return m_vScal;}
	void			SetScale(Vector2D val)
	{
		m_dSize *= MaxOf(val.x, val.y)/MaxOf(m_vScal.x, m_vScal.y); 
		m_vScal = val;	
	}
	void			SetScale(double val)
	{	
		m_dSize *= (val/MaxOf(m_vScal.x, m_vScal.y)); 
		m_vScal = Vector2D(val, val);	
	} 

	ePlayerType    EntityType()const		{return m_itype;}
	void           SetEntityType(ePlayerType new_type){m_itype = new_type;}

protected:

	Vector2D	  m_vPosition;		///> ʵ������
	Vector2D	  m_vScal;			///> ʵ�����
	double		  m_dSize;			///> ��С
	static int    m_iNextValidID;	///> ��һ����Ч��ID

private: 
	long	      m_id;				///> ʵ��id
	ePlayerType	  m_itype;			///> ʵ������
	
};