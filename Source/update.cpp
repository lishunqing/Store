#include "stdafx.h"
#include "update.h"
#include "sftp.h"
#include "util.h"

CDialogEx *_dlg;
int _x;
int _y;
int _w;
int _h;
CDC *_dc;

void setEcho(CDialogEx*dlg, int x, int y,int w,int h)
{
	_dlg = dlg;
	_x = x;
	_y = y;
	_w = w;
	_h = h;
}

void step(float pensent, UINT total, UINT current)
{
	CString t;
	CDC MemDC;
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SetStretchBltMode(HALFTONE);
	CBitmap MemBitmap;
	MemBitmap.CreateCompatibleBitmap(_dc, _w, _h);
	MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(0, 0, _w, _h, RGB(255, 255, 255));

	int x = 5, y = 5;
	CFont f;
	f.CreateFont(16,                           //   nHeight     
		0,                                     //   nWidth     
		0,                                     //   nEscapement   
		0,                                     //   nOrientation     
		FW_NORMAL,                             //   nWeight     
		FALSE,                                 //   bItalic     
		FALSE,                                 //   bUnderline     
		0,                                     //   cStrikeOut     
		DEFAULT_CHARSET,                       //   nCharSet     
		OUT_DEFAULT_PRECIS,                    //   nOutPrecision     
		CLIP_DEFAULT_PRECIS,                   //   nClipPrecision     
		DEFAULT_QUALITY,                       //   nQuality     
		DEFAULT_PITCH | FF_SWISS,              //   nPitchAndFamily       
		_T("����"));
	MemDC.SelectObject(&f);
	t.Format(_T("�����:%d"), current);
	MemDC.TextOut(x, y, t);
	y += 20;
	t.Format(_T("���ֽ�:%d"), total);
	MemDC.TextOut(x, y, t);
	y += 20;
	t.Format(_T("���ؽ���:%.2f%%"), pensent * 100);
	MemDC.TextOut(x, y, t);
	y += 20;

	_dc->BitBlt(_x, _y, _w, _h, &MemDC, 0, 0, SRCCOPY);
	MemBitmap.DeleteObject();
	MemDC.DeleteDC();
}


void checkupdate(CString installPath)
{
	//connect sftp to get file list
	sftp *inst = sftp::getInst();

	//login
	if (!inst->login())
	{
		CString t;
		t.Format(_T("���ӷ�����ʧ��:%s"), inst->strLastError);
		_dlg->MessageBox(t,_T("����"));
		return;
	}
	std::map<CStringA, LIBSSH2_SFTP_ATTRIBUTES> m;
	inst->list("Store/bin",m);

	CStringA currentVersion = _ProductVersion;
	//currentVersion = "0.01.20160615";
	CStringA lastVersion = "";
	CStringA lastFilename;
	LIBSSH2_SFTP_ATTRIBUTES attr;
	for (auto it = m.begin(); it != m.end(); ++it)
	{
		CStringA fileversion = it->first.Mid(10,13);

		if ((currentVersion < fileversion)&&(lastVersion < fileversion))
		{
			lastVersion = fileversion;
			lastFilename = it->first;
			attr = it->second;
		}
	}

	if (lastVersion.GetLength() > 0)
	{
		int rc = _dlg->MessageBox(_T("�����°汾���Ƿ����ظ��£�"), _T("��ʾ"), MB_YESNO|MB_ICONQUESTION);
		if (rc != IDYES)
			return;
	}
	else
	{
		_dlg->MessageBox(_T("û�з��ָ��°汾��"), _T("��ʾ"));
		return;
	}

	inst->setCallback(step);
	_dc = _dlg->GetDC();

	CString localPath;
	localPath.Format(_T("%sPatch"), installPath);
	CreateDirectory(localPath, NULL);
	localPath.Format(_T("%sPatch\\%s"), installPath, toWString(lastFilename));

	inst->download("Store/bin", lastFilename, localPath, attr);
	_dlg->ReleaseDC(_dc);

	_dlg->MessageBox(_T("������ɣ��������³���"), _T("��ʾ"));
	localPath.Format(_T("%sPatch"), installPath);
	int rc = exec(localPath, toWString(lastFilename));
	_dlg->MessageBox(_T("����ʧ�ܣ�������ǣ�") + rc, _T("��ʾ"));
}
