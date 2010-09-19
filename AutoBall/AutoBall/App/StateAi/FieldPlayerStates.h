
#pragma  once
//------------------------------------------------------------------------
//
//  Name: FieldPlayerStates.h
//
//  Desc: States for the field players of Simple Soccer. See my book
//        for detailed descriptions
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <string>

#include "State.h"

class FieldPlayer;
class SoccerPitch;

/// ȫ�ֶ�Ա״̬
//------------------------------------------------------------------------
class GlobalPlayerState : public State<FieldPlayer>
{
private:

	GlobalPlayerState(){}

public:

	//this is a singleton
	static GlobalPlayerState* Instance();

	void Enter(FieldPlayer* player){}

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player){}

	bool OnMessage(FieldPlayer*, const Telegram&);
};

/// ׷��
//------------------------------------------------------------------------
class ChaseBall : public State<FieldPlayer>
{
private:

	ChaseBall(){}

public:

	//this is a singleton
	static ChaseBall* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/// ����
//------------------------------------------------------------------------
class Dribble : public State<FieldPlayer>
{
private:

	Dribble(){}

public:

	//this is a singleton
	static Dribble* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player){}

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/// ��λ
//------------------------------------------------------------------------
class ReturnToHomeRegion: public State<FieldPlayer>
{
private:

	ReturnToHomeRegion(){}

public:

	//this is a singleton
	static ReturnToHomeRegion* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/// �ȴ�
class Wait: public State<FieldPlayer>
{
private:

	Wait(){}

public:

	//this is a singleton
	static Wait* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};


/// �����򣬷���
class FollowBall :public State<FieldPlayer>
{
private:

	FollowBall() { }

public:

	//this is a singleton
	static FollowBall* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player){}

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}

};

/// ����
class KickBall: public State<FieldPlayer>
{
private:

	KickBall(){}

public:

	//this is a singleton
	static KickBall* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player){}

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/// ����
//------------------------------------------------------------------------
class ReceiveBall: public State<FieldPlayer>
{
private:

	ReceiveBall(){}

public:

	//this is a singleton
	static ReceiveBall* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

/// ��Ӧ
//------------------------------------------------------------------------
class SupportAttacker: public State<FieldPlayer>
{
private:

	SupportAttacker(){}

public:

	//this is a singleton
	static SupportAttacker* Instance();

	void Enter(FieldPlayer* player);

	void Execute(FieldPlayer* player);

	void Exit(FieldPlayer* player);

	bool OnMessage(FieldPlayer*, const Telegram&){return false;}
};

