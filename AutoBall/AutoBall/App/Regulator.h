
#pragma  once 

#include "../Render/Vector2D.h"
#include "../Render/Utils.h"
#include "../Render/VGdi.h"
#include "../Public/Singleton.h"
#include "../Public/GameSetup.h"
#include "../Render/Utils.h"
#include "Region.h"
#include "SoccerPitch.h"
#include "SoccerTeam.h"

/// ������
class PlayerBase;
class Regulator
{
private:

	/// 2�θ��¼��
	double m_dUpdatePeriod;

	/// ��һ�θ���ʱ��
	DWORD m_dwNextUpdateTime;

public:

	Regulator(double NumUpdatesPerSecondRqd)  ;

	bool isReady();
};
