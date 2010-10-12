#include "Stdafx.h"
#include "Entity/PlayerBase.h"
#include "Goal.h"
#include "Entity/SoccerBall.h"
#include "SoccerTeam.h"
#include "SoccerPitch.h"
#include "../Public/GameSetup.h"
#include "SupportSpotCalculator.h"


SupportSpotCalculator::~SupportSpotCalculator()
{
	SAFE_DELETE( m_pRegulator );
}


SupportSpotCalculator::SupportSpotCalculator(int           numX,
											 int           numY,
											 SoccerTeam*   team):m_pBestSupportingSpot(NULL),
											 m_pTeam(team)
{
	const Region* PlayingField = team->Pitch()->PlayingArea();

	//calculate the positions of each sweet spot, create them and 
	//store them in m_Spots
	double HeightOfSSRegion = PlayingField->Height() * 0.8;
	double WidthOfSSRegion  = PlayingField->Width() * 0.9;
	double SliceX = WidthOfSSRegion / numX ;
	double SliceY = HeightOfSSRegion / numY;

	double left  = PlayingField->Left() + (PlayingField->Width()-WidthOfSSRegion)/2.0 + SliceX/2.0;
	double right = PlayingField->Right() - (PlayingField->Width()-WidthOfSSRegion)/2.0 - SliceX/2.0;
	double top   = PlayingField->Top() + (PlayingField->Height()-HeightOfSSRegion)/2.0 + SliceY/2.0;

	for (int x=0; x<(numX/2)-1; ++x)
	{
		for (int y=0; y<numY; ++y)
		{      
			if (m_pTeam->Color() == SoccerTeam::blue)
			{
				m_Spots.push_back(SupportSpot(Vector2D(left+x*SliceX, top+y*SliceY), 0.0));
			}

			else
			{
				m_Spots.push_back(SupportSpot(Vector2D(right-x*SliceX, top+y*SliceY), 0.0));
			}
		}
	}

	m_pRegulator = new Regulator(GetInstObj(CGameSetup).SupportSpotUpdateFreq);
}


Vector2D SupportSpotCalculator::DetermineBestSupportingPosition()
{
	if (!m_pRegulator->isReady() && m_pBestSupportingSpot)
	{
		return m_pBestSupportingSpot->m_vPos;
	}

	/// ������ѽ�Ӧ��
	m_pBestSupportingSpot = NULL;

	double BestScoreSoFar = 0.0;

	std::vector<SupportSpot>::iterator curSpot;

	for (curSpot = m_Spots.begin(); curSpot != m_Spots.end(); ++curSpot)
	{
		/// ����ɾ����ǰ�ķ���
		curSpot->m_dScore = 1.0;

		/// ���ȴ������λ���Ƿ�ȫ
		if(m_pTeam->isPassSafeFromAllOpponents(m_pTeam->ControllingPlayer()->Pos(),
			curSpot->m_vPos,
			NULL,
			GetInstObj(CGameSetup).MaxPassingForce))
		{
			curSpot->m_dScore += GetInstObj(CGameSetup).Spot_PassSafeScore;
		}

		/// ���������λ������
		if( m_pTeam->CanShoot(curSpot->m_vPos,            
			GetInstObj(CGameSetup).MaxShootingForce))
		{
			curSpot->m_dScore += GetInstObj(CGameSetup).Spot_CanScoreFromPositionScore;
		}   

		/// �����������Ա��Զ,ԽԶ����Խ��
		/// ͬʱ��һ����Զ��OptimalDistance�ľ���
		if (m_pTeam->SupportingPlayer())
		{
			const double OptimalDistance = 200.0;

			double dist = Vec2DDistance(m_pTeam->ControllingPlayer()->Pos(),
				curSpot->m_vPos);

			double temp = fabs(OptimalDistance - dist);

			if (temp < OptimalDistance)
			{
				/// ��׼�����룬�����ӵ�������
				curSpot->m_dScore += GetInstObj(CGameSetup).Spot_DistFromControllingPlayerScore *
					(OptimalDistance-temp)/OptimalDistance;  
			}
		}

		/// ��鵽Ŀǰλ��������Ƿ�����߷�
		if (curSpot->m_dScore > BestScoreSoFar)
		{
			BestScoreSoFar = curSpot->m_dScore;

			m_pBestSupportingSpot = &(*curSpot);
		}    

	}

	return m_pBestSupportingSpot->m_vPos;
}

Vector2D SupportSpotCalculator::GetBestSupportingSpot()
{
	if (m_pBestSupportingSpot)
	{
		return m_pBestSupportingSpot->m_vPos;
	}

	else
	{ 
		return DetermineBestSupportingPosition();
	}
}

void SupportSpotCalculator::Render()const
{
	GetInstObj(CGDI).HollowBrush();
	GetInstObj(CGDI).GreyPen();

	for (unsigned int spt=0; spt<m_Spots.size(); ++spt)
	{
		GetInstObj(CGDI).Circle(m_Spots[spt].m_vPos, m_Spots[spt].m_dScore);
	}

	if (m_pBestSupportingSpot)
	{
		GetInstObj(CGDI).GreenPen();
		GetInstObj(CGDI).Circle(m_pBestSupportingSpot->m_vPos, m_pBestSupportingSpot->m_dScore);
	}
}