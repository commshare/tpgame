
#pragma  once 

#include <string>
#include "State.h"




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

