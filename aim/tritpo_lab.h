
// tritpo_lab.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить stdafx.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CtritpolabApp:
// Сведения о реализации этого класса: tritpo_lab.cpp
//

class CtritpolabApp : public CWinApp
{
public:
	CtritpolabApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CtritpolabApp theApp;
