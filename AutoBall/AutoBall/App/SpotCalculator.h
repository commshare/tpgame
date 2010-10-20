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
class SpotCalculator
{
	/// ������ѽ�Ӧ��
	struct BestSpot
	{
		Vector2D  m_vPos;
		double    m_dScore;
		BestSpot(Vector2D pos, double value):m_vPos(pos),
				m_dScore(value)
		{}
	};

private:

	FootBallTeam*             m_pTeam;				///> ��
	std::vector<BestSpot>  m_Spots;				///> ���еĴ��͵�
	BestSpot*              m_pBestSupportingSpot;///> ��Ѵ��͵�
	TimeCount*                m_pPassTimer;			///> ���͵Ĺ����У�ÿ��֡���� 

public:

	SpotCalculator(int numX,
						  int numY,
						  FootBallTeam* team);

	~SpotCalculator();

	void       Render()const;
			
	/// �����ĸ�������ѵĴ���㣬��Ӧ��Ա�����Ǹ�λ�ã��ȴ�����
	Vector2D  BestSupportingPosition();

	/// ������Ѵ��͵�
	Vector2D  GetBestSupportingSpot();
};


#endif