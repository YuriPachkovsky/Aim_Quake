#include "stdafx.h"
#include "HeaderClassBot.h"


Bot::Bot()
{
	if(!(hwnd = FindWindow(0, "Quake 3: Arena"))) exit(0);
	//SetFocus(hwnd);
	EmuKeyb key;
	PuthForBot[0] = "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww   wwwwwwwwwwwwwwwwwwwwww";
	PuthForBot[1] = "aaaaaaaaaaaaaaaaaaaa      aaaaaaaaaaaaaaaaaaaaaaaa aaaaaaaaaa";
	PuthForBot[2] = "ddddddddddddddddddddddddddddddddddd  dddddddddddddddddddddd";
	PuthForBot[3] = "sssssssssss   sssssssssssssssssssssssssss";
	PuthForBot[4] = "lllllllllllllllllllllllllllllll";
	PuthForBot[5] = "jjjjjjjjjjjjjjjjjjjjjjjjjjjjjjj";
}

void Bot::MoveBotForward(unsigned int Step)
{
	wchar_t tmp[200];
	for (unsigned int i = 0; i < Step; i++)
		tmp[i] = L'w';
	tmp[Step] = L'\0';
	Sleep(10);
	key.SendString((LPCTSTR)tmp);														// эмулируем нажатие  клавиш	для ходьбы назад/   
}

void Bot::MoveBotBack(unsigned int Step)
{
	wchar_t tmp[200];
	for (unsigned int i = 0; i < Step; i++)
		tmp[i] = L's';
	tmp[Step] = L'\0';
	key.SendString((LPCTSTR)tmp);												// эмулируем нажатие  клавиш	для ходьбы назад
}

void Bot::MoveBotLeft(unsigned int Step)
{
	wchar_t tmp[200];
	for (unsigned int i = 0; i < Step; i++)
		tmp[i] = L'a';
	tmp[Step] = L'\0';
	key.SendString((LPCTSTR)tmp);												// эмулируем нажатие  клавиш	для ходьбы влево
}

void Bot::MoveBotRight(unsigned int Step)
{
	wchar_t tmp[200];
	for (unsigned int i = 0; i < Step; i++)
		tmp[i] = L'd';
	tmp[Step] = L'\0';
	key.SendString((LPCTSTR)tmp);												// эмулируем нажатие  клавиш	для ходьбы вправо
}

void Bot::Rotate(unsigned int Step, int chos)							//1-влево 2-вверх 3-вправо 4-вниз		вращение камеры
{
	switch (chos)
	{
	case 1:
	{
		wchar_t tmp[800];
		for (unsigned int i = 0; i < Step; i++)
			tmp[i] = L'j';
		tmp[Step] = L'\0';
		key.SendString((LPCTSTR)tmp);
		break;
	}
	case 2:
	{
		wchar_t tmp[800];
		for (unsigned int i = 0; i < Step; i++)
			tmp[i] = L'i';
		tmp[Step] = L'\0';
		key.SendString((LPCTSTR)tmp);
		break;
	}
	case 3:
	{
		wchar_t tmp[800];
		for (unsigned int i = 0; i < Step; i++)
			tmp[i] = L'l';
		tmp[Step] = L'\0';
		key.SendString((LPCTSTR)tmp);
		break;
	}
	case 4:
	{
		wchar_t tmp[800];
		for (unsigned int i = 0; i < Step; i++)
			tmp[i] = L'k';
		tmp[Step] = L'\0';
		key.SendString((LPCTSTR)tmp);
		break;
	}
	default:
		break;
	}
}

void Bot::AttackDown()
{
	//GetCursorPos(&pt);
	//pt.x = pt.x - lpRect.left;
	//pt.y = pt.y - lpRect.top;
	//нажимаем левую кнопку мыши по координатам
	//SendMessage(hwnd, WM_LBUTTONDOWN, MK_LBUTTON,0);
	//	отпускаем левую кнопку мыши по координатам
	mouse_event(0x0002, 0, 0, 0, 0);
	//SendMessage(hwnd, WM_LBUTTONUP, 0, 0);
	//MAKELONG(pt.x, pt.y)
}

void Bot::AttackUp()
{
	mouse_event(0x0004, 0, 0, 0, 0);
}

void Bot::Jump()
{
	TCHAR tmp;
	tmp = ' ';
	key.SendChar(tmp, 0);
	Sleep(10);
	key.SendChar(tmp, 1);
}

void Bot::SetCourses(double X, double Y)
{
	mouse_event(0x0001, X, Y, 0, 0);
}

void Bot::RunOnMapAWSD()
{
	key.SendString(PuthForBot[rand() % 4]);
}

void Bot::RunOnMapX()
{
	key.SendString(PuthForBot[rand() % 2 + 4]);
}
