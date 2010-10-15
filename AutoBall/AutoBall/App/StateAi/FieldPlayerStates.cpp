#include "Stdafx.h"
#include "State.h"
#include "StateMachine.h"
#include "../Entity/PlayerBase.h"
#include "../Entity/FieldPlayer.h"	  
#include "../Goal.h"
#include "../SoccerTeam.h"
#include "../SoccerPitch.h"
#include "../Messageing/MessageDispatcher.h"
#include "../SteeringBehaviors.h"
#include "../../Public/MsgImpl.h"
#include "../../Config.h"	


#define PLAYER_STATE_INFO_ON

EmptyFun(void,GlobalPlayerState,Enter,FieldPlayer);
EmptyFun(void,GlobalPlayerState,Exit,FieldPlayer);

void GlobalPlayerState::Execute(FieldPlayer* player)                                     
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

bool GlobalPlayerState::OnMessage(FieldPlayer* player, const tagMessage& telegram)
{
	switch(telegram.Msg)
	{
	case Msg_ReceiveBall:
		{
			//set the target
			player->Steering()->SetTarget(*(static_cast<Vector2D*>(telegram.ExtraInfo)));

			//change state 
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
			//set the target to be the best supporting position
			player->Steering()->SetTarget(player->Team()->GetSupportSpot());

			//change the state
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
			//get the position of the player requesting the pass 
			FieldPlayer* receiver = static_cast<FieldPlayer*>(telegram.ExtraInfo);

#ifdef PLAYER_STATE_INFO_ON
			// debug_con << "Player " << player->GetID() << " received request from " <<
			//              receiver->ID() << " to make pass" << "";
#endif

			/// ���������Ա�ɴ����ķ�Χ���������Ա���߱����ŵ��������Ķ�Ա���ܴ���������Ա
			if (player->Team()->Receiver() != NULL ||
				!player->BallWithinKickingRange() )
			{
#ifdef PLAYER_STATE_INFO_ON
				//debug_con << "Player " << player->GetID() << " cannot make requested pass <cannot kick ball>" << "";
#endif

				return true;
			}

			//make the pass   ����
			player->Ball()->Kick(receiver->Pos() - player->Ball()->Pos(),
				GetInstObj(CGameSetup).MaxPassingForce,player);


#ifdef PLAYER_STATE_INFO_ON
			//debug_con << "Player " << player->GetID() << " Passed ball to requesting player" << "";
#endif

			///֪ͨ�����Ա����ʼ����
			Dispatcher->DispatchMsg(SEND_MSG_IMMEDIATELY,
				player->GetID(),
				receiver->GetID(),
				Msg_ReceiveBall,
				&receiver->Pos());



			//change state   
			player->GetFSM()->ChangeState(&GetInstObj(Wait));

			player->FindSupport();

			return true;
		}

		break;

	}//end switch

	return false;
}

EmptyMsg(bool,ChaseBall,OnMessage,FieldPlayer);

void ChaseBall::Enter(FieldPlayer* player)
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

#ifdef PLAYER_STATE_INFO_ON
	// debug_con << "Player " << player->GetID() << " enters chase state" << "";
#endif
}

void ChaseBall::Execute(FieldPlayer* player)                                     
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


void ChaseBall::Exit(FieldPlayer* player)
{
	player->Steering()->SeekOff();
}


void SupportAttacker::Enter(FieldPlayer* player)
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

#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters support state" << "";
#endif
}

EmptyMsg(bool,SupportAttacker,OnMessage,FieldPlayer);

void SupportAttacker::Execute(FieldPlayer* player)                                     
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
	if( player->Team()->CanShoot(player->Pos(),
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


void SupportAttacker::Exit(FieldPlayer* player)
{
	player->Team()->SetSupportingPlayer(NULL);

	player->Steering()->ArriveOff();
}




void ReturnToHomeRegion::Enter(FieldPlayer* player)
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

#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters ReturnToHome state" << "";
#endif
}


EmptyMsg(bool,ReturnToHomeRegion,OnMessage,FieldPlayer);

void ReturnToHomeRegion::Execute(FieldPlayer* player)
{
	if( !player->Team()->IsChaseBall() ) return;

	if (player->Pitch()->GameOn())
	{
		//if the ball is nearer this player than any other team member  &&
		//there is not an assigned receiver && the goalkeeper does not gave
		//the ball, go chase it
		if ( player->isClosestTeamMemberToBall() &&
			player->Team()->IsReceiver() &&
			!player->Pitch()->GoalKeeperHasBall())
		{
			player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

			return;
		}
	}

	//if game is on and close enough to home, change state to wait and set the 
	//player target to his current position.(so that if he gets jostled out of 
	//position he can move back to it)
	if (player->Pitch()->GameOn() && player->HomeRegion()->Inside(player->Pos(),
		Region::halfsize))
	{
		player->Steering()->SetTarget(player->Pos());
		player->GetFSM()->ChangeState(&GetInstObj(Wait));
	}
	//if game is not on the player must return much closer to the center of his
	//home region
	else if(!player->Pitch()->GameOn() && player->AtTarget())
	{
		player->GetFSM()->ChangeState(&GetInstObj(Wait));
	}
}

void ReturnToHomeRegion::Exit(FieldPlayer* player)
{
	player->Steering()->ArriveOff();
}


EmptyMsg(bool,Wait,OnMessage,FieldPlayer);
EmptyFun(void,Wait,Exit,FieldPlayer);

void Wait::Enter(FieldPlayer* player)
{
	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 Wait x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 Wait x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters wait state" << "";
#endif

	//if the game is not on make sure the target is the center of the player's
	//home region. This is ensure all the players are in the correct positions
	//ready for kick off
	if (!player->Pitch()->GameOn())
	{
		player->Steering()->SetTarget(player->HomeRegion()->Center());
	}
}

void Wait::Execute(FieldPlayer* player)
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
			player->Steering()->SetTarget(player->HomeRegion()->Center());
			player->GetFSM()->ChangeState(&GetInstObj(ReturnToHomeRegion));
		}


		/// ���������		  && player->Role() == FieldPlayer::defender
		if ( !player->Team()->InControl() && player->FollowTarget()  )
		{
			 player->Steering()->SetTarget(player->Ball()->Pos());
			 player->GetFSM()->ChangeState(&GetInstObj(FollowBall));
		}

	} 
}


EmptyMsg(bool,FollowBall,OnMessage,FieldPlayer);

void FollowBall::Enter(FieldPlayer* player)
{
	player->Steering()->SeekOn();
}

void FollowBall::Execute(FieldPlayer* player)
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
		///	 player->Role() == FieldPlayer::defender 
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

void FollowBall::Exit(FieldPlayer *player)
{
	player->Steering()->SeekOff();
}


EmptyMsg(bool,KickBall,OnMessage,FieldPlayer);

void KickBall::Enter(FieldPlayer* player)
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

	/// �ö�Աÿ��ֻ�ܾ�ϲ���޴���������
	if (!player->isReadyForNextKick()) 
	{
		player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));
	}


#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters kick state" << "";
#endif
}

EmptyFun(void,KickBall,Exit,FieldPlayer);

void KickBall::Execute(FieldPlayer* player)
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
#ifdef PLAYER_STATE_INFO_ON
		//    debug_con << "Goaly has ball / ball behind player" << "";
#endif

		player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

		return;
	}

	/// ����ָ�������������Ա�Լ��ĳ��������ĵ��
	Vector2D    BallTarget;

	/// ����ָ�������������Ա�Լ��ĳ��������ĵ��
	double power = GetInstObj(CGameSetup).MaxShootingForce * dot;

	/// ���ȷ�ϸö�Ա���������λ�����ţ�����������������ĸ���һ�����Ǹö�Ա����ͼ����
	if (player->Team()->CanShoot(player->Ball()->Pos(),
		power,
		BallTarget)                   || 
		(RandFloat() < GetInstObj(CGameSetup).ChancePlayerAttemptsPotShot))
	{
#ifdef PLAYER_STATE_INFO_ON
		// debug_con << "Player " << player->GetID() << " attempts a shot at " << BallTarget << "";
#endif

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
	PlayerBase* receiver = NULL;

	power = GetInstObj(CGameSetup).MaxPassingForce * dot;

	/// �����Ƿ����κ�Ǳ�ڵĺ�ѡ�˿��Խ���
	//test if there are any potential candidates available to receive a pass
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

#ifdef PLAYER_STATE_INFO_ON
		//    debug_con << "Player " << player->GetID() << " passes the ball with force " << power << "  to player " 
		//             << receiver->ID() << "  Target is " << BallTarget << "";
#endif


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

EmptyFun(void,Dribble,Exit,FieldPlayer);
EmptyMsg(bool,Dribble,OnMessage,FieldPlayer);

void Dribble::Enter(FieldPlayer* player)
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

#ifdef PLAYER_STATE_INFO_ON
	//  debug_con << "Player " << player->GetID() << " enters dribble state" << "";
#endif
}

void Dribble::Execute(FieldPlayer* player)
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
	//kick the ball down the field
	else
	{
		player->Ball()->Kick(player->Team()->HomeGoalFacing(),
			GetInstObj(CGameSetup).MaxDribbleForce,player); 
		player->Team()->SetThrowIn(false);
	}

	/// �Ķ�Ա�Ѿ������ˣ�����������ı�״̬ȥ׷��
	//the player has kicked the ball so he must now change state to follow it
	player->GetFSM()->ChangeState(&GetInstObj(ChaseBall));

	return;  
}


EmptyMsg(bool,ReceiveBall,OnMessage,FieldPlayer);

void ReceiveBall::Enter(FieldPlayer* player)
{
	if( player->GetID() == 9 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 9 ReceiveBall x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}
	if( player->GetID() == 2 )
	{
		char  str[256];
	    sprintf_s(str,"Player Enter 2 ReceiveBall x=%f,y=%f",player->Pos().x,player->Pos().y);
		PutFileLog(str);
	}

	/// ����Ա֪���������Ա���ڽ���
	player->Team()->SetReceiver(player);

	/// �ö�Ա����Ҳ�ǿ����Ա
	//this player is also now the controlling player
	player->Team()->SetControllingPlayer(player);

	/// ��2�������Ϊ��1.��arriveָ�������Ա���ﴫ���Ա���͵���Ϣ���ƶ���λ�á�
	/// 2.��Pursuit��Ϊ��׷����
	/// ����������ChanceOfUsingArriveTypeReceiveBehavior�Ŀ�����ѡ����һ���ж��Ƿ��жԷ���Ա���������Ա
	/// �Ƿ�����Ա�ڶԷ������������ж�Ա����Է����ŵ������ģ�
	const double PassThreatRadius = 70.0;

	if (( player->InHotRegion() ||
		RandFloat() < GetInstObj(CGameSetup).ChanceOfUsingArriveTypeReceiveBehavior) &&
		!player->Team()->isOpponentWithinRadius(player->Pos(), PassThreatRadius))
	{
		player->Steering()->ArriveOn();

#ifdef PLAYER_STATE_INFO_ON
		//    debug_con << "Player " << player->GetID() << " enters receive state (Using Arrive)" << "";
#endif
	}
	else
	{
		player->Steering()->PursuitOn();

#ifdef PLAYER_STATE_INFO_ON
		//    debug_con << "Player " << player->GetID() << " enters receive state (Using Pursuit)" << "";
#endif
	}
}

void ReceiveBall::Execute(FieldPlayer* player)
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

void ReceiveBall::Exit(FieldPlayer* player)
{
	player->Steering()->ArriveOff();
	player->Steering()->PursuitOff();

	player->Team()->SetReceiver(NULL);
}








