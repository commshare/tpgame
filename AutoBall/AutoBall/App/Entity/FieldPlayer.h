
#pragma  once 

#include "../../Public/Common/Autolist.h"
#include "../../Public/Singleton.h"
#include "../Script/StateMachineScript.h"
#include "../TimeCount.h"
#include "EntityManager.h"
#include "PlayerBase.h"

class SoccerTeam;
class SoccerPitch;
class SoccerBall;
class Region;
class TimeCount;
template < class T>
class  StateMachine;
template < class T>
class  State;

/// �����˶�Ա
class FieldPlayer : public PlayerBase
{

public:

	FieldPlayer(SoccerTeam*    home_team,
				int			   home_region,
				State<FieldPlayer>* start_state,
				Vector2D	   heading,
				Vector2D       velocity,
				double         mass,
				double         max_force,
				double         max_speed,
				double         max_turn_rate,
				double         scale,
				player_role    role); 

	FieldPlayer(SoccerTeam*    home_team,
				int			   home_region,
				string		   State,
				Vector2D	   heading,
				Vector2D       velocity,
				double         mass,
				double         max_force,
				double         max_speed,
				double         max_turn_rate,
				double         scale,
				player_role    role);

	~FieldPlayer();

	void        Update();   

	void        Render();

	bool        HandleMessage(const tagMessage& msg);

	StateMachine<FieldPlayer>* GetFSM()const{return m_pStateMachine;}

	StateMachineScript<FieldPlayer>* GetScriptFSM()const{return m_pStateMachineScript;}
								 
	/// ������kick��ʱ���´δ��������׼��ʱ���Ƿ��Ѿ�����
	bool        isReadyForNextKick()const{return m_pKickLimiter->isReadyOK();}

	long		GetScriptValue() ;

	void		SetScriptValue(long value){ m_ScriptValue = value;}

private:
	
	/// ��ǰ�ű�����ֵ
	long  m_ScriptValue;

	/// ״̬��
	StateMachine<FieldPlayer>*  m_pStateMachine;

	/// �ű�״̬��
	StateMachineScript<FieldPlayer>* m_pStateMachineScript;

	/// �������޴������������
	TimeCount*                  m_pKickLimiter;

};

inline FieldPlayer*	GetExtraInfoField(const tagMessage* msg)	
{	
	return (FieldPlayer*)msg->ExtraInfo;
}

inline long   GetGlobalEntityID( int Revid )
{
	FieldPlayer*  player = static_cast<FieldPlayer*>( GetInstObj(EntityManager).FindEntity( Revid ) );

	if ( player != NULL )
	{
		return player->GetID();
	}

	return 0;
}