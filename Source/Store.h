
// Store.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CStoreApp: 
// �йش����ʵ�֣������ Store.cpp
//

class CStoreApp : public CWinApp
{
public:
	CStoreApp();

// ��д
public:
	virtual BOOL InitInstance();
	CString installPath;
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CStoreApp theApp;