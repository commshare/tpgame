
#pragma  once 

/// �����Ƿ�׼��������һ������
class TimeCount
{

public:

	TimeCount(double Updates)  ;

	bool isReadyOK();

private:

	/// 2�θ��¼��
	double m_dUpdateNum;

	/// ��һ�θ���ʱ��
	DWORD m_dwNextUpdateTime;
};
