
// InstallerDlg.h : ͷ�ļ�
//

#pragma once


// CInstallerDlg �Ի���
class CInstallerDlg : public CDialogEx
{
// ����
public:
	CInstallerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSTALLER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	bool checkInstall(void);
	void doInstall(void);
	void copyFiles(TCHAR*);
	void copyFile(TCHAR*, TCHAR*, TCHAR*);
};
