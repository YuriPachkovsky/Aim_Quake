
// tritpo_labDlg.cpp: файл реализации
//
#include "stdafx.h"
#include "afxdialogex.h"
#include "tritpo_lab.h"
#include "tritpo_labDlg.h"
#include "resource.h"
//#include "ScreenCL.h"
#include "source.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif




// Диалоговое окно CAboutDlg используется для описания сведений о приложении

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD =IDD_ABOUTBOX};
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

// Реализация
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAboutDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// Диалоговое окно CtritpolabDlg



CtritpolabDlg::CtritpolabDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_TRITPO_LAB_DIALOG, pParent)
	, User(_T(""))
	, Info(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	PassWord = new PassW();
	Sett = new Settings();
	this->CurProf = new Profile();
	InitSemForWork();
}

CtritpolabDlg::~CtritpolabDlg()
{
	PassWord->AccFile.~AccFileManager();
	delete CurProf;
}

void CtritpolabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, User);
	DDX_Text(pDX, IDC_EDIT1, Info);
}



BEGIN_MESSAGE_MAP(CtritpolabDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON3, &CtritpolabDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CtritpolabDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CtritpolabDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CtritpolabDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CtritpolabDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CtritpolabDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_RADIO1, &CtritpolabDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CtritpolabDlg::OnBnClickedRadio2)
	ON_EN_CHANGE(IDC_EDIT1, &CtritpolabDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()


// Обработчики сообщений CtritpolabDlg

BOOL CtritpolabDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Добавление пункта "О программе..." в системное меню.

	// IDM_ABOUTBOX должен быть в пределах системной команды.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	
	

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

void CtritpolabDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CtritpolabDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CtritpolabDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtritpolabDlg::OnBnClickedButton3()
{
	this->PostMessage(WM_CLOSE, 0, 0);
}


void CtritpolabDlg::OnBnClickedButton2()
{
	MessageBox("GameBot для Quake3Arena", "Информация", MB_ICONINFORMATION);
}


void CtritpolabDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	//Начать работу
}


void CtritpolabDlg::OnBnClickedButton4()
{
	int res = PassWord->DoModal();
	if (res == -1)
	{
		User = "Не авторизован";
		UpdateData(0);
	}
	else
	{
		char tmp[20];
		CurProf = &PassWord->AccFile.profile[res];
		User = CurProf->getname().c_str();
		Info = "Quake 3: Arena\n";
		Info += "R=";
		_itoa_s(CurProf->getR(), tmp, 10);
		Info += tmp;
		Info += "G=";
		_itoa_s(CurProf->getG(), tmp, 10);
		Info += tmp;
		Info += "B=";
		_itoa_s(CurProf->getB(), tmp, 10);
		Info += tmp;
		UpdateData(0);
	}
	
}


void CtritpolabDlg::OnBnClickedButton5()
{
	char tmpR[20],tmpG[20],tmpB[20];
	if (Sett->DoModal() == 1)
	{
		CurProf->setR(atoi((LPCSTR)Sett->R_Value));
		CurProf->setG(atoi((LPCSTR)Sett->G_Value));
		CurProf->setB(atoi((LPCSTR)Sett->B_Value));
		Info = "Quake 3: Arena\n";
		Info += "R=";
		_itoa_s(CurProf->getR(), tmpR, 10);
		Info += tmpR;
		Info += "G=";
		_itoa_s(CurProf->getG(), tmpG, 10);
		Info += tmpG;
		Info += "B=";
		_itoa_s(CurProf->getB(), tmpB, 10);
		Info += tmpB;
		UpdateData(0);
		MessageBox("Успешно", "Результат измениния", MB_ICONINFORMATION);
	}
}


void CtritpolabDlg::OnBnClickedButton6()
{
	// TODO: добавьте свой код обработчика уведомлений
	//logout
	CurProf = new Profile();
	User = "Не авторизован";
	Info = "Quake 3: Arena\n R=0\n G=0\n B=0\n";
	UpdateData(0);
}


void CAboutDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	CDialogEx::OnOK();
}



void CtritpolabDlg::OnBnClickedRadio1()
{
	StartWork();
}


void CtritpolabDlg::OnBnClickedRadio2()
{
	StopWorks();
}


void CtritpolabDlg::OnEnChangeEdit1()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}
