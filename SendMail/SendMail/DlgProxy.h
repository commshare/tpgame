// DlgProxy.h: ͷ�ļ�
//

#pragma once

class CSendMailDlg;


// CSendMailDlgAutoProxy ����Ŀ��

class CSendMailDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CSendMailDlgAutoProxy)

	CSendMailDlgAutoProxy();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��

// ����
public:
	CSendMailDlg* m_pDialog;

// ����
public:

// ��д
	public:
	virtual void OnFinalRelease();

// ʵ��
protected:
	virtual ~CSendMailDlgAutoProxy();

	// ���ɵ���Ϣӳ�亯��

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CSendMailDlgAutoProxy)

	// ���ɵ� OLE ����ӳ�亯��

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

