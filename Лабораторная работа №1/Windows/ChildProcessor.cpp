#include <windows.h>
#include <iostream>
#include<WinBase.h>
#include"CreateFiles.h"
#include"Product.h"
using namespace std;
volatile HANDLE hInEvent;
int main()
{
	MySpace s;
	char c;
	hInEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"InEventName");
	if (hInEvent == NULL)
	{
		cout << "Open event failed." << endl;
		cout << "Input any char to exit." << endl;
		cin >> c;
		return GetLastError();
	}
	setlocale(LC_ALL, "Russian");
	s.askManToPutMoney();
	// устанавливаем событие о вводе символа
	PulseEvent(hInEvent);
	DWORD dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);

	s.askManToEnterProducts();
	PulseEvent(hInEvent);
	dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);

	s.showPurchases();
	PulseEvent(hInEvent);
	dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
	// закрываем дескриптор события в текущем процессе
	CloseHandle(hInEvent);

	return 0;
}