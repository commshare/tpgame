
#pragma  once

#include <string>

#include "State.h"

class FieldPlayer;
class SoccerPitch;


/// ȫ�ֶ�Ա״̬
CREATESTATE(GlobalPlayerState,Telegram);

/// ׷��״̬
CREATESTATE(ChaseBall,Telegram);

/// ����״̬
CREATESTATE(Dribble,Telegram);

/// �ص�����λ��
CREATESTATE(ReturnToHomeRegion,Telegram);

/// �ȴ�״̬
CREATESTATE(Wait,Telegram);

/// ������λ��
CREATESTATE(FollowBall,Telegram);

/// 
CREATESTATE(KickBall,Telegram);

///
CREATESTATE(ReceiveBall,Telegram);

///
CREATESTATE(SupportAttacker,Telegram);

// 
// 
// /// ȫ�ֶ�Ա״̬
// //------------------------------------------------------------------------
// template< typename type>
// class GlobalPlayerState : public State<type>
// {
// public:
// 
// 	CLASSFUNTION(GlobalPlayerState,type,Telegram);
// };
// 
// /// ׷��
// //------------------------------------------------------------------------
// template< typename type>
// class ChaseBall : public State<type>
// {
// public:
// 
// 	CLASSFUNTION(ChaseBall,type,Telegram);
// 
// 	/*
// private:
// 
// 	ChaseBall(){}
// 
// public:
// 
// 	//this is a singleton
// 	static ChaseBall* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player);
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
// /// ����
// //------------------------------------------------------------------------
// template< typename type>
// class Dribble : public State<type>
// {
// public:
// 
// 	CLASSFUNTION(Dribble,type,Telegram);
// 
// 	/*
// private:
// 
// 	Dribble(){}
// 
// public:
// 
// 	//this is a singleton
// 	static Dribble* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player){}
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
// /// ��λ
// template< typename type>
// class ReturnToHomeRegion: public State<type>
// {
// public:
// 
// 	CLASSFUNTION(ReturnToHomeRegion,type,Telegram);
//  /*
// private:
// 
// 	ReturnToHomeRegion(){}
// 
// public:
// 
// 	//this is a singleton
// 	static ReturnToHomeRegion* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player);
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
// /// �ȴ�
// template< typename type>
// class Wait: public State<type>
// {
// public:
// 
// 	CLASSFUNTION(Wait,type,Telegram);
// 
// 	/*
// private:
// 
// 	Wait(){}
// 
// public:
// 
// 	//this is a singleton
// 	static Wait* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player);
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
// 
// /// �����򣬷���
// template< typename type>
// class FollowBall :public State<type>
// {
// public:
// 
// 	CLASSFUNTION(FollowBall,type,Telegram);
// 
// 	/*
// private:
// 
// 	FollowBall() { }
// 
// public:
// 
// 	//this is a singleton
// 	static FollowBall* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player);
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
//   */
// };
// 
// /// ����
// template< typename type>
// class KickBall: public State<type>
// {
// public:
// 
// 	CLASSFUNTION(KickBall,type,Telegram);
// 
// 	/*
// private:
// 
// 	KickBall(){}
// 
// public:
// 
// 	//this is a singleton
// 	static KickBall* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player){}
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
// /// ����
// template< typename type>
// class ReceiveBall: public State<type>
// {
// public:
// 
// 	CLASSFUNTION(ReceiveBall,type,Telegram);
// 
// 	/*
// private:
// 
// 	ReceiveBall(){}
// 
// public:
// 
// 	//this is a singleton
// 	static ReceiveBall* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player);
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
// /// ��Ӧ
// template< typename type>
// class SupportAttacker: public State<type>
// {
// public:
// 	CLASSFUNTION(SupportAttacker,type,Telegram);
// 	/*
// private:
// 
// 	SupportAttacker(){}
// 
// public:
// 
// 	//this is a singleton
// 	static SupportAttacker* Instance();
// 
// 	void Enter(FieldPlayer* player);
// 
// 	void Execute(FieldPlayer* player);
// 
// 	void Exit(FieldPlayer* player);
// 
// 	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
// 	*/
// };
// 
