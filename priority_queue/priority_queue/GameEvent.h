#pragma once

class CGameEvent
{
public:
	//��ʱ��Ӧ
	virtual long Execute(const void* var)=0;
};
