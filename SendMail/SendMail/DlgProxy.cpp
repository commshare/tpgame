// DlgProxy.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SendMail.h"
#include "DlgProxy.h"
#include "SendMailDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSendMailDlgAutoProxy

IMPLEMENT_DYNCREATE(CSendMailDlgAutoProxy, CCmdTarget)

CSendMailDlgAutoProxy::CSendMailDlgAutoProxy()
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
		ASSERT_KINDOF(CSendMailDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CSendMailDlg)))
		{
			m_pDialog = reinterpret_cast<CSendMailDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CSendMailDlgAutoProxy::~CSendMailDlgAutoProxy()
{
	// Ϊ������ OLE �Զ����������ж������ֹӦ�ó���
	// 	������������ AfxOleUnlockApp��
	//  ���������������⣬�⻹���������Ի���
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CSendMailDlgAutoProxy::OnFinalRelease()
{
	// �ͷ��˶��Զ�����������һ�����ú󣬽�����
	// OnFinalRelease�����ཫ�Զ�
	// ɾ���ö����ڵ��øû���֮ǰ�����������
	// ��������ĸ���������롣

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CSendMailDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CSendMailDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// ע��: ��������˶� IID_ISendMail ��֧��
//  ��֧������ VBA �����Ͱ�ȫ�󶨡��� IID ����ͬ���ӵ� .IDL �ļ��е�
//  ���Ƚӿڵ� GUID ƥ�䡣

// {C5BE5524-3082-42CF-96C2-593FFCD61681}
static const IID IID_ISendMail =
{ 0xC5BE5524, 0x3082, 0x42CF, { 0x96, 0xC2, 0x59, 0x3F, 0xFC, 0xD6, 0x16, 0x81 } };

BEGIN_INTERFACE_MAP(CSendMailDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CSendMailDlgAutoProxy, IID_ISendMail, Dispatch)
END_INTERFACE_MAP()

// IMPLEMENT_OLECREATE2 ���ڴ���Ŀ�� StdAfx.h �ж���
// {676DA748-4C70-45F6-A2C5-50F651227F93}
IMPLEMENT_OLECREATE2(CSendMailDlgAutoProxy, "SendMail.Application", 0x676da748, 0x4c70, 0x45f6, 0xa2, 0xc5, 0x50, 0xf6, 0x51, 0x22, 0x7f, 0x93)


// CSendMailDlgAutoProxy ��Ϣ�������
