// DlgProxy.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "QQTool.h"
#include "DlgProxy.h"
#include "QQToolDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQQToolDlgAutoProxy

IMPLEMENT_DYNCREATE(CQQToolDlgAutoProxy, CCmdTarget)

CQQToolDlgAutoProxy::CQQToolDlgAutoProxy()
{
	EnableAutomation();
	
	// ΪʹӦ�ó������Զ��������ڻ״̬ʱһֱ���� 
	//	���У����캯������ AfxOleLockApp��
	AfxOleLockApp();

	// ͨ��Ӧ�ó����������ָ��
	//  �����ʶԻ������ô�����ڲ�ָ��
	//  ָ��Ի��򣬲����öԻ���ĺ���ָ��ָ��
	//  �ô���
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CQQToolDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CQQToolDlg)))
		{
			m_pDialog = reinterpret_cast<CQQToolDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CQQToolDlgAutoProxy::~CQQToolDlgAutoProxy()
{
	// Ϊ������ OLE �Զ����������ж������ֹӦ�ó���
	// 	������������ AfxOleUnlockApp��
	//  ���������������⣬�⻹���������Ի���
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CQQToolDlgAutoProxy::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CQQToolDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CQQToolDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_IQQTool ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {CB5CE328-C140-47CF-B529-A203B421EB83}
static const IID IID_IQQTool =
{ 0xCB5CE328, 0xC140, 0x47CF, { 0xB5, 0x29, 0xA2, 0x3, 0xB4, 0x21, 0xEB, 0x83 } };

BEGIN_INTERFACE_MAP(CQQToolDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CQQToolDlgAutoProxy, IID_IQQTool, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 ���ڴ���Ŀ�� StdAfx.h �ж���
// {947F7674-4BD9-4BEA-A579-FC92EC020E80}
IMPLEMENT_OLECREATE2(CQQToolDlgAutoProxy, "QQTool.Application", 0x947f7674, 0x4bd9, 0x4bea, 0xa5, 0x79, 0xfc, 0x92, 0xec, 0x2, 0xe, 0x80)


// CQQToolDlgAutoProxy ��Ϣ�������
