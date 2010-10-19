#ifndef SUPPORTSPOTCALCULATOR
#define SUPPORTSPOTCALCULATOR
#pragma warning (disable:4786)

#include <vector>

#include "TimeCount.h"
#include "Region.h"
#include "../Render/Vector2D.h"
#include "../Render/VGdi.h"

class BasePlayer;
class Goal;
class FootBall;
class FootBallTeam;
class TimeCount;

/// ��Ϊ������
class SupportSpotCalculator
{
private:

	/// ������ѽ�Ӧ��
	struct SupportSpot
	{

		Vector2D  m_vPos;

		double    m_dScore;

		SupportSpot(Vector2D pos, double value):m_vPos(pos),
			m_dScore(value)
		{}
	};

private:

	FootBallTeam*               m_pTeam;				///> ��
	std::vector<SupportSpot>  m_Spots;				///> ���еĴ��͵�
	SupportSpot*              m_pBestSupportingSpot;///> ��Ѵ��͵�
	TimeCount*                m_pPassTimer;			///> ���͵Ĺ����У�ÿ��֡���� 

public:

	SupportSpotCalculator(int numX,
						  int numY,
						  FootBallTeam* team);

	~SupportSpotCalculator();

	void       Render()const;
			
	/// �����ĸ�������ѵĴ���㣬��Ӧ��Ա�����Ǹ�λ�ã��ȴ�����
	Vector2D  DetermineBestSupportingPosition();

	/// ������Ѵ��͵�
	Vector2D  GetBestSupportingSpot();
};


#endif