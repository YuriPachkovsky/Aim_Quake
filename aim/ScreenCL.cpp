#include "stdafx.h"
#include "ScreenCl.h"




ScreenCL::ScreenCL(Bot tmp)
{
	bot = tmp;
	SemBitMap = CreateSemaphore(NULL, 0, 1, "SemForBitMapSynch");//семафор для разрешения анализа bitmap
	SemBitMapForStart = CreateSemaphore(NULL, 1, 1, "SemForBitMapSynchStart");//семафор для разрешения анализа bitmap		//1 =4
	EventForExFinPix = CreateEvent(NULL, false, 0, "EveForExFindPix"); // event для выхода из потоков поиска
	EventeForTrue = CreateEvent(NULL, false, 0, "EveForTrue"); // event для разрешения наведения
}

ScreenCL::~ScreenCL()
{
}

int ScreenCL::CaptureBMP(LPCTSTR szFile)
{
	HDC hdcScr, hdcMem;
	HBITMAP hbmScr;
	BITMAP bmp;
	int iXRes, iYRes;
	// Create a normal DC and a memory DC for the entire screen. The
	// normal DC provides a "snapshot" of the screen contents. The
	// memory DC keeps a copy of this "snapshot" in the associated
	// bitmap.
	//hdcScr = CreateDC("DISPLAY", NULL, NULL, NULL);
	hdcScr = GetWindowDC(bot.hwnd);
	hdcMem = CreateCompatibleDC(hdcScr);

	iXRes = GetDeviceCaps(hdcScr, HORZRES);
	iYRes = GetDeviceCaps(hdcScr, VERTRES);

	// Create a compatible bitmap for hdcScreen.
	hbmScr = CreateCompatibleBitmap(hdcScr, iXRes, iYRes);
	if (hbmScr == 0) return 0;

	// Select the bitmaps into the compatible DC.
	if (!SelectObject(hdcMem, hbmScr)) return 0;

	// Copy color data for the entire display into a
	// bitmap that is selected into a compatible DC.
	if (!StretchBlt(hdcMem, 0, 0, iXRes, iYRes, hdcScr, 0, 0, iXRes, iYRes, SRCCOPY))
		return 0;
	// Source[2]
	PBITMAPINFO pbmi;
	WORD cClrBits;

	// Retrieve the bitmap's color format, width, and height.
	if (!GetObject(hbmScr, sizeof(BITMAP), (LPSTR)&bmp)) return 0;

	// Convert the color format to a count of bits.
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;

	// Allocate memory for the BITMAPINFO structure. (This structure
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD
	// data structures.)
	if (cClrBits != 24)
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1 << cClrBits));

	// There is no RGBQUAD array for the 24-bit-per-pixel format.
	else
		pbmi = (PBITMAPINFO)LocalAlloc(LPTR, sizeof(BITMAPINFOHEADER));

	// Initialize the fields in the BITMAPINFO structure.
	pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if (cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1 << cClrBits);

	// If the bitmap is not compressed, set the BI_RGB flag.
	pbmi->bmiHeader.biCompression = BI_RGB;

	// Compute the number of bytes in the array of color
	// indices and store the result in biSizeImage.
	pbmi->bmiHeader.biSizeImage = (pbmi->bmiHeader.biWidth + 7) / 8
		* pbmi->bmiHeader.biHeight * cClrBits;

	// Set biClrImportant to 0, indicating that all of the
	// device colors are important.
	pbmi->bmiHeader.biClrImportant = 0;

	HANDLE hf;					// file handle
	BITMAPFILEHEADER hdr;		// bitmap file-header
	PBITMAPINFOHEADER pbih;		// bitmap info-header
	LPBYTE lpBits;				// memory pointer
	DWORD dwTotal;				// total count of bytes
	DWORD cb;					// incremental count of bytes
	BYTE *hp;					// byte pointer
	DWORD dwTmp;

	pbih = (PBITMAPINFOHEADER)pbmi;
	lpBits = (LPBYTE)GlobalAlloc(GMEM_FIXED, pbih->biSizeImage);

	if (!lpBits) return 0;

	// Retrieve the color table (RGBQUAD array) and the bits
	// (array of palette indices) from the DIB.
	if (!GetDIBits(hdcMem, hbmScr, 0, (WORD)pbih->biHeight, lpBits, pbmi, DIB_RGB_COLORS)) return 0;

	// Create the .BMP file.
	hf = CreateFile(szFile, GENERIC_READ | GENERIC_WRITE, (DWORD)0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if (hf == INVALID_HANDLE_VALUE) return 0;

	hdr.bfType = 0x4d42;		// 0x42 = "B" 0x4d = "M"

								// Compute the size of the entire file.
	hdr.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD) + pbih->biSizeImage);
	hdr.bfReserved1 = 0;
	hdr.bfReserved2 = 0;

	// Compute the offset to the array of color indices.
	hdr.bfOffBits = (DWORD) sizeof(BITMAPFILEHEADER) + pbih->biSize + pbih->biClrUsed * sizeof(RGBQUAD);

	// Copy the BITMAPFILEHEADER into the .BMP file.
	if (!WriteFile(hf, (LPVOID)&hdr, sizeof(BITMAPFILEHEADER), (LPDWORD)&dwTmp, NULL)) return 0;

	// Copy the BITMAPINFOHEADER and RGBQUAD array into the file.
	if (!WriteFile(hf, (LPVOID)pbih, sizeof(BITMAPINFOHEADER) + pbih->biClrUsed * sizeof(RGBQUAD), (LPDWORD)&dwTmp, NULL))
		return 0;

	// Copy the array of color indices into the .BMP file.
	dwTotal = cb = pbih->biSizeImage;
	hp = lpBits;
	if (!WriteFile(hf, (LPSTR)hp, (int)cb, (LPDWORD)&dwTmp, NULL)) return 0;

	// Close the .BMP file.
	if (!CloseHandle(hf)) return 0;
	// Free memory.
	GlobalFree((HGLOBAL)lpBits);
	ReleaseDC(0, hdcScr);
	ReleaseDC(0, hdcMem);

	return 1;
}

bool ScreenCL::FindPix()
{
	COLORREF Color;
	int iXRes, iYRes;

	//hdcScr = CreateDC(L"DISPLAY", NULL, NULL, NULL);
	hdcScr = GetWindowDC(bot.hwnd);
	hdcMem = CreateCompatibleDC(hdcScr);

	iXRes = GetDeviceCaps(hdcScr, HORZRES);
	iYRes = GetDeviceCaps(hdcScr, VERTRES);


	hbmScr = CreateCompatibleBitmap(hdcScr, iXRes, iYRes);
	if (hbmScr == 0) return 0;


	if (!SelectObject(hdcMem, hbmScr)) return 0;


	if (!StretchBlt(hdcMem, 0, 0, iXRes, iYRes, hdcScr, 0, 0, iXRes, iYRes, SRCCOPY))
		return 0;

	//	COLORREF minLimit = RGB(190, 5, 5); //диапазон минимального цвета		
	//	COLORREF maxLimit = RGB(210, 20, 10); //диапазон максимального цвета	

	//	COLORREF minLimit = RGB(75, 240, 220); 
	//	COLORREF maxLimit = RGB(85, 255, 245);					// диапозон цвета пикселей для нахождения врага bones

	//	COLORREF minLimit = RGB(41, 0, 150);
	//	COLORREF maxLimit = RGB(91, 5, 245);					// диапозон цвета пикселей для нахождения врага chert

	//	COLORREF minLimit = RGB(0, 10, 50);
	//	COLORREF maxLimit = RGB(20, 40, 160);					// диапозон цвета пикселей для нахождения врага Keel

	int shagX, shagY;
	shagX = rand() % 2 + 3;
	shagY = rand() % 2 + 8;
	for (Y = 768 / 2; Y >70; Y -= shagY) {																					//2 четверть проверяется на наличие пикселя
		for (X = 512; X >50; X -= shagX)
		{
			Color = GetPixel(hdcMem, X, Y);
			if (GetRValue(Color) >= GetRValue(minLimit) && GetRValue(Color) <= GetRValue(maxLimit))
			{
				if (GetGValue(Color) >= GetGValue(minLimit) && GetGValue(Color) <= GetGValue(maxLimit))							//Дописать до поиска по четвертям
				{
					if (GetBValue(Color) >= GetBValue(minLimit) && GetBValue(Color) <= GetBValue(maxLimit))
					{
						DeleteObject(hbmScr);
						DeleteDC(hdcMem);
						ReleaseDC(bot.hwnd, hdcScr);
						return true;
					}
				}
			}
		}
	}
	for (Y = 768 / 2; Y > 70; Y -= shagY) {																						//Проверяем 1 четверть на наличие пикселя
		for (X = 512; X<968; X += shagX)
		{
			Color = GetPixel(hdcMem, X, Y);
			if (GetRValue(Color) >= GetRValue(minLimit) && GetRValue(Color) <= GetRValue(maxLimit))
			{
				if (GetGValue(Color) >= GetGValue(minLimit) && GetGValue(Color) <= GetGValue(maxLimit))
				{
					if (GetBValue(Color) >= GetBValue(minLimit) && GetBValue(Color) <= GetBValue(maxLimit))
					{
						DeleteObject(hbmScr);
						DeleteDC(hdcMem);
						ReleaseDC(bot.hwnd, hdcScr);
						return true;
					}
				}
			}
		}
	}
	for (Y = 768 / 2; Y <668; Y += shagY) {																		// 4 четверть проверяется на наличие пикселя
		for (X = 512; X <974; X += shagX)
		{
			Color = GetPixel(hdcMem, X, Y);
			if (GetRValue(Color) >= GetRValue(minLimit) && GetRValue(Color) <= GetRValue(maxLimit))
			{
				if (GetGValue(Color) >= GetGValue(minLimit) && GetGValue(Color) <= GetGValue(maxLimit))
				{
					if (GetBValue(Color) >= GetBValue(minLimit) && GetBValue(Color) <= GetBValue(maxLimit))
					{
						DeleteObject(hbmScr);
						DeleteDC(hdcMem);
						ReleaseDC(bot.hwnd, hdcScr);
						return true;
					}
				}
			}
		}
	}
	for (Y = 768 / 2; Y <668; Y += shagY) {																		// 3 четверть проверяется на наличие пикселя
		for (X = 512; X >70; X -= shagX)
		{
			Color = GetPixel(hdcMem, X, Y);
			if (GetRValue(Color) >= GetRValue(minLimit) && GetRValue(Color) <= GetRValue(maxLimit))
			{
				if (GetGValue(Color) >= GetGValue(minLimit) && GetGValue(Color) <= GetGValue(maxLimit))
				{
					if (GetBValue(Color) >= GetBValue(minLimit) && GetBValue(Color) <= GetBValue(maxLimit))
					{
						DeleteObject(hbmScr);
						DeleteDC(hdcMem);
						ReleaseDC(bot.hwnd, hdcScr);
						return true;
					}
				}
			}
		}
	}
	DeleteObject(hbmScr);
	DeleteDC(hdcMem);
	ReleaseDC(bot.hwnd, hdcScr);
	return false;
}

bool ScreenCL::FindPix1Sektor()
{
	HDC hdcScr1S, hdcMem1S;
	HBITMAP hbmScr1S;
	COLORREF Color;
	int shagX, shagY;
	shagX = rand() % 2 + 2;
	shagY = rand() % 2 + 4;

	//hdcScr1S = CreateDC("DISPLAY", NULL, NULL, NULL);
	hdcScr1S = GetWindowDC(bot.hwnd);
	//hdcScr1S = GetDC(0);
	if (hdcScr1S == NULL)
		printf("error getwindowDC\n");

	hdcMem1S = CreateCompatibleDC(hdcScr1S);
	if (hdcMem1S == NULL)
		printf("error CreateCompatibleDC\n");

	int iXRes1S = GetDeviceCaps(hdcScr1S, HORZRES);
	int iYRes1S = GetDeviceCaps(hdcScr1S, VERTRES);

	hbmScr1S = CreateCompatibleBitmap(hdcScr1S, iXRes1S, iYRes1S);

	if (hbmScr1S == 0) return 0;
	if (!SelectObject(hdcMem1S, hbmScr1S)) return 0;
	if (!StretchBlt(hdcMem1S, 0, 0, iXRes1S, iYRes1S, hdcScr1S, 0, 0, iXRes1S, iYRes1S, SRCCOPY))
		return 0;

	for (Y1S = 768 / 2; Y1S > 70; Y1S -= shagY) {																		//Проверяем 1 четверть на наличие пикселя
		for (X1S = 512; X1S<968; X1S += shagX)
		{
			Color = GetPixel(hdcMem1S, X1S, Y1S);
			if (GetRValue(Color) >= GetRValue(minLimit) && GetRValue(Color) <= GetRValue(maxLimit))
			{
				if (GetGValue(Color) >= GetGValue(minLimit) && GetGValue(Color) <= GetGValue(maxLimit))
				{
					if (GetBValue(Color) >= GetBValue(minLimit) && GetBValue(Color) <= GetBValue(maxLimit))
					{
						if (WaitForSingleObject(EventForExFinPix, 100) == WAIT_TIMEOUT) {
							DeleteObject(hbmScr1S);
							DeleteDC(hdcMem1S);
							ReleaseDC(bot.hwnd, hdcScr1S);
							return 0;
						}
						X = X1S;
						Y = Y1S;
						DeleteObject(hbmScr1S);
						DeleteDC(hdcMem1S);
						ReleaseDC(bot.hwnd, hdcScr1S);
						return 1;
					}
				}
			}
		}
	}
	DeleteObject(hbmScr1S);
	DeleteDC(hdcMem1S);
	ReleaseDC(bot.hwnd, hdcScr1S);
	return 0;
}

bool ScreenCL::FindPix2Sektor()
{
	HDC hdcScr2S, hdcMem2S;
	HBITMAP hbmScr2S;
	COLORREF Color;
	int shagX, shagY;
	shagX = rand() % 2 + 2;
	shagY = rand() % 2 + 4;

	//hdcScr2S = CreateDC("DISPLAY", NULL, NULL, NULL);
	hdcScr2S = GetWindowDC(bot.hwnd);
	//hdcScr2S = GetDC(0);
	if (hdcScr2S == NULL)
		printf("error getwindowDC\n");

	hdcMem2S = CreateCompatibleDC(hdcScr2S);
	if (hdcMem2S == NULL)
		printf("error CreateCompatibleDC\n");

	int iXRes2S = GetDeviceCaps(hdcScr2S, HORZRES);
	int iYRes2S = GetDeviceCaps(hdcScr2S, VERTRES);

	hbmScr2S = CreateCompatibleBitmap(hdcScr2S, iXRes2S, iYRes2S);

	if (hbmScr2S == 0) return 0;
	if (!SelectObject(hdcMem2S, hbmScr2S)) return 0;
	if (!StretchBlt(hdcMem2S, 0, 0, iXRes2S, iYRes2S, hdcScr2S, 0, 0, iXRes2S, iYRes2S, SRCCOPY))
		return 0;

	for (Y2S = 768 / 2; Y2S > 70; Y2S -= shagY) {																		//Проверяем 2 четверть на наличие пикселя
		for (X2S = 512; X2S>50; X2S -= shagX)
		{
			Color = GetPixel(hdcMem2S, X2S, Y2S);
			if (GetRValue(Color) >= GetRValue(minLimit) && GetRValue(Color) <= GetRValue(maxLimit))
			{
				if (GetGValue(Color) >= GetGValue(minLimit) && GetGValue(Color) <= GetGValue(maxLimit))
				{
					if (GetBValue(Color) >= GetBValue(minLimit) && GetBValue(Color) <= GetBValue(maxLimit))
					{
						if (WaitForSingleObject(EventForExFinPix, 100) == WAIT_TIMEOUT) {
							DeleteObject(hbmScr2S);
							DeleteDC(hdcMem2S);
							ReleaseDC(bot.hwnd, hdcScr2S);
							return 0;
						}
						X = X2S;
						Y = Y2S;
						DeleteObject(hbmScr2S);
						DeleteDC(hdcMem2S);
						ReleaseDC(bot.hwnd, hdcScr2S);
						return 1;
					}
				}
			}
		}
	}
	DeleteObject(hbmScr2S);
	DeleteDC(hdcMem2S);
	ReleaseDC(bot.hwnd, hdcScr2S);
	return 0;
}

bool ScreenCL::FindPix3Sektor()
{
	HDC hdcScr3S, hdcMem3S;
	HBITMAP hbmScr3S;
	COLORREF Color;

	int shagX, shagY;
	shagX = rand() % 2 + 2;
	shagY = rand() % 2 + 4;
	//hdcScr3S = CreateDC("DISPLAY", NULL, NULL, NULL);
	hdcScr3S = GetWindowDC(bot.hwnd);
	//hdcScr3S = GetDC(0);
	if (hdcScr3S == NULL)
		printf("error getwindowDC\n");

	hdcMem3S = CreateCompatibleDC(hdcScr3S);
	if (hdcMem3S == NULL)
		printf("error CreateCompatibleDC\n");

	int iXRes3S = GetDeviceCaps(hdcScr3S, HORZRES);
	int iYRes3S = GetDeviceCaps(hdcScr3S, VERTRES);

	hbmScr3S = CreateCompatibleBitmap(hdcScr3S, iXRes3S, iYRes3S);

	if (hbmScr3S == 0) return 0;
	if (!SelectObject(hdcMem3S, hbmScr3S)) return 0;
	if (!StretchBlt(hdcMem3S, 0, 0, iXRes3S, iYRes3S, hdcScr3S, 0, 0, iXRes3S, iYRes3S, SRCCOPY))
		return 0;

	for (Y3S = 768 / 2; Y3S < 668; Y3S += shagY) {																		//Проверяем 3 четверть на наличие пикселя
		for (X3S = 512; X3S>50; X3S -= shagX)
		{
			Color = GetPixel(hdcMem3S, X3S, Y3S);
			if (GetRValue(Color) >= GetRValue(minLimit) && GetRValue(Color) <= GetRValue(maxLimit))
			{
				if (GetGValue(Color) >= GetGValue(minLimit) && GetGValue(Color) <= GetGValue(maxLimit))
				{
					if (GetBValue(Color) >= GetBValue(minLimit) && GetBValue(Color) <= GetBValue(maxLimit))
					{
						if (WaitForSingleObject(EventForExFinPix, 100) == WAIT_TIMEOUT) {
							DeleteObject(hbmScr3S);
							DeleteDC(hdcMem3S);
							ReleaseDC(bot.hwnd, hdcScr3S);
							return 0;
						}
						X = X3S;
						Y = Y3S;
						DeleteObject(hbmScr3S);
						DeleteDC(hdcMem3S);
						ReleaseDC(bot.hwnd, hdcScr3S);
						return 1;
					}
				}
			}
		}
	}
	DeleteObject(hbmScr3S);
	DeleteDC(hdcMem3S);
	ReleaseDC(bot.hwnd, hdcScr3S);
	return 0;
}

bool ScreenCL::FindPix4Sektor()
{
	HDC hdcScr4S, hdcMem4S;
	HBITMAP hbmScr4S;
	COLORREF Color;

	int shagX, shagY;
	shagX = rand() % 2 + 2;
	shagY = rand() % 2 + 4;

	//hdcScr4S = CreateDC("DISPLAY", NULL, NULL, NULL);
	hdcScr4S = GetWindowDC(bot.hwnd);
	//hdcScr4S = GetDC(0);
	if (hdcScr4S == NULL)
		printf("error getwindowDC\n");

	hdcMem4S = CreateCompatibleDC(hdcScr4S);
	if (hdcMem4S == NULL)
		printf("error CreateCompatibleDC\n");

	int iXRes4S = GetDeviceCaps(hdcScr4S, HORZRES);
	int iYRes4S = GetDeviceCaps(hdcScr4S, VERTRES);

	hbmScr4S = CreateCompatibleBitmap(hdcScr4S, iXRes4S, iYRes4S);

	if (hbmScr4S == 0) return 0;
	if (!SelectObject(hdcMem4S, hbmScr4S)) return 0;
	if (!StretchBlt(hdcMem4S, 0, 0, iXRes4S, iYRes4S, hdcScr4S, 0, 0, iXRes4S, iYRes4S, SRCCOPY))
		return 0;

	for (Y4S = 768 / 2; Y4S < 668; Y4S += shagY) {																		//Проверяем 4 четверть на наличие пикселя
		for (X4S = 512; X4S<974; X4S += shagX)
		{
			Color = GetPixel(hdcMem4S, X4S, Y4S);
			if (GetRValue(Color) >= GetRValue(minLimit) && GetRValue(Color) <= GetRValue(maxLimit))
			{
				if (GetGValue(Color) >= GetGValue(minLimit) && GetGValue(Color) <= GetGValue(maxLimit))
				{
					if (GetBValue(Color) >= GetBValue(minLimit) && GetBValue(Color) <= GetBValue(maxLimit))
					{
						if (WaitForSingleObject(EventForExFinPix, 1000) == WAIT_TIMEOUT) {
							DeleteObject(hbmScr4S);
							DeleteDC(hdcMem4S);
							ReleaseDC(bot.hwnd, hdcScr4S);
							return 0;
						}
						X = X4S;
						Y = Y4S;
						DeleteObject(hbmScr4S);
						DeleteDC(hdcMem4S);
						ReleaseDC(bot.hwnd, hdcScr4S);
						return 1;
					}
				}
			}
		}
	}
	DeleteObject(hbmScr4S);
	DeleteDC(hdcMem4S);
	ReleaseDC(bot.hwnd, hdcScr4S);
	return 0;
}


void ScreenCL::SetCoursesOnPix()
{
	int PosX = X - 1024 / 2;
	int PosY = Y - 768 / 2;
	int i = 0, count = 0;
	while (1)
	{
		if (FindPix()) {
			PosX = X - 1024 / 2;
			PosY = Y - 768 / 2;
			if ((PosX < -5) || (PosX > 5) || (PosY > 5) || (PosY < -5))
			{
				printf("yep\nPosX =%d PosY=%d, X=%d Y=%d\n", PosX, PosY, X, Y);
				if (PosX < 0) {
					if (abs(PosX) / 30 != 0) {
						bot.Rotate(abs(PosX) / 30, 1);
						stkX.push(-PosX / 30);
					}
					else {
						bot.SetCourses(-7, 0);
						stkX.push(0);
					}
				}
				else {
					if (abs(PosX) / 30 != 0) {
						bot.Rotate(abs(PosX) / 30, 3);
						stkX.push(-PosX / 30);
					}
					else {
						bot.SetCourses(7, 0);
						stkX.push(-35);
					}
				}

				if (PosY < 0) {
					if (abs(PosY) / 30 != 0) {
						bot.Rotate(abs(PosY) / 30, 2);
						stkY.push(-PosY / 30);
					}
					else {
						bot.SetCourses(0, -7);
						stkY.push(0);
					}
				}
				else
				{
					if (abs(PosY) / 30 != 0) {
						bot.Rotate(abs(PosY) / 30, 4);
						stkY.push(-PosY / 30);
					}
					else {
						bot.SetCourses(0, 7);
						stkY.push(-35);
					}
				}
			}
			else {
				break;
				//bot.Attack();
			}
		}
	}

}

void ScreenCL::SetCoursesOnPixX()
{
	int PosX = X - 1024 / 2;
	if ((PosX < -5) || (PosX > 5))
	{
		if (PosX < 0) {
			if (abs(PosX) / 35 != 0) {
				bot.Rotate(abs(PosX) / 30, 1);
				stkX.push(-PosX / 30);
			}
			else
				bot.SetCourses(-3, 0);
		}
		else {
			if (abs(PosX) / 35 != 0)
			{
				bot.Rotate(abs(PosX) / 30, 3);
				stkX.push(-PosX / 30);
			}
			else
				bot.SetCourses(3, 0);
		}
	}
}

void ScreenCL::SetCoursesOnPixY()
{
	int PosY;
	PosY = Y - 798 / 2;
	if ((PosY > 5) || (PosY < -5))
	{
		if (PosY < 0) {
			if (abs(PosY) / 35 != 0) {
				bot.Rotate(abs(PosY) / 30, 2);
				stkY.push(-PosY / 30);
			}
			else
				bot.SetCourses(0, -3);
		}
		else
		{
			if (abs(PosY) / 35 != 0) {
				bot.Rotate(abs(PosY) / 30, 4);
				stkY.push(-PosY / 30);
			}
			else
				bot.SetCourses(0, 3);
		}
	}
}

void ScreenCL::ReturnCurs()
{
	int PsX;
	int PsY;
	while (!stkX.empty())
	{
		PsX = stkX.top();
		stkX.pop();
		if (PsX < 0) {
			if (abs(PsX) != 0 && PsX != -35) {
				bot.Rotate(abs(PsX), 1);
			}
			else
				bot.SetCourses(-3, 0);
		}
		else {
			if (abs(PsX) != 0)
			{
				bot.Rotate(abs(PsX), 3);
			}
			else
				bot.SetCourses(3, 0);
		}
	}

	while (!stkY.empty())
	{
		PsY = stkY.top();
		stkY.pop();
		if (PsY < 0) {
			if (abs(PsY) != 0 && PsY != -35) {
				bot.Rotate(abs(PsY), 2);
			}
			else
				bot.SetCourses(-7, 0);
		}
		else {
			if (abs(PsY) != 0)
			{
				bot.Rotate(abs(PsY), 4);
			}
			else
				bot.SetCourses(7, 0);
		}
	}
}

bool ScreenCL::SynchBitMap()
{
	WaitForSingleObject(SemBitMapForStart, INFINITE);
	printf("qwert\n");
	SetEvent(EventForExFinPix);
	ReleaseSemaphore(SemBitMap, 1, 0);
	return 1;
}

void ScreenCL::FreeBitMap()
{
	DeleteObject(hbmScr);
	DeleteDC(hdcMem);
	ReleaseDC(bot.hwnd, hdcScr);
}

