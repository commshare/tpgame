
#pragma  once 

#include "../../Public/MsgImpl.h"

class FootBallTeam;
class GoalKeeper;
class FootBall;
class FootBaller;

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

template <class entity>
class State
{
public:

	virtual ~State()	{}

	virtual void Enter(entity*)=0;

	virtual void Execute(entity*)=0;

	virtual void Exit(entity*)=0;

	virtual bool OnMessage(entity*, const tagMessage&)=0;
};


/// ȫ�ֶ�Ա״̬
CREATESTATE(GlobalPlayerState,tagMessage,FootBaller);

/// ׷��״̬
CREATESTATE(ChaseBall,tagMessage,FootBaller);

/// ����״̬
CREATESTATE(Dribble,tagMessage,FootBaller);

/// �ص�����
CREATESTATE(ReturnToHomeRegion,tagMessage,FootBaller);

/// �ȴ�״̬
CREATESTATE(Wait,tagMessage,FootBaller);

/// ������
CREATESTATE(FollowBall,tagMessage,FootBaller);

/// ����
CREATESTATE(KickBall,tagMessage,FootBaller);

///	����״̬
CREATESTATE(ReceiveBall,tagMessage,FootBaller);

///	��Ӧ״̬
CREATESTATE(SupportAttacker,tagMessage,FootBaller);

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
CREATESTATE(Attacking,tagMessage,FootBallTeam);

/// ����״̬
CREATESTATE(Defending,tagMessage,FootBallTeam);

/// ׼������
CREATESTATE(PrepareForKickOff,tagMessage,FootBallTeam);

/// �߽���
CREATESTATE(Throw_In,tagMessage,FootBallTeam);

