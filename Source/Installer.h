
// Installer.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CInstallerApp: 
// �йش����ʵ�֣������ Installer.cpp
//

class CInstallerApp : public CWinApp
{
public:
	CInstallerApp();

// ��д
public:
	virtual BOOL InitInstance();
	CString installPath;
	// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CInstallerApp theApp;