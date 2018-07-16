#include "stdafx.h"
#include "Settings.h"




// Диалоговое окно CAboutDlg используется для описания сведений о приложении



Settings::Settings() : CDialogEx(IDD_Edit)
, R_Value(_T(""))
, G_Value(_T(""))
, B_Value(_T(""))
{
}

void Settings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, R_Value);
	DDX_Text(pDX, IDC_EDIT2, G_Value);
	DDX_Text(pDX, IDC_EDIT3, B_Value);
}


BEGIN_MESSAGE_MAP(Settings, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Settings::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Settings::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT3, &Settings::OnEnChangeEdit3)
END_MESSAGE_MAP()



void Settings::OnBnClickedButton1()
{
	EndDialog(0);
}


void Settings::OnBnClickedButton2()
{
	UpdateData(true);
	EndDialog(1);
}


void Settings::OnEnChangeEdit3()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}
