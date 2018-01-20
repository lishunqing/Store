#include "stdafx.h"
#include "model.h"
//#include "sftp.h"
#include "util.h"
#include <fstream>  

void model::setJson(Json::Value v)
{

	ID = toWString(v["ID"].asCString());
	Name = toWString(v["Name"].asCString());
	Model = toWString(v["Model"].asCString());
	Color = toWString(v["Color"].asCString());
	M1 = toWString(v["M1"].asCString());
	M2 = toWString(v["M2"].asCString());
	M3 = toWString(v["M3"].asCString());
	Price = toWString(v["Price"].asCString());
}

Json::Value model::getJson(void)
{
	Json::Value ret;
#ifdef  UNICODE
	ret["ID"] = Json::Value(toAString(ID));
	ret["Name"] = Json::Value(toAString(Name));
	ret["Model"] = Json::Value(toAString(Model));
	ret["Color"] = Json::Value(toAString(Color));
	ret["M1"] = Json::Value(toAString(M1));
	ret["M2"] = Json::Value(toAString(M2));
	ret["M3"] = Json::Value(toAString(M3));
	ret["Price"] = Json::Value(toAString(Price));



#else   /* UNICODE */               // r_winnt

	ret["ID"] = Json::Value(ID);
	ret["Name"] = Json::Value(Name);
	ret["Model"] = Json::Value(Model);
	ret["Color"] = Json::Value(Color);
	ret["M1"] = Json::Value(M1);
	ret["M2"] = Json::Value(M2);
	ret["M3"] = Json::Value(M3);
	ret["Price"] = Json::Value(Price);

#endif /* UNICODE */                // r_winnt
	return ret;
}

bool modellist::add(model mo)
{
	ma[mo.ID] = mo;
	if (mo.Name == _T("饰品标签"))
		return conditionsp(mo);
	else
		return condition(mo);
}

void modellist::del(CString id)
{
	ma.erase(id);
}

model modellist::find(CString ID)
{
	auto it = ma.find(ID);
	if (ma.end() != it)
		return it->second;
	return model();
}

bool modellist::load(CDialog *w)
{
	//mkdir
	TCHAR *lp = localFileName.GetBuffer();

	for (UINT i = 0; *(lp + i); ++i)
	{
		if (*(lp + i) == _T('\\'))
		{
			*(lp + i) = _T('\0');
			CreateDirectory(lp, NULL);
			*(lp + i) = _T('\\');
		}
	}
	localFileName.ReleaseBuffer();
	//download
	//sftp *s = sftp::getInst();
	//if (!s->download("Store/data/model/1.json", localFileName))
	//{
	//	CString msg = _T("无法从远端下载数据，错误原因：\n") + s->strLastError + _T("\n数据无法上传服务器。");
	//	w->MessageBox(msg, _T("提示"));
	//}


	Json::Value root;
	Json::Reader reader;

	std::ifstream ifs;
	ifs.open(localFileName, std::ios::binary);
	if (reader.parse(ifs, root, false))
	{

		ma.clear();
		DateTime = toWString(root["time"].asCString());

		auto data = root["data"];
		if (data.isArray())
		{
			int c = root["data"].size();
			for (int i = 0; i < c; ++i)
			{
				model m;
				m.setJson(data[i]);
				add(m);
			}
		}
	}
	ifs.close();

	return true;
}

bool modellist::save(CDialog *w)
{
	CTime tm = CTime::GetCurrentTime();
	DateTime = tm.Format(_T("%Y-%m-%d %H:%M:%S"));
	Json::Value root;
	root["time"] = Json::Value(toAString(DateTime));

	int i = 0;
	for (auto it = ma.begin(); it != ma.end(); ++it)
	{
		auto item = it->second.getJson();
		root["data"][i++] = Json::Value(item);
	}

	Json::FastWriter writer;

	std::ofstream ofs;
	ofs.open(localFileName);
	ofs << writer.write(root);
	
	ofs.close();

	//upload
	//sftp *s = sftp::getInst();
	//s->upload(localFileName, "Store/data/model/1.json");
	return true;
}

std::vector<model> modellist::query(CString CondID = _T(""), CString CondNa = _T(""), CString CondPMin = _T(""), CString CondPMax = _T(""))
{
	CID = CondID;
	CNa = CondNa;
	CMINP = CondPMin;
	CMAXP = CondPMax;
	std::vector<model> r;
	for (auto it = ma.begin(); it != ma.end(); ++it)
	{
		auto m = it->second;
		if (condition(m))
			r.push_back(m);
	}
	return r;
}

std::vector<model> modellist::querysp(CString CondID = _T(""), CString CondNa = _T(""), CString CondPMin = _T(""), CString CondPMax = _T(""))
{
	CID = CondID;
	CNa = CondNa;
	CMINP = CondPMin;
	CMAXP = CondPMax;
	std::vector<model> r;
	for (auto it = ma.begin(); it != ma.end(); ++it)
	{
		auto m = it->second;
		if (conditionsp(m))
			r.push_back(m);
	}
	return r;
}

bool modellist::condition(model m)
{
	if (m.ID.Find(CID) != 0)
		return false;
	if (m.Name.Find(CNa) != 0)
		return false;

	if (m.Name == _T("饰品标签"))
		return false;

	int l = m.Price.Find(_T("￥"));

	int p = _ttoi(m.Price.Mid(l + 1));

	int minp = _ttoi(CMINP);
	if ((minp > 0) && (minp > p))
		return false;

	int maxp = _ttoi(CMAXP);
	if ((maxp > 0) && (maxp < p))
		return false;

	return true;
}


bool modellist::conditionsp(model m)
{
	if (m.ID.Find(CID) != 0)
		return false;

	if (m.Name != _T("饰品标签"))
		return false;

	int l = m.Price.Find(_T("￥"));

	int p = _ttoi(m.Price.Mid(l + 1));

	int minp = _ttoi(CMINP);
	if ((minp > 0) && (minp > p))
		return false;

	int maxp = _ttoi(CMAXP);
	if ((maxp > 0) && (maxp < p))
		return false;

	return true;
}

