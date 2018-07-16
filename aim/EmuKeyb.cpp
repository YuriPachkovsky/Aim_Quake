#include "stdafx.h"
#include "EmuKeyb.h"



EmuKeyb::EmuKeyb()
{
}

EmuKeyb::~EmuKeyb()
{
}

// посылает нажатие или отпускание клавиши со сканкодом scan
// если up == TRUE то это событие отпускани€, иначе нажати€
// возвращает NO_ERROR в случае успеха или код ошибки†
DWORD EmuKeyb::SendScanCode(WORD scan, BOOL up)
{
	INPUT inp = { 0 };
	inp.type = INPUT_KEYBOARD;
	inp.ki.wScan = scan;
	inp.ki.dwFlags = KEYEVENTF_SCANCODE | (up ? KEYEVENTF_KEYUP : 0);
	return SendInput(1, &inp, sizeof(inp)) ? NO_ERROR : GetLastError();
}

// посылает нажатие или отпускание виртуальной клавиши с кодом vk
// если up == TRUE то это событие отпускани€, иначе нажати€
// возвращает NO_ERROR в случае успеха или код ошибки†††
DWORD	EmuKeyb::SendVirtualKey(UINT vk, BOOL up)
{
	UINT scan = MapVirtualKey(vk, MAPVK_VK_TO_VSC);
	// конвертируем виртуальный код в сканкод
	return scan ? SendScanCode(scan, up) : ERROR_NO_UNICODE_TRANSLATION;
	// если конверси€ не удалась, то возвращаем†
	// ERROR_NO_UNICODE_TRANSLATION, хоть это и не вполне точно описывает
	// произошедшее, но ты будешь знать что трансл€ци€ навернулась.
	// ≈сли трансл€ци€ есть, то посылаем полученный сканкод
}



// посылает нажатие или отпускание клавиши с кодом символа ch
// если up == TRUE то это событие отпускани€, иначе нажати€
// возвращает NO_ERROR в случае успеха или код ошибки†


DWORD EmuKeyb::SendChar(TCHAR ch, BOOL up)
{
	SHORT vk = VkKeyScan(ch);																		// конвертируем символ в код виртуальной клавиши
	DWORD Result;
	if (0xFFFF == (USHORT)vk) {																		// если не вышло возвращаем ошибку
		Result = ERROR_NO_UNICODE_TRANSLATION;
		printf("vm key error :%d\n", GetLastError());
		return Result;
	}
	else {
		if (up) {																					// если это отпускание то игнорируем состо€ние shift, ctrl, alt
			Result = SendVirtualKey(LOBYTE(vk), TRUE);												// и посылаем отпускание полученного виртуального кода	
		}
		else {
			Result = NO_ERROR;																		// если это нажатие то нужно учесть shift, ctrl, alt
			if (HIBYTE(vk) & 1) Result = SendVirtualKey(VK_LSHIFT, FALSE);							// анализируем старший байт и при необходимости посылаем†
			if (NO_ERROR == Result) {																// виртуальные коды VK_LSHIFT, VK_LCONTROL, VK_LMENU соответственно
				if (HIBYTE(vk) & 2) Result = SendVirtualKey(VK_LCONTROL, FALSE);
				if (NO_ERROR == Result) {
					if (HIBYTE(vk) & 4)Result = SendVirtualKey(VK_LMENU, FALSE);					// каждый шаг продолжаем только если нет ошибки, иначе просто возвращаем еЄ код
					if (NO_ERROR == Result) {
						Result = SendVirtualKey(LOBYTE(vk), FALSE);
						if (NO_ERROR == Result) {													// посылаем нажатиме полученного виртуального кода
							if (HIBYTE(vk) & 4) Result = SendVirtualKey(VK_LMENU, TRUE);			// снова анализируем старший байт и при необходимости посылаем отпускани€ виртуальных
							if (NO_ERROR == Result) {												// кодов VK_LSHIFT, VK_LCONTROL, VK_LMENU, нажати€ которых мы послали выше
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
		Result = SendChar(s[0], FALSE);																// идем по всем символам строки пока не получили ошибку
		Sleep(30);
		if (NO_ERROR == Result) Result = SendChar(s[0], TRUE);										// посылаем нажатие каждого символа
	}																								// и если нет ошибки, то его отпускание
	return Result;
}
