#pragma once

class reg
{
public:
	static void setAutoRun(bool startOpen = true);
	static bool saveConfig(const CString k, BYTE* v, DWORD &l);
	static bool loadConfig(const CString k, BYTE* v, DWORD &l);

};
