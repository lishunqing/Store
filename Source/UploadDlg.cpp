
// UploadDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Upload.h"
#include "UploadDlg.h"
#include "afxdialogex.h"
#include "sftp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUploadDlg 对话框



CUploadDlg::CUploadDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UPLOAD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUploadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUploadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CUploadDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CUploadDlg 消息处理程序

BOOL CUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUploadDlg::OnPaint()
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
HCURSOR CUploadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CStatic *text;


void step(float pensent,UINT total,UINT current)
{
	CString t;
	t.Format(_T("上传进度%2.2f%%,%d/%d"), pensent * 100, current, total);
	text->SetWindowText(t);
}



void CUploadDlg::OnBnClickedOk()
{
	text = (CStatic*)GetDlgItem(IDC_TEXT);
	CStatic *text = (CStatic*)GetDlgItem(IDC_TEXT);
	text->SetWindowText(_T("链接服务器"));
	CString t;

	sftp *inst = sftp::getInst();
	
	inst->setCallback(step);
	if (!inst->login())
	{
		t.Format(_T("链接服务器失败:%s"),inst->strLastError);
		text->SetWindowText(t);
		return;
	}
	CStringA remote;
	remote.Format("Store/bin/Installer.%s.exe", _ProductVersion);

	HRSRC hrSrcSys = FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(IDR_INSTALLER), _T("File"));
	HGLOBAL hGlobalSys = LoadResource(AfxGetResourceHandle(), hrSrcSys);
	LPVOID lpGlobalSys = LockResource(hGlobalSys);
	if (!hrSrcSys)
	{
		((CStatic*)GetDlgItem(IDC_TEXT))->SetWindowText(_T("无法释放文件。"));
	}
	UINT s = SizeofResource(AfxGetResourceHandle(), hrSrcSys);
	inst->upload((char*)lpGlobalSys, s, remote.GetBuffer());
	UnlockResource(hGlobalSys);
	FreeResource(hGlobalSys);

}
