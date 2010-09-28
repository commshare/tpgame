#include "Stdafx.h"
#include "TeamStates.h"
#include "StateMachine.h"
#include "State.h"
#include "../SoccerTeam.h"
#include "../Entity/PlayerBase.h"
#include "../SoccerPitch.h"
#include "../Messageing/MessageDispatcher.h"
#include "../Messageing/SoccerMessages.h"
#include "../../Config.h"			   

extern SoccerPitch* g_SoccerPitch;

void ChangePlayerHomeRegions(SoccerTeam* team, const int NewRegions[TeamSize])
{
	for (int plyr=0; plyr<TeamSize; ++plyr)
	{
		team->SetPlayerHomeRegion(plyr, NewRegions[plyr]);
	}
}

//************************************************************************ ATTACKING

Attacking* Attacking::Instance()
{
	static Attacking instance;

	return &instance;
}


void Attacking::Enter(SoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
	debug_con << team->Name() << " entering Attacking state" << "";
#endif

	//these define the home regions for this state of each of the players
	//these define the home regions for this state of each of the players
	const int BlueRegions[TeamSize] = {1,22,19,16,11,8,6};
	const int RedRegions[TeamSize]  = {45,25,28,31,32,35,41};

	//set up the player's home regions
	if (team->Color() == SoccerTeam::blue)
	{
		ChangePlayerHomeRegions(team, BlueRegions);
	}
	else
	{
		ChangePlayerHomeRegions(team, RedRegions);
	}

	//if a player is in either the Wait or ReturnToHomeRegion states, its
	//steering target must be updated to that of its new home region to enable
	//it to move into the correct position.
	team->UpdateTargetsOfWaitingPlayers();
}


void Attacking::Execute(SoccerTeam* team)
{
	/// ������鲻�ڿ��򣬸ı�״̬
	//if this team is no longer in control change states
	if (!team->InControl())
	{
		team->GetFSM()->ChangeState(Defending::Instance()); return;
	}

	/// ����Ӧ��Ա�������λ��
	//calculate the best position for any supporting attacker to move to
	team->DetermineBestSupportingPosition();
}

void Attacking::Exit(SoccerTeam* team)
{
	//there is no supporting player for defense
	team->SetSupportingPlayer(NULL);
}



//************************************************************************ DEFENDING

Defending* Defending::Instance()
{
	static Defending instance;

	return &instance;
}

void Defending::Enter(SoccerTeam* team)
{
#ifdef DEBUG_TEAM_STATES
	debug_con << team->Name() << " entering Defending state" << "";
#endif

	//these define the home regions for this state of each of the players
  const int BlueRegions[TeamSize] = {1,22,19,16,11,8,6};
  const int RedRegions[TeamSize]  = {45,25,28,31,32,35,41};

	//set up the player's home regions
	if (team->Color() == SoccerTeam::blue)
	{
		ChangePlayerHomeRegions(team, BlueRegions);
	}
	else
	{
		ChangePlayerHomeRegions(team, RedRegions);
	}

	//if a player is in either the Wait or ReturnToHomeRegion states, its
	//steering target must be updated to that of its new home region
	team->UpdateTargetsOfWaitingPlayers();
}

void Defending::Execute(SoccerTeam* team)
{
	//if in control change states
	if (team->InControl())
	{
		team->GetFSM()->ChangeState(Attacking::Instance()); return;
	}
}


void Defending::Exit(SoccerTeam* team){}


//************************************************************************ KICKOFF
PrepareForKickOff* PrepareForKickOff::Instance()
{
	static PrepareForKickOff instance;

	return &instance;
}

void PrepareForKickOff::Enter(SoccerTeam* team)
{
	/// ���ùؼ������ָ��
	//reset key player pointers
	team->SetControllingPlayer(NULL);
	team->SetSupportingPlayer(NULL);
	team->SetReceiver(NULL);
	team->SetPlayerClosestToBall(NULL);

	//send Msg_GoHome to each player.
	team->ReturnAllFieldPlayersToHome();
}

void PrepareForKickOff::Execute(SoccerTeam* team)
{
	//if both teams in position, start the game
	if (team->AllPlayersAtHome() && team->Opponents()->AllPlayersAtHome())
	{
		team->GetFSM()->ChangeState(Defending::Instance());
	}
}

void PrepareForKickOff::Exit(SoccerTeam* team)
{
	team->Pitch()->SetGameOn();
}



//************************************************************************ KICKOFF
Throw_In* Throw_In::Instance()
{
	static Throw_In instance;

	return &instance;
}

void Throw_In::Enter(SoccerTeam* team)
{
	team->SetThrowIn(true);
}

void Throw_In::Execute(SoccerTeam* team)
{
	//if both teams in position, start the game
	if ( !team->IsThrowIn() )
	{
		team->GetFSM()->ChangeState(Attacking::Instance());
	}
}

void Throw_In::Exit(SoccerTeam* team)
{
	g_SoccerPitch->m_pBlueTeam->SetChaseBall(true);
	g_SoccerPitch->m_pRedTeam->SetChaseBall(true);

	team->SetThrowIn(false);
}
