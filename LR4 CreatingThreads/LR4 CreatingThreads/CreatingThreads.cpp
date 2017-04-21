#include<iostream>
#include<windows.h>
#include <conio.h>
#include <fstream>
#include<vector>
#include<queue>
#include"FileWorker.h"
using namespace std;
HANDLE readingThread, writtingThread, creatingNamesThread;
vector<HANDLE> Threads;
DWORD WINAPI Client(LPVOID);
DWORD WINAPI Server(LPVOID);
DWORD WINAPI WrittingNames(LPVOID);

int main1()
{
	FileWorker workWithFiles;
	DWORD waitResult;
	HANDLE setNameMutex = CreateMutex(NULL, FALSE, L"GetAndSetName");
	HANDLE settingDisconnect = CreateEvent(NULL, TRUE, FALSE, L"SettingDisconnect");
	SECURITY_ATTRIBUTES sa; // атрибуты защиты
	SECURITY_DESCRIPTOR sd; // дескриптор защиты
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE; // дескриптор канала ненаследуемый
							   // инициализируем дескриптор защиты
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	// устанавливаем атрибуты защиты, разрешая доступ всем пользователям
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;
	
	
	HANDLE workWithNames = CreateNamedPipe(L"\\\\.\\pipe\\$MyPipe$",
		PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE  | PIPE_READMODE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES, 512, 512, INFINITE, &sa);
	if (workWithNames == INVALID_HANDLE_VALUE)
	{
		cerr << "Creation of the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		return 0;
	}
	creatingNamesThread = CreateThread(NULL, 0, &WrittingNames, NULL, 0, NULL);
	BOOL isConnected = ConnectNamedPipe(workWithNames, NULL);


	char names[52], newnames[52];
	DWORD amountOfBytes, newamountOfBytes;	
	/*if (!ReadFile(workWithNames, newnames, 52, &newamountOfBytes, (LPOVERLAPPED)NULL))
	{
		// ошибка записи
		cerr << "Reading from the named pipe failed: " << endl
			<< "The last error code: " << GetLastError() << endl;
		return 0;
	}

	cout << "Names are read" << endl;
	for (int i = 0; i < 52; i++)
		cout << newnames[i] << " ";*/
	waitResult = WaitForSingleObject(settingDisconnect, INFINITE);
	BOOL isDisconnected = DisconnectNamedPipe(workWithNames);

	setlocale(LC_ALL, "Russian");

	cout << "Введите интервал, с которым потоки будут автоматически создаваться" << endl;
	int timeToCreateNewThread;
	cin >> timeToCreateNewThread;
	//readingThread = CreateThread(NULL, 0, &Server, NULL, 0, NULL);
	//while (true)
//	{
	Sleep(timeToCreateNewThread * 1000);
	Threads.push_back(CreateThread(NULL, 0, &Client, NULL, 0, NULL));
	cout << "Ожидание подключения к пайпу" << endl;
	BOOL fConnected = ConnectNamedPipe(workWithNames, NULL);
	waitResult = WaitForSingleObject(settingDisconnect, INFINITE);
	//isDisconnected = DisconnectNamedPipe(workWithNames);
	//if (isDisconnected) cout << "Соединение разорвано" << endl << endl;
	//}
	return 0;
}



DWORD WINAPI WrittingNames(LPVOID t)
{
	char names[52], newnames[52];
	DWORD amountOfBytes, newamountOfBytes;

	for (int i = 0; i < 52; i++)
		if (i < 26) names[i] = 'A'+i;
		else names[i] = i - 26 + 'a';
		HANDLE settingDisconnect = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"SettingDisconnect");

		HANDLE work = CreateFile(L"\\\\.\\pipe\\$MyPipe$", GENERIC_READ | GENERIC_WRITE, 0,
			(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
		if (work == INVALID_HANDLE_VALUE)
		{
			cerr << "Connection with the named pipe failed." << endl
				<< "The last error code: " << GetLastError() << endl;
			return 0;
		}

		if (!WriteFile(work, names, 52, &amountOfBytes, (LPOVERLAPPED)NULL))
		{
			// ошибка записи
			cerr << "Writing to the named pipe failed: " << endl
				<< "The last error code: " << GetLastError() << endl;
			return 0;
		}
		cout << "Names are written, amountOfBytes " << amountOfBytes << endl;
		PulseEvent(settingDisconnect);

}


DWORD WINAPI Server(LPVOID t)
{

	cout << "Введите интервал, с которым будут выводиться имена потоков на экран" << endl;
	int interThreadInterval;
	cin >> interThreadInterval;


	/*	MySpace s;
		StartReadingEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"startReadingEvent");
		s.ClearUserID();
		s.ClearHelpUserID();
		while (true)
		{
			WaitForSingleObject(StartReadingEvent, INFINITE);
			vector<char> childIDs = s.ReadUsersID();
			cout << endl;
			for (int i = 0; i < childIDs.size(); i++)
				cout << " " << childIDs[i];
			PulseEvent(StartReadingEvent);
		}*/

	return 0;
}

DWORD WINAPI Client(LPVOID t)
{
	cout << "Новый поток создан" << endl;
	DWORD amountOfBytes;
	char nameOfThread;
	//vector<char> names;
	char names[52];
	FileWorker workWithFiles;

	/*HANDLE getNameMutex= OpenMutex(MUTEX_ALL_ACCESS, FALSE, L"Mutex");
	WaitForSingleObject(getNameMutex, INFINITE);*/
	HANDLE settingDisconnect = OpenEvent(EVENT_ALL_ACCESS, FALSE, L"SettingDisconnect");
	WaitNamedPipe(L"\\\\.\\pipe\\$MyPipe$", NMPWAIT_WAIT_FOREVER);
	HANDLE workWithNames = CreateFile(L"\\\\.\\pipe\\$MyPipe$", GENERIC_READ | GENERIC_WRITE, 0,
		(LPSECURITY_ATTRIBUTES)NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, (HANDLE)NULL);
	if (workWithNames == INVALID_HANDLE_VALUE)
	{
		cerr << "Connection with the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		return 0;
	}
	else cout << "Соединение корректно" << endl;


	BOOL correct = ReadFile(workWithNames, names, 52, &amountOfBytes, NULL);
	if (!correct) cout << "Vse Ploho" << endl;
	cout << "Names are read" << endl;
	for (int i = 0; i < 52; i++)
		cout << names[i] << " "; 


	nameOfThread = names[0];
	//names.erase(names.begin());
	cout << "Имя потока" << nameOfThread << endl;
	Sleep(5000);
	PulseEvent(settingDisconnect);
	//ReleaseMutex(getNameMutex);
	return 0;
}
