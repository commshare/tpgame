// SendMailDlg.h : ͷ�ļ�
//

#pragma once

class CSendMailDlgAutoProxy;


// CSendMailDlg �Ի���
class CSendMailDlg : public CDialog
{
	DECLARE_DYNAMIC(CSendMailDlg);
	friend class CSendMailDlgAutoProxy;

// ����
public:
	CSendMailDlg(CWnd* pParent = NULL);	// ��׼���캯��
	virtual ~CSendMailDlg();

// �Ի�������
	enum { IDD = IDD_SENDMAIL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	CSendMailDlgAutoProxy* m_pAutoProxy;
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
