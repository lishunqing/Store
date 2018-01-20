
// StoreDlg.h : 头文件
//

#pragma once
#include "model.h"

// CStoreDlg 对话框
class CStoreDlg : public CDialogEx
{
// 构造
public:
	CStoreDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STORE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	CEdit* eid;
	CComboBox* ena;
	CComboBox* emo;
	CComboBox* eco;
	CComboBox* emm;
	CEdit* em1;
	CEdit* em2;
	CEdit* em3;
	CEdit* epr;
	CEdit* est;
	CEdit* epc;

	CEdit* qid;
	CEdit* qna;
	CEdit* qminp;
	CEdit* qmaxp;

	CListCtrl* eql;
	CListCtrl* epl;

	CButton* r1;
	CButton* r2;
	CButton* r3;


	CPrintDialog _pd;
	bool setprint = false;

	modellist ml;



	void query();
	void add(int);
	void del(int);
public:
	afx_msg void OnBnClickedModify();
	afx_msg void OnBnClickedDelete();
	afx_msg void OnBnClickedSetPrinter();
	afx_msg void OnBnClickedPrint();
	afx_msg void OnNMClickQl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkQl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickPl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkPl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRemove();
	afx_msg void OnBnClickedAdd10();
	afx_msg void OnBnClickedAdd5();
	afx_msg void OnBnClickedAdd3();
	afx_msg void OnBnClickedAdd1();
	afx_msg void OnBnClickedDel1();
	afx_msg void OnBnClickedDel3();
	afx_msg void OnBnClickedDel5();
	afx_msg void OnBnClickedDel10();
	afx_msg void OnCbnSelchangeMm();
	afx_msg void OnEnKillfocusPr();
	afx_msg void OnBnClickedUpdate();
	afx_msg void preview();
	afx_msg void OnCbnSelchangeNa();
	afx_msg void OnCbnSelchangeMo();
	afx_msg void OnCbnSelchangeCo();
	afx_msg void OnBnClickedR1();
	afx_msg void OnBnClickedR2();
	afx_msg void OnBnClickedR3();
};
