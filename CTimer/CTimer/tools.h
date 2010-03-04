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