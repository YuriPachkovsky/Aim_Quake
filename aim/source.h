#pragma once
//#include "stdafx.h"
#include <iostream>
#include "ScreenCl.h"
using namespace std;

Bot mybot;																																//��������� ������ ��� ������ � �������
ScreenCL SCREEN(mybot);																													//��������� ������ ��� ������ � ������������
HANDLE hAIMY, hRunOnMapX, hAIMX, hReturnCours, PrintScreen, hSynchFind, hAttack, hFindPix1S, hFindPix2S, hFindPix3S, hFindPix4S, hRunOnMap;//Handle �������
HANDLE Sem, Sem2, Sem1S, Sem2S, Sem3S, Sem4S, Sem1RetC, Sem2RetC, Sem3RetC, Sem4RetC, Sem1R, Sem2R, Sem3R, Sem4R;							//Semaphore
HANDLE EventX, EventY, EventForSynchAttack[2], EventXBit, EventYBit, EventForSynchToBitMap[2];											//Eventi ����� ��������� � �������
HANDLE EventAtt, EventeToReap, Evente1s, Evente2s, Evente3s, Evente4s, EventeForReturnCours[4], Evente1R, Evente2R, Evente3R, Evente4R;	//Event ������ � ��������
HANDLE EventeForRoteate[4];


DWORD WINAPI AIMX(LPVOID t)																			//����� ��� ��������� ������� �� ���������� X
{
	while (1)
	{
		WaitForSingleObject(Sem, INFINITE);
		SCREEN.SetCoursesOnPixX();
		//	ReleaseSemaphore(Sem2, 1, 0);
		SetEvent(EventForSynchAttack[0]);	//���������� �� �������� �� X
		SetEvent(EventForSynchToBitMap[0]); //���������� �� event �� �
	}
}

DWORD WINAPI AIMY(LPVOID t)																//����� ��� ��������� ������� �� ���������� Y
{
	while (1)
	{
		WaitForSingleObject(Sem, INFINITE);
		SCREEN.SetCoursesOnPixY();
		//	ReleaseSemaphore(Sem2, 1, 0);													//������������ ���� ����� �� � �������
		SetEvent(EventForSynchAttack[1]);	//���������� �� �������� �� Y
		SetEvent(EventForSynchToBitMap[1]); //���������� �� event �� Y		
	}
}

DWORD WINAPI Attack(LPVOID t)																			//����� ��� ��������� ������� ����������� ����� � ��������
{
	while (1)
	{
		if ((WaitForSingleObject(EventAtt, 0) == WAIT_TIMEOUT))
		{
			mybot.AttackUp();
			WaitForMultipleObjects(2, EventForSynchAttack, 1, INFINITE);
			mybot.AttackDown();
		}
	}
}

DWORD WINAPI Print(LPVOID t)
{
	while (1)
	{
		WaitForSingleObject(Sem2, INFINITE);	//�������� ��������� ��������� X
		WaitForSingleObject(Sem2, INFINITE);	//�������� ��������� ��������� Y
		if (SCREEN.FindPix()) {
			ReleaseSemaphore(Sem, 2, 0);
			SetEvent(EventAtt);
		}
		else {
			ResetEvent(EventAtt);
			ReleaseSemaphore(Sem2, 2, 0);
		}

	}
}

DWORD WINAPI SynchFind(LPVOID t)																		//���������������� ����� ��� ��������� � ������
{
	while (1) {
		WaitForMultipleObjects(2, EventForSynchToBitMap, 1, INFINITE);
		SetEvent(SCREEN.EventForExFinPix);
		ReleaseSemaphore(Sem1S, 1, 0);
		ReleaseSemaphore(Sem2S, 1, 0);
		ReleaseSemaphore(Sem3S, 1, 0);
		ReleaseSemaphore(Sem4S, 1, 0);
	}
}

DWORD WINAPI ReturnCours(LPVOID t)																				//������� ������� �� ����� �� ������� ����������� �����
{
	while (1) {
		WaitForMultipleObjects(4, EventeForReturnCours, 1, INFINITE);
		//SCREEN.ReturnCurs();
		ReleaseSemaphore(Sem1RetC, 1, 0);
		ReleaseSemaphore(Sem2RetC, 1, 0);
		ReleaseSemaphore(Sem3RetC, 1, 0);
		ReleaseSemaphore(Sem4RetC, 1, 0);
	}
}

DWORD WINAPI RunOnMapAWSD(LPVOID t)																				//��������� ������������ ����
{
	for (;; Sleep(125)) {
		mybot.RunOnMapAWSD();
	}
}

DWORD WINAPI RunOnMapX(LPVOID t)																				//��������� ��������  ����
{
	while (1) {
		WaitForMultipleObjects(4, EventeForRoteate, 1, INFINITE);
		mybot.RunOnMapX();
		//printf("Rotate!!!!!!!!!!!\n");
		ReleaseSemaphore(Sem1R, 1, 0);
		ReleaseSemaphore(Sem2R, 1, 0);
		ReleaseSemaphore(Sem3R, 1, 0);
		ReleaseSemaphore(Sem4R, 1, 0);
	}
}

DWORD WINAPI FindPix1Sec(LPVOID t)																		//����� ����� � ������ �������� ������
{
	while (1) {
		WaitForSingleObject(Sem1RetC, INFINITE);
		WaitForSingleObject(Sem1R, INFINITE);
		for (int i = 0; i < 5; i++) {
			WaitForSingleObject(Sem1S, INFINITE);
			if (SCREEN.FindPix1Sektor())
			{
				ReleaseSemaphore(Sem1RetC, 1, 0);
				ReleaseSemaphore(Sem2RetC, 1, 0);
				ReleaseSemaphore(Sem3RetC, 1, 0);
				ReleaseSemaphore(Sem4RetC, 1, 0);
				ReleaseSemaphore(Sem1R, 1, 0);
				ReleaseSemaphore(Sem2R, 1, 0);
				ReleaseSemaphore(Sem3R, 1, 0);
				ReleaseSemaphore(Sem4R, 1, 0);
				SetEvent(EventAtt);
				ResetEvent(Evente1s);
				ResetEvent(Evente1R);
				ReleaseSemaphore(Sem, 2, 0);
			}
			else {
				ResetEvent(EventAtt);
				if (i == 4) {
					SetEvent(Evente1s);
					SetEvent(Evente1R);
				}
				ReleaseSemaphore(Sem1S, 1, 0);
			}
		}
	}
}

DWORD WINAPI FindPix2Sec(LPVOID t)																		//����� ����� � 2 �������� ������
{
	while (1) {
		WaitForSingleObject(Sem2RetC, INFINITE);
		WaitForSingleObject(Sem2R, INFINITE);
		for (int i = 0; i < 5; i++) {
			WaitForSingleObject(Sem2S, INFINITE);
			if (SCREEN.FindPix2Sektor())
			{
				ReleaseSemaphore(Sem1RetC, 1, 0);
				ReleaseSemaphore(Sem2RetC, 1, 0);
				ReleaseSemaphore(Sem3RetC, 1, 0);
				ReleaseSemaphore(Sem4RetC, 1, 0);
				ReleaseSemaphore(Sem1R, 1, 0);
				ReleaseSemaphore(Sem2R, 1, 0);
				ReleaseSemaphore(Sem3R, 1, 0);
				ReleaseSemaphore(Sem4R, 1, 0);
				SetEvent(EventAtt);
				ResetEvent(Evente2s);
				ResetEvent(Evente2R);
				ReleaseSemaphore(Sem, 2, 0);
			}
			else {
				ResetEvent(EventAtt);
				if (i == 4) {
					SetEvent(Evente2s);
					SetEvent(Evente2R);
				}
				ReleaseSemaphore(Sem2S, 1, 0);
			}
		}
	}
}

DWORD WINAPI FindPix3Sec(LPVOID t)																			//����� ����� � 3 �������� ������
{
	while (1) {
		WaitForSingleObject(Sem3RetC, INFINITE);
		WaitForSingleObject(Sem3R, INFINITE);
		for (int i = 0; i < 5; i++) {
			WaitForSingleObject(Sem3S, INFINITE);
			if (SCREEN.FindPix3Sektor())
			{
				ReleaseSemaphore(Sem1RetC, 1, 0);
				ReleaseSemaphore(Sem2RetC, 1, 0);
				ReleaseSemaphore(Sem3RetC, 1, 0);
				ReleaseSemaphore(Sem4RetC, 1, 0);
				ReleaseSemaphore(Sem1R, 1, 0);
				ReleaseSemaphore(Sem2R, 1, 0);
				ReleaseSemaphore(Sem3R, 1, 0);
				ReleaseSemaphore(Sem4R, 1, 0);
				SetEvent(EventAtt);
				ResetEvent(Evente3s);
				ResetEvent(Evente3R);
				ReleaseSemaphore(Sem, 2, 0);
			}
			else {
				ResetEvent(EventAtt);
				if (i == 4) {
					SetEvent(Evente3s);
					SetEvent(Evente3R);
				}
				ReleaseSemaphore(Sem3S, 1, 0);
			}
		}
	}
}

DWORD WINAPI FindPix4Sec(LPVOID t)																				//����� ����� � 4 �������� ������
{
	while (1) {
		WaitForSingleObject(Sem4RetC, INFINITE);
		WaitForSingleObject(Sem4R, INFINITE);
		for (int i = 0; i < 5; i++) {
			WaitForSingleObject(Sem4S, INFINITE);
			if (SCREEN.FindPix4Sektor())
			{
				ReleaseSemaphore(Sem1RetC, 1, 0);
				ReleaseSemaphore(Sem2RetC, 1, 0);
				ReleaseSemaphore(Sem3RetC, 1, 0);
				ReleaseSemaphore(Sem4RetC, 1, 0);
				ReleaseSemaphore(Sem1R, 1, 0);
				ReleaseSemaphore(Sem2R, 1, 0);
				ReleaseSemaphore(Sem3R, 1, 0);
				ReleaseSemaphore(Sem4R, 1, 0);
				SetEvent(EventAtt);
				ResetEvent(Evente4s);
				ResetEvent(Evente4R);
				ReleaseSemaphore(Sem, 2, 0);
			}
			else {
				ResetEvent(EventAtt);
				if (i == 4) {
					SetEvent(Evente4s);
					SetEvent(Evente4R);
				}
				ReleaseSemaphore(Sem4S, 1, 0);
			}
		}
	}
}


void InitSemForWork()
{
	EventAtt = CreateEvent(NULL, true, 0, "EventeAtt");
	EventX = CreateEvent(NULL, false, 0, "EventeX");
	EventY = CreateEvent(NULL, false, 0, "EventeY");
	EventXBit = CreateEvent(NULL, false, 1, "EventeXBit");
	EventYBit = CreateEvent(NULL, false, 1, "EventeYBit");
	EventeToReap = CreateEvent(NULL, false, 1, "EventeToReap");
	Evente1s = CreateEvent(NULL, false, 0, "EventeReturnCours_1s");
	Evente2s = CreateEvent(NULL, false, 0, "EventeReturnCours_2s");
	Evente3s = CreateEvent(NULL, false, 0, "EventeReturnCours_3s");
	Evente4s = CreateEvent(NULL, false, 0, "EventeReturnCours_4s");
	Evente1R = CreateEvent(NULL, false, 0, "EventeRotateCours_1s");
	Evente2R = CreateEvent(NULL, false, 0, "EventeRotateCours_2s");
	Evente3R = CreateEvent(NULL, false, 0, "EventeRotateCours_3s");
	Evente4R = CreateEvent(NULL, false, 0, "EventeRotateCours_4s");

	EventeForRoteate[0] = Evente1R;
	EventeForRoteate[1] = Evente2R;
	EventeForRoteate[2] = Evente3R;
	EventeForRoteate[3] = Evente4R;
	EventeForReturnCours[0] = Evente1s;
	EventeForReturnCours[1] = Evente2s;
	EventeForReturnCours[2] = Evente3s;
	EventeForReturnCours[3] = Evente4s;
	EventForSynchToBitMap[0] = EventXBit;
	EventForSynchToBitMap[1] = EventXBit;
	EventForSynchAttack[0] = EventX;
	EventForSynchAttack[1] = EventY;

	Sem = CreateSemaphore(NULL, 0, 2, "SEMFORSYNCH");//������� ��� ����������� ���������� ��������
	Sem2 = CreateSemaphore(NULL, 2, 2, "SEMFORSYNCH1 ");//������� ��� ����������� ���������� ��������
	Sem1S = CreateSemaphore(NULL, 1, 1, "SEMFORSYNCH1S");//������� ��� ����������� ���������� ������ � 1 ��������		
	Sem2S = CreateSemaphore(NULL, 1, 1, "SEMFORSYNCH2S");//������� ��� ����������� ���������� ������ � 2 ��������
	Sem3S = CreateSemaphore(NULL, 1, 1, "SEMFORSYNCH3S");//������� ��� ����������� ���������� ������ � 3 ��������
	Sem4S = CreateSemaphore(NULL, 1, 1, "SEMFORSYNCH4S");//������� ��� ����������� ���������� ������ � 4 ��������
	Sem1RetC = CreateSemaphore(NULL, 1, 1, "RetCSemaphor1");//������� ��� ����������� ���������� �������� �������			
	Sem2RetC = CreateSemaphore(NULL, 1, 1, "RetCSemaphor2");//������� ��� ����������� ���������� �������� �������		 
	Sem3RetC = CreateSemaphore(NULL, 1, 1, "RetCSemaphor3");//������� ��� ����������� ���������� �������� �������		
	Sem4RetC = CreateSemaphore(NULL, 1, 1, "RetCSemaphor4");//������� ��� ����������� ���������� �������� �������	
	Sem1S = CreateSemaphore(NULL, 1, 1, "SemForRotat1R");//������� ��� ����������� ���������� ������ ����� ��������		
	Sem2S = CreateSemaphore(NULL, 1, 1, "SemForRotat2R");//������� ��� ����������� ���������� ������ ����� ��������		
	Sem3S = CreateSemaphore(NULL, 1, 1, "SemForRotat3R");//������� ��� ����������� ���������� ������ ����� ��������		
	Sem4S = CreateSemaphore(NULL, 1, 1, "SemForRotat4R");//������� ��� ����������� ���������� ������ ����� ��������		

}


void StartWork()
{


	Sleep(5000);

	hAIMX = CreateThread(NULL, 0, &AIMX, NULL, 0, NULL);
	hAIMY = CreateThread(NULL, 0, &AIMY, NULL, 0, NULL);

	hSynchFind = CreateThread(NULL, 0, &SynchFind, NULL, 0, NULL);
	hFindPix1S = CreateThread(NULL, 0, &FindPix1Sec, NULL, 0, NULL);
	hFindPix2S = CreateThread(NULL, 0, &FindPix2Sec, NULL, 0, NULL);
	hFindPix3S = CreateThread(NULL, 0, &FindPix3Sec, NULL, 0, NULL);
	hFindPix4S = CreateThread(NULL, 0, &FindPix4Sec, NULL, 0, NULL);
	//hReturnCours = CreateThread(NULL, 0, &ReturnCours, NULL, 0, NULL);
	//hRunOnMap = CreateThread(NULL, 0, &RunOnMapAWSD, NULL, 0, NULL);
	//hRunOnMapX = CreateThread(NULL, 0, &RunOnMapX, NULL, 0, NULL);
	hAttack = CreateThread(NULL, 0, &Attack, NULL, 0, NULL);
}

void StopWorks()
{
	TerminateThread(hAIMX, 0);
	TerminateThread(hAIMY, 0);
	TerminateThread(hSynchFind, 0);
	TerminateThread(hFindPix1S, 0);
	TerminateThread(hFindPix2S, 0);
	TerminateThread(hFindPix3S, 0);
	TerminateThread(hFindPix4S, 0);
	TerminateThread(hReturnCours, 0);
	TerminateThread(hRunOnMap, 0);
	TerminateThread(hRunOnMapX, 0);
	TerminateThread(hAttack, 0);
}