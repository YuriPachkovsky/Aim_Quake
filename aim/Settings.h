#pragma once
#include "resource.h"


class Settings : public CDialogEx
{
public:
	Settings();

	// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Edit };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

												
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	// Значение диапазона красного
	CString R_Value;
	// значение зеленой части спектра
	CString G_Value;
	// Голубая часть спектра
	CString B_Value;
	afx_msg void OnEnChangeEdit3();
};


