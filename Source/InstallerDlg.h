
// InstallerDlg.h : 头文件
//

#pragma once


// CInstallerDlg 对话框
class CInstallerDlg : public CDialogEx
{
// 构造
public:
	CInstallerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_INSTALLER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	bool checkInstall(void);
	void doInstall(void);
	void copyFiles(TCHAR*);
	void copyFile(TCHAR*, TCHAR*, TCHAR*);
};
