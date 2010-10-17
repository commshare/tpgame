/**
 *  @brief �����װһ������״̬֮�����Ϣ���ݵ����ݽṹ
 */

#ifndef TELEGRAM_H
#define TELEGRAM_H

#include <iostream>
#include <math.h>
#include "../Public/BaseDef.h"
#include "../Render/Vector2D.h"

class  tagMessage
{
public:

	int          Sender;
	int          Receiver;
	int          Msg;

	LONG64       DispatchTime;
	void*        ExtraInfo;


	tagMessage():DispatchTime(-1),
		Sender(-1),
		Receiver(-1),
		Msg(-1)
	{}


	tagMessage(LONG64 time,
		int    sender,
		int    receiver,
		int    msg,
		void*  info = NULL): DispatchTime(time),
		Sender(sender),
		Receiver(receiver),
		Msg(msg),
		ExtraInfo(info)
	{}


public:

	int				GetMsg()	{	return Msg; }
	Vector2D		GetVec()	{	return *(Vector2D*)ExtraInfo;}
};

/// ��С�ӳ�
const double SmallestDelay = 0.25;

inline bool operator==(const tagMessage& t1, const tagMessage& t2)
{
	return ( fabs( (double)(t1.DispatchTime-t2.DispatchTime) ) < SmallestDelay) &&
				 (t1.Sender == t2.Sender)&&
				 (t1.Receiver == t2.Receiver)&&(t1.Msg == t2.Msg);
}

inline bool operator<(const tagMessage& t1, const tagMessage& t2)
{
	if (t1 == t2)
	{
		return false;
	}
	else
	{
		return  (t1.DispatchTime < t2.DispatchTime);
	}
}

inline std::ostream& operator<<(std::ostream& os, const tagMessage& t)
{
	os << "time: " << t.DispatchTime << "  Sender: " << t.Sender
	   << "   Receiver: " << t.Receiver << "   Msg: "<< t.Msg;

	return os;
}

/// ����������� 
template <class T>
inline T DereferenceToType(void* p)
{
	return *(T*)(p);
}


#endif