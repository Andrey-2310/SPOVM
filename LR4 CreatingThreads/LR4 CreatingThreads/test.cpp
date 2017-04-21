#include <windows.h>
#include <iostream>
using namespace std;
DWORD WINAPI main5(LPVOID);

int main3()
{
	char c; // служебный символ
	SECURITY_ATTRIBUTES sa; // атрибуты защиты
	SECURITY_DESCRIPTOR sd; // дескриптор защиты
	HANDLE hNamedPipe;
	char lpszInMessage[80]; // для сообщения от клиента
	DWORD dwBytesRead; // для числа прочитанных байтов
	char lpszOutMessage[] = "The server has received a message."; // обратное сообщение
	DWORD dwBytesWrite; // для числа записанных байтов
						// инициализация атрибутов защиты
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE; // дескриптор канала ненаследуемый
							   // инициализируем дескриптор защиты
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	// устанавливаем атрибуты защиты, разрешая доступ всем пользователям
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;
	// создаем именованный канал для чтения
	hNamedPipe = CreateNamedPipe(
		L"\\\\.\\pipe\\demo_pipe", // имя канала
		PIPE_ACCESS_DUPLEX, // читаем из канала и пишем в канал
		PIPE_TYPE_MESSAGE | PIPE_WAIT, // синхронная передача сообщений
		1, // максимальное количество экземпляров канала
		0, // размер выходного буфера по умолчанию
		0, // размер входного буфера по умолчанию
		INFINITE, // клиент ждет связь 500 мс
		&sa // доступ для всех пользователей
		);
	// проверяем на успешное создание
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Creation of the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish server: ";
		cin >> c;
		return 0;
	}
	CreateThread(NULL, 0, &main5, NULL, 0, NULL);
	// ждем, пока клиент свяжется с каналом
	cout << "The server is waiting for connection with a client." << endl;
	if (!ConnectNamedPipe(
		hNamedPipe, // дескриптор канала
		(LPOVERLAPPED)NULL // связь синхронная
		))
	{
		cerr << "The connection failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
		cin >> c;
		return 0;
	}
	// читаем сообщение от клиента
	if (!ReadFile(
		hNamedPipe, // дескриптор канала
		lpszInMessage, // адрес буфера для ввода данных
		sizeof(lpszInMessage), // число читаемых байтов
		&dwBytesRead, // число прочитанных байтов
		(LPOVERLAPPED)NULL // передача данных синхронная
		))
	{
		cerr << "Data reading from the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
		cin >> c;
		return 0;
	}
	// выводим полученное от клиента сообщение на консоль
	cout << "The server has receivrd the following message from a client: "
		<< endl << "\t" << lpszInMessage << endl;
	// отвечаем клиенту
	if (!WriteFile(
		hNamedPipe, // дескриптор канала
		lpszOutMessage, // адрес буфера для вывода данных
		sizeof(lpszOutMessage), // число записываемых байтов
		&dwBytesWrite, // число записанных байтов
		(LPOVERLAPPED)NULL // передача данных синхронная
		))
	{
		cerr << "Data writing to the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
			cin >> c;
		return 0;
	}
	// выводим посланное клиенту сообщение на консоль
	cout << "The server send the following message to a client: "
		<< endl << "\t" << lpszOutMessage << endl;
	Sleep(10000);
	// закрываем дескриптор канала
	CloseHandle(hNamedPipe);
	// завершаем процесс
	cout << "Press any char to finish the server: ";
	cin >> c;
	return 0;
}

DWORD WINAPI main5(LPVOID)
{
	char c; // служебный символ
	HANDLE hNamedPipe;
	char machineName[80];
	char pipeName[80];
	char lpszOutMessage[] = "How do you do server?"; // сообщение серверу
	DWORD dwBytesWritten; // для числа записанных байтов
	char lpszInMessage[80]; // для сообщения от сервера
	DWORD dwBytesRead; // для числа прочитанных байтов

					   // вводим имя машины в сети, на которой работает сервер
	cout << "Enter a name of the server machine: ";
	cin >> machineName;
	// подставляем имя машины в имя канала
	/*wsprintf(pipeName, L"\\\\%s\\pipe\\demo_pipe",
		machineName);*/

	// связываемся с именованным каналом
	hNamedPipe = CreateFile(
		L"\\\\.\\pipe\\demo_pipe", // имя канала
		GENERIC_READ | GENERIC_WRITE, // читаем и записываем в канал
		FILE_SHARE_READ | FILE_SHARE_WRITE, // разрешаем чтение и запись в канал
		(LPSECURITY_ATTRIBUTES)NULL, // защита по умолчанию
		OPEN_EXISTING, // открываем существующий канал
		FILE_ATTRIBUTE_NORMAL, // атрибуты по умолчанию
		(HANDLE)NULL // дополнительных атрибутов нет
		);

	// проверяем связь с каналом
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Connection with the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		cin >> c;
		return 0;
	}
	// пишем в именованный канал
	if (!WriteFile(
		hNamedPipe, // дескриптор канала
		lpszOutMessage, // данные
		sizeof(lpszOutMessage), // размер данных
		&dwBytesWritten, // количество записанных байтов
		(LPOVERLAPPED)NULL // синхронная запись
		))
	{
		// ошибка записи
		cerr << "Writing to the named pipe failed: " << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		cin >> c;
		CloseHandle(hNamedPipe);
		return 0;
	}
	// выводим посланное сообщение на консоль
	cout << "The client has send the following message to a server: "
		<< endl << "\t" << lpszOutMessage << endl;
	// читаем из именованного канала
	if (!ReadFile(
		hNamedPipe, // дескриптор канала
		lpszInMessage, // данные
		sizeof(lpszInMessage), // размер данных
		&dwBytesRead, // количество записанных байт
		(LPOVERLAPPED)NULL // синхронная запись
		))
	{
		// ошибка записи
		cerr << "Reading to the named pipe failed: " << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		cin >> c;
		CloseHandle(hNamedPipe);
		return 0;
	}
	// выводим полученное сообщение на консоль
	cout << "The client has received the following message from a server: "
		<< endl << "\t" << lpszInMessage << endl;
	// закрываем дескриптор канала
	CloseHandle(hNamedPipe);
	// завершаем процесс
	cout << "Press any char to finish the client: ";
	cin >> c;
	return 0;
}