
#pragma  once 

#include "../Telegram.h"

class SoccerTeam;
class GoalKeeper;
class SoccerPitch;
class FieldPlayer;

#define  DIS(s)		#s
#define  DISTROY(s) DIS(s)

#define  EntityFun(Ret,FunName,Entity)\
	Ret  FunName(Entity* entity)
#define  MessageFun(Ret,FunName,Entity,Msg)\
	Ret  FunName(Entity* entity,const Msg&)

#define  EmptyFun(Ret,Class,FunName,ENTITY)\
	Ret Class::FunName(ENTITY* type) {}

#define  EmptyMsg(Ret,Class,FunName,ENTITY)\
	Ret Class::FunName(ENTITY* entity,const Telegram&) { return false;}

#define  CLASSFUNTION(CLASS,ENTITY,MSG)\
	   EntityFun(void,Enter,ENTITY);\
	   EntityFun(void,Execute,ENTITY);\
	   EntityFun(void,Exit,ENTITY);\
	   MessageFun(bool,OnMessage,ENTITY,MSG)

#define  CREATESTATE(CLASSNAME,MSG,Player) \
	class CLASSNAME : public State<Player> \
	{ \
	public:\
		CLASSFUNTION(GlobalPlayerState,Player,MSG);\
	}

template <class entity_type>
class State
{
public:

	virtual ~State()	{}

	//this will execute when the state is entered
	virtual void Enter(entity_type*)=0;

	//this is the states normal update function
	virtual void Execute(entity_type*)=0;

	//this will execute when the state is exited. 
	virtual void Exit(entity_type*)=0;

	//this executes if the agent receives a message from the 
	//message dispatcher
	virtual bool OnMessage(entity_type*, const Telegram&)=0;
};


/// ȫ�ֶ�Ա״̬
CREATESTATE(GlobalPlayerState,Telegram,FieldPlayer);

/// ׷��״̬
CREATESTATE(ChaseBall,Telegram,FieldPlayer);

/// ����״̬
CREATESTATE(Dribble,Telegram,FieldPlayer);

/// �ص�����
CREATESTATE(ReturnToHomeRegion,Telegram,FieldPlayer);

/// �ȴ�״̬
CREATESTATE(Wait,Telegram,FieldPlayer);

/// ������
CREATESTATE(FollowBall,Telegram,FieldPlayer);

/// ����
CREATESTATE(KickBall,Telegram,FieldPlayer);

///	����״̬
CREATESTATE(ReceiveBall,Telegram,FieldPlayer);

///	��Ӧ״̬
CREATESTATE(SupportAttacker,Telegram,FieldPlayer);

/// ȫ�ֶ�Ա״̬
CREATESTATE(GlobalKeeperState,Telegram,GoalKeeper);

/// ������
CREATESTATE(TendGoal,Telegram,GoalKeeper);

/// ����
CREATESTATE(InterceptBall,Telegram,GoalKeeper);

/// ��λ
CREATESTATE(ReturnHome,Telegram,GoalKeeper);

/// �򴫻ص�����
CREATESTATE(PutBallBackInPlay,Telegram,GoalKeeper);

/// ����״̬
CREATESTATE(Attacking,Telegram,SoccerTeam);

/// ����״̬
CREATESTATE(Defending,Telegram,SoccerTeam);

/// ׼������
CREATESTATE(PrepareForKickOff,Telegram,SoccerTeam);

/// �߽���
CREATESTATE(Throw_In,Telegram,SoccerTeam);

