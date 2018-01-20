#pragma once

#include "model.h"

class print
{
public:
	float ratox;//每千分之一毫米的像素数
	float ratoy;//每千分之一毫米的像素数
	void setDPI(int,int);

	void drawModel(model&, CString, CDC*,int,int, int w = 40000, int h = 80000);
	void drawModel2010(model&, CString, CDC*, int, int, int w = 20000, int h = 10000);
	void drawModel7012(model&, CString, CDC*, int, int, int w = 70000, int h = 12000);
};
