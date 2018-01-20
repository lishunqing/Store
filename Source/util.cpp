#include "stdafx.h"
#include "util.h"

CStringW toWString(const char *v)
{
	int unicodeLen = MultiByteToWideChar(CP_UTF8, 0, v, -1, NULL, 0) + 1;
	wchar_t * pUnicode = new wchar_t[unicodeLen];
	MultiByteToWideChar(CP_UTF8, 0, v, -1, pUnicode, unicodeLen);
	CStringW rt(pUnicode);
	delete pUnicode;
	return rt;
}

CStringA toAString(const wchar_t *v)
{
	int ansiLen = WideCharToMultiByte(CP_UTF8, 0, v, -1, NULL, 0, NULL, NULL) + 1;
	char* pAnsi = new char[ansiLen];
	WideCharToMultiByte(CP_UTF8, 0, v, -1, pAnsi, ansiLen, NULL, NULL);
	CStringA rt(pAnsi);
	delete pAnsi;
	return rt;
}

int exec(CString instpath, CString file)
{
	CString p;
	p.Format(_T("%s\\%s"), instpath, file);
	char ansip[MAX_PATH];
	WideCharToMultiByte(CP_UTF8, 0, p, -1, ansip, sizeof(ansip), NULL, NULL);
	char ansif[MAX_PATH];
	WideCharToMultiByte(CP_UTF8, 0, file, -1, ansif, sizeof(ansif), NULL, NULL);
	int rc = _execl(ansip, ansif, NULL);

	return rc;
}
