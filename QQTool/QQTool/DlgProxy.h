// DlgProxy.h: ͷ�ļ�
//

#pragma once

class CQQToolDlg;


// CQQToolDlgAutoProxy ����Ŀ��

class CQQToolDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CQQToolDlgAutoProxy)

	CQQToolDlgAutoProxy();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

// ����
public:
	CQQToolDlg* m_pDialog;

// ����
public:

// ��д
	public:
	virtual void OnFinalRelease();

// ʵ��
protected:
	virtual ~CQQToolDlgAutoProxy();

	// ���ɵ���Ϣӳ�亯��

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CQQToolDlgAutoProxy)

	// ���ɵ� OLE ����ӳ�亯��

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

