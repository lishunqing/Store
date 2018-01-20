#pragma once

class BarCode
{
public:
	static bool Code39Encode(CString&, CString&);
	static bool Code39Verify(CString&);

	static bool Code39Print(CString&,CDC*,int,int,int,int,float,float);
};
