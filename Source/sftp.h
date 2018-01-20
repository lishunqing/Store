#pragma once
#include<map>
#include "libssh2_sftp.h" 

typedef void(*SFTP_CALLBACK)(float, UINT, UINT);

class sftp
{
public:
	static bool init();
	static void exit();
	bool upload(PCSTR host, PCSTR port, PCSTR username, PCSTR password, PCSTR localpath, PCSTR remotepath);
	bool download(PCSTR host, PCSTR port, PCSTR username, PCSTR password, PCSTR remotepath, PCSTR localpath);

	//测试SFTP服务器是否可以链接  
	static bool IsAbilityConn(PCSTR ip, PCSTR port, PCSTR username, PCSTR password);

	//设置回掉函数,用于写进度.  
	void setCallback(SFTP_CALLBACK);

	//存放最近的错误信息  
	CString strLastError;

	static sftp* getInst(void);
	bool login(void);
	bool upload(char* buff, UINT size, LPCSTR remotepath);
	bool upload(CString, LPCSTR);
	bool download(LPCSTR,CString);
	bool download(LPCSTR,LPCSTR, CString, LIBSSH2_SFTP_ATTRIBUTES&);

	bool list(LPCSTR, std::map<CStringA, LIBSSH2_SFTP_ATTRIBUTES>&);

	//用于停止当前上传或下载线程  
	void stop() { isBreak = true; }
	~sftp() { shutdown(); };
private:
	bool login(LPCSTR host, LPCSTR port, LPCSTR username, LPCSTR password);
	void shutdown();

	//void (CObject::*callback)(float, UINT, UINT);
	SFTP_CALLBACK callback = NULL;
	bool isBreak = false;
	LIBSSH2_SESSION *ssh_session = NULL;
	LIBSSH2_SFTP *sftp_session = NULL;
	int sock = -1;
};
