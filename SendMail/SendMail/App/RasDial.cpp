#include "stdafx.h"
#include "RasDial.h"
#include <string>
using namespace std;

RasDial::RasDial()
{
	hRasConn = NULL;
}

RasDial::RasDial(CString user, CString pword, CString name)
{
	m_User = user;
	m_PassWord = pword;
	m_Name = name;
	hRasConn = NULL;
}

bool RasDial::Listen()
{
	RASDIALPARAMS rdParams;							//�������ӵ���Ϣ
	ZeroMemory(&rdParams, sizeof(RASDIALPARAMS));
	rdParams.dwSize = sizeof(RASDIALPARAMS);

	lstrcpy(rdParams.szEntryName,m_Name.GetBuffer(0) );
	lstrcpy(rdParams.szUserName, m_User.GetBuffer(0));
	lstrcpy(rdParams.szPassword, m_PassWord.GetBuffer(0));

	for( ; ; )
	{
		DWORD dwRet;
		dwRet = ::RasDial(NULL, NULL, &rdParams, 0L, NULL , &hRasConn);  //��������ɹ����򷵻�0
		if (dwRet != 0)
		{
			char   szBuf[256] = "";
			::RasGetErrorStringA((UINT)dwRet, szBuf, 256);
			std::string Buf = "�������Ӵ���!����Ϊ: " + string(szBuf);
			//CString msg = "���Ӵ���!����Ϊ: "+CString(szBuf);
			continue;		
		}
		break;
	}

	return true;
}

void  RasDial::Stop()
{
	RASCONNSTATUS rStatus;
	ZeroMemory(&rStatus, sizeof(RASCONNSTATUS));
	rStatus.dwSize = sizeof(RASCONNSTATUS);
	while((RasGetConnectStatus(hRasConn, &rStatus) != ERROR_INVALID_HANDLE))
	{
		//�Ҷ�����
		RasHangUp(hRasConn);
	}
}