#pragma once
#include "resource.h"


class Settings : public CDialogEx
{
public:
	Settings();

	// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Edit };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

												
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	// �������� ��������� ��������
	CString R_Value;
	// �������� ������� ����� �������
	CString G_Value;
	// ������� ����� �������
	CString B_Value;
	afx_msg void OnEnChangeEdit3();
};


