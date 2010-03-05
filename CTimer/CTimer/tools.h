#pragma  once

/// �õ���ǰʱ�䣬��ȷ������
inline  ulong GetCurrSystemTime()
{
	SYSTEMTIME  sys;
	GetLocalTime(&sys);

	tm      time;
	time.tm_year = sys.wYear - 1900;
	time.tm_mon  = sys.wMonth-1;
	time.tm_mday = sys.wDay;
	time.tm_hour = sys.wHour;
	time.tm_min  = sys.wMinute;
	time.tm_sec  = sys.wSecond;
	return (ulong)(_mktime64(&time) * 1000 + sys.wMilliseconds);
}

/// ��ʱ��ָ����
struct ListNode
{
	ListNode *next,*prev;
};

///
/// ��ʱ������
/// 
enum eTimerType
{
	eTimer1 = 10,
	eTimer2 ,
	eTimer3 
};

/// 
/// ��ʱ�����,tlist��ʾ����ָ��,expiresѭ������ʱ��
/// etime ��������ʱ��,pFun��������.
/// 
struct timernode
{
	ListNode    tlist;
	ulong       expires;
	ulong       etime;
	void        *pFun;
	eTimerType  eType;
};


/// ��Ϸ�¼�����
class   CGameEvent
{
public:
	virtual  long  TimeOut( eTimerType type) = 0;
};