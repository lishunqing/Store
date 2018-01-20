
// InstallerDlg.cpp : ʵ���ļ�
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


// CInstallerDlg �Ի���



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


// CInstallerDlg ��Ϣ�������

BOOL CInstallerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if (checkInstall())
	{
		exit(0);
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CInstallerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
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
		MessageBox(_T("��⵽�״��ڱ��������б��������Ҫѡ��װĿ¼��\r���鰲װ�ڡ�D:\\������"), _T("��ʾ"));
		doInstall();
		if (!reg::loadConfig(INSTPATH, (BYTE*)inst, l))
		{
			((CStatic*)GetDlgItem(IDC_TEXT))->SetWindowText(_T("δ֪ԭ��װʧ�ܡ�"));
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
	//ѡ��һ��Ŀ¼
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = inst;
	bi.lpszTitle = _T("������ѡ��װĿ¼:");
	bi.ulFlags = BIF_NEWDIALOGSTYLE;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		MessageBox(_T("û��ѡ��װĿ¼��������ֹ��"), _T("��ʾ"));
		exit(0);
	}
	SHGetPathFromIDList(idl, inst);
	reg::saveConfig(INSTPATH, (BYTE*)inst, l);
}

void CInstallerDlg::copyFiles(TCHAR *inst)
{
	//���Դ���Ŀ¼
	CreateDirectory(inst, NULL);
	StrCat(inst, _T("bin"));
	CreateDirectory(inst, NULL);

	//Store.exe
	copyFile(inst, MAKEINTRESOURCE(IDR_STORE_EXE),_T("Store.exe"));
	MessageBox(_T("��װ�������������Ӧ�á�"), _T("��ʾ"));
	int rc = exec(inst, _T("Store.exe"));
	MessageBox(_T("����ʧ�ܣ�������ǣ�" ) + rc, _T("��ʾ"));
}

void CInstallerDlg::copyFile(TCHAR *inst, TCHAR *res, TCHAR *file)
{
	HRSRC hrSrcSys = FindResource(AfxGetResourceHandle(),res , _T("File"));
	HGLOBAL hGlobalSys = LoadResource(AfxGetResourceHandle(), hrSrcSys);
	LPVOID lpGlobalSys = LockResource(hGlobalSys);
	CString filename;
	if (!hrSrcSys)
	{
		filename.Format(_T("�޷��ͷ�:%s"), file);
		((CStatic*)GetDlgItem(IDC_TEXT))->SetWindowText(_T("�޷��ͷ�Store.exe��"));
	}
	UINT s = SizeofResource(AfxGetResourceHandle(), hrSrcSys);
	filename.Format(_T("%s\\%s"), inst, file);
	CFile ofs(filename, CFile::modeCreate | CFile::modeWrite);
	ofs.Write(lpGlobalSys, s);
	ofs.Close();
	UnlockResource(hGlobalSys);
	FreeResource(hGlobalSys);
}

