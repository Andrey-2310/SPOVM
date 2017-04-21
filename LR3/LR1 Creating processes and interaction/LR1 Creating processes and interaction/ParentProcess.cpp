#include<iostream>
#include<windows.h>
#include<WinBase.h>
#include"CreateFiles.h"
#include"Product.h"
using namespace std;

HANDLE hInEvent, hSemaphore1, readingThread, mutex, getNameMutex, StartReadingEvent;
DWORD WINAPI thread2(LPVOID);

int main()
{
	MySpace s;
	DWORD dwWaitResult;
	STARTUPINFO startupinfo;
	PROCESS_INFORMATION processInformation;
	char c;
	setlocale(LC_ALL, "Russian");
	hSemaphore1 = CreateSemaphore(NULL, 1, 1, L"Semaphore");
	hInEvent = CreateEvent(NULL, TRUE, FALSE, L"InEventName");
	if (hInEvent == NULL)
		return GetLastError();
	mutex = CreateMutex(NULL, FALSE, L"Mutex");
	getNameMutex = CreateMutex(NULL, FALSE, L"GetNameMutex");
	StartReadingEvent= CreateEvent(NULL, TRUE, FALSE, L"startReadingEvent");
	readingThread = CreateThread(NULL, 0, thread2, NULL, 0, NULL);
	s.CreateUserNames();

	
	while(true)
	{
		dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
		cout << "Приветствую вас" << endl;
		
		PulseEvent(hInEvent);
		// ждем переключения события в сигнальное состояние из дочернего процесса
		dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
		if (dwWaitResult != WAIT_OBJECT_0) {
			cout << dwWaitResult;
			return 0;
		}

		do
		{
			s.showAmountOfMoneyInVendingMachine();
			cout << endl << "Пожалуйста, ознакомтесь с прейскурантом";
			do {
				s.showProducts();
				cout << "Вы ознакомились с товаром? y/n  ";
				cin >> c;
			} while (c != 'y');
			PulseEvent(hInEvent);
			dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
			if (dwWaitResult != WAIT_OBJECT_0) {
				cout << dwWaitResult;
				return 0;
			}

			cout << endl << "Заказ получен" << endl;
			s.performOrder();

			PulseEvent(hInEvent);
			dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
			if (dwWaitResult != WAIT_OBJECT_0) {
				cout << dwWaitResult;
				return 0;
			}
			PulseEvent(hInEvent);
			cout << "Хотите закончить покупку?" << endl;
			PulseEvent(hInEvent);
			dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
			c = s.OutputAnswer();
			if (c != 'y')
				dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
		} while (c != 'y');
	}
	CloseHandle(hInEvent);
	CloseHandle(hSemaphore1);
	CloseHandle(readingThread);
	CloseHandle(mutex);
	return 0;
}

DWORD WINAPI thread2(LPVOID t)
{
	MySpace s;
	StartReadingEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"startReadingEvent");
	s.ClearUserID();
	s.ClearHelpUserID();
	while(true)
	{
		WaitForSingleObject(StartReadingEvent, INFINITE);
		vector<char> childIDs = s.ReadUsersID();
		cout << endl;
		for (int i = 0; i < childIDs.size(); i++)
			cout << " " << childIDs[i];
		PulseEvent(StartReadingEvent);
	}
	return 0;
}