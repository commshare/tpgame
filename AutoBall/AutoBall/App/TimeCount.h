
#pragma  once 

#include "../Render/Utils.h"

/// �����Ƿ�׼��������һ������
class TimeCount
{

public:

	TimeCount(double NumUpdatesPerSecondRqd)  ;

	bool isReadyOK();

private:

	/// 2�θ��¼��
	double m_dUpdatePeriod;

	/// ��һ�θ���ʱ��
	DWORD m_dwNextUpdateTime;
};
