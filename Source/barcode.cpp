#include "stdafx.h"
#include "barcode.h"

static CString code39set[] = {
	_T("111221211"),//0
	_T("211211112"),//1
	_T("112211112"),//2
	_T("212211111"),//3
	_T("111221112"),//4
	_T("211221111"),//5
	_T("112221111"),//6
	_T("111211212"),//7
	_T("211211211"),//8
	_T("112211211"),//9
	_T("211112112"),//A
	_T("112112112"),//B
	_T("212112111"),//C
	_T("111122112"),//D
	_T("211122111"),//E
	_T("112122111"),//F
	_T("111112212"),//G
	_T("211112211"),//H
	_T("112112211"),//I
	_T("111122211"),//J
	_T("211111122"),//K
	_T("112111122"),//L
	_T("212111121"),//M
	_T("111121122"),//N
	_T("211121121"),//O
	_T("112121121"),//P
	_T("111111222"),//Q
	_T("211111221"),//R
	_T("112111221"),//S
	_T("111121221"),//T
	_T("221111112"),//U
	_T("122111112"),//V
	_T("222111111"),//W
	_T("121121112"),//X
	_T("221121111"),//Y
	_T("122121111"),//Z
	_T("121111212"),//-
	_T("221111211"),//.
	_T("122111211"),//Space
	_T("121212111"),//$
	_T("121211121"),///
	_T("121112121"),//+
	_T("111212121"),//%
	_T("121121211"),//*
};


bool BarCode::Code39Encode(CString& text, CString& partial)
{
	//如果字符串为空，则返回
	int kk = text.GetLength();
	if (kk <= 0)
	{
		return false;
	}

	//首字符为*
	partial = code39set[43] + _T("2");

	for (int i = 0; i < kk; i++)
	{
		TCHAR c = text.GetAt(i);
		if (c >= _T('A') && c <= _T('Z'))
			partial += code39set[c - _T('A') + 10];
		else if (c >= _T('0') && c <= _T('9'))
			partial += code39set[c - _T('0') + 10];
		else if (c == _T('-'))
			partial += code39set[36];
		else if (c == _T('.'))
			partial += code39set[37];
		else if (c == _T(' '))
			partial += code39set[38];
		else if (c == _T('$'))
			partial += code39set[39];
		else if (c == _T('/'))
			partial += code39set[40];
		else if (c == _T('+'))
			partial += code39set[41];
		else if (c == _T('%'))
			partial += code39set[42];
		else if (c == _T('*'))
			partial += code39set[43];
		else if (c >= _T('0') && c <= _T('9'))
			partial += code39set[c - _T('0') + 10];
		else
		{
			partial = _T("");
			return false;
		}

		//每两个字符之间加一个白线
		partial += _T("2");
	}
	//结束符为*
	partial += code39set[43];

	return true;
}

bool BarCode::Code39Verify(CString &text)
{
	//如果字符串为空，则返回
	int kk = text.GetLength();
	if (kk <= 0)
	{
		return false;
	}
	for (int i = 0; i < kk; i++)
	{
		TCHAR c = text.GetAt(i);
		if (c >= _T('A') && c <= _T('Z'))
			continue;
		else if (c >= _T('0') && c <= _T('9'))
			continue;
		else if (c == _T('-'))
			continue;
		else if (c == _T('.'))
			continue;
		else if (c == _T(' '))
			continue;
		else if (c == _T('$'))
			continue;
		else if (c == _T('/'))
			continue;
		else if (c == _T('+'))
			continue;
		else if (c == _T('%'))
			continue;
		else if (c == _T('*'))
			continue;
		else if (c >= _T('0') && c <= _T('9'))
			continue;
		else
		{
			return false;
		}
	}	return true;
}

bool BarCode::Code39Print(CString & text, CDC *dc, int x, int y, int w, int h, float ratox,float ratoy)
{
	int pix = int(ratox * w);//可用的像素点

	CString s;
	if (!Code39Encode(text, s))
		return false;

	int kk = s.GetLength();
	if (kk <= 0)
	{
		return false;
	}

	float ppc = float(pix / (text.GetLength() + 2));//每字符多少像素
	int ps = 1;
	float pw = 2;
	if (ppc < 14)
		return false;//每字符少于14像素，不能绘图
	else
	{
		ps = int(ppc / 14 + 0.3);//窄线像素
		pw = float(ppc - ps * 6) / 4;//宽线像素
	}

	float xx = ratox * x;
	int xi = int(xx);
	dc->SelectStockObject(BLACK_PEN);
	for (int i = 0; i < kk;)
	{
		TCHAR B = s.GetAt(i++);
		if (B == _T('2'))
		{
			//画宽线
			xx += pw;
		}
		else
		{
			//窄线
			xx += ps;
		}
		for (; xi < xx; ++xi)
		{
			dc->MoveTo(xi, int(ratoy * y));
			dc->LineTo(xi, int(ratoy * (y + h)));
		}
		TCHAR W = s.GetAt(i++);
		if (W == _T('2'))
		{
			//画宽线
			xx += pw;
		}
		else
		{
			//窄线
			xx += ps;
		}
		for (; xi < xx; ++xi);
	}

	return false;
}
