#include "stdafx.h"
#include "sftp.h"  
#include <WS2tcpip.h>

static char _host[20] = "121.40.147.50";
static char _port[20] = "22";
static char _username[20] = "lisq";
static char _password[20] = "dsybs1121";

//��ʼ�����̵�ʱ�����  
//�����0��ʾ��ʼ��ʧ�ܣ�  
bool sftp::init()
{
	static bool inited = false;
	if (inited) return true;
	WSADATA wsadata;
	int rc = WSAStartup(MAKEWORD(2, 0), &wsadata);
	if (rc != 0) {
		return false;
	}

	rc = libssh2_init(0);
	if (rc != 0) {
		return false;
	}
	inited = true;
	return true;
}
void sftp::exit()
{
	libssh2_exit();
	WSACleanup();
}

void sftp::setCallback(SFTP_CALLBACK f)
{
	callback = f;
}

sftp * sftp::getInst(void)
{
	static sftp inst;
	init();
	return &inst;
}

bool sftp::login(void)
{
	return login(_host,_port,_username,_password);
}

bool sftp::upload(char* buff, UINT size, LPCSTR remotepath)
{
	if (NULL == ssh_session)
		login(_host, _port, _username, _password);
	//��SFTP�����������½��ļ�����  
	LIBSSH2_SFTP_HANDLE *sftp_handle = libssh2_sftp_open(sftp_session, remotepath,
			LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT | LIBSSH2_FXF_TRUNC,	LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR |		LIBSSH2_SFTP_S_IRGRP);

	if (!sftp_handle) {
		strLastError = _T("�޷���Զ���ļ�.");
		shutdown();
		return false;
	}
	size_t total = size;
	UINT count = 0,left = total;
	do {
		// �������д���ݣ�ֱ������д���  
		int wc = libssh2_sftp_write(sftp_handle, buff, left < 102400? left :102400);
		if (wc <= 0)
			break;
		buff += wc;
		left -= wc;
		count += wc;

		if (isBreak == true)
		{
			strLastError = _T("�û��жϴ���.");
			libssh2_sftp_close(sftp_handle);
			return false;
		}

		//��������˻ص������лص�  
		if (callback)
		{
			float p = (float)count / (float)total;
			callback(p,total,count);
		}
	} while (count < total);

	libssh2_sftp_close(sftp_handle);
	shutdown();
	return true;
}

bool sftp::upload(CString localpath, LPCSTR remotepath)
{
	if (NULL == ssh_session)
		login(_host, _port, _username, _password);
	//��SFTP�����������½��ļ�����  
	LIBSSH2_SFTP_HANDLE *sftp_handle = libssh2_sftp_open(sftp_session, remotepath,
		LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT | LIBSSH2_FXF_TRUNC, LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR | LIBSSH2_SFTP_S_IRGRP);

	if (!sftp_handle) {
		strLastError = _T("�޷���Զ���ļ�.");
		shutdown();
		return false;
	}

	CFile ifs(localpath, CFile::modeRead);
	ULONGLONG size = ifs.GetLength();
	if ((size >> 31) != 0)
	{
		strLastError = _T("�ļ�̫�󳬹�2GB.");
		libssh2_sftp_close(sftp_handle);
		return false;
	}
	UINT total = (UINT)size;
	UINT count = 0;
	while (true)
	{
		char buff[10240];
		UINT nread;
		nread = ifs.Read(buff, sizeof(buff));
		if (nread <= 0)
			break;
		char* wp = buff;
		UINT left = nread;

		while(left > 0)
		{
			// �������д���ݣ�ֱ������д���  
			int wc = libssh2_sftp_write(sftp_handle, wp, left);
			if (wc <= 0)
				break;
			wp += wc;
			left -= wc;
			count += wc;

			if (isBreak == true)
			{
				strLastError = _T("�û��жϴ���.");
				libssh2_sftp_close(sftp_handle);
				return false;
			}

			//��������˻ص������лص�  
			if (callback)
			{
				float p = (float)count / (float)total;
				callback(p, total, count);
			}
		}
	}

	ifs.Close();

	libssh2_sftp_close(sftp_handle);
	shutdown();
	return true;
}

bool sftp::download(LPCSTR remotepath,CString localpath)
{
	if (NULL == ssh_session)
		if (!login(_host, _port, _username, _password))
			return false;

	char remotedir[MAX_PATH];
	strcpy_s(remotedir, remotepath);
	int l = strlen(remotedir);
	for (; l > 0; --l)
	{
		if (remotedir[l] == '/')
		{
			remotedir[l] = '\0';
			break;
		}
	}
	char *remotefile = remotedir + l + 1;
	ULONGLONG size = 0;

	LIBSSH2_SFTP_HANDLE *sftp_handle = libssh2_sftp_opendir(sftp_session, remotedir);
	if (!sftp_handle) {
		strLastError = _T("�޷���Զ��Ŀ¼.");
		shutdown();
		return false;
	}
	while (true)
	{
		char longentry[MAX_PATH];
		LIBSSH2_SFTP_ATTRIBUTES attrs;

		/* loop until we fail */
		int rc = libssh2_sftp_readdir(sftp_handle,longentry, sizeof(longentry), &attrs);
		if (rc > 0) {
			if (strcmp(longentry,remotefile) == 0)
			{
				size = attrs.filesize;
			}
		}
		else
			break;

	}
	libssh2_sftp_closedir(sftp_handle);

	if ((size >> 31) != 0)
	{
		strLastError = _T("�ļ�̫�󳬹�2GB.");
		return false;
	}
	CFile ofs(localpath, CFile::modeCreate | CFile::modeWrite);

	//��SFTP�����������½��ļ�����  
	sftp_handle = libssh2_sftp_open(sftp_session, remotepath, LIBSSH2_FXF_READ, 0);
	if (!sftp_handle) {
		strLastError = _T("�޷���Զ���ļ�.");
		shutdown();
		return false;
	}

	UINT total = (UINT)size;
	UINT count = 0;
	while (true)
	{
		char buff[102400];
		UINT nread;
		nread = libssh2_sftp_read(sftp_handle, buff, sizeof(buff));
		if (nread <= 0)
			break;
		char* wp = buff;
		UINT left = nread;

		while (left > 0)
		{
			// �������д���ݣ�ֱ������д���  
			ofs.Write(wp, left);
			int wc = left;
			if (wc <= 0)
				break;
			wp += wc;
			left -= wc;
			count += wc;

			if (isBreak == true)
			{
				strLastError = _T("�û��жϴ���.");
				libssh2_sftp_close(sftp_handle);
				return false;
			}

			//��������˻ص������лص�  
			if (callback)
			{
				float p = (float)count / (float)total;
				callback(p, total, count);
			}
		}
	}

	libssh2_sftp_close(sftp_handle);
	ofs.Close();
	shutdown();
	return true;
}

bool sftp::download(LPCSTR remotepath, LPCSTR remotefile, CString localpath, LIBSSH2_SFTP_ATTRIBUTES &attr)
{
	auto size = attr.filesize;
	if ((size >> 31) != 0)
	{
		strLastError = _T("�ļ�̫�󳬹�2GB.");
		return false;
	}
	CFile ofs(localpath, CFile::modeCreate | CFile::modeWrite);
	CStringA rpf;
	rpf.Format("%s/%s", remotepath, remotefile);
	//��SFTP�����������½��ļ�����  
	LIBSSH2_SFTP_HANDLE *sftp_handle = libssh2_sftp_open(sftp_session, rpf, LIBSSH2_FXF_READ, 0);
	if (!sftp_handle) {
		strLastError = _T("�޷���Զ���ļ�.");
		shutdown();
		return false;
	}

	UINT total = (UINT)size;
	UINT count = 0;
	while (true)
	{
		char buff[102400];
		UINT nread;
		nread = libssh2_sftp_read(sftp_handle, buff, sizeof(buff));
		if (nread <= 0)
			break;
		char* wp = buff;
		UINT left = nread;

		while (left > 0)
		{
			// �������д���ݣ�ֱ������д���  
			ofs.Write(wp, left);
			int wc = left;
			if (wc <= 0)
				break;
			wp += wc;
			left -= wc;
			count += wc;

			if (isBreak == true)
			{
				strLastError = _T("�û��жϴ���.");
				libssh2_sftp_close(sftp_handle);
				return false;
			}

			//��������˻ص������лص�  
			if (callback)
			{
				float p = (float)count / (float)total;
				callback(p, total, count);
			}
		}
	}

	libssh2_sftp_close(sftp_handle);
	ofs.Close();
	shutdown();
	return true;
}

bool sftp::list(LPCSTR remotedir, std::map<CStringA, LIBSSH2_SFTP_ATTRIBUTES>&m)
{
	m.clear();
	LIBSSH2_SFTP_HANDLE *sftp_handle = libssh2_sftp_opendir(sftp_session, remotedir);
	if (!sftp_handle) {
		strLastError.Format(_T("[%s][%s]:�޷���Զ��Ŀ¼."), __FILE__, __LINE__);
		shutdown();
		return false;
	}
	while (true)
	{
		char longentry[MAX_PATH];
		LIBSSH2_SFTP_ATTRIBUTES attrs;

		/* loop until we fail */
		int rc = libssh2_sftp_readdir(sftp_handle, longentry, sizeof(longentry), &attrs);
		if (rc > 0) {
			CStringA filename = longentry;
			if (filename.Find("Installer") == 0)
			{
				m[filename] = attrs;
			}
		}
		else
			break;

	}
	libssh2_sftp_closedir(sftp_handle);
	return true;
}

bool sftp::login(LPCSTR host, LPCSTR port, LPCSTR username, LPCSTR password)
{
	const char *fingerprint;
	int rc;

	//�½����� 
	ADDRINFO hints;
	memset(&hints, 0, sizeof(ADDRINFO));
	hints.ai_family = AF_INET;     /* Allow IPv4 */
	hints.ai_flags = AI_PASSIVE;/* For wildcard IP address */
	hints.ai_protocol = 0;         /* Any protocol */
	hints.ai_socktype = SOCK_STREAM;

	ADDRINFO *hostent;
	rc = GetAddrInfoA(host, port, &hints, &hostent);
	if (rc != 0) {
		strLastError = _T("�Ƿ���IP��ַ��������.");
		return false;
	}

	sock = socket(hostent->ai_family, hostent->ai_socktype, hostent->ai_protocol);
	if (sock < 0)
	{
		strLastError = _T("�޷�����SOCKET�׽���.");
		return false;
	}
	rc = connect(sock, hostent->ai_addr, hostent->ai_addrlen);
	if (rc != 0) {
		strLastError = _T("�޷�����Զ�˷����������������������");
		shutdown();
		return false;
	}

	//�½��Ի�ʵ��  
	ssh_session = libssh2_session_init();
	if (!ssh_session)
	{
		shutdown();
		return false;
	}

	//���õ�������  
	libssh2_session_set_blocking(ssh_session, 1);

	//��������  
	rc = libssh2_session_handshake(ssh_session, sock);
	if (rc) {
		strLastError = _T("SSH����ʧ�ܡ�");
		shutdown();
		return false;
	}

	//�������ָ��  
	fingerprint = libssh2_hostkey_hash(ssh_session, LIBSSH2_HOSTKEY_HASH_SHA1);

	//ͨ��������֤��½�û����  
	if (libssh2_userauth_password(ssh_session, username, password)) {
		strLastError = _T("�û��������ȷ��");
		shutdown();
		return false;
	}

	sftp_session = libssh2_sftp_init(ssh_session);

	if (!sftp_session) {
		strLastError = _T("SFTP�Ựʧ�ܡ�");
		shutdown();
		return false;
	}

	return true;
}

void sftp::shutdown()
{
	if (sftp_session)
	{
		libssh2_sftp_shutdown(sftp_session);
		sftp_session = NULL;
	}
	if (ssh_session)
	{
		libssh2_session_disconnect(ssh_session, "Normal Shutdown, Thank you for playing");
		libssh2_session_free(ssh_session);
		ssh_session = NULL;
	}
	if (sock > 0)
	{
		closesocket(sock);
		sock = -1;
	}
}


bool sftp::IsAbilityConn(PCSTR host, PCSTR port, PCSTR username, PCSTR password)
{
	sftp inst;
	if (inst.login(host, port, username, password))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool sftp::upload(PCSTR host, PCSTR port, PCSTR username, PCSTR password, PCSTR localpath, PCSTR remotepath)
{
	int nR = 0;
	int rc = -1;
	LIBSSH2_SFTP_HANDLE *sftp_handle;

	FILE *local = NULL;
	if (fopen_s(&local, localpath, "rb") != 0) {
		strLastError = _T("�޷��򿪱����ļ�.");
		return false;
	}

	//ȡ���ϴ��ļ�����size.  
	fseek(local, 0, SEEK_END);
	size_t filesize = ftell(local);//local file��С����readFromDisk�б�����  
	fseek(local, 0, SEEK_SET);//�ļ�ָ�����õ��ļ�ͷ  

	//�½����� 
	if (!login(host, port, username, password))
		return false;


	//��SFTP�����������½��ļ�����  
	sftp_handle =
		libssh2_sftp_open(sftp_session, remotepath,
			LIBSSH2_FXF_WRITE | LIBSSH2_FXF_CREAT | LIBSSH2_FXF_TRUNC,
			LIBSSH2_SFTP_S_IRUSR | LIBSSH2_SFTP_S_IWUSR |
			LIBSSH2_SFTP_S_IRGRP);

	if (!sftp_handle) {
		strLastError = _T("�޷���Զ���ļ�.");
		fclose(local);
		shutdown();
		return false;
	}


	char mem[1024 * 16];
	size_t nread;
	char *ptr;
	size_t count = 0;

	do {
		nread = fread(mem, 1, sizeof(mem), local);
		if (nread <= 0) {
			//�����ļ�β��  
			break;
		}
		ptr = mem;
		do {
			// �������д���ݣ�ֱ������д���  
			rc = libssh2_sftp_write(sftp_handle, ptr, nread);
			if (rc < 0)
				break;
			ptr += rc; count += nread;
			nread -= rc;

			if (isBreak == true)
			{
				strLastError.Format(_T("[%s][%s]:Task be breaked."), __FILE__, __LINE__);
				libssh2_sftp_close(sftp_handle);
				fclose(local);
				shutdown();
				return false;
			}

			//��������˻ص������лص�  
			if (callback)
			{
				float p = count / (float)filesize;
				callback(p, filesize,count);
			}
			//callback.end  
		} while (nread);
	} while (rc > 0);

	libssh2_sftp_close(sftp_handle);
	fclose(local);
	shutdown();

	return true;
}

bool sftp::download(PCSTR host, PCSTR port, PCSTR username, PCSTR password, PCSTR remotepath, PCSTR localpath)
{
	int  auth_pw = 0;
	int rc;
	LIBSSH2_SFTP_HANDLE *sftp_handle;

	//�½����� 
	if (!login(host, port, username, password))
		return false;

	/* Request a file via SFTP */
	sftp_handle = libssh2_sftp_open(sftp_session, remotepath, LIBSSH2_FXF_READ, 0);
	if (!sftp_handle) {
		strLastError = _T("�޷���Զ���ļ�.");
		shutdown();
		return false;
	}

	FILE *local = NULL;
	if (fopen_s(&local, localpath, "wb") != 0) {
		strLastError = _T("�޷��򿪱����ļ�.");
		libssh2_sftp_close(sftp_handle);
		shutdown();
		return false;
	}

	do {
		char mem[1024];

		/* loop until we fail */
		rc = libssh2_sftp_read(sftp_handle, mem, sizeof(mem));

		if (rc > 0) {
			//���ڴ浽����  
			fwrite(mem, 1, rc, local);
		}
		else {
			break;
		}
	} while (1);

	fclose(local);

	libssh2_sftp_close(sftp_handle);

	shutdown();

	return 0;
}
