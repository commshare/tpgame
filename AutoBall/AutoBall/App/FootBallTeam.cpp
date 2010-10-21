#include "stdafx.h"
#include "FootBallTeam.h"
#include "FootBallPitch.h"
#include "Goal.h"
#include "TimeCount.h"
#include "SpotCalculator.h"
#include "Steering.h"
#include "Entity/BasePlayer.h"
#include "Entity/FootBall.h"
#include "Entity/GoalKeeper.h"
#include "Entity/FootBaller.h"
#include "Entity/EntityManager.h"
#include "StateAi/StateMachine.h"
#include "Messageing/MessageDispatcher.h"
#include "../Public/Singleton.h"
#include "../Render/VGdi.h"
#include "../Render/Vector2D.h"
#include "../Render/MathGeo.h"

FootBallTeam::FootBallTeam(Goal*    home_goal,
					   Goal*        opponents_goal,
					   FootBallPitch* pitch,
					   team_color   color):m_pOpponentsGoal(opponents_goal),
					   m_pHomeGoal(home_goal),
					   m_pOpponents(NULL),
					   m_pPitch(pitch),
					   m_Color(color),
					   m_dDistSqToBallOfClosestPlayer(0.0),
					   m_pSupportingPlayer(NULL),
					   m_pReceivingPlayer(NULL),
					   m_pControllingPlayer(NULL),
					   m_pPlayerClosestToBall(NULL)
{
	m_pStateMachine = new StateMachine<FootBallTeam>(this);

	m_pStateMachine->SetCurrentState(&GetInstObj(Defending));
	m_pStateMachine->SetPreviousState(&GetInstObj(Defending));
	m_pStateMachine->SetGlobalState(NULL);

	CreatePlayers();

	std::vector<BasePlayer*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		(*it)->Steering()->SeparationOn();   
	}

	m_pSupportSpotCalc = new SpotCalculator(GetInstObj(CGameSetup).NumSupportSpotsX,
							GetInstObj(CGameSetup).NumSupportSpotsY,
							this);

	m_IsChase = true;
}

FootBallTeam::~FootBallTeam()
{
	delete m_pStateMachine;

	std::vector<BasePlayer*>::iterator it = m_Players.begin();
	for (it; it != m_Players.end(); ++it)
	{
		SAFE_DELETE( *it );
	}

	SAFE_DELETE( m_pSupportSpotCalc );
}

void FootBallTeam::Update()
{
	CalculateClosestPlayerToBall();

	m_pStateMachine->Update();

	std::vector<BasePlayer*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		(*it)->Update();
	}
}

void FootBallTeam::CalculateClosestPlayerToBall()
{
	double ClosestSoFar = MaxFloat;

	std::vector<BasePlayer*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		double dist = Vec2DDistanceSq((*it)->Pos(), Pitch()->Ball()->Pos());

		(*it)->SetDistSqToBall(dist);

		if (dist < ClosestSoFar && (*it)->GetID()!=1 && (*it)->GetID()!=8 )
		{
			ClosestSoFar = dist;

			m_pPlayerClosestToBall = *it;
		}
	}

	m_dDistSqToBallOfClosestPlayer = ClosestSoFar;
}

BasePlayer* FootBallTeam::CalcBestSupportingAttacker()
{

	/// �����ж�Ա���б���һ�Σ��������ֵ��Ok....
	double ClosestSoFar = MaxFloat;

	BasePlayer* BestPlayer = NULL;

	std::vector<BasePlayer*>::iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		if ( ((*it)->Role() == BasePlayer::attacker) && ((*it) != m_pControllingPlayer) )
		{
			double dist = Vec2DDistanceSq((*it)->Pos(), m_pSupportSpotCalc->GetBestSupportingSpot());

			if ((dist < ClosestSoFar) )
			{
				ClosestSoFar = dist;

				BestPlayer = (*it);
			}
		}
	}

	return BestPlayer;
}


int  FootBallTeam::FindRecvPass(const BasePlayer*const passer,
								Vector2D& PassTarget, double power,
								double MinPassingDistance)
{
   BasePlayer*  player =  NULL;

   if ( FindPass(passer,player,PassTarget,power,MinPassingDistance) )
   {
	   return player->GetID();
   }
   return 0;
}


bool FootBallTeam::FindPass(const BasePlayer*const passer,
						  BasePlayer*&           receiver,
						  Vector2D&              PassTarget,
						  double                  power,
						  double                  MinPassingDistance)const
{  


	std::vector<BasePlayer*>::const_iterator curPlyr = Members().begin();

	double    ClosestToGoalSoFar = MaxFloat;
	Vector2D Target;


	/// ���������Ա������ӵ����ж�Ա������˭�ǽ������
	for (curPlyr; curPlyr != Members().end(); ++curPlyr)
	{   
		/// ȷ��Ǳ�ڵĽ����Ա���������Ա�Լ�����������λ���봫���Ա�ľ��������С�������
		if ( (*curPlyr != passer) &&            
			(Vec2DDistanceSq(passer->Pos(), (*curPlyr)->Pos()) > 
			MinPassingDistance*MinPassingDistance))                  
		{           
			if (GetBestPassToReceiver(passer, *curPlyr, Target, power))
			{
				///	�������Ŀ���ǵ�Ŀ��λ�����ҵ�����Է�����������ļ�¼��
				double Dist2Goal = fabs(Target.x - OpponentsGoal()->Center().x);

				if (Dist2Goal < ClosestToGoalSoFar)
				{
					ClosestToGoalSoFar = Dist2Goal;

					/// ��¼�����Ա
					receiver = *curPlyr;

					/// �����Ŀ��
					PassTarget = Target;
				}     
			}
		}
	}//next team member

	if (receiver) return true;

	else return false;
}


bool FootBallTeam::GetBestPassToReceiver(const BasePlayer* const passer,
									   const BasePlayer* const receiver,
									   Vector2D&               PassTarget,
									   double                   power)const
{  
	/// ���ȣ������򵽴���������ԱҪ���೤ʱ��
	double time = Pitch()->Ball()->TimeToCoverDistance(Pitch()->Ball()->Pos(),
		receiver->Pos(),
		power);

	/// ����ڸ����������������޷�ʧ�򵽴�����Ա������ؼ�
	if (time < 0) return false;

	/// ���ʱ���У������Ա�ܸ��ǵ�������
	double InterceptRange = time * receiver->MaxSpeed();

	/// ��С����Χ
	const double ScalingFactor = 0.3;
	InterceptRange *= ScalingFactor;

	/// ��������������ԱȦ�����߷�Χ�ڵĴ���Ŀ��
	Vector2D ip1, ip2;

	GetTangentPoints(receiver->Pos(),
		InterceptRange,
		Pitch()->Ball()->Pos(),
		ip1,
		ip2);

	const int NumPassesToTry = 3;
	Vector2D Passes[NumPassesToTry] = {ip1, receiver->Pos(), ip2};


	/// ĿǰΪֹ����������ѵģ����
	/// 1.ĿǰΪֹ�ҵ��ı��������Ч�����ǰ����λ��
	/// 2.��������
	/// 3.�򲻻ᱻ�Է�����

	double ClosestSoFar = MaxFloat;
	bool  bResult      = false;

	for (int pass=0; pass<NumPassesToTry; ++pass)
	{    
		double dist = fabs(Passes[pass].x - OpponentsGoal()->Center().x);

		if (( dist < ClosestSoFar) &&
			Pitch()->PlayingArea()->Inside(Passes[pass]) &&
			isPassSafeFromAllOpponents(Pitch()->Ball()->Pos(),
			Passes[pass],
			receiver,
			power))

		{
			ClosestSoFar = dist;
			PassTarget   = Passes[pass];
			bResult      = true;
		}
	}

	return bResult;
}

bool FootBallTeam::isPassSafeFromOpponent(Vector2D    from,
										Vector2D    target,
										const BasePlayer* const receiver,
										const BasePlayer* const opp,
										double       PassingForce)const
{
	/// �Ѷ����ƶ����ؿռ�
	Vector2D ToTarget = target - from;
	Vector2D ToTargetNormalized = Vec2DNormalize(ToTarget);

	Vector2D LocalPosOpp = PointToLocalSpace(opp->Pos(),
		ToTargetNormalized,
		ToTargetNormalized.Perp(),
		from);

	/// ��������������ߺ��棬��ô����Ϊ���Դ���
	/// ���ǻ��������ļ��裬�򽫱��߳����ٶ�Զ���ڶ��ֵ�����ٶ�
	if ( LocalPosOpp.x < 0 )
	{     
		return true;
	}

	/// ������ֵ�Ŀ��ľ����Զ
	/// ��ô������Ҫ���Ƕ��ֿ��Խ����Ա�ȵ����λ��
	if (Vec2DDistanceSq(from, target) < Vec2DDistanceSq(opp->Pos(), from))
	{
		/// ��������������������Ϊ���µ�����������ǣ�û�жԽ����ߵ�����
		/// ���磬�������֪�����Ƿ���Ը��ڶ���֮ǰ���ﳡ�ϵ�ĳ��λ��
		if (receiver)
		{
			if ( Vec2DDistanceSq(target, opp->Pos())  > 
				Vec2DDistanceSq(target, receiver->Pos()) )
			{
				return true;
			}

			else
			{
				return false;
			}

		}

		else
		{
			return true;
		} 
	}

	/// �������õ��������������λ��
	double TimeForBall = 
		Pitch()->Ball()->TimeToCoverDistance(Vector2D(0,0),
		Vector2D(LocalPosOpp.x, 0),
		PassingForce);

	/// ���ڼ��������ʱ���ж������ܶ�Զ
	double reach = opp->MaxSpeed() * TimeForBall +
		Pitch()->Ball()->GetSize()+
		opp->GetSize();

	/// ���������Yλ�õľ���С�������ܶ���Χ������뾶�Ͷ��ְ뾶����ô����Ա��ص�
	if ( fabs(LocalPosOpp.y) < reach )
	{
		return false;
	}

	return true;
}

bool FootBallTeam::isPassSafeFromAllOpponents(Vector2D                from,
											Vector2D                target,
											const BasePlayer* const receiver,
											double     PassingForce)const
{
	std::vector<BasePlayer*>::const_iterator opp = Opponents()->Members().begin();

	for (opp; opp != Opponents()->Members().end(); ++opp)
	{
		if (!isPassSafeFromOpponent(from, target, receiver, *opp, PassingForce))
		{
			//debug_on

				return false;
		}
	}

	return true;
}

bool FootBallTeam::CanShootGoal(Vector2D  BallPos,
						  double     power, 
						  Vector2D& ShotTarget)const
{

	/// �������Ҫ���Ե����ѡȡ������Ŀ�������
	int NumAttempts = 5;

	while (NumAttempts--)
	{
		/// ���ŶԷ�������ѡ���һ�����λ�ã�ȷ����������İ뾶��
		ShotTarget = OpponentsGoal()->Center();

		/// ����λ�õ�YֵӦ��������������֮�䣨Ҫ�������ֱ����
		int MinYVal = (int)(OpponentsGoal()->LeftPost().y + Pitch()->Ball()->GetSize());
		int MaxYVal = (int)(OpponentsGoal()->RightPost().y - Pitch()->Ball()->GetSize());

		ShotTarget.y = (double)RandInt(MinYVal, MaxYVal);

		/// ȷ���������㹻ʹ��Խ��������
		double time = Pitch()->Ball()->TimeToCoverDistance(BallPos,
			ShotTarget,
			power);

		/// �����������������������Ƿ�ᱻ�κζ��ֽص�
		if (time >= 0)
		{
			if (isPassSafeFromAllOpponents(BallPos, ShotTarget, NULL, power))
			{
				return true;
			}
		}
	}

	return false;
}


void FootBallTeam::ReturnAllFootBallerToHome()const
{
	std::vector<BasePlayer*>::const_iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		if ((*it)->Role() != BasePlayer::goal_keeper)
		{
			Dispatcher->DispatchMsg(1,(*it)->GetID(),Msg_GoHome,NULL);
		}
	}
}

void FootBallTeam::Render()const
{
	std::vector<BasePlayer*>::const_iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		(*it)->Render();
	}

	//show the controlling team and player at the top of the display
	if (GetInstObj(CGameSetup).bShowControllingTeam)
	{
		GetInstObj(CGDI).TextColor(CGDI::white);

		if ( (Color() == blue) && InControl())
		{
			GetInstObj(CGDI).TextAtPos(20,3,"Blue in Control");
		}
		else if ( (Color() == red) && InControl())
		{
			GetInstObj(CGDI).TextAtPos(20,3,"Red in Control");
		}
		if (m_pControllingPlayer != NULL)
		{
			GetInstObj(CGDI).TextAtPos(Pitch()->cxClient()-150, 3, "Controlling Player: " + ttos(m_pControllingPlayer->GetID()));
		}
	}
}

void FootBallTeam::CreatePlayers()
{
  if (Color() == blue)
  {
    //goalkeeper
     m_Players.push_back(new GoalKeeper(this,
                               1,
							   &GetInstObj(TendGoal),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale));
 
	m_Players.push_back(new FootBaller(this,
                               22,
                               "State_Wait",//&GetInstObj(Wait),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               BasePlayer::attacker));

    //create the players
    m_Players.push_back(new FootBaller(this,
                               19,
                               "State_Wait",//&GetInstObj(Wait),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               BasePlayer::attacker));



        m_Players.push_back(new FootBaller(this,
                               16,
                               "State_Wait",//&GetInstObj(Wait),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               BasePlayer::attacker));
 

        m_Players.push_back(new FootBaller(this,
                               11,
                               "State_Wait",//&GetInstObj(Wait),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                              BasePlayer::defender));

		m_Players.push_back(new FootBaller(this,
                               8,
                               "State_Wait",//&GetInstObj(Wait),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               BasePlayer::defender));

		m_Players.push_back(new FootBaller(this,
                               6,
                               "State_Wait",//&GetInstObj(Wait),
                               Vector2D(0,1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               BasePlayer::defender));


  }

  else
  {
     //goalkeeper
    m_Players.push_back(new GoalKeeper(this,
                               45,
                               &GetInstObj(TendGoal),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale));


    //create the players
    m_Players.push_back(new FootBaller(this,
                               25,
                               &GetInstObj(Wait),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               BasePlayer::attacker));

	//create the players
    m_Players.push_back(new FootBaller(this,
                               28,
                               &GetInstObj(Wait),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               BasePlayer::attacker));

    m_Players.push_back(new FootBaller(this,
                               31,
                               &GetInstObj(Wait),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               BasePlayer::attacker));


    m_Players.push_back(new FootBaller(this,
                               32,
                               &GetInstObj(Wait),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               BasePlayer::defender));

	 m_Players.push_back(new FootBaller(this,
                               35,
                               &GetInstObj(Wait),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               BasePlayer::defender));

	  m_Players.push_back(new FootBaller(this,
                               41,
                               &GetInstObj(Wait),
                               Vector2D(0,-1),
                               Vector2D(0.0, 0.0),
                               GetInstObj(CGameSetup).PlayerMass,
                               GetInstObj(CGameSetup).PlayerMaxForce,
                               1.6,
                               GetInstObj(CGameSetup).PlayerMaxTurnRate,
                               GetInstObj(CGameSetup).PlayerScale,
                               BasePlayer::defender));
                      
  }

  //register the players with the entity manager
  std::vector<BasePlayer*>::iterator it = m_Players.begin();

  for (it; it != m_Players.end(); ++it)
  {
    GetInstObj(EntityManager).RegEntity(*it);
  }
}


BasePlayer* FootBallTeam::GetPlayerFromID(int id)const
{
	std::vector<BasePlayer*>::const_iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		if ((*it)->GetID() == id) return *it;
	}

	return NULL;
}


void FootBallTeam::SetPlayerHomeRegion(int plyr, int region)const
{
	assert ( (plyr>=0) && (plyr<(int)m_Players.size()) );

	m_Players[plyr]->SetHomeRegion(region);
}

void FootBallTeam::UpdateWaitingPlayers()const
{
	std::vector<BasePlayer*>::const_iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{  
		if ( (*it)->Role() != BasePlayer::goal_keeper && Color() != blue )
		{
			FootBaller* plyr = static_cast<FootBaller*>(*it);

			if ( plyr->GetFSM()->isInState(*&GetInstObj(Wait)) ||
				plyr->GetFSM()->isInState(GetInstObj(ReturnToHomeRegion)) )
			{
				plyr->Steering()->SetTarget(plyr->HomeRegion()->Center());
			}
		}
	}
}

bool FootBallTeam::AllPlayersAtHome()const
{
	std::vector<BasePlayer*>::const_iterator it = m_Players.begin();

	for (it; it != m_Players.end(); ++it)
	{
		if ((*it)->InHomeRegion() == false)
		{
			return false;
		}
	}

	return true;
}

void FootBallTeam::RequestPass(FootBaller* requester)const
{
	if (RandFloat() > 0.1) return;

	if (isPassSafeFromAllOpponents(ControllingPlayer()->Pos(),
		requester->Pos(),
		requester,
		3))
	{

		Dispatcher->DispatchMsg(requester->GetID(),
			ControllingPlayer()->GetID(),
			Msg_PassToMe,
			&requester->Pos()); 
	}
}

bool FootBallTeam::isOpponentWithinRadius(Vector2D pos, double rad)
{
	std::vector<BasePlayer*>::const_iterator end = Opponents()->Members().end();
	std::vector<BasePlayer*>::const_iterator it;

	for (it=Opponents()->Members().begin(); it !=end; ++it)
	{
		if (Vec2DDistanceSq(pos, (*it)->Pos()) < rad*rad)
		{
			return true;
		}
	}

	return false;
}