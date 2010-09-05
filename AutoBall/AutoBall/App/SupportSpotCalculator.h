#ifndef SUPPORTSPOTCALCULATOR
#define SUPPORTSPOTCALCULATOR
#pragma warning (disable:4786)
//------------------------------------------------------------------------
//
//  Name:   SupportSpotCalculator.h
//
//  Desc:   Class to determine the best spots for a suppoting soccer
//          player to move to.
//
//  Author: Mat Buckland 2003 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------

#include <vector>

#include "Regulator.h"
#include "Region.h"
#include "../Render/Vector2D.h"
#include "../Render/VGdi.h"


class PlayerBase;
class Goal;
class SoccerBall;
class SoccerTeam;
class Regulator;

/// ��Ϊ������
class SupportSpotCalculator
{
private:

	/// ������ѽ�Ӧ��
	//a data structure to hold the values and positions of each spot
	struct SupportSpot
	{

		Vector2D  m_vPos;

		double    m_dScore;

		SupportSpot(Vector2D pos, double value):m_vPos(pos),
			m_dScore(value)
		{}
	};

private:


	SoccerTeam*               m_pTeam;	 /// ��

	std::vector<SupportSpot>  m_Spots;	 /// ���еĴ��͵�

	//a pointer to the highest valued spot from the last update
	SupportSpot*              m_pBestSupportingSpot;	  /// ��Ѵ��͵�

	//this will regulate how often the spots are calculated (default is
	//one update per second)
	Regulator*                m_pRegulator;///���͵Ĺ����У�ÿ��֡���� 

public:

	SupportSpotCalculator(int numX,
		int numY,
		SoccerTeam* team);

	~SupportSpotCalculator();

	//draws the spots to the screen as a hollow circles. The higher the 
	//score, the bigger the circle. The best supporting spot is drawn in
	//bright green.
	void       Render()const;

	//this method iterates through each possible spot and calculates its
	//score.			
	/// �����ĸ�������ѵĴ���㣬��Ӧ��Ա�����Ǹ�λ�ã��ȴ�����
	Vector2D  DetermineBestSupportingPosition();

	//returns the best supporting spot if there is one. If one hasn't been
	//calculated yet, this method calls DetermineBestSupportingPosition and
	//returns the result.
	/// ������Ѵ��͵�
	Vector2D  GetBestSupportingSpot();
};


#endif