#pragma once
#include "HeaderClassBot.h"
#include <stack>
using namespace std;

class ScreenCL
{
public:
	Bot bot;
	stack<int> stkX, stkY;
	int X = 0, X1S, X2S, X3S, X4S;
	int Y = 0, Y1S, Y2S, Y3S, Y4S;
	int iXRes, iYRes;
	HDC hdcScr, hdcMem;
	HBITMAP hbmScr;
	HANDLE SemBitMap, EventForExFinPix, EventeForTrue, SemBitMapForStart;
	COLORREF minLimit = RGB(0, 10, 50);
	COLORREF maxLimit = RGB(20, 40, 160);					// диапозон цвета пикселей дл€ нахождени€ врага Keel															
public:
	ScreenCL(Bot tmp);
	~ScreenCL();
	int CaptureBMP(LPCTSTR szFile);
	bool FindPix();
	bool FindPix1Sektor();
	bool FindPix2Sektor();
	bool FindPix3Sektor();
	bool FindPix4Sektor();
	void SetCoursesOnPix();
	void SetCoursesOnPixY();
	void SetCoursesOnPixX();
	void ReturnCurs();
	bool SynchBitMap();
	void FreeBitMap();
};
