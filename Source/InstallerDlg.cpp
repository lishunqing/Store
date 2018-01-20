
// InstallerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Installer.h"
#include "InstallerDlg.h"
#include "afxdialogex.h"
#include "reg.h"
#include "util.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CInstallerDlg 对话框



CInstallerDlg::CInstallerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INSTALLER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CInstallerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CInstallerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CInstallerDlg 消息处理程序

BOOL CInstallerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if (checkInstall())
	{
		exit(0);
	}
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CInstallerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CInstallerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

bool CInstallerDlg::checkInstall(void)
{
	TCHAR inst[MAX_PATH] = _T("D:\\");
	DWORD l = sizeof(inst);
	if (!reg::loadConfig(INSTPATH, (BYTE*)inst, l))
	{
		MessageBox(_T("检测到首次在本机上运行本软件，需要选择安装目录！\r建议安装在“D:\\”即可"), _T("提示"));
		doInstall();
		if (!reg::loadConfig(INSTPATH, (BYTE*)inst, l))
		{
			((CStatic*)GetDlgItem(IDC_TEXT))->SetWindowText(_T("未知原因安装失败。"));
			return false;
		}
	}
	copyFiles(inst);
	return true;
}

void CInstallerDlg::doInstall(void)
{
	TCHAR inst[MAX_PATH] = _T("D:\\");
	DWORD l = sizeof(inst);
	//选择一个目录
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = inst;
	bi.lpszTitle = _T("从下面选择安装目录:");
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		MessageBox(_T("没有选择安装目录，程序终止！"), _T("提示"));
		exit(0);
	}
	SHGetPathFromIDList(idl, inst);
	reg::saveConfig(INSTPATH, (BYTE*)inst, l);
}

void CInstallerDlg::copyFiles(TCHAR *inst)
{
	//尝试创建目录
	CreateDirectory(inst, NULL);
	StrCat(inst, _T("bin"));
	CreateDirectory(inst, NULL);

	//Store.exe
	copyFile(inst, MAKEINTRESOURCE(IDR_STORE_EXE),_T("Store.exe"));
	MessageBox(_T("安装程序结束，重启应用。"), _T("提示"));
	int rc = exec(inst, _T("Store.exe"));
	MessageBox(_T("启动失败，结果码是：" ) + rc, _T("提示"));
}

void CInstallerDlg::copyFile(TCHAR *inst, TCHAR *res, TCHAR *file)
{
	HRSRC hrSrcSys = FindResource(AfxGetResourceHandle(),res , _T("File"));
	HGLOBAL hGlobalSys = LoadResource(AfxGetResourceHandle(), hrSrcSys);
	LPVOID lpGlobalSys = LockResource(hGlobalSys);
	CString filename;
	if (!hrSrcSys)
	{
		filename.Format(_T("无法释放:%s"), file);
		((CStatic*)GetDlgItem(IDC_TEXT))->SetWindowText(_T("无法释放Store.exe。"));
	}
	UINT s = SizeofResource(AfxGetResourceHandle(), hrSrcSys);
	filename.Format(_T("%s\\%s"), inst, file);
	CFile ofs(filename, CFile::modeCreate | CFile::modeWrite);
	ofs.Write(lpGlobalSys, s);
	ofs.Close();
	UnlockResource(hGlobalSys);
	FreeResource(hGlobalSys);
}

