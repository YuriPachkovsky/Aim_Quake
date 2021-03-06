
// tritpo_labDlg.h: файл заголовка
//

#pragma once
#include "PassW.h"
#include "Settings.h"
#include "AccFileManager.h"


// Диалоговое окно CtritpolabDlg
class CtritpolabDlg : public CDialog
{
// Создание
public:
	CtritpolabDlg(CWnd* pParent = NULL);	// стандартный конструктор
	~CtritpolabDlg();
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRITPO_LAB_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;
	PassW *PassWord;
	Settings* Sett;
	Profile * CurProf;
	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	// отображение авторизовонного юзера
	CString User;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnEnChangeEdit1();
	CString Info;
};
