
#pragma  once 

#include "../../Public/MsgImpl.h"

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
	Ret Class::FunName(ENTITY* entity,const tagMessage&) { return false;}

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
	virtual bool OnMessage(entity_type*, const tagMessage&)=0;
};


/// ȫ�ֶ�Ա״̬
CREATESTATE(GlobalPlayerState,tagMessage,FieldPlayer);

/// ׷��״̬
CREATESTATE(ChaseBall,tagMessage,FieldPlayer);

/// ����״̬
CREATESTATE(Dribble,tagMessage,FieldPlayer);

/// �ص�����
CREATESTATE(ReturnToHomeRegion,tagMessage,FieldPlayer);

/// �ȴ�״̬
CREATESTATE(Wait,tagMessage,FieldPlayer);

/// ������
CREATESTATE(FollowBall,tagMessage,FieldPlayer);

/// ����
CREATESTATE(KickBall,tagMessage,FieldPlayer);

///	����״̬
CREATESTATE(ReceiveBall,tagMessage,FieldPlayer);

///	��Ӧ״̬
CREATESTATE(SupportAttacker,tagMessage,FieldPlayer);

/// ȫ�ֶ�Ա״̬
CREATESTATE(GlobalKeeperState,tagMessage,GoalKeeper);

/// ������
CREATESTATE(TendGoal,tagMessage,GoalKeeper);

/// ����
CREATESTATE(InterceptBall,tagMessage,GoalKeeper);

/// ��λ
CREATESTATE(ReturnHome,tagMessage,GoalKeeper);

/// �򴫻ص�����
CREATESTATE(PutBallBackInPlay,tagMessage,GoalKeeper);

/// ����״̬
CREATESTATE(Attacking,tagMessage,SoccerTeam);

/// ����״̬
CREATESTATE(Defending,tagMessage,SoccerTeam);

/// ׼������
CREATESTATE(PrepareForKickOff,tagMessage,SoccerTeam);

/// �߽���
CREATESTATE(Throw_In,tagMessage,SoccerTeam);

