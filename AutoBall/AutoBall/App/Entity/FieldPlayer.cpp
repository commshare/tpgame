#include "Stdafx.h"
#include "PlayerBase.h"
#include "FieldPlayer.h"
#include "GoalKeeper.h"
#include "EntityFun.h"
#include "../Region.h"
#include "../SoccerTeam.h"
#include "../SoccerPitch.h"
#include "../Goal.h"		
#include "../SteeringBehaviors.h"
#include "../Messageing/MessageDispatcher.h"
#include "../StateAi/StateMachine.h"
#include "../StateAi/State.h"
#include "../../Render/Transformations.h"
#include "../../Render/Vector2D.h"
#include "../../Render/VGdi.h"
#include "../../Render/Geometry.h"
#include "../../Render/Utils.h"


FieldPlayer::~FieldPlayer()
{
	SAFE_DELETE(m_pKickLimiter);
	SAFE_DELETE(m_pStateMachine);
}

FieldPlayer::FieldPlayer(SoccerTeam* home_team,
						 int   home_region,
						 State<FieldPlayer>* start_state,
						 Vector2D  heading,
						 Vector2D velocity,
						 double    mass,
						 double    max_force,
						 double    max_speed,
						 double    max_turn_rate,
						 double    scale,
						 player_role role): PlayerBase(home_team,
						 home_region,
						 heading,
						 velocity,
						 mass,
						 max_force,
						 max_speed,
						 max_turn_rate,
						 scale,
						 role)                                    
{
	//set up the state machine
	m_pStateMachine =  new StateMachine<FieldPlayer>(this);

	if (start_state)
	{    
		m_pStateMachine->SetCurrentState(start_state);
		m_pStateMachine->SetPreviousState(start_state);
		m_pStateMachine->SetGlobalState(&GetInstObj(GlobalPlayerState));

		m_pStateMachine->CurrentState()->Enter(this);
	}    

	m_pSteering->SeparationOn();

	//set up the kick regulator
	m_pKickLimiter = new Regulator(GetInstObj(CGameSetup).PlayerKickFrequency);
}

void FieldPlayer::Update()
{ 
   if( GetID() == 5 )
   {
	  char  str[256];
	  sprintf_s(str,"Position x=%f,y=%f",m_vPosition.x,m_vPosition.y);
	  char  p;
	  p='1';
   }
	m_pStateMachine->Update();

	m_pSteering->Calculate();

	/// ���û�в��������ṩһ��������
	if (m_pSteering->Force().IsZero())
	{
		const double BrakingRate = 0.8; 

		m_vVelocity = m_vVelocity * BrakingRate;                                     
	}

	/// ���������ת��������
	double TurningForce =   m_pSteering->SideComponent();

	Clamp(TurningForce, -GetInstObj(CGameSetup).PlayerMaxTurnRate, GetInstObj(CGameSetup).PlayerMaxTurnRate);

	/// ��ת
	Vec2DRotateAroundOrigin(m_vHeading, TurningForce);

	/// ���µ�ǰ�ٶ�
	m_vVelocity = m_vHeading * m_vVelocity.Length();

	m_vSide = m_vHeading.Perp();


	Vector2D accel = m_vHeading * m_pSteering->ForwardComponent() / m_dMass;

	m_vVelocity += accel;

	/// ��������ٶ�
	m_vVelocity.Truncate(m_dMaxSpeed);
	m_vPosition += m_vVelocity;

	//enforce a non-penetration constraint if desired
	if(GetInstObj(CGameSetup).bNonPenetrationConstraint)
	{
		EnforceNonPenetrationContraint(this, CAutoList<PlayerBase>::GetAllMembers());
	}
}


bool FieldPlayer::HandleMessage(const tagMessage& msg)
{
	return m_pStateMachine->HandleMessage(msg);
}

void FieldPlayer::Render()                                         
{
	GetInstObj(CGDI).TransparentText();
	GetInstObj(CGDI).TextColor(CGDI::grey);

	//set appropriate team color
	if (Team()->Color() == SoccerTeam::blue)
	{
		GetInstObj(CGDI).BluePen();
	}
	else
	{
		GetInstObj(CGDI).RedPen();
	}


	//render the player's body
	m_vecPlayerVBTrans = WorldTransform(m_vecPlayerVB,
										Pos(),
										Heading(),
										Side(),
										Scale());  
	GetInstObj(CGDI).ClosedShape(m_vecPlayerVBTrans); /**/ 

	//and 'is 'ead
	GetInstObj(CGDI).BrownBrush();
	if (GetInstObj(CGameSetup).bHighlightIfThreatened && 
		(Team()->ControllingPlayer() == this) && 
		isThreatened() ) 
		GetInstObj(CGDI).YellowBrush();

	GetInstObj(CGDI).Circle(Pos(), 5);

	/**
		GetInstObj(CGDI).TextColor(255, 0, 0);
		GetInstObj(CGDI).TextAtPos(Pos().x-3, Pos().y-5, ttos(GetID()) );
	 */

	//render the state
	if (GetInstObj(CGameSetup).bStates)
	{  
		GetInstObj(CGDI).TextColor(0, 170, 0);
		GetInstObj(CGDI).TextAtPos(m_vPosition.x, m_vPosition.y -20, std::string(m_pStateMachine->GetNameOfCurrentState()));
	}

	//show IDs
	if (GetInstObj(CGameSetup).bIDs)
	{
		GetInstObj(CGDI).TextColor(0, 170, 0);
		GetInstObj(CGDI).TextAtPos(Pos().x-20, Pos().y-20, ttos(GetID()));
	}


	if (GetInstObj(CGameSetup).bViewTargets)
	{
		GetInstObj(CGDI).RedBrush();
		GetInstObj(CGDI).Circle(Steering()->Target(), 3);
		GetInstObj(CGDI).TextAtPos(Steering()->Target(), ttos(GetID()));
	} 
}
