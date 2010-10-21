
#pragma  once 

#include <vector>
#include "../Render/Vector2D.h"
#include "SpotCalculator.h"
#include "Goal.h"

class BasePlayer;
class FootBaller;
class FootBallPitch;
class GoalKeeper;
class SpotCalculator;
template<class T>
class StateMachine;
class SpotCalculator;
	  
class FootBallTeam 
{
public:

	enum team_color {blue, red};		  /// �������

public:

	FootBallTeam(Goal*  home_goal,
			   Goal*  opponents_goal,
			   FootBallPitch* pitch,
			   team_color   color);

	~FootBallTeam();

	void        Render()const;
	void        Update();

	/// �����еĶ�Ա��λ
	void        ReturnAllFootBallerToHome()const;

	///  �����Ƿ������Ž���
	bool        CanShootGoal(Vector2D  BallPos,
						 double    power, 
						 Vector2D& ShotTarget = Vector2D())const;

	///
	/// ��Ա���ôη����������Ƿ��ܹ�����������ԣ���ô˭����ѵĴ������
	/// ʲôλ��������ѵĴ���λ��
	/// 
	bool        FindPass(const BasePlayer*const passer,
						 BasePlayer*&  receiver,
						 Vector2D&     PassTarget,
						 double        power,
						 double        MinPassingDistance)const;

	/// 
	int		    FindRecvPass(const BasePlayer*const passer,
							 Vector2D& PassTarget,
							 double power,
							 double MinPassingDistance);

	/// ����һ�������ߺ�һ����Ӧ�ߣ�����������Խ����߸��������ɲ�ͬλ�ã�
	/// �ж��Ƿ���԰�ȫ�Ĵ���������Դ���
	/// ��ô�÷�������Ѵ��򱣴��ڲ���PassTarget��
	bool        GetBestPassToReceiver(const BasePlayer* const passer,
									  const BasePlayer* const receiver,
									  Vector2D& PassTarget,
									  const double power)const;

	/// ����FORM��target�Է���Աopp�Ƿ���Զ�������
	bool        isPassSafeFromOpponent( Vector2D    from,
										Vector2D    target,
										const BasePlayer* const receiver,
										const BasePlayer* const opp,
										double       PassingForce)const;

	/// ����FORM��target�Է����е���Ա�Ƿ���Զ�������
	bool        isPassSafeFromAllOpponents(Vector2D from,
						Vector2D target,
						const BasePlayer* const receiver,
						double     PassingForce)const;

	/// �Է���Ա���ж�Ա�ķ�����Χ
	bool        isOpponentWithinRadius(Vector2D pos, double rad);

	/// ��������Ա����
	void        RequestPass(FootBaller* requester)const;

	/// ������Ѵ���λ�ã�����������ʵĶԷ���Աǰ������ص�
	BasePlayer* CalcBestSupportingAttacker();

	/// �õ����еĶ��ڳ�Ա
	const std::vector<BasePlayer*>& Members()const{return m_Players;}  
	StateMachine<FootBallTeam>* GetFSM()const {return m_pStateMachine;}   
	Goal*const           HomeGoal()const	{return m_pHomeGoal;}
	Goal*const           OpponentsGoal()const{return m_pOpponentsGoal;}
	bool				 AllPlayersAtHome()const;
	FootBallPitch*const  Pitch()const		{return m_pPitch;}           
	FootBallTeam*const   Opponents()const	{return m_pOpponents;}

	team_color           Color()const{return m_Color;}

	void                 SetOpponents(FootBallTeam* opps){m_pOpponents = opps;}
	void                 SetPlayerClosestToBall(BasePlayer* plyr){m_pPlayerClosestToBall=plyr;}

	BasePlayer*          PlayerClosestToBall()const{return m_pPlayerClosestToBall;}
	double               ClosestDistToBallSq()const{return m_dDistSqToBallOfClosestPlayer;}

	Vector2D             GetSupportSpot()const{return m_pSupportSpotCalc->GetBestSupportingSpot();}

	BasePlayer*          SupportingPlayer()const{return m_pSupportingPlayer;}
	void                 SetSupportingPlayer(BasePlayer* plyr=NULL){m_pSupportingPlayer = plyr;}
	void                 SetSupportingNULLPlayer()				   {m_pSupportingPlayer = NULL;}

	BasePlayer*          Receiver()const	{ return m_pReceivingPlayer;	   }
	bool				 IsReceiver() const { return m_pReceivingPlayer!=NULL; }

	void                 SetReceiver(BasePlayer* plyr){m_pReceivingPlayer = plyr;}
	void				 SetNullReceiver()			  {m_pReceivingPlayer = NULL;}

	BasePlayer*          ControllingPlayer()const{return m_pControllingPlayer;}
	void                 SetControllingPlayer(BasePlayer* plyr)
	{
		m_pControllingPlayer = plyr;
		Opponents()->LostControl();
	}

	bool  InControl()const{
		if(m_pControllingPlayer)
			return true; 
		else return false;
	}

	bool IsControl()const 
	{
		if(m_pControllingPlayer)
			return true; 
		else return false;
	}
	void  LostControl(){m_pControllingPlayer = NULL;}
	BasePlayer*  GetPlayerFromID(int id)const;


	void SetPlayerHomeRegion(int plyr, int region)const;
	void BestSupportingPosition()const
	{
		m_pSupportSpotCalc->BestSupportingPosition();
	}

	void UpdateWaitingPlayers()const;

	std::string Name() const 
	{
		if (m_Color == blue) 
			return "Blue"; 
		return "Red";
	}

	bool  IsChaseBall() const	   { return m_IsChase; }
	void  SetChaseBall(bool chase) { m_IsChase = chase;}
	bool  IsThrowIn() const	       { return m_IsThrowIn; }
	void  SetThrowIn(bool throwin) { m_IsThrowIn = throwin;}

	void  SetThrow(int value)      { SetThrowIn(false);}

	Vector2D  HomeGoalFacing()	   { return m_pHomeGoal->Facing();}

private:
	///> ������Ա
	void CreatePlayers();

	///> ÿһ֡���㵱ǰ�����������Ա
	void CalculateClosestPlayerToBall();			

	bool m_IsChase; 
	bool m_IsThrowIn;

private:

	StateMachine<FootBallTeam>*  m_pStateMachine;		 ///> ��ǰ�����״̬��
	team_color					 m_Color;				 ///> ������ɫ
	std::vector<BasePlayer*>	 m_Players;				 ///> �������еĳ�Ա
	GoalKeeper*					 m_GoalKeeper;			 ///> ����Ա
	FootBallPitch*				 m_pPitch;				 ///> ָ�����ָ��
	Goal*						 m_pOpponentsGoal;		 ///> �Է�����
	Goal*						 m_pHomeGoal;			 ///> �Լ�����
	FootBallTeam*				 m_pOpponents;			 ///> �жԶ���ָ��
	BasePlayer*                  m_pControllingPlayer;	 ///> �����Ա
	BasePlayer*					 m_pSupportingPlayer;	 ///> ��Ӧ��Ա
	BasePlayer*					 m_pReceivingPlayer;	 ///> ָ������Ա
	BasePlayer*					 m_pPlayerClosestToBall; ///> ָ����������Ķ�Ա
	SpotCalculator*       m_pSupportSpotCalc;	 ///>  �������߶�Ա����Ϊ��

	double                    m_dDistSqToBallOfClosestPlayer;///> �������Ա����ľ���


};