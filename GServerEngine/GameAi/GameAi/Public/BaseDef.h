#pragma once

// ʵ������
enum	eEntityType
{
	Ent_InValid	=	-1,
	Ent_Monster		  ,
	Ent_Player	
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