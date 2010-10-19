
#pragma  once 

#include "../../Public/Common/Autolist.h"
#include "../../Public/Singleton.h"
#include "../Script/StateMachineScript.h"
#include "../TimeCount.h"
#include "EntityManager.h"
#include "BasePlayer.h"

class FootBallTeam;
class FootBall;
class FootBall;
class Region;
class TimeCount;
template < class T>
class  StateMachine;
template < class T>
class  State;

/// �����˶�Ա
class FootBaller : public BasePlayer
{

public:

	FootBaller(FootBallTeam*    home_team,
				int			   home_region,
				State<FootBaller>* start_state,
				Vector2D	   heading,
				Vector2D       velocity,
				double         mass,
				double         max_force,
				double         max_speed,
				double         max_turn_rate,
				double         scale,
				player_role    role); 

	FootBaller(FootBallTeam*    home_team,
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

	~FootBaller();

	void        Update();   

	void        Render();

	bool        OnMessage(const tagMessage& msg);

	StateMachine<FootBaller>* GetFSM()const{return m_pStateMachine;}

	StateMachineScript<FootBaller>* GetScriptFSM()const{return m_pStateMachineScript;}
								 
	/// ������kick��ʱ���´δ��������׼��ʱ���Ƿ��Ѿ�����
	bool        isReadyForNextKick()const{return m_pKickLimiter->isReadyOK();}

	long		GetScriptValue() ;

	void		SetScriptValue(long value){ m_ScriptValue = value;}

private:
	
	/// ��ǰ�ű�����ֵ
	long  m_ScriptValue;

	/// ״̬��
	StateMachine<FootBaller>*  m_pStateMachine;

	/// �ű�״̬��
	StateMachineScript<FootBaller>* m_pStateMachineScript;

	/// �������޴������������
	TimeCount*                  m_pKickLimiter;

};

inline FootBaller*	GetExtraInfoField(const tagMessage* msg)	
{	
	return (FootBaller*)msg->ExtraInfo;
}

inline long   GetGlobalEntityID( int Revid )
{
	FootBaller*  player = static_cast<FootBaller*>( GetInstObj(EntityManager).FindEntity( Revid ) );

	if ( player != NULL )
	{
		return player->GetID();
	}

	return 0;
}