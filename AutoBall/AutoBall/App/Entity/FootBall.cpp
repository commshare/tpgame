#include "Stdafx.h"
#include "FootBall.h"
#include "BasePlayer.h"
#include "../FootBallTeam.h"
#include "../FootBallPitch.h"
#include "../StateAi/StateMachine.h"
#include "../StateAi/State.h"


extern FootBallPitch* g_FootBallPitch;

void FootBall::Kick(Vector2D direction, double force,CMoveEntity* entity)
{  
	direction.Normalize();

	//ͨ��force������ٶ�
	Vector2D acceleration = (direction * force) / m_dMass;

	m_vVelocity = acceleration;
	m_LastPlayer= entity;

}

void FootBall::Update()
{
	m_vOldPos = m_vPosition;

	TestCollisionWithWalls(m_PitchBoundary);

	if (m_vVelocity.LengthSq() > GetInstObj(CGameSetup).Friction * GetInstObj(CGameSetup).Friction)
	{
		m_vVelocity += Vec2DNormalize(m_vVelocity) * GetInstObj(CGameSetup).Friction;

		m_vPosition += m_vVelocity;

		//update heading
		m_vHeading = Vec2DNormalize(m_vVelocity);
	}

#ifdef _PUT_LOG_
	char  str[256];
	sprintf_s(str,"Ball Position x=%f,y=%f",m_vPosition.x,m_vPosition.y);
	PutLog("ball.txt",str);
#endif
}

double FootBall::TimeToCoverDistance(Vector2D A,
									   Vector2D B,
									   double force)const
{
	/// �����Ա������������ٶ�
	double speed = force / m_dMass;
	
	/// ����A,B֮��ľ���
	double DistanceToCover =  Vec2DDistance(A, B);
	
	double term = speed*speed + 2.0*DistanceToCover*GetInstObj(CGameSetup).Friction;

	if (term <= 0.0) return -1.0;

	/// �����B���ٶ�
	double  v = sqrt(term);

	return (v-speed)/GetInstObj(CGameSetup).Friction;
}

Vector2D FootBall::FuturePosition(double time)const
{
	///ʹ�ù�ʽ s = ut + 1/2at^2,  s = ����, a = ���ٶ�, uΪ��ʼ�ٶ�

	///����UT
	Vector2D ut = m_vVelocity * time;

	double half_a_t_squared = 0.5 * GetInstObj(CGameSetup).Friction * time * time;

	///ͨ�������ٶȵı�׼���������ѱ���ת��Ϊ����
	Vector2D ScalarToVector = half_a_t_squared * Vec2DNormalize(m_vVelocity);

	///��ǰλ�ü������λ��
	return Pos() + ut + ScalarToVector;
}

void FootBall::Render()
{
	GetInstObj(CGDI).GreyBrush();

	GetInstObj(CGDI).Circle(m_vPosition, m_dSize);

}


void FootBall::TestCollisionWithWalls(const std::vector<Wall2D>& walls)
{  
	//test ball against each wall, find out which is closest
	int idxClosest = -1;

	Vector2D VelNormal = Vec2DNormalize(m_vVelocity);

	Vector2D IntersectionPoint, CollisionPoint;

	double DistToIntersection = MaxFloat;

	/// �ȴ��߽���
	if( m_vVelocity.Length() <= 0 )
		return ;

	for (unsigned int w=0; w<walls.size(); ++w)
	{
		Vector2D ThisCollisionPoint = Pos() - (walls[w].Normal() * GetSize());

		if (WhereIsPoint(ThisCollisionPoint,
			walls[w].From(),
			walls[w].Normal()) == plane_backside)
		{
			double DistToWall = DistanceToRayPlaneIntersection(ThisCollisionPoint,
				walls[w].Normal(),
				walls[w].From(),
				walls[w].Normal());

			IntersectionPoint = ThisCollisionPoint + (DistToWall * walls[w].Normal());
		}
		else
		{
			double DistToWall = DistanceToRayPlaneIntersection(ThisCollisionPoint,
				VelNormal,
				walls[w].From(),
				walls[w].Normal());

			IntersectionPoint = ThisCollisionPoint + (DistToWall * VelNormal);
		}

		bool OnLineSegment = false;

		if (LineIntersection2D(walls[w].From(), 
			walls[w].To(),
			ThisCollisionPoint - walls[w].Normal()*20.0,
			ThisCollisionPoint + walls[w].Normal()*20.0))
		{

			OnLineSegment = true;                                               
		}


		double distSq = Vec2DDistanceSq(ThisCollisionPoint, IntersectionPoint);

		if ((distSq <= m_vVelocity.LengthSq()) && (distSq < DistToIntersection) && OnLineSegment)            
		{        
			DistToIntersection = distSq;
			idxClosest = w;
			CollisionPoint = IntersectionPoint;
		}     
	}//next wall

	if ( (idxClosest >= 0 ) && VelNormal.Dot(walls[idxClosest].Normal()) < 0)
	{
		m_vVelocity = Vector2D(0,0);
		BasePlayer* Player = (BasePlayer*)m_LastPlayer;

		if( Player->Team()->Color() == FootBallTeam::blue )
		{
			/// ��ֹblue�Ӵ���
			g_FootBallPitch->m_pRedTeam->GetFSM()->ChangeState( &GetInstObj(Throw_In) );
			g_FootBallPitch->m_pBlueTeam->SetChaseBall(false);
		}
		else
		{
			g_FootBallPitch->m_pBlueTeam->GetFSM()->ChangeState(&GetInstObj(Throw_In) );
			g_FootBallPitch->m_pRedTeam->SetChaseBall(false);
		}
	}
}


void FootBall::PlaceAtPosition(Vector2D NewPos)
{
	m_vPosition = NewPos;

	m_vOldPos = m_vPosition;

	m_vVelocity.Zero();
}