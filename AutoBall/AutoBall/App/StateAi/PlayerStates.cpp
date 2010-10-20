#include "Stdafx.h"
#include "State.h"
#include "StateMachine.h"
#include "../Entity/BasePlayer.h"
#include "../Entity/FootBaller.h"	  
#include "../Goal.h"
#include "../FootBallTeam.h"
#include "../FootBallPitch.h"
#include "../Messageing/MessageDispatcher.h"
#include "../Steering.h"
#include "../../Public/MsgImpl.h"
#include "../../Config.h"	


#define PLAYER_STATE_INFO_ON

EmptyFun(void,GlobalPlayerState,Enter,FootBaller);
EmptyFun(void,GlobalPlayerState,Exit,FootBaller);

void GlobalPlayerState::Execute(FootBaller* player)                                     
{
	if( !player->Team()->IsChaseBall() ) return;

	/// �����Առ�в��ӽ�����ô������������ٶ�
	if((player->BallWithinReceivingRange()) && (player->isControllingPlayer()))
	{
		player->SetMaxSpeed(GetInstObj(CGameSetup).PlayerMaxSpeedWithBall);
	}
	else
	{
		player->SetMaxSpeed(GetInstObj(CGameSetup).PlayerMaxSpeedWithoutBall);
	}

}

bool GlobalPlayerState::OnMessage(FootBaller* player, const tagMessage& telegram)
{
	switch(telegram.Msg)
	{
	case Msg_ReceiveBall:
		{
			player->Steering()->SetTarget(*(static_cast<Vector2D*>(telegram.ExtraInfo)));

			player->GetFSM()->ChangeState(&GetInstObj(ReceiveBall));

			return true;
		}

		break;

	case Msg_SupportAttacker:
		{
			//����Ѿ��ڽ�Ӧ����������
			if (player->GetFSM()->isInState(GetInstObj(SupportAttacker)))
			{
				return true;
			}

			/// ����Ŀ����ѽ�Ӧλ��
			player->Steering()->SetTarget(player->Team()->GetSupportSpot());

			player->GetFSM()->ChangeState(&GetInstObj(SupportAttacker));

			return true;
		}

		break;

	case Msg_Wait:
		{
			//change the state
			player->GetFSM()->ChangeState(&GetInstObj(Wait));

			return true;
		}

		break;

	case Msg_GoHome:
		{
			player->SetDefaultHomeRegion();

			player->GetFSM()->ChangeState(&GetInstObj(ReturnToHomeRegion));

			return true;
		}

		break;

	case Msg_PassToMe:
		{  
			/// �õ�������Ķ�Աλ��
			FootBaller* receiver = static_cast<FootBaller*>(telegram.ExtraInfo);

			/// ���������Ա�ɴ����ķ�Χ���������Ա���߱����ŵ��������Ķ�Ա���ܴ���������Ա
			if (player->Team()->Receiver() != NULL ||
				!player->BallWithinKickingRange() )
			{
				return true;
			}

			//����
			player->Ball()->Kick(receiver->Pos() - player->Ball()->Pos(),
				GetInstObj(CGameSetup).MaxPassingForce,player);

			///֪ͨ�����Ա����ʼ����
			Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
				player->GetID(),
				receiver->GetID(),
				Msg_ReceiveBall,
				&receiver->Pos());

			player->GetFSM()->ChangeState(&GetInstObj(Wait));

			player->FindSupport();

			return true;
		}
		break;
	}

	return false;
}

EmptyMsg(bool,ChaseBall,OnMessage,FootBaller);

void ChaseBall::Enter(FootBaller* player)
{
	if( !player->Team()->IsChaseBall() ) return;
	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 ChaseBall x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2  )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 ChaseBall x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	player->Steering()->SeekOn();
}

void ChaseBall::Execute(FootBaller* player)                                     
{
	/// ������������ߵ� ����ô�ö�Ա�ı�״̬ΪKickBall		  BallWithinReceivingRange
	if (player->BallWithinKickingRange())
	{
		player->GetFSM()->ChangeState(&GetInstObj(KickBall));

		return;
	}
	
	/// ��������Ա�������������׷��
	if (player->isClosestTeamMemberToBall())
	{
		player->Steering()->SetTarget(player->Ball()->Pos());

		return;
	}

	/// ����ö�Ա���������������ô��Ӧ�ûص��Լ��ĳ�ʼλ�ã��ȴ���һ��
	player->GetFSM()->ChangeState(&GetInstObj(ReturnToHomeRegion));
}


void ChaseBall::Exit(FootBaller* player)
{
	player->Steering()->SeekOff();
}


void SupportAttacker::Enter(FootBaller* player)
{
	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 SupportAttacker x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 SupportAttacker x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	player->Steering()->ArriveOn();

	player->Steering()->SetTarget(player->Team()->GetSupportSpot());

}

EmptyMsg(bool,SupportAttacker,OnMessage,FootBaller);

void SupportAttacker::Execute(FootBaller* player)                                     
{
	if( !player->Team()->IsChaseBall() ) return;

	/// ����Լ���Ӷ�ʧ����Ŀ���Ȩ����Ա�ص���ʼ����
	if (!player->Team()->InControl())
	{
		player->GetFSM()->ChangeState(&GetInstObj(ReturnToHomeRegion)); return;
	} 

	/// �����ѽ�Ӧ��ı��ˣ���ô�ı�ٿ�Ŀ��
	if (player->Team()->GetSupportSpot() != player->Steering()->Target())
	{    
		player->Steering()->SetTarget(player->Team()->GetSupportSpot());

		player->Steering()->ArriveOn();
	}

	/// �����ô��Ա�������ţ��н�����Ա���԰��򴫸�������ô���򴫸��ö�Ա
	if( player->Team()->CanShootGoal(player->Pos(),
		GetInstObj(CGameSetup).MaxShootingForce))
	{
		player->Team()->RequestPass(player);
	}

	/// ���������Ա�ڽ�Ӧ�㣬����������Կ�������Ӧ��ͣ���Ƕ���ת���������
	if (player->AtTarget())
	{
		player->Steering()->ArriveOff();

		/// ��Ա������
		player->TrackBall();

		player->SetVelocity(Vector2D(0,0));

		/// ���û���յ�������Ա����в����ô������
		if (!player->isThreatened())
		{
			player->Team()->RequestPass(player);
		}
	}
}


void SupportAttacker::Exit(FootBaller* player)
{
	player->Team()->SetSupportingPlayer();

	player->Steering()->ArriveOff();
}




void ReturnToHomeRegion::Enter(FootBaller* player)
{
	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 ReturnToHomeRegion x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 ReturnToHomeRegion x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	player->Steering()->ArriveOn();

	if (!player->InsideHomeRegion(player->Steering()->Target(), Region::halfsize))
	{
		player->Steering()->SetTarget(player->GetHomeCenter());
	}

}


EmptyMsg(bool,ReturnToHomeRegion,OnMessage,FootBaller);

void ReturnToHomeRegion::Execute(FootBaller* player)
{
	if( !player->Team()->IsChaseBall() ) return;

	if (player->Pitch()->GameOn())
	{
		if ( player->isClosestTeamMemberToBall() &&
			player->Team()->IsReceiver() &&
			!player->Pitch()->GoalKeeperHasBall())
		{
			player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

			return;
		}
	}

	if (player->Pitch()->GameOn() && player->HomeRegion()->Inside(player->Pos(),
		Region::halfsize))
	{
		player->Steering()->SetTarget(player->Pos());
		player->GetFSM()->ChangeState(&GetInstObj(Wait));
	}
	else if(!player->Pitch()->GameOn() && player->AtTarget())
	{
		player->GetFSM()->ChangeState(&GetInstObj(Wait));
	}
}

void ReturnToHomeRegion::Exit(FootBaller* player)
{
	player->Steering()->ArriveOff();
}


EmptyMsg(bool,Wait,OnMessage,FootBaller);
EmptyFun(void,Wait,Exit,FootBaller);

void Wait::Enter(FootBaller* player)
{
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter %d Wait x=%f,y=%f",player->GetID(),player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}

	if (!player->Pitch()->GameOn())
	{
		player->Steering()->SetTarget(player->GetHomeCenter() );
	}
}

void Wait::Execute(FootBaller* player)
{    
	if( !player->Team()->IsChaseBall() ) return;

	if( player->GetID() == 11 )
	{
		char p ='1';
		p = '0';
	}
	/// �����Ա������λ�ã���ôҪ����ԭ��λ��
	if (!player->AtTarget())
	{
		player->Steering()->ArriveOn();

		return;
	}
	else
	{
		player->Steering()->ArriveOff();

		player->SetVelocity(Vector2D(0,0));

		///�ö�Ա����
		player->TrackBall();
	}

	///
	/// ����ö�Ա�������ڿ������򣬸Ķ�Ա���ǽ�����Ա��
	/// ����ö�Ա�Ƚ�����Ա������ǰ��
	/// ��ô��Ӧ��������
	/// 
	if ( player->Team()->InControl()    &&
		(!player->isControllingPlayer()) &&
		player->isAheadOfAttacker() )
	{
		player->Team()->RequestPass(player);

		return;
	}

	if (player->Pitch()->GameOn())
	{
		/// ����ö�Ա���������������ģ����Ҳû�з�������Ա��ͬʱ����Աû����������ôȥ׷��
		if (player->isClosestTeamMemberToBall() &&
			player->Team()->Receiver() == NULL  &&
			!player->Pitch()->GoalKeeperHasBall())
		{
			player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

			return;
		}

		/// ���̫Զ��Ҫ��λ
		if( !player->IsSelfRegin() && (player->Team()->InControl() ||  player->FollowReturn() ) )
		{
			player->Steering()->SetTarget(player->GetHomeCenter());
			player->GetFSM()->ChangeState(&GetInstObj(ReturnToHomeRegion));
		}


		/// ���������		  && player->Role() == FootBaller::defender
		if ( !player->Team()->InControl() && player->FollowTarget()  )
		{
			 player->Steering()->SetTarget(player->Ball()->Pos());
			 player->GetFSM()->ChangeState(&GetInstObj(FollowBall));
		}

	} 
}


EmptyMsg(bool,FollowBall,OnMessage,FootBaller);

void FollowBall::Enter(FootBaller* player)
{
	player->Steering()->SeekOn();
}

void FollowBall::Execute(FootBaller* player)
{
	if( !player->Team()->IsChaseBall() ) return;

	if (player->Pitch()->GameOn())
	{
		/// ���̫Զ��Ҫ��λ
		if( player->Team()->InControl() ||  player->FollowReturn() )
		{
			player->Steering()->SetTarget(player->HomeRegion()->Center());
			player->GetFSM()->ChangeState(&GetInstObj(ReturnToHomeRegion));
		}
		
		/// ���������Ա���Ƿ����ߣ����ǿ����Ա����Ҫ���и�������з���	 &&
		///	 player->Role() == FootBaller::defender 
		if (!player->isClosestTeamMemberToBall() &&
			!player->Team()->InControl()  )
		{
			player->Steering()->SetTarget(player->Ball()->Pos());
			return;
		}

		/// ���������ģ��������Ǳ߽���Ӧ��ȥ����
		if (player->isClosestTeamMemberToBall() &&
			player->Team()->IsThrowIn() )
		{
			player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

			return;
		}

		/// ����ö�Ա���������������ģ����Ҳû�з�������Ա��ͬʱ����Աû����������ôȥ׷��
		if (player->isClosestTeamMemberToBall() &&
			player->Team()->Receiver() == NULL  &&
			!player->Pitch()->GoalKeeperHasBall())
		{
			player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

			return;
		}
	}
}

void FollowBall::Exit(FootBaller *player)
{
	player->Steering()->SeekOff();
}


EmptyMsg(bool,KickBall,OnMessage,FootBaller);

void KickBall::Enter(FootBaller* player)
{
	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 KickBall x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 KickBall x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}

	/// ʹ���ֱ���ö�Ա���ڿ�����
	player->Team()->SetControllingPlayer(player);

	/// �ö�Աÿ��ֻ�ܽ������޴���������
	if (!player->isReadyForNextKick()) 
	{
		player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));
	}

}

EmptyFun(void,KickBall,Exit,FootBaller);

void KickBall::Execute(FootBaller* player)
{ 
	if( !player->Team()->IsChaseBall() ) 
	{
		player->GetFSM()->ChangeState(&GetInstObj(Wait));
		return;
	}

	/// ����ָ�������������Ա�Լ��ĳ��������ĵ��
	Vector2D ToBall = player->Ball()->Pos() - player->Pos();
	double   dot    = player->Heading().Dot(Vec2DNormalize(ToBall)); 

	/// �������Ա�������򣬻������ڸö�Ա�ĺ��棬
	/// �����Ѿ������˽����Ա���Ͳ�����������ֻ�Ǽ���׷��
	if (player->Team()->Receiver() != NULL   ||
		player->Pitch()->GoalKeeperHasBall() ||
		(dot < 0) ) 
	{
		player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

		return;
	}

	/// ����ָ�������������Ա�Լ��ĳ��������ĵ��
	Vector2D    BallTarget;

	/// ����ָ�������������Ա�Լ��ĳ��������ĵ��
	double power = GetInstObj(CGameSetup).MaxShootingForce * dot;

	/// ���ȷ�ϸö�Ա���������λ�����ţ�����������������ĸ���һ�����Ǹö�Ա����ͼ����
	if (player->Team()->CanShootGoal(player->Ball()->Pos(),
		power,
		BallTarget)                   || 
		(RandFloat() < GetInstObj(CGameSetup).ChancePlayerAttemptsPotShot))
	{
		/// ����������һЩ���ţ����ǲ����ö�Ա�ߵ�̫׼��
		/// ͨ���ı�PlayerKickingAccuracyֵ���Ե���������ֵ
		BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

		///��������ķ���
		Vector2D KickDirection = BallTarget - player->Ball()->Pos();

		player->Ball()->Kick(KickDirection, power,player);

		//change state   
		player->GetFSM()->ChangeState(&GetInstObj(Wait));

		player->FindSupport();

		player->Team()->SetThrowIn(false);
		return;
	}

	/// �ҵ������Ա����ôreceiver��ָ����
	BasePlayer* receiver = NULL;

	power = GetInstObj(CGameSetup).MaxPassingForce * dot;

	/// �����Ƿ����κ�Ǳ�ڵĺ�ѡ�˿��Խ���
	if (player->isThreatened()  &&
		player->Team()->FindPass(player,
		receiver,
		BallTarget,
		power,
		GetInstObj(CGameSetup).MinPassDist))
	{     
		/// ����������һЩ����
		BallTarget = AddNoiseToKick(player->Ball()->Pos(), BallTarget);

		Vector2D KickDirection = BallTarget - player->Ball()->Pos();

		player->Ball()->Kick(KickDirection, power,player);

		player->Team()->SetThrowIn(false);

		/// �ý����Ա֪��Ҫ����
		Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
			player->GetID(),
			receiver->GetID(),
			Msg_ReceiveBall,
			&BallTarget);                            

		/// �ö�ԱӦ�õ������ĵ�ǰλ�ã���������ָʾ
		player->GetFSM()->ChangeState(&GetInstObj(Wait));

		player->FindSupport();

		return;
	}

	/// �������źʹ���ֻ�ܴ���ǰ��
	else
	{   
		player->FindSupport();

		player->GetFSM()->ChangeState(&GetInstObj(Dribble));
	}   
}

EmptyFun(void,Dribble,Exit,FootBaller);
EmptyMsg(bool,Dribble,OnMessage,FootBaller);

void Dribble::Enter(FootBaller* player)
{

	if( !player->Team()->IsChaseBall() ) return;

	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 Dribble x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 Dribble x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}

	/// ������ڿ���
	player->Team()->SetControllingPlayer(player);

}

void Dribble::Execute(FootBaller* player)
{

	if( !player->Team()->IsChaseBall() ) return;

	double dot = player->Team()->HomeGoalFacing().Dot(player->Heading());


	/// ������ڶ�Ա���Լ�������֮�䣬������Ҫͨ��������ߺ�Сת��
	if (dot < 0)
	{
		
		/// ��Ա�ĳ�����΢ת���£�PI/4����Ȼ�����Ǹ���������
		Vector2D direction = player->Heading();

		/// �����Ա�ĳ�������ŵĳ���֮��Ƕȵ�������(+/-)
		/// ʹ�ö�Ա����ת����ȷ����
		double angle = QuarterPi * -1 *
			player->Team()->HomeGoalFacing().Sign(player->Heading());

		Vec2DRotateAroundOrigin(direction, angle);

		/// ����Ա����ʵ���������ͬʱת��ʱ�����ֵ�𵽺ܺõ�����
		const double KickingForce = 0.8;

		player->Ball()->Kick(direction, KickingForce,player);
		player->Team()->SetThrowIn(false);
	}

	/// ����
	else
	{
		player->Ball()->Kick(player->Team()->HomeGoalFacing(),
			GetInstObj(CGameSetup).MaxDribbleForce,player); 
		player->Team()->SetThrowIn(false);
	}

	/// �Ķ�Ա�Ѿ������ˣ�����������ı�״̬ȥ׷��
	player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

	return;  
}


EmptyMsg(bool,ReceiveBall,OnMessage,FootBaller);

void ReceiveBall::Enter(FootBaller* player)
{
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter %d ReceiveBall x=%f,y=%f",player->GetID(),player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}

	/// ����Ա֪���������Ա���ڽ���
	player->Team()->SetReceiver(player);

	/// �ö�Ա����Ҳ�ǿ����Ա
	player->Team()->SetControllingPlayer(player);

	/// 
	/// ��2�������Ϊ��1.��arriveָ�������Ա���ﴫ���Ա���͵���Ϣ���ƶ���λ�á�
	/// 2.��Pursuit��Ϊ��׷����
	/// ����������ChanceOfUsingArriveTypeReceiveBehavior�Ŀ�����ѡ����һ��
	/// �ж��Ƿ��жԷ���Ա���������Ա
	/// �Ƿ�����Ա�ڶԷ������������ж�Ա����Է����ŵ������ģ�
	/// 
	const double PassThreatRadius = 70.0;

	if (( player->InHotRegion() ||
		  RandFloat()    < GetInstObj(CGameSetup).ChanceOfUsingArriveTypeReceiveBehavior) &&
		  !player->Team()->isOpponentWithinRadius(player->Pos(), PassThreatRadius))
	{
		player->Steering()->ArriveOn();

	}
	else
	{
		player->Steering()->PursuitOn();
	}
}

void ReceiveBall::Execute(FootBaller* player)
{
	if( !player->Team()->IsChaseBall() ) return;

	/// ����������㹻�������������������Ŀ���Ȩ����ô��Ӧ��״̬ȥ׷��
	if (player->BallWithinReceivingRange() || !player->Team()->InControl())
	{
		player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

		return;
	}  

	/// ���Pursuit�ٿ���Ϊ������׷����ô�ö�Ա��Ŀ������������λ�ò��ϵĸ���
	if (player->Steering()->PursuitIsOn())
	{
		player->Steering()->SetTarget(player->Ball()->Pos());
	}

	/// ����Ķ�Ա�ﵽ�˲ٿ�Ŀ���λ�ã���ô��Ӧ�õ����Ķ�����ת���������
	if (player->AtTarget())
	{
		player->Steering()->ArriveOff();
		player->Steering()->PursuitOff();
		player->TrackBall();    
		player->SetVelocity(Vector2D(0,0));
	} 
}

void ReceiveBall::Exit(FootBaller* player)
{
	player->Steering()->ArriveOff();
	player->Steering()->PursuitOff();

	player->Team()->SetReceiver(NULL);
}


