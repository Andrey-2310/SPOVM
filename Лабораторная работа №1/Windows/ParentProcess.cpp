#include<iostream>
#include<windows.h>
#include<WinBase.h>
#include"CreateFiles.h"
#include"Product.h"
using namespace std;

HANDLE hInEvent;

int main()
{
    MySpace s;
	DWORD dwWaitResult;
	STARTUPINFO startupinfo;
	PROCESS_INFORMATION processInformation;
	char c;
	setlocale(LC_ALL, "Russian");
	do
	{
		//������ ����� ����������, ���� ��� ��� �� �������
		cout << "�� ������ ������� ����� � �������� � ���������� (y/n)?" << endl;
		cin >> c;
		if (c == 'y') {
			s.createFiles();
		}


		hInEvent = CreateEvent(NULL, TRUE, FALSE, L"InEventName");
		if (hInEvent == NULL)
			return GetLastError();
		ZeroMemory(&startupinfo, sizeof(startupinfo));
		startupinfo.cb = sizeof(processInformation);
		if (!CreateProcess(L"D:\\C\\Visual Studio 2015\\Projects\\LR1 part2(Child Process)\\Debug\\LR1 part2(Child Process).exe",
			NULL, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &startupinfo, &processInformation))
		{
			cout << "Damn" << endl;
			return 0;
		}

		// ���� ������������ ������� � ���������� ��������� �� ��������� ��������
		dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
		if (dwWaitResult != WAIT_OBJECT_0) {
			cout << dwWaitResult;
			return 0;
		}
	
		
		s.showAmountOfMoneyInVendingMachine();
		cout << "����������, ����������� � �������������" << endl;
		do {
			s.showProducts();
			cout << "�� ������������ � �������? y/n" << endl;
			cin >> c;
		} while (c != 'y');
		PulseEvent(hInEvent);
		dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
		if (dwWaitResult != WAIT_OBJECT_0) {
			cout << dwWaitResult;
			return 0;
		}

		cout << endl << "����� �������" << endl;
		s.performOrder();

		PulseEvent(hInEvent);
		dwWaitResult = WaitForSingleObject(hInEvent, INFINITE);
		if (dwWaitResult != WAIT_OBJECT_0) {
			cout << dwWaitResult;
			return 0;
		}
		cout << "Do you want to exit: y/n ";
		cin >> c;
		PulseEvent(hInEvent);

	} while (c != 'y');
	CloseHandle(hInEvent);
	return 0;
}