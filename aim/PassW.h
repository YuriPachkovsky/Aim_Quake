#pragma once

#include "resource.h"
#include "AccFileManager.h"

class PassW : public CDialogEx
{
public:
	PassW();
	AccFileManager AccFile;
	// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG };
#endif
protected:
	DECLARE_MESSAGE_MAP()// ����������
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClicked2();
	CString Login;
	CString Password;
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton1();
};

