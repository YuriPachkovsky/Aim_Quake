#pragma once
#include <Windows.h>

class EmuKeyb
{
public:
	EmuKeyb();
	~EmuKeyb();
	DWORD SendScanCode(WORD scan, BOOL up);
	DWORD SendVirtualKey(UINT vk, BOOL up);
	DWORD SendChar(TCHAR ch, BOOL up);
	DWORD SendString(LPCTSTR s);
};


