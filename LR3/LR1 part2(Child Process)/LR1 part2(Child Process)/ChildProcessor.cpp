#include <windows.h>
#include <iostream>
#include<WinBase.h>
#include"CreateFiles.h"
#include"Product.h"
using namespace std;
HANDLE hInEvent, hSemaphore, mutex, getNameMutex, WrittingThread, StartReadingEvent;
DWORD dwWaitResult;
DWORD WINAPI writting(LPVOID);
char name;

int main()
{
	MySpace s;
	char c;
	setlocale(LC_ALL, "Russian");
	
	getNameMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"GetNameMutex");
	dwWaitResult = WaitForSingleObject(getNameMutex, INFINITE);
	name = s.GetNameOfProcess();
	cout << endl << "��� ��������: " << name << endl << endl;
	ReleaseMutex(getNameMutex);
	WrittingThread = CreateThread(NULL, 0, writting, NULL, 0, NULL);

	hSemaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, false, L"Semaphore");
	cout << "� ������ ������ ������� �� ��������, �� ���������� � �������" << endl;
	dwWaitResult = WaitForSingleObject(hSemaphore, INFINITE);
	cout << "�� ������ ������� �����" << endl;



	hInEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"InEventName");
	if (hInEvent == NULL)
	{
		cout << "Open event failed." << endl;
		cout << "Input any char to exit." << endl;
		cin >> c;
		return GetLastError();
	}
	s.createFiles();
	PulseEvent(hInEvent);
	do {
		s.askManToPutMoney();
		// ������������� ������� � ����� �������
		PulseEvent(hInEvent);
		dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
		s.askManToEnterProducts();
		PulseEvent(hInEvent);
		dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);

		s.showPurchases();
		PulseEvent(hInEvent);
		dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
		cout << "y/n  ";
		cin >> c;
		s.InputAnswer(c);
		PulseEvent(hInEvent);
	} while (c != 'y');
	ReleaseSemaphore(hSemaphore, 1, NULL);
	// ��������� ���������� ������� � ������� ��������
	CloseHandle(hInEvent);

	return 0;
}

DWORD WINAPI writting(LPVOID t)
{
	MySpace s;
	StartReadingEvent=OpenEvent(EVENT_ALL_ACCESS, FALSE, L"startReadingEvent");
	mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"Mutex");
	while (true) {
		WaitForSingleObject(mutex, INFINITE);
		if (s.WriteUserID(name)) {
			PulseEvent(StartReadingEvent);
			WaitForSingleObject(StartReadingEvent, INFINITE);
		}
		ReleaseMutex(mutex);
		//Sleep(400);
	}

}
