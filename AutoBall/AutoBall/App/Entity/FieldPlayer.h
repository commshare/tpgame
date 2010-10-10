
#pragma  once 

#include "../../Public/Common/Autolist.h"
#include "../Regulator.h"

class SoccerTeam;
class SoccerPitch;
class SoccerBall;
class Region;
class Regulator;
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

	~FieldPlayer();

	void        Update();   

	void        Render();

	bool        HandleMessage(const Telegram& msg);

	StateMachine<FieldPlayer>* GetFSM()const{return m_pStateMachine;}
								 
	/// ������kick��ʱ���´δ��������׼��ʱ���Ƿ��Ѿ�����
	bool        isReadyForNextKick()const{return m_pKickLimiter->isReady();}

private:

	/// ״̬��
	StateMachine<FieldPlayer>*  m_pStateMachine;

	/// �������޴������������
	Regulator*                  m_pKickLimiter;


};