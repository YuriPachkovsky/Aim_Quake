#include "stdafx.h"
#include "PassW.h"
#include "tritpo_labDlg.h"


//IDD_PROPPAGE_SMALL

PassW::PassW() : CDialogEx(IDD_DIALOG)
, Login(_T(""))
, Password(_T(""))
{
}



void PassW::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, Login);
	DDX_Text(pDX, IDC_EDIT2, Password);
}

BEGIN_MESSAGE_MAP(PassW, CDialogEx)
	ON_BN_CLICKED(IDOK, &PassW::OnBnClickedOk)
	ON_BN_CLICKED(2, &PassW::OnBnClicked2)
	ON_EN_CHANGE(IDC_EDIT2, &PassW::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON1, &PassW::OnBnClickedButton1)
END_MESSAGE_MAP()




void PassW::OnBnClickedOk()
{
	Profile Prof;
	UpdateData(true);
	Prof.setname((LPCSTR)Login);
	Prof.setPassword((LPCSTR)Password);
	if (AccFile.loginCheck(Prof))
	{
		MessageBox("Вход осуществлен успешно", "Вход");
		EndDialog(AccFile.IndexOfElem(Prof));
		return;
	}
	else
		MessageBox("Данный профиль отсутствует","Вход");
}


void PassW::OnBnClicked2()
{
	EndDialog(-1);
	return;
}


void PassW::OnEnChangeEdit2()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}


void PassW::OnBnClickedButton1()
{
	Profile Prof;
	UpdateData(true);
	Prof.setname((LPCSTR)Login);
	Prof.setPassword((LPCSTR)Password);
	AccFile.AddNewProf(Prof);
	MessageBox("Профиль добален", "Информация");
}

