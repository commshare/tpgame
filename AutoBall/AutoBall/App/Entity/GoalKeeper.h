
#pragma  once

#include "../../Public/Common/Autolist.h"
#include "../StateAi/StateMachine.h"
#include "MoveEntity.h"

class SoccerTeam;
class SoccerPitch;
class SoccerBall;
class Region;
class PlayerBase;
template < class T>
class  State;

/// ��װ����Ա��
class  GoalKeeper : public PlayerBase
{

public:
	GoalKeeper(SoccerTeam*        home_team,
			   int                home_region,
			   State<GoalKeeper>* start_state,
			   Vector2D           heading,
			   Vector2D           velocity,
			   double             mass,
			   double             max_force,
			   double             max_speed,
			   double             max_turn_rate,
		       double             scale);

	~GoalKeeper() { SAFE_DELETE(m_pStateMachine); }

	void        Update();
	void        Render();
	bool        HandleMessage(const Telegram& msg);

	///  �Ƿ������ط�Χ��
	bool        BallWithinRangeForIntercept()const;

	///  �Ƿ�������̫Զ
	bool        TooFarFromGoalMouth()const;

	///  �õ���������ص�
	Vector2D    GetRearInterposeTarget()const;

	StateMachine<GoalKeeper>* GetFSM()const{return m_pStateMachine;}


	Vector2D    LookAt()const		 {return m_vLookAt;}
	void        SetLookAt(Vector2D v){m_vLookAt=v;}

private:
	
	StateMachine<GoalKeeper>*  m_pStateMachine;/// ״̬��
	Vector2D				   m_vLookAt;	   /// ��Ұ����

};