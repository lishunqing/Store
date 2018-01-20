
// UploadDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Upload.h"
#include "UploadDlg.h"
#include "afxdialogex.h"
#include "sftp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUploadDlg �Ի���



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


// CUploadDlg ��Ϣ�������

BOOL CUploadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUploadDlg::OnPaint()
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
HCURSOR CUploadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CStatic *text;


void step(float pensent,UINT total,UINT current)
{
	CString t;
	t.Format(_T("�ϴ�����%2.2f%%,%d/%d"), pensent * 100, current, total);
	text->SetWindowText(t);
}



void CUploadDlg::OnBnClickedOk()
{
	text = (CStatic*)GetDlgItem(IDC_TEXT);
	CStatic *text = (CStatic*)GetDlgItem(IDC_TEXT);
	text->SetWindowText(_T("���ӷ�����"));
	CString t;

	sftp *inst = sftp::getInst();
	
	inst->setCallback(step);
	if (!inst->login())
	{
		t.Format(_T("���ӷ�����ʧ��:%s"),inst->strLastError);
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
		((CStatic*)GetDlgItem(IDC_TEXT))->SetWindowText(_T("�޷��ͷ��ļ���"));
	}
	UINT s = SizeofResource(AfxGetResourceHandle(), hrSrcSys);
	inst->upload((char*)lpGlobalSys, s, remote.GetBuffer());
	UnlockResource(hGlobalSys);
	FreeResource(hGlobalSys);

}
