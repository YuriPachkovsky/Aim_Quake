#pragma once
#include "EmuKeyb.h"
using namespace std;

class Bot {
public:
	EmuKeyb key;
	RECT lpRect;
	POINT pt;
	HWND hwnd;
	TCHAR buff[100];
	LPCTSTR PuthForBot[15];							//набор передвижений бота по карте
public:												//Функции для управления ботом
	Bot();
	void MoveBotForward(unsigned int Step);
	void MoveBotBack(unsigned int Step);
	void MoveBotLeft(unsigned int Step);
	void MoveBotRight(unsigned int Step);
	void Rotate(unsigned int Step, int chos);
	void AttackDown();
	void AttackUp();
	void Jump();
	void SetCourses(double X, double Y);
	void RunOnMapAWSD();
	void RunOnMapX();
};