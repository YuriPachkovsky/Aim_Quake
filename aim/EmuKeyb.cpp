#include "stdafx.h"
#include "EmuKeyb.h"



EmuKeyb::EmuKeyb()
{
}

EmuKeyb::~EmuKeyb()
{
}

// �������� ������� ��� ���������� ������� �� ��������� scan
// ���� up == TRUE �� ��� ������� ����������, ����� �������
// ���������� NO_ERROR � ������ ������ ��� ��� ������
DWORD EmuKeyb::SendScanCode(WORD scan, BOOL up)
{
	INPUT inp = { 0 };
	inp.type = INPUT_KEYBOARD;
	inp.ki.wScan = scan;
	inp.ki.dwFlags = KEYEVENTF_SCANCODE | (up ? KEYEVENTF_KEYUP : 0);
	return SendInput(1, &inp, sizeof(inp)) ? NO_ERROR : GetLastError();
}

// �������� ������� ��� ���������� ����������� ������� � ����� vk
// ���� up == TRUE �� ��� ������� ����������, ����� �������
// ���������� NO_ERROR � ������ ������ ��� ��� �����蠠�
DWORD	EmuKeyb::SendVirtualKey(UINT vk, BOOL up)
{
	UINT scan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
	// ������������ ����������� ��� � �������
	return scan ? SendScanCode(scan, up) : ERROR_NO_UNICODE_TRANSLATION;
	// ���� ��������� �� �������, �� ����������
	// ERROR_NO_UNICODE_TRANSLATION, ���� ��� � �� ������ ����� ���������
	// ������������, �� �� ������ ����� ��� ���������� �����������.
	// ���� ���������� ����, �� �������� ���������� �������
}



// �������� ������� ��� ���������� ������� � ����� ������� ch
// ���� up == TRUE �� ��� ������� ����������, ����� �������
// ���������� NO_ERROR � ������ ������ ��� ��� ������


DWORD EmuKeyb::SendChar(TCHAR ch, BOOL up)
{
	SHORT vk = VkKeyScan(ch);																		// ������������ ������ � ��� ����������� �������
	DWORD Result;
	if (0xFFFF == (USHORT)vk) {																		// ���� �� ����� ���������� ������
		Result = ERROR_NO_UNICODE_TRANSLATION;
		printf("vm key error :%d\n", GetLastError());
		return Result;
	}
	else {
		if (up) {																					// ���� ��� ���������� �� ���������� ��������� shift, ctrl, alt
			Result = SendVirtualKey(LOBYTE(vk), TRUE);												// � �������� ���������� ����������� ������������ ����	
		}
		else {
			Result = NO_ERROR;																		// ���� ��� ������� �� ����� ������ shift, ctrl, alt
			if (HIBYTE(vk) & 1) Result = SendVirtualKey(VK_LSHIFT, FALSE);							// ����������� ������� ���� � ��� ������������� ��������
			if (NO_ERROR == Result) {																// ����������� ���� VK_LSHIFT, VK_LCONTROL, VK_LMENU ��������������
				if (HIBYTE(vk) & 2) Result = SendVirtualKey(VK_LCONTROL, FALSE);
				if (NO_ERROR == Result) {
					if (HIBYTE(vk) & 4)Result = SendVirtualKey(VK_LMENU, FALSE);					// ������ ��� ���������� ������ ���� ��� ������, ����� ������ ���������� � ���
					if (NO_ERROR == Result) {
						Result = SendVirtualKey(LOBYTE(vk), FALSE);
						if (NO_ERROR == Result) {													// �������� �������� ����������� ������������ ����
							if (HIBYTE(vk) & 4) Result = SendVirtualKey(VK_LMENU, TRUE);			// ����� ����������� ������� ���� � ��� ������������� �������� ���������� �����������
							if (NO_ERROR == Result) {												// ����� VK_LSHIFT, VK_LCONTROL, VK_LMENU, ������� ������� �� ������� ����
								if (HIBYTE(vk) & 2)
									Result = SendVirtualKey(VK_LCONTROL, TRUE);
								if (NO_ERROR == Result) {
									if (HIBYTE(vk) & 1) Result = SendVirtualKey(VK_LSHIFT, TRUE);
								}
							}
						}
					}
				}
			}
			return Result;
		}
	}
}

DWORD EmuKeyb::SendString(LPCTSTR s)
{
	DWORD Result = NO_ERROR;
	for (; *s && !Result; ++s) {
		fflush(stdin);
		Result = SendChar(s[0], FALSE);																// ���� �� ���� �������� ������ ���� �� �������� ������
		Sleep(30);
		if (NO_ERROR == Result) Result = SendChar(s[0], TRUE);										// �������� ������� ������� �������
	}																								// � ���� ��� ������, �� ��� ����������
	return Result;
}
