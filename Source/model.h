#pragma once
#include <map>
#include "json/json.h"
class model
{
public:
	CString Type;
	CString ID;
	CString Name;
	CString Model;
	CString Color;
	CString M1;
	CString M2;
	CString M3;
	CString Price;

	void setJson(Json::Value);
	Json::Value getJson(void);

};

class modellist
{
public:
	std::map<CString, model> ma;

	bool add(model);//���ر�������Ƿ��ڹ��������ڡ�������ӻ��޸��б�
	void del(CString);
	model find(CString);
	bool load(CDialog*);
	bool save(CDialog*);
	std::vector<model> query(CString,CString, CString, CString);
	std::vector<model> querysp(CString, CString, CString, CString);

	CString localFileName;

private:
	CString DateTime;
	CString CID;
	CString CNa;
	CString CMINP;
	CString CMAXP;
	bool condition(model);
	bool conditionsp(model);
};
