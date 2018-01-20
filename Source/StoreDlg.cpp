
// StoreDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Store.h"
#include "StoreDlg.h"
#include "reg.h"
#include "print.h"
//#include "update.h"

#include <WinSpool.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//资源文件中1单位=2像素
//打印区域，左边10单位，上边174单位，宽126单位，高252单位



#define TAGX 10
#define TAGY 230
#define TAGW 126
#define TAGH 252
//上面1单位=0.001毫米
//120DPI,1毫米=4.7244像素,1像素=216.67单元
//像素=单位*2
//英寸=像素/DPI
//毫米=英寸*25.4=像素/DPI*25.4=单位*2/DPI*25.4
//输入参数=单位*50800/DPI
#define DPU 50800

CStoreDlg::CStoreDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_STORE_DIALOG, pParent), _pd(FALSE, PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStoreDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MODIFY, &CStoreDlg::OnBnClickedModify)
	ON_BN_CLICKED(IDC_DELETE, &CStoreDlg::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_SETPRINTER, &CStoreDlg::OnBnClickedSetPrinter)
	ON_BN_CLICKED(IDC_PRINT, &CStoreDlg::OnBnClickedPrint)
	ON_BN_CLICKED(IDC_REMOVE, &CStoreDlg::OnBnClickedRemove)
	ON_NOTIFY(NM_CLICK, IDC_QL, &CStoreDlg::OnNMClickQl)
	ON_NOTIFY(NM_DBLCLK, IDC_QL, &CStoreDlg::OnNMDblclkQl)
	ON_NOTIFY(NM_CLICK, IDC_PL, &CStoreDlg::OnNMClickPl)
	ON_NOTIFY(NM_DBLCLK, IDC_PL, &CStoreDlg::OnNMDblclkPl)
	ON_BN_CLICKED(IDC_ADD10, &CStoreDlg::OnBnClickedAdd10)
	ON_BN_CLICKED(IDC_ADD5, &CStoreDlg::OnBnClickedAdd5)
	ON_BN_CLICKED(IDC_ADD3, &CStoreDlg::OnBnClickedAdd3)
	ON_BN_CLICKED(IDC_ADD1, &CStoreDlg::OnBnClickedAdd1)
	ON_BN_CLICKED(IDC_DEL1, &CStoreDlg::OnBnClickedDel1)
	ON_BN_CLICKED(IDC_DEL3, &CStoreDlg::OnBnClickedDel3)
	ON_BN_CLICKED(IDC_DEL5, &CStoreDlg::OnBnClickedDel5)
	ON_BN_CLICKED(IDC_DEL10, &CStoreDlg::OnBnClickedDel10)
	ON_EN_CHANGE(IDC_QID, &CStoreDlg::query)
	ON_EN_CHANGE(IDC_QNA, &CStoreDlg::query)
	ON_EN_CHANGE(IDC_QMINP, &CStoreDlg::query)
	ON_EN_CHANGE(IDC_QMAXP, &CStoreDlg::query)
	ON_CBN_SELCHANGE(IDC_MM, &CStoreDlg::OnCbnSelchangeMm)
	ON_EN_KILLFOCUS(IDC_PR, &CStoreDlg::OnEnKillfocusPr)
	ON_BN_CLICKED(IDC_UPDATE, &CStoreDlg::OnBnClickedUpdate)
	ON_EN_CHANGE(IDC_ID, &CStoreDlg::preview)
	ON_CBN_EDITCHANGE(IDC_NA, &CStoreDlg::preview)
	ON_CBN_EDITCHANGE(IDC_MO, &CStoreDlg::preview)
	ON_CBN_EDITCHANGE(IDC_CO, &CStoreDlg::preview)
	ON_EN_CHANGE(IDC_M1, &CStoreDlg::preview)
	ON_EN_CHANGE(IDC_M2, &CStoreDlg::preview)
	ON_EN_CHANGE(IDC_M3, &CStoreDlg::preview)
	ON_EN_CHANGE(IDC_PR, &CStoreDlg::preview)
	ON_CBN_SELCHANGE(IDC_NA, &CStoreDlg::OnCbnSelchangeNa)
	ON_CBN_SELCHANGE(IDC_MO, &CStoreDlg::OnCbnSelchangeMo)
	ON_CBN_SELCHANGE(IDC_CO, &CStoreDlg::OnCbnSelchangeCo)
	ON_BN_CLICKED(IDC_R1, &CStoreDlg::OnBnClickedR1)
	ON_BN_CLICKED(IDC_R2, &CStoreDlg::OnBnClickedR2)
	ON_BN_CLICKED(IDC_R3, &CStoreDlg::OnBnClickedR3)
END_MESSAGE_MAP()

void setupPrint(void)
{
	DWORD dwSize, dwPrinters;
	EnumPrinters(PRINTER_ENUM_LOCAL, NULL, 2, NULL, 0, &dwSize, &dwPrinters);
	BYTE *pBuffer = new BYTE[dwSize];
	EnumPrinters(PRINTER_ENUM_CONNECTIONS | PRINTER_ENUM_LOCAL, NULL, 2, pBuffer, dwSize, &dwSize, &dwPrinters);

	if (dwPrinters != 0)
	{
		PRINTER_INFO_2 *pPrnInfo = (PRINTER_INFO_2 *)pBuffer;
		for (DWORD i = 0; i < dwPrinters; i++)
		{
			if (StrCmp(pPrnInfo->pPrinterName, _T("ZDesigner GK888t (EPL)")) == 0)
			{

				HANDLE Printer;
				PRINTER_DEFAULTS pDefault;
				memset(&pDefault, 0, sizeof(pDefault));
				pDefault.DesiredAccess = PRINTER_ALL_ACCESS;
				OpenPrinter(pPrnInfo->pPrinterName, &Printer, &pDefault);
				pPrnInfo->pDevMode->dmPaperSize = DMPAPER_USER;
				pPrnInfo->pDevMode->dmPaperWidth = 800;
				pPrnInfo->pDevMode->dmPaperLength = 400;
				pPrnInfo->pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
				SetPrinter(Printer, 2, (BYTE *)pPrnInfo, NULL);
			}
			else if (StrCmp(pPrnInfo->pPrinterName, _T("Microsoft XPS Document Writer")) == 0)
			{

				HANDLE Printer;
				PRINTER_DEFAULTS pDefault;
				memset(&pDefault, 0, sizeof(pDefault));
				pDefault.DesiredAccess = PRINTER_ALL_ACCESS;
				OpenPrinter(pPrnInfo->pPrinterName, &Printer, &pDefault);
				pPrnInfo->pDevMode->dmPaperSize = DMPAPER_USER;
				pPrnInfo->pDevMode->dmPaperWidth = 800;
				pPrnInfo->pDevMode->dmPaperLength = 400;
				pPrnInfo->pDevMode->dmOrientation = DMORIENT_LANDSCAPE;
				SetPrinter(Printer, 2, (BYTE *)pPrnInfo, NULL);
			}

			pPrnInfo++;//指针后移   
		}
	}
	delete[]pBuffer;
}


// CStoreDlg 消息处理程序

BOOL CStoreDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CStoreApp *app = (CStoreApp *)AfxGetApp();
	if (app->installPath.GetLength() == 0)
	{
		MessageBox(_T("无法获取安装路径。"), _T("错误"));

		TCHAR inst[MAX_PATH] = _T("D:\\");
		DWORD l = sizeof(inst);
		//选择一个目录
		BROWSEINFO bi;
		ZeroMemory(&bi, sizeof(BROWSEINFO));
		bi.hwndOwner = NULL;
		bi.pszDisplayName = inst;
		bi.lpszTitle = _T("从下面选择安装目录:");
		bi.ulFlags = BIF_NEWDIALOGSTYLE;
		LPITEMIDLIST idl = SHBrowseForFolder(&bi);
		if (NULL == idl)
		{
			MessageBox(_T("没有选择安装目录，程序终止！"), _T("提示"));
			exit(0);
		}
		SHGetPathFromIDList(idl, inst);
		reg::saveConfig(INSTPATH, (BYTE*)inst, l);
		CreateDirectory(inst, NULL);
		StrCat(inst, _T("bin"));
		CreateDirectory(inst, NULL);
		if (NULL == idl)
		{
			MessageBox(_T("安装完成，请重新启动本程序！"), _T("提示"));
			exit(0);
		}

	}
	ml.localFileName.Format(_T("%s\\data\\model.json"), app->installPath);
	ml.load(this);

	//setupPrint();


	eid = (CEdit*)GetDlgItem(IDC_ID);
	ena = (CComboBox*)GetDlgItem(IDC_NA);
	emo = (CComboBox*)GetDlgItem(IDC_MO);
	eco = (CComboBox*)GetDlgItem(IDC_CO);
	emm = (CComboBox*)GetDlgItem(IDC_MM);
	em1 = (CEdit*)GetDlgItem(IDC_M1);
	em2 = (CEdit*)GetDlgItem(IDC_M2);
	em3 = (CEdit*)GetDlgItem(IDC_M3);
	epr = (CEdit*)GetDlgItem(IDC_PR);
	est = (CEdit*)GetDlgItem(IDC_ST);
	epc = (CEdit*)GetDlgItem(IDC_PC);

	qid = (CEdit*)GetDlgItem(IDC_QID);
	qna = (CEdit*)GetDlgItem(IDC_QNA);
	qminp = (CEdit*)GetDlgItem(IDC_QMINP);
	qmaxp = (CEdit*)GetDlgItem(IDC_QMAXP);

	r1 = (CButton*)GetDlgItem(IDC_R1);
	r2 = (CButton*)GetDlgItem(IDC_R2);
	r3 = (CButton*)GetDlgItem(IDC_R3);


	eql = (CListCtrl*)GetDlgItem(IDC_QL);
	DWORD dwStyle = eql->GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	eql->SetExtendedStyle(dwStyle);


	epl = (CListCtrl*)GetDlgItem(IDC_PL);
	dwStyle = epl->GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	epl->SetExtendedStyle(dwStyle);

	eql->InsertColumn(0, _T("货号"), LVCFMT_CENTER, 100);//插入列
	eql->InsertColumn(1, _T("品名"), LVCFMT_CENTER, 100);//插入列
	eql->InsertColumn(2, _T("型号"), LVCFMT_CENTER, 60);//插入列
	eql->InsertColumn(3, _T("编号"), LVCFMT_CENTER, 90);//插入列
	eql->InsertColumn(4, _T("价格"), LVCFMT_CENTER, 80);//插入列

	epl->InsertColumn(0, _T("货号"), LVCFMT_CENTER, 100);//插入列
	epl->InsertColumn(1, _T("品名"), LVCFMT_CENTER, 100);//插入列
	epl->InsertColumn(2, _T("型号"), LVCFMT_CENTER, 60);//插入列
	epl->InsertColumn(3, _T("编号"), LVCFMT_CENTER, 90);//插入列
	epl->InsertColumn(4, _T("打印数量"), LVCFMT_CENTER, 80);//插入列

	OnBnClickedR1();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CStoreDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CStoreDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CStoreDlg::OnBnClickedModify()
{

	model m;
	eid->GetWindowText(m.ID);
	ena->GetWindowText(m.Name);
	emo->GetWindowText(m.Model);
	eco->GetWindowText(m.Color);
	em1->GetWindowText(m.M1);
	em2->GetWindowText(m.M2);
	em3->GetWindowText(m.M3);
	epr->GetWindowText(m.Price);

	CString PrintCount;
	epc->GetWindowText(PrintCount);
	unsigned count = _ttoi(PrintCount);
	CString PrintNo;
	est->GetWindowText(PrintNo);
	if (PrintNo.GetLength() <= 0)
		PrintNo = _T("001");

	if (m.ID.GetLength() <= 0)
	{
		MessageBox(_T("款号不能为空"), _T("错误"));
		return;
	}
	if (m.ID.GetLength() > 10)
	{
		MessageBox(_T("款号不能为超过10位"), _T("错误"));
		return;
	}

	if (ml.add(m))
	{

		int nRow = eql->GetItemCount();
		for (; nRow > 0; --nRow)
		{
			CString ID = eql->GetItemText(nRow - 1, 0);
			if (ID == m.ID)
			{
				break;
			}
		}
		if (nRow <= 0)
		{
			nRow = eql->InsertItem(0, m.ID);
		}
		else
		{
			--nRow;
		}
		eql->SetItemText(nRow, 1, m.Name);
		eql->SetItemText(nRow, 2, m.Model);
		eql->SetItemText(nRow, 3, m.M3);
		eql->SetItemText(nRow, 4, m.Price);
	}
	ml.save(this);

	int nRow = epl->GetItemCount();
	for (; nRow > 0; --nRow)
	{
		CString ID = epl->GetItemText(nRow - 1, 0);
		if (ID == m.ID)
		{
			break;
		}
	}
	if (nRow <= 0)
	{
		CString c;
		c.Format(_T("%d"), count);
		nRow = epl->InsertItem(0, m.ID);
		epl->SetItemText(nRow, 1, m.Name);
		epl->SetItemText(nRow, 2, m.Model);
		epl->SetItemText(nRow, 3, PrintNo);
		epl->SetItemText(nRow, 4, c);
	}
	else
	{
		--nRow;
		epl->SetItemText(nRow, 4, PrintCount);
	}
}

void CStoreDlg::OnBnClickedDelete()
{
	int rc = MessageBox(_T("确认废弃的型号才可以删除。\n提示：你可以尝试上面的“查询货号”和“查询名称”来过滤列表。\n\n真的要删除么？"),_T("警告"), MB_YESNO);

	if (rc != IDYES)
		return;

	// TODO: 在此添加控件通知处理程序代码
	CString ID;
	eid->GetWindowText(ID);

	ml.del(ID);
	int nRow = eql->GetItemCount();
	for (; nRow > 0; --nRow)
	{
		CString LID = eql->GetItemText(nRow - 1, 0);
		if (ID == LID)
		{
			eql->DeleteItem(nRow - 1);
			break;
		}
	}
	ml.save(this);
}

void CStoreDlg::query()
{
	CString QID, QNA, QMINP,QMAXP;
	qid->GetWindowText(QID);
	qna->GetWindowText(QNA);
	qminp->GetWindowText(QMINP);
	qmaxp->GetWindowText(QMAXP);

	eql->DeleteAllItems();
	if (r1->GetCheck() == 1)
	{
		auto v = ml.query(QID, QNA, QMINP, QMAXP);

		for (auto it = v.begin(); it != v.end(); ++it)
		{
			int nRow = eql->InsertItem(0, it->ID);
			eql->SetItemText(nRow, 1, it->Name);
			eql->SetItemText(nRow, 2, it->Model);
			eql->SetItemText(nRow, 3, it->M3);
			eql->SetItemText(nRow, 4, it->Price);
		}
	}
	else if ((r2->GetCheck() == 1)|| (r3->GetCheck() == 1))
	{
		auto v = ml.querysp(QID, QNA, QMINP, QMAXP);

		for (auto it = v.begin(); it != v.end(); ++it)
		{
			int nRow = eql->InsertItem(0, it->ID);
			eql->SetItemText(nRow, 1, it->Name);
			eql->SetItemText(nRow, 2, it->Model);
			eql->SetItemText(nRow, 3, it->M3);
			eql->SetItemText(nRow, 4, it->Price);
		}
	}

}

void CStoreDlg::add(int count)
{
	//查询列表中获取行号
	int nRow = eql->GetSelectionMark();
	if (nRow < 0)
		return;
	CString ID = eql->GetItemText(nRow, 0);
	//打印列表中添加
	auto m = ml.ma.at(ID);

	nRow = epl->GetItemCount();
	for (; nRow > 0; --nRow)
	{
		ID = epl->GetItemText(nRow - 1, 0);
		if (ID == m.ID)
		{
			break;
		}
	}
	if (nRow <= 0)
	{
		CString c;
		c.Format(_T("%d"),count);
		nRow = epl->InsertItem(0, m.ID);
		epl->SetItemText(nRow, 1, m.Name);
		epl->SetItemText(nRow, 2, m.Model);
		epl->SetItemText(nRow, 3, m.M3);
		epl->SetItemText(nRow, 4, c);
	}
	else
	{
		--nRow;
		CString c = epl->GetItemText(nRow, 4);
		count = _ttoi(c) + count;
		c.Format(_T("%d"), count);
		epl->SetItemText(nRow, 4, c);
	}
}

void CStoreDlg::del(int count)
{
	//打印列表中获取行号，删除
	int nRow = epl->GetSelectionMark();
	if (nRow < 0)
		return;
	CString c = epl->GetItemText(nRow, 4);
	count = _ttoi(c) - count;
	if (count > 0)
	{
		c.Format(_T("%d"), count);
		epl->SetItemText(nRow, 4, c);
	}
	else
	{
		epl->DeleteItem(nRow);
	}
}

void CStoreDlg::OnNMClickQl(NMHDR *pNMHDR, LRESULT *pResult)
{
	//QL单击事件，刷新左边区域和打印标签预览
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem < 0)
	{
		*pResult = -1;
		return;
	}

	CString ID = eql->GetItemText(pNMItemActivate->iItem, 0);
	auto m = ml.ma.at(ID);
	eid->SetWindowText(m.ID);
	ena->SetWindowText(m.Name);
	emo->SetWindowText(m.Model);
	eco->SetWindowText(m.Color);
	em1->SetWindowText(m.M1);
	em2->SetWindowText(m.M2);
	em3->SetWindowText(m.M3);
	est->SetWindowText(m.M3);
	epr->SetWindowText(m.Price);
	epc->SetWindowText(_T(""));

	print p;
	HDC hdcScreen = ::GetDC(NULL);

	CDC *dc = GetDC();
	if (r1->GetCheck() == 1)
	{
		int xdpi = GetDeviceCaps(hdcScreen, LOGPIXELSX);
		int ydpi = GetDeviceCaps(hdcScreen, LOGPIXELSY);
		p.setDPI(xdpi, ydpi);
		p.drawModel(m, _T("001"), dc, TAGX * DPU / xdpi, TAGY * DPU / ydpi);
	}
	else if (r2->GetCheck() == 1)
	{
		int xdpi = GetDeviceCaps(hdcScreen, LOGPIXELSX) * 2;
		int ydpi = GetDeviceCaps(hdcScreen, LOGPIXELSY) * 2;
		p.setDPI(xdpi, ydpi);
		p.drawModel2010(m, _T("001"), dc, TAGX * DPU / xdpi, TAGY * DPU / ydpi);
	}
	ReleaseDC(dc);
	*pResult = 0;
}

void CStoreDlg::OnNMDblclkQl(NMHDR *pNMHDR, LRESULT *pResult)
{
	//QL双击事件，添加到右边打印列表
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem < 0)
	{
		*pResult = -1;
		return;
	}
	CString ID = eql->GetItemText(pNMItemActivate->iItem, 0);

	add(1);

	*pResult = 0;
}

void CStoreDlg::OnNMClickPl(NMHDR *pNMHDR, LRESULT *pResult)
{
	//PL单击事件，刷新左边区域和打印标签预览
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem < 0)
	{
		*pResult = -1;
		return;
	}

	CString ID = epl->GetItemText(pNMItemActivate->iItem, 0);
	auto m = ml.ma.at(ID);
	eid->SetWindowText(m.ID);
	ena->SetWindowText(m.Name);
	emo->SetWindowText(m.Model);
	eco->SetWindowText(m.Color);
	em1->SetWindowText(m.M1);
	em2->SetWindowText(m.M2);
	em3->SetWindowText(m.M3);
	est->SetWindowText(m.M3);
	epr->SetWindowText(m.Price);
	epc->SetWindowText(epl->GetItemText(pNMItemActivate->iItem, 4));

	print p;
	HDC hdcScreen = ::GetDC(NULL);
	CDC *dc = GetDC();
	if (r1->GetCheck() == 1)
	{
		int xdpi = GetDeviceCaps(hdcScreen, LOGPIXELSX);
		int ydpi = GetDeviceCaps(hdcScreen, LOGPIXELSY);
		p.setDPI(xdpi, ydpi);
		p.drawModel(m, _T("001"), dc, TAGX * DPU / xdpi, TAGY * DPU / ydpi);
	}
	else if (r2->GetCheck() == 1)
	{
		int xdpi = GetDeviceCaps(hdcScreen, LOGPIXELSX) * 2;
		int ydpi = GetDeviceCaps(hdcScreen, LOGPIXELSY) * 2;
		p.setDPI(xdpi, ydpi);
		p.drawModel2010(m, _T("001"), dc, TAGX * DPU / xdpi, TAGY * DPU / ydpi);
	}
	ReleaseDC(dc);
	*pResult = 0;
}

void CStoreDlg::OnNMDblclkPl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if (pNMItemActivate->iItem < 0)
	{
		*pResult = -1;
		return;
	}
	del(1);
	*pResult = 0;
}

void CStoreDlg::OnBnClickedSetPrinter()
{
	if (_pd.DoModal() == IDOK)
		setprint = true;
}


void CStoreDlg::OnBnClickedPrint()
{
	// TODO: 在此添加控件通知处理程序代码
	CPrintDialog dpd(FALSE, PD_ALLPAGES | PD_COLLATE | PD_NOPAGENUMS | PD_HIDEPRINTTOFILE);
	CString WID;
	eid->GetWindowText(WID);
	dpd.GetDefaults();
	CPrintDialog *pd = NULL;
	if (setprint)
	{
		pd = &_pd;
	}
	else
	{
		pd = &dpd;
	}
	//重新定义纸张大小
	HDC   hDC = pd->CreatePrinterDC();
	DEVMODE* lpDevMode = (DEVMODE*)pd->GetDevMode();
	lpDevMode->dmFields = DM_ORIENTATION| DM_PAPERSIZE| DM_PAPERLENGTH| DM_PAPERWIDTH;
	lpDevMode->dmPaperSize = DMPAPER_USER;
	if (r1->GetCheck() == 1)
	{
		lpDevMode->dmPaperWidth = 800;
		lpDevMode->dmPaperLength = 400;
		lpDevMode->dmOrientation = DMORIENT_LANDSCAPE;
	}
	else if (r2->GetCheck() == 1)
	{
		lpDevMode->dmPaperWidth = 880;
		lpDevMode->dmPaperLength = 100;
		lpDevMode->dmOrientation = DMORIENT_PORTRAIT;
	}
	else if (r3->GetCheck() == 1)
	{
		lpDevMode->dmPaperWidth = 700;
		lpDevMode->dmPaperLength = 120;
		lpDevMode->dmOrientation = DMORIENT_PORTRAIT;
	}
	ResetDC(hDC, lpDevMode); //使设置的参数发挥作用
	CDC   dc;
	dc.Attach(hDC);

	int dpix = dc.GetDeviceCaps(LOGPIXELSX);
	int dpiy = dc.GetDeviceCaps(LOGPIXELSY);

	print p;
	p.setDPI(dpix, dpiy);
	DOCINFO   di;
	memset(&di, 0, sizeof(DOCINFO));
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = _T("TAG Print");
	di.lpszOutput = (LPTSTR)NULL;
	di.fwType = 0;

	dc.StartDoc(&di);

	int nRow = epl->GetItemCount();
	int printcol = 0;
	for (int i = 0; i < nRow; ++i)
	{
		CString ID = epl->GetItemText(i, 0);
		CString NO = epl->GetItemText(i, 3);
		auto m = ml.ma.at(ID);
		int c = _ttoi(epl->GetItemText(i, 4));
		for (; c > 0; --c)
		{
			if (r1->GetCheck() == 1)
			{
				dc.StartPage();
				p.drawModel(m, NO, &dc, 0, 0);
				dc.EndPage();
			}
			else if (r2->GetCheck() == 1)
			{
				if (printcol == 0)
					dc.StartPage();

				p.drawModel2010(m, NO, &dc, printcol * 22000, 0);

				++printcol;
				if (printcol == 4)
				{
					dc.EndPage();
					printcol = 0;
				}
			}
			else if (r3->GetCheck() == 1)
			{
				dc.StartPage();
				p.drawModel7012(m, NO, &dc, 0, 0);
				dc.EndPage();
			}

			TCHAR *ch = NO.GetBuffer();
			int l = NO.GetLength() - 1;
			while (l >= 0)
			{
				if ((ch[l] >= '0')&(ch[l] < '9'))
				{
					ch[l] += 1;
					break;
				}
				else if (ch[l] == '9')
				{
					ch[l] = '0';
					--l;
					continue;
				}
				break;
			}
		}
		m.M3 = NO;
		ml.add(m);

		if (ID == WID)
		{
			est->SetWindowText(NO);
		}
	}
	ml.save(this);
	if ((r2->GetCheck() == 1)&&(printcol != 0))
		dc.EndPage();
	dc.EndDoc();
	dc.DeleteDC();
	epl->DeleteAllItems();
	query();
}

void CStoreDlg::OnBnClickedRemove()
{
	//打印列表中获取行号，删除
	int nRow = epl->GetSelectionMark();
	if (nRow < 0)
		return;
	epl->DeleteItem(nRow);
}


void CStoreDlg::OnBnClickedAdd10()
{
	add(10);
}

void CStoreDlg::OnBnClickedAdd5()
{
	add(5);
}


void CStoreDlg::OnBnClickedAdd3()
{
	add(3);
}


void CStoreDlg::OnBnClickedAdd1()
{
	add(1);
}

void CStoreDlg::OnBnClickedDel1()
{
	del(1);
}


void CStoreDlg::OnBnClickedDel3()
{
	del(3);
}


void CStoreDlg::OnBnClickedDel5()
{
	del(5);
}


void CStoreDlg::OnBnClickedDel10()
{
	del(10);
}


void CStoreDlg::OnCbnSelchangeMm()
{
	int idx = emm->GetCurSel();
	switch (idx)
	{
		case 0:
			em1->SetWindowText(_T("100%聚酯纤维"));
			em2->SetWindowText(_T(""));
			em3->SetWindowText(_T(""));
			break;
		case 1:
			em1->SetWindowText(_T("95%棉"));
			em2->SetWindowText(_T("5%氨纶"));
			em3->SetWindowText(_T(""));
			break;
		case 2:
			em1->SetWindowText(_T("35%棉"));
			em2->SetWindowText(_T("65%麻"));
			em3->SetWindowText(_T(""));
			break;
		case 3:
			em1->SetWindowText(_T("32.3%纤维  30.7%羊毛"));
			em2->SetWindowText(_T("19.8%晴纶  17.2%粘合"));
			em3->SetWindowText(_T(""));
			break;
		case 4:
			em1->SetWindowText(_T("面料：100%聚酯纤维"));
			em2->SetWindowText(_T("毛领：狐狸毛"));
			em3->SetWindowText(_T("填充：90%白鸭毛"));
			break;
		case 5:
			em1->SetWindowText(_T("面料：100%棉"));
			em2->SetWindowText(_T("里料：100%聚酯纤维"));
			em3->SetWindowText(_T("填充：90%白鸭绒10%毛片"));
			break;
		case 6:
			em1->SetWindowText(_T("面料：100%聚酯纤维"));
			em2->SetWindowText(_T("里料：100%聚酯纤维"));
			em3->SetWindowText(_T("填充：90%绒"));
			break;
		case 7:
			em1->SetWindowText(_T("面料：100%涤纶"));
			em2->SetWindowText(_T("里料：100%涤纶"));
			em3->SetWindowText(_T("填充：80%白鸭绒20%羽毛"));
			break;
		case 8:
			em1->SetWindowText(_T("PU"));
			em2->SetWindowText(_T(""));
			em3->SetWindowText(_T(""));
			break;
		default:
			em1->SetWindowText(_T(""));
			em2->SetWindowText(_T(""));
			em3->SetWindowText(_T(""));
	}
	preview();
}


void CStoreDlg::OnEnKillfocusPr()
{
	CString P;
	epr->GetWindowText(P);
	if (P.Find(_T("￥")) != 0)
		P = _T("￥") + P;
	epr->SetWindowText(P);
}


void CStoreDlg::OnBnClickedUpdate()
{
//	setEcho(this, 20, 280,180,100);//左侧空间150像素，预留20像素边界，有110像素的width.
//	checkupdate(((CStoreApp *)AfxGetApp())->installPath);
}


void CStoreDlg::preview()
{
	model m;
	eid->GetWindowText(m.ID);
	ena->GetWindowText(m.Name);
	emo->GetWindowText(m.Model);
	eco->GetWindowText(m.Color);
	em1->GetWindowText(m.M1);
	em2->GetWindowText(m.M2);
	em3->GetWindowText(m.M3);
	epr->GetWindowText(m.Price);
	//epc->SetWindowText(epl->GetItemText(pNMItemActivate->iItem, 4));

	print p;
	HDC hdcScreen = ::GetDC(NULL);

	CDC *dc = GetDC();
	if (r1->GetCheck() == 1)
	{
		int xdpi = GetDeviceCaps(hdcScreen, LOGPIXELSX);
		int ydpi = GetDeviceCaps(hdcScreen, LOGPIXELSY);
		p.setDPI(xdpi, ydpi);
		p.drawModel(m, _T("001"), dc, TAGX * DPU / xdpi, TAGY * DPU / ydpi);
	}
	else if (r2->GetCheck() == 1)
	{
		int xdpi = GetDeviceCaps(hdcScreen, LOGPIXELSX) * 2;
		int ydpi = GetDeviceCaps(hdcScreen, LOGPIXELSY) * 2;
		p.setDPI(xdpi, ydpi);
		p.drawModel2010(m, _T("001"), dc, TAGX * DPU / xdpi, TAGY * DPU / ydpi);
	}
	ReleaseDC(dc);
}


void CStoreDlg::OnCbnSelchangeNa()
{
	int s;
	CString v;
	s = ena->GetCurSel();
	ena->GetLBText(s, v);
	ena->SetWindowTextW(v);
	preview();
}


void CStoreDlg::OnCbnSelchangeMo()
{
	int s;
	CString v;
	s = emo->GetCurSel();
	emo->GetLBText(s, v);
	emo->SetWindowTextW(v);
	preview();
}


void CStoreDlg::OnCbnSelchangeCo()
{
	int s;
	CString v;
	s = eco->GetCurSel();
	eco->GetLBText(s, v);
	eco->SetWindowTextW(v);
	preview();
}


void CStoreDlg::OnBnClickedR1()
{
	r1->SetCheck(1);
	r2->SetCheck(0);
	r3->SetCheck(0);

	eql->SetColumnWidth(0, 100);
	eql->SetColumnWidth(1, 100);
	eql->SetColumnWidth(2, 60);
	eql->SetColumnWidth(3, 90);
	eql->SetColumnWidth(4, 80);
	epl->SetColumnWidth(0, 100);
	epl->SetColumnWidth(1, 100);
	epl->SetColumnWidth(2, 60);
	epl->SetColumnWidth(3, 90);
	epl->SetColumnWidth(4, 80);

	ena->ShowWindow(SW_NORMAL);
	GetDlgItem(IDT_NA)->ShowWindow(SW_NORMAL);
	emo->ShowWindow(SW_NORMAL);
	GetDlgItem(IDT_MO)->ShowWindow(SW_NORMAL);
	eco->ShowWindow(SW_NORMAL);
	GetDlgItem(IDT_CO)->ShowWindow(SW_NORMAL);
	qna->ShowWindow(SW_NORMAL);
	GetDlgItem(IDT_QNA)->ShowWindow(SW_NORMAL);
	ena->SetWindowText(_T(""));
	qna->SetWindowText(_T(""));

}


void CStoreDlg::OnBnClickedR2()
{
	r1->SetCheck(0);
	r2->SetCheck(1);
	r3->SetCheck(0);

	eql->SetColumnWidth(0, 100);
	eql->SetColumnWidth(1, 0);
	eql->SetColumnWidth(2, 0);
	eql->SetColumnWidth(3, 90);
	eql->SetColumnWidth(4, 80);
	epl->SetColumnWidth(0, 100);
	epl->SetColumnWidth(1, 0);
	epl->SetColumnWidth(2, 0);
	epl->SetColumnWidth(3, 90);
	epl->SetColumnWidth(4, 80);

	ena->ShowWindow(SW_HIDE);
	GetDlgItem(IDT_NA)->ShowWindow(SW_HIDE);
	emo->ShowWindow(SW_HIDE);
	GetDlgItem(IDT_MO)->ShowWindow(SW_HIDE);
	eco->ShowWindow(SW_HIDE);
	GetDlgItem(IDT_CO)->ShowWindow(SW_HIDE);
	qna->ShowWindow(SW_HIDE);
	GetDlgItem(IDT_QNA)->ShowWindow(SW_HIDE);
	ena->SetWindowText(_T("饰品标签"));
	qna->SetWindowText(_T("饰品标签"));
}


void CStoreDlg::OnBnClickedR3()
{
	r1->SetCheck(0);
	r2->SetCheck(0);
	r3->SetCheck(1);

	eql->SetColumnWidth(0, 100);
	eql->SetColumnWidth(1, 0);
	eql->SetColumnWidth(2, 0);
	eql->SetColumnWidth(3, 90);
	eql->SetColumnWidth(4, 80);
	epl->SetColumnWidth(0, 100);
	epl->SetColumnWidth(1, 0);
	epl->SetColumnWidth(2, 0);
	epl->SetColumnWidth(3, 90);
	epl->SetColumnWidth(4, 80);

	ena->ShowWindow(SW_HIDE);
	GetDlgItem(IDT_NA)->ShowWindow(SW_HIDE);
	emo->ShowWindow(SW_HIDE);
	GetDlgItem(IDT_MO)->ShowWindow(SW_HIDE);
	eco->ShowWindow(SW_HIDE);
	GetDlgItem(IDT_CO)->ShowWindow(SW_HIDE);
	qna->ShowWindow(SW_HIDE);
	GetDlgItem(IDT_QNA)->ShowWindow(SW_HIDE);
	ena->SetWindowText(_T("饰品标签"));
	qna->SetWindowText(_T("饰品标签"));
}
