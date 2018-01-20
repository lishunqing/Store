#include "stdafx.h"
#include "reg.h"
#include <atlbase.h>

void reg::setAutoRun(bool startOpen)
{
	const LPCSTR lpRun = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	if (startOpen)
	{
		//��������
		HKEY hKey;
		//---------------�ҵ�ϵͳ�������� 
		long lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
		//-----------��������Key  
		if (lRet == ERROR_SUCCESS)
		{
			char pFileName[MAX_PATH] = { 0 };
			DWORD dwRet = GetModuleFileNameA(NULL, pFileName, MAX_PATH);
			//----------------�õ����������ȫ·�� 
			lRet = RegSetValueExA(hKey, "Store", 0, REG_SZ, (BYTE *)pFileName, dwRet);
			//----------------��ӻ����޸�ע�����
			RegCloseKey(hKey);
			//----------------�ر�ע���
		}
	}
	else
	{
		//ȡ����������
		HKEY hKey;
		//---------------�ҵ�ϵͳ�������� 
		long lRet = RegOpenKeyExA(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
		if (lRet == ERROR_SUCCESS)
		{
			RegDeleteValueA(hKey, "DeskPro");
			//----------------ɾ��ע�����
			RegCloseKey(hKey);
			//----------------�ر�ע���
		}
	}
}

static LPCTSTR lpConfig = _T("Software\\Store\\Config");

bool reg::saveConfig(const CString k, BYTE* v, DWORD &l)
{
	HKEY hKey;
	//---------------�ҵ�ϵͳ�������� 
	long lRet = RegCreateKey(HKEY_CURRENT_USER, lpConfig, &hKey);
	if (lRet != ERROR_SUCCESS)
		return false;


	//	DWORD l = v.GetLength();
	//	BYTE* b = (BYTE*)v.GetBuffer(l);
	lRet = RegSetValueEx(hKey, k, 0, REG_SZ, v, l);
	RegCloseKey(hKey);
	return (lRet == ERROR_SUCCESS);
}

bool reg::loadConfig(const CString k, BYTE* v, DWORD &l)
{
	HKEY hKey;
	//	BYTE buff[1024] = {0};
	//	DWORD l = 1024;
	long lRet = RegOpenKeyEx(HKEY_CURRENT_USER, lpConfig, NULL, KEY_QUERY_VALUE, &hKey);
	if (lRet != ERROR_SUCCESS)
		return false;
	lRet = RegQueryValueEx(hKey, k, NULL, NULL, v, &l);
	RegCloseKey(hKey);
	if (lRet == ERROR_SUCCESS)
	{
		return true;
	}
	return false;
}
