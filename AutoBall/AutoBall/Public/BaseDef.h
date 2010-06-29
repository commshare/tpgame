#pragma once

// ʵ������
enum	eEntityType
{
	Ent_InValid	=	-1,
	Ent_Goal  ,			 /// ����Ա
	Ent_FieldPlayer		 /// ��Ա
};

enum   ePlayerType
{
	ePlayer_GoalKeeper,
	ePlayer_Attacker,
	ePlayer_Defender
};

// ״̬����
enum	eStateAi
{
	Ent_InValidState=-1,
	Ent_PeaceState	   ,
	Ent_FleeState	   ,
	Ent_FightState	   ,
	Ent_FollowState	   ,
	Ent_DeadState	   ,
	Ent_GoHomeState	   ,


	Ent_StateNumer			
};


// �¼�����
enum	eEventAi
{
	Ent_InValidEvent=-1,
	Ent_MoveEvent	= 0,
	Ent_StopEvent	   ,
	Ent_UseSkillEvent  ,


	Ent_EventNum
};


struct  tagEevent
{
	void*		Sender;
	void*		Receiver;

	eEventAi	eEvent;
};