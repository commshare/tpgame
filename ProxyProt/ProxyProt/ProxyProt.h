// ProxyProt.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CProxyProtApp:
// �йش����ʵ�֣������ ProxyProt.cpp
//

class CProxyProtApp : public CWinApp
{
public:
	CProxyProtApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CProxyProtApp theApp;