#pragma once

// ʵ������
enum	eEntityType
{
	Ent_InValid	=	-1,
	Ent_Goal  ,			 /// ����Ա
	Ent_FootBaller		 /// ��Ա
};

enum   ePlayerType
{
	default_entity_type,
	ePlayer_GoalKeeper,
	ePlayer_Attacker,
	ePlayer_Defender
};

// ״̬����
enum	eStateAi
{
	Ent_InValidState= -1,
	Ent_PeaceState	   ,
	Ent_FleeState	   ,
	Ent_FightState	   ,
	Ent_FollowState	   ,
	Ent_DeadState	   ,
	Ent_GoHomeState	   ,


	Ent_StateNumer			
};

enum  eEventType
{
	Msg_ReceiveBall,
	Msg_PassToMe,
	Msg_SupportAttacker,
	Msg_GoHome,
	Msg_Wait
};
