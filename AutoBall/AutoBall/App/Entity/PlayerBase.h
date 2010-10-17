

#pragma  once

#include "../../Public/Common/Autolist.h"
#include "../../Public/MsgImpl.h"
#include "../Region.h"
#include "../SoccerTeam.h"
#include "MoveEntity.h"

class SoccerPitch;
class SoccerBall;
class SteeringBehaviors;

/// ��Ա������ 
class  PlayerBase : public  CMoveEntity,
					public  CAutoList<PlayerBase>
{

public:
	enum player_role{goal_keeper, attacker, defender};
public:
	PlayerBase(SoccerTeam*    home_team,
				int            home_region,
				Vector2D       heading,
				Vector2D       velocity,
				double          mass,
				double          max_force,
				double          max_speed,
				double          max_turn_rate,
				double          scale,
				player_role    role);

	virtual ~PlayerBase();


	/// ��Ҽ�ط�Χ���Ƿ��жԷ���Ա
	bool        isThreatened()const;

	/// ׷���׷��Ŀ�� 
	void        TrackBall();
	void        TrackTarget();

	/// ��ʼ��֯���飬���������������Ҹı�Ϊ��Ӧ״̬��������н���
	void        FindSupport()const;

	/// �ж����Ƿ��ܱ�����Ա����
	bool        BallWithinKeeperRange()const;

	/// �жϵ�ǰ�����Ƿ��ܹ��ߵ���
	bool        BallWithinKickingRange()const;

	/// �жϵ�ǰ�����Ƿ��ܹ����յ���
	bool        BallWithinReceivingRange()const;

	/// �Ƿ��Ѿ��ص������
	bool        InHomeRegion()const;

	/// �Ƿ��ڵ�ǰ����������Ա��ǰ��
	bool        isAheadOfAttacker()const;

	/// �Ƿ񵽴��Ӧ��ķ�Χ
	bool        AtTarget()const;

	/// ����Ŀ��
	bool		FollowTarget()const;

	/// ����
	bool		FollowReturn()const;

	/// �Ƿ��Ѿ��ڳ���λ������
	bool        IsSelfRegin()const;

	/// �Ƿ��Ƕ�����������Ķ�Ա
	bool        isClosestTeamMemberToBall()const;

	//returns true if the point specified by 'position' is located in
	//front of the player
	///	 �鿴position�Ƿ��ڶ�Աǰ��
	bool        PositionInFrontOfPlayer(Vector2D position)const;

	/// �Ƿ��������������Ա
	bool        isClosestPlayerOnPitchToBall()const;

	/// �����Ƿ��ǿ�������
	bool        isControllingPlayer()const;

	/// �Ƿ��ڽ���
	bool        InHotRegion()const;

	player_role Role()const{return m_PlayerRole;}

	double      DistSqToBall()const{return m_dDistSqToBall;}
	void        SetDistSqToBall(double val){m_dDistSqToBall = val;}

	/// ��Է����������ŵľ���
	double       DistToOppGoal()const;
	double       DistToHomeGoal()const;

	void        SetDefaultHomeRegion(){m_iHomeRegion = m_iDefaultRegion;}

	SoccerBall*  const       Ball()const;
	SoccerPitch* const       Pitch()const;
	SteeringBehaviors*const  Steering()const{return m_pSteering;}
	const Region* const      HomeRegion()const;
	bool					 InsideHomeRegion(Vector2D pos, Region::region_modifier r)const;
	void                     SetHomeRegion(int NewRegion){m_iHomeRegion = NewRegion;}
	SoccerTeam*const         Team()const{return m_pTeam;}

	bool					 IsChaseBall()const { return m_pTeam->IsChaseBall();}
	Vector2D				 GetHomeCenter()    { return HomeRegion()->Center();}

protected:
	player_role				m_PlayerRole;	 ///> ��ǰ��Ա��ɫ
	SoccerTeam*             m_pTeam;		 ///> ��ǰ����ָ��
	SteeringBehaviors*      m_pSteering;	 ///> ������Ϊָ��
	int                     m_iHomeRegion;	 ///> ��Ա��������
	int                     m_iDefaultRegion;///> ����ǰ������ 
	double                  m_dDistSqToBall; ///> ����ľ���
	std::vector<Vector2D>   m_vecPlayerVB;	 ///> ��Ա����۾���
	std::vector<Vector2D>   m_vecPlayerVBTrans;///> ת����ľ���

};