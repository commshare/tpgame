// QQToolDlg.h : ͷ�ļ�
//

#pragma once

class CQQToolDlgAutoProxy;


// CQQToolDlg �Ի���
class CQQToolDlg : public CDialog
{
	DECLARE_DYNAMIC(CQQToolDlg);
	friend class CQQToolDlgAutoProxy;

// ����
public:
	CQQToolDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CQQToolDlg();

// �Ի�������
	enum { IDD = IDD_QQTOOL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	CQQToolDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
};
