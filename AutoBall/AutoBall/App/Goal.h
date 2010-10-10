
#pragma  once 

#include "Entity/SoccerBall.h"
#include "../Render/Vector2D.h"
#include "../Render/Geometry.h"

/// ������
class Goal 
{

public:

	Goal(Vector2D left, Vector2D right, Vector2D facing):m_vLeftPost(left),
		m_vRightPost(right),
		m_vCenter((left+right)/2.0),
		m_iNumGoalsScored(0),
		m_vFacing(facing)
	{  }

	/// ���������ſڣ��ķ���������
	inline bool Scored(const SoccerBall*const ball);

	Vector2D Center()const		{return m_vCenter;}
	Vector2D Facing()const		{return m_vFacing;}
	Vector2D LeftPost()const	{return m_vLeftPost;}
	Vector2D RightPost()const	{return m_vRightPost;}

	int    NumGoalsScored()const{return m_iNumGoalsScored;}
	void   ResetGoalsScored()	{m_iNumGoalsScored = 0;}

private:

	Vector2D   m_vLeftPost;				   ///> ����2������
	Vector2D   m_vRightPost;
	Vector2D   m_vFacing;				   ///> ���ŵĳ�������
	Vector2D   m_vCenter;				   ///> �����ߵ��м�λ��

	int        m_iNumGoalsScored;		  ///> ��������

};


/// ���������ſڣ��ķ���������
bool Goal::Scored(const SoccerBall*const ball)
{
	if (LineIntersection2D(ball->Pos(), ball->OldPos(), m_vLeftPost, m_vRightPost))
	{
		++m_iNumGoalsScored;

		return true;
	}

	return false;
}