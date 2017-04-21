#include <windows.h>
#include <iostream>
using namespace std;
DWORD WINAPI main5(LPVOID);

int main3()
{
	char c; // ��������� ������
	SECURITY_ATTRIBUTES sa; // �������� ������
	SECURITY_DESCRIPTOR sd; // ���������� ������
	HANDLE hNamedPipe;
	char lpszInMessage[80]; // ��� ��������� �� �������
	DWORD dwBytesRead; // ��� ����� ����������� ������
	char lpszOutMessage[] = "The server has received a message."; // �������� ���������
	DWORD dwBytesWrite; // ��� ����� ���������� ������
						// ������������� ��������� ������
	sa.nLength = sizeof(sa);
	sa.bInheritHandle = FALSE; // ���������� ������ �������������
							   // �������������� ���������� ������
	InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
	// ������������� �������� ������, �������� ������ ���� �������������
	SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
	sa.lpSecurityDescriptor = &sd;
	// ������� ����������� ����� ��� ������
	hNamedPipe = CreateNamedPipe(
		L"\\\\.\\pipe\\demo_pipe", // ��� ������
		PIPE_ACCESS_DUPLEX, // ������ �� ������ � ����� � �����
		PIPE_TYPE_MESSAGE | PIPE_WAIT, // ���������� �������� ���������
		1, // ������������ ���������� ����������� ������
		0, // ������ ��������� ������ �� ���������
		0, // ������ �������� ������ �� ���������
		INFINITE, // ������ ���� ����� 500 ��
		&sa // ������ ��� ���� �������������
		);
	// ��������� �� �������� ��������
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Creation of the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish server: ";
		cin >> c;
		return 0;
	}
	CreateThread(NULL, 0, &main5, NULL, 0, NULL);
	// ����, ���� ������ �������� � �������
	cout << "The server is waiting for connection with a client." << endl;
	if (!ConnectNamedPipe(
		hNamedPipe, // ���������� ������
		(LPOVERLAPPED)NULL // ����� ����������
		))
	{
		cerr << "The connection failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
		cin >> c;
		return 0;
	}
	// ������ ��������� �� �������
	if (!ReadFile(
		hNamedPipe, // ���������� ������
		lpszInMessage, // ����� ������ ��� ����� ������
		sizeof(lpszInMessage), // ����� �������� ������
		&dwBytesRead, // ����� ����������� ������
		(LPOVERLAPPED)NULL // �������� ������ ����������
		))
	{
		cerr << "Data reading from the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
		cin >> c;
		return 0;
	}
	// ������� ���������� �� ������� ��������� �� �������
	cout << "The server has receivrd the following message from a client: "
		<< endl << "\t" << lpszInMessage << endl;
	// �������� �������
	if (!WriteFile(
		hNamedPipe, // ���������� ������
		lpszOutMessage, // ����� ������ ��� ������ ������
		sizeof(lpszOutMessage), // ����� ������������ ������
		&dwBytesWrite, // ����� ���������� ������
		(LPOVERLAPPED)NULL // �������� ������ ����������
		))
	{
		cerr << "Data writing to the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		CloseHandle(hNamedPipe);
		cout << "Press any char to finish the server: ";
			cin >> c;
		return 0;
	}
	// ������� ��������� ������� ��������� �� �������
	cout << "The server send the following message to a client: "
		<< endl << "\t" << lpszOutMessage << endl;
	Sleep(10000);
	// ��������� ���������� ������
	CloseHandle(hNamedPipe);
	// ��������� �������
	cout << "Press any char to finish the server: ";
	cin >> c;
	return 0;
}

DWORD WINAPI main5(LPVOID)
{
	char c; // ��������� ������
	HANDLE hNamedPipe;
	char machineName[80];
	char pipeName[80];
	char lpszOutMessage[] = "How do you do server?"; // ��������� �������
	DWORD dwBytesWritten; // ��� ����� ���������� ������
	char lpszInMessage[80]; // ��� ��������� �� �������
	DWORD dwBytesRead; // ��� ����� ����������� ������

					   // ������ ��� ������ � ����, �� ������� �������� ������
	cout << "Enter a name of the server machine: ";
	cin >> machineName;
	// ����������� ��� ������ � ��� ������
	/*wsprintf(pipeName, L"\\\\%s\\pipe\\demo_pipe",
		machineName);*/

	// ����������� � ����������� �������
	hNamedPipe = CreateFile(
		L"\\\\.\\pipe\\demo_pipe", // ��� ������
		GENERIC_READ | GENERIC_WRITE, // ������ � ���������� � �����
		FILE_SHARE_READ | FILE_SHARE_WRITE, // ��������� ������ � ������ � �����
		(LPSECURITY_ATTRIBUTES)NULL, // ������ �� ���������
		OPEN_EXISTING, // ��������� ������������ �����
		FILE_ATTRIBUTE_NORMAL, // �������� �� ���������
		(HANDLE)NULL // �������������� ��������� ���
		);

	// ��������� ����� � �������
	if (hNamedPipe == INVALID_HANDLE_VALUE)
	{
		cerr << "Connection with the named pipe failed." << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		cin >> c;
		return 0;
	}
	// ����� � ����������� �����
	if (!WriteFile(
		hNamedPipe, // ���������� ������
		lpszOutMessage, // ������
		sizeof(lpszOutMessage), // ������ ������
		&dwBytesWritten, // ���������� ���������� ������
		(LPOVERLAPPED)NULL // ���������� ������
		))
	{
		// ������ ������
		cerr << "Writing to the named pipe failed: " << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		cin >> c;
		CloseHandle(hNamedPipe);
		return 0;
	}
	// ������� ��������� ��������� �� �������
	cout << "The client has send the following message to a server: "
		<< endl << "\t" << lpszOutMessage << endl;
	// ������ �� ������������ ������
	if (!ReadFile(
		hNamedPipe, // ���������� ������
		lpszInMessage, // ������
		sizeof(lpszInMessage), // ������ ������
		&dwBytesRead, // ���������� ���������� ����
		(LPOVERLAPPED)NULL // ���������� ������
		))
	{
		// ������ ������
		cerr << "Reading to the named pipe failed: " << endl
			<< "The last error code: " << GetLastError() << endl;
		cout << "Press any char to finish the client: ";
		cin >> c;
		CloseHandle(hNamedPipe);
		return 0;
	}
	// ������� ���������� ��������� �� �������
	cout << "The client has received the following message from a server: "
		<< endl << "\t" << lpszInMessage << endl;
	// ��������� ���������� ������
	CloseHandle(hNamedPipe);
	// ��������� �������
	cout << "Press any char to finish the client: ";
	cin >> c;
	return 0;
}