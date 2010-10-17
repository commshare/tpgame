 /**
  *	@brief ����ʵ��Ļ���
  *
  */
#pragma once 

#include "../../Render/Vector2D.h"
#include "../../Render/Geometry.h"
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
	virtual	  bool  HandleMessage(const tagMessage& e ){ return false;}

	static    int   GetNextValidId()		{ return m_iNextValidID; }
	static    void  ResetNextValidId()		{ m_iNextValidID = 0 ;   }

	Vector2D		Pos()const			    {return m_vPosition;}
	void			SetPos(Vector2D new_pos){m_vPosition = new_pos;}

	double			BRadius()const			{return m_dBoundingRadius;}
	void			SetBRadius(double r)	{m_dBoundingRadius = r;}
	long			GetID()const			{return m_id;}

	bool			IsTagged()const			{return m_bTag;}
	void			Tag()					{m_bTag = true;}
	void			UnTag()					{m_bTag = false;}

	Vector2D		Scale()const			{return m_vScal;}
	void			SetScale(Vector2D val)
	{
		m_dBoundingRadius *= MaxOf(val.x, val.y)/MaxOf(m_vScal.x, m_vScal.y); 
		m_vScal = val;	
	}
	void			SetScale(double val)
	{	
		m_dBoundingRadius *= (val/MaxOf(m_vScal.x, m_vScal.y)); 
		m_vScal = Vector2D(val, val);	
	} 

	ePlayerType    EntityType()const		{return m_itype;}
	void           SetEntityType(ePlayerType new_type){m_itype = new_type;}

protected:

	Vector2D	  m_vPosition;		///> ʵ������
	Vector2D	  m_vScal;			///> ʵ�����
	double		  m_dBoundingRadius;///> �߽����
	static int    m_iNextValidID;	///> ��һ����Ч��ID

private:

	bool		  m_bTag;			///> ʵ���Ƿ���Ŀ�� 
	long	      m_id;				///> ʵ��id
	ePlayerType	  m_itype;			///> ʵ������
	
};