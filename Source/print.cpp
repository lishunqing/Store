#include "stdafx.h"
#include "Resource.h"
#include "print.h"
#include "barcode.h"

void print::setDPI(int dpix,int dpiy)
{
	ratox = (float)dpix / 25400;//每千分之一毫米的像素数
	ratoy = (float)dpiy / 25400;//每千分之一毫米的像素数
}

void print::drawModel(model&m, CString NO, CDC *dc, int x, int y,int w,int h)
{
	const int width = w;
	const int height = h;
	const int fontl = 4000;
	const int fontm = 3200;
	const int fonts = 2200;
	const int edge = 2800;
	CRect rectB(int(ratox * x - 2), int(ratoy * y - 2), int(ratox * (x + width) + 2), int(ratoy * (y + height) + 2));
	FillRect(dc->GetSafeHdc(), rectB, CreateSolidBrush(RGB(0, 0, 0)));
	CRect rect(int(ratox * x) , int(ratoy * y), int(ratox * (x + width)), int(ratoy * (y + height)));
	FillRect(dc->GetSafeHdc(), rect, CreateSolidBrush(RGB(255, 255, 255)));
	//dc->FillRect(&rect, WHITE_BRUSH, CreateSolidBrush(RGB(0,0,0)));

	CFont fl, fm, fs;
	fl.CreateFont(int(ratoy * fontl),                //   nHeight     
		0,                                     //   nWidth     
		0,                                     //   nEscapement   
		0,                                     //   nOrientation     
		FW_HEAVY,                              //   nWeight     
		FALSE,                                 //   bItalic     
		FALSE,                                 //   bUnderline     
		0,                                     //   cStrikeOut     
		DEFAULT_CHARSET,                       //   nCharSet     
		OUT_DEFAULT_PRECIS,                    //   nOutPrecision     
		CLIP_DEFAULT_PRECIS,                   //   nClipPrecision     
		DEFAULT_QUALITY,                       //   nQuality     
		DEFAULT_PITCH | FF_SWISS,              //   nPitchAndFamily       
		_T("宋体"));

	fm.CreateFont(int(ratoy * fontm),                //   nHeight     
		0,                                     //   nWidth     
		0,                                     //   nEscapement   
		0,                                     //   nOrientation     
		FW_NORMAL,                             //   nWeight     
		FALSE,                                 //   bItalic     
		FALSE,                                 //   bUnderline     
		0,                                     //   cStrikeOut     
		DEFAULT_CHARSET,                       //   nCharSet     
		OUT_DEFAULT_PRECIS,                    //   nOutPrecision     
		CLIP_DEFAULT_PRECIS,                   //   nClipPrecision     
		DEFAULT_QUALITY,                       //   nQuality     
		DEFAULT_PITCH | FF_SWISS,              //   nPitchAndFamily       
		_T("宋体"));

	fs.CreateFont(int(ratoy * fonts),                //   nHeight     
		0,                                     //   nWidth     
		0,                                     //   nEscapement   
		0,                                     //   nOrientation     
		FW_LIGHT,                              //   nWeight     
		FALSE,                                 //   bItalic     
		FALSE,                                 //   bUnderline     
		0,                                     //   cStrikeOut     
		DEFAULT_CHARSET,                       //   nCharSet     
		OUT_DEFAULT_PRECIS,                    //   nOutPrecision     
		CLIP_DEFAULT_PRECIS,                   //   nClipPrecision     
		DEFAULT_QUALITY,                       //   nQuality     
		DEFAULT_PITCH | FF_SWISS,              //   nPitchAndFamily       
		_T("宋体"));

	int yy = y + edge;
	dc->SelectObject(&fl);
	dc->TextOut(int(ratox * (x + (width - fontl * 3) / 2)), int(ratoy * (yy)), _T("合格证"));
	yy += 6000;

	dc->SelectObject(&fm);
	//dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("款号："));
	//dc->TextOut(int(ratox * (x + width / 2)), int(ratoy * (yy)), m.ID);
	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("款号：") + m.ID);
	yy += 4200;

	//dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("品名："));
	//dc->TextOut(int(ratox * (x + width / 2)), int(ratoy * (yy)), m.Name);
	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("品名：") + m.Name);
	yy += 4200;

	//dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("型号："));
	//dc->TextOut(int(ratox * (x + width / 2)), int(ratoy * (yy)), m.Model);
	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("型号：") + m.Model);
	yy += 4200;

	//dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("颜色："));
	//dc->TextOut(int(ratox * (x + width / 2)), int(ratoy * (yy)), m.Color);
	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("颜色：") + m.Color);
	yy += 5000;
	//unsigned int r = rand();
	//r %= 70;
	//r += 10;
	//CString s;
	//s.Format(_T("检验员：%d"),r);
	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("检验员：") + NO);
	yy += 7000;

	//dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("成分："));
	//dc->TextOut(int(ratox * (x + width / 2)), int(ratoy * (yy)), m.M1);
	/*
	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("成分："));
	dc->SelectObject(&fs);
	dc->TextOut(int(ratox * (x + edge + fontm * 3)), int(ratoy * (yy)), +m.M1);


	yy += 4200;
	if (m.M2.Find(_T("\\"),0) == 0)
	{
		dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), m.M2.Mid(1));
	}
	else
	{
		//dc->TextOut(int(ratox * (x + width / 2)), int(ratoy * (yy)), m.M2);
		dc->TextOut(int(ratox * (x + edge + fontm * 3)), int(ratoy * (yy)), + m.M2);
	}
	yy += 4200;
	if (m.M3.Find(_T("\\"), 0) == 0)
	{
		dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), m.M3.Mid(1));
	}
	else
	{
		//dc->TextOut(int(ratox * (x + width / 2)), int(ratoy * (yy)), m.M3);
		dc->TextOut(int(ratox * (x + edge + fontm * 3)), int(ratoy * (yy)), + m.M3);
	}
	yy += 4500;
	*/
	BarCode::Code39Print(m.ID, dc, x + edge, yy,width - edge * 2,6000,ratox,ratoy);
	yy += 6500;

	dc->SelectObject(&fs);
	dc->TextOut(int(ratox * (x + (width - fonts * m.ID.GetLength() / 2) / 2)), int(ratoy * (yy)), m.ID);
	yy += 6500;

	dc->SelectObject(&fm);
	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("价格："));
	dc->SelectObject(&fl);
	dc->TextOut(int(ratox * (x + width / 2)), int(ratoy * (yy - (fontl - fontm) / 2)), m.Price);
	yy += 6000;

	dc->SelectObject(&fs);
	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("执行标准：FZ/T73005-2002"));
	yy += 2800;

	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("质量等级：合格品"));
	yy += 2800;

	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("安全技术类别："));
	yy += 2800;

	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("符合GB18401-2010B类"));
	yy += 2800;

	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), _T("（直接接触皮肤类）"));
	yy += 7000;

	CBitmap tag;
	BITMAP bmInfo;
	tag.LoadBitmap(IDB_TAG);
	tag.GetObject(sizeof(bmInfo), &bmInfo);


	CDC memDC;
	memDC.CreateCompatibleDC(dc);
	memDC.SelectObject(&tag);

	int a = bmInfo.bmWidth;

	dc->StretchBlt(int(ratox * (x + edge)), int(ratoy * (yy)), int(ratox * (width - edge*2)), int(ratoy*(y + height - yy - edge / 2)), &memDC, 0, 0, bmInfo.bmWidth, bmInfo.bmHeight, SRCCOPY);
}

void print::drawModel2010(model&m, CString NO, CDC *dc, int x, int y, int w, int h)
{
	const int width = w;
	const int height = h;
	const int font = 2000;
	const int edge = 1500;
//	CRect rectB(int(ratox * x - 2), int(ratoy * y - 2), int(ratox * (x + width) + 2), int(ratoy * (y + height) + 2));
//	FillRect(dc->GetSafeHdc(), rectB, CreateSolidBrush(RGB(0, 0, 0)));
	CRect rect(int(ratox * x), int(ratoy * y), int(ratox * (x + width)), int(ratoy * (y + height)));
	FillRect(dc->GetSafeHdc(), rect, CreateSolidBrush(RGB(255, 255, 255)));

//	dc->MoveTo(int(ratox * x), int(ratox * y));
//	dc->LineTo(int(ratox * (x + w)), int(ratox *(y + h)));
//	dc->MoveTo(int(ratox * x), int(ratox * (y + h)));
//	dc->LineTo(int(ratox * (x + w)), int(ratox * y));
//	return;

	CFont f;
	f.CreateFont(int(ratoy * font),                //   nHeight     
		0,                                     //   nWidth     
		0,                                     //   nEscapement   
		0,                                     //   nOrientation     
		FW_HEAVY,                              //   nWeight     
		FALSE,                                 //   bItalic     
		FALSE,                                 //   bUnderline     
		0,                                     //   cStrikeOut     
		DEFAULT_CHARSET,                       //   nCharSet     
		OUT_DEFAULT_PRECIS,                    //   nOutPrecision     
		CLIP_DEFAULT_PRECIS,                   //   nClipPrecision     
		DEFAULT_QUALITY,                       //   nQuality     
		DEFAULT_PITCH | FF_SWISS,              //   nPitchAndFamily       
		_T("宋体"));

	int yy = y + edge;
	BarCode::Code39Print(m.ID, dc, x + edge, yy, width - edge * 2, 3000, ratox, ratoy);
	dc->SelectObject(&f);
	yy += 5500;
	dc->TextOut(int(ratox * (x + edge)), int(ratoy * (yy)), m.ID + NO);
	dc->TextOut(int(ratox * (x + width / 2 + font)), int(ratoy * (yy)), m.Price);
}

void print::drawModel7012(model&m, CString NO, CDC *dc, int x, int y, int w, int h)
{
//	const int width = w;
//	const int height = h;
//	CRect rectB(int(ratox * x - 2), int(ratoy * y - 2), int(ratox * (x + width) + 2), int(ratoy * (y + height) + 2));
//		FillRect(dc->GetSafeHdc(), rectB, CreateSolidBrush(RGB(0, 0, 0)));
//	CRect rect(int(ratox * x), int(ratoy * y), int(ratox * (x + width)), int(ratoy * (y + height)));
//	FillRect(dc->GetSafeHdc(), rect, CreateSolidBrush(RGB(255, 255, 255)));
//
//		dc->MoveTo(int(ratox * x), int(ratox * y));
//	dc->LineTo(int(ratox * (x + w)), int(ratox *(y + h)));
//		dc->MoveTo(int(ratox * x), int(ratox * (y + h)));
//		dc->LineTo(int(ratox * (x + w)), int(ratox * y));
//		return;
	drawModel2010(m, NO, dc, x + 2000, y + 1000);
	drawModel2010(m, NO, dc, x + 48000, y + 1000);
}