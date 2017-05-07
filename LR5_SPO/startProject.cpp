//#define _CRT_SECURE_NO_WARNINGS	
#pragma warning(disable:4996)
#include "project.h"

void main() {
	Project project5;
	project5.START();
}

Project::Project(char *directory, char *outputFileName)
{

	strcpy(this->directory, directory);

	this->outputFileName = new char[sizeof(outputFileName)];
	strcpy(this->outputFileName, outputFileName);
}

void Project::START()
{
	HINSTANCE hLib = LoadLibrary("DLL_for_LR5.dll");
	if (!hLib)
	{
		cout << GetLastError() << endl;
		cout << "To work we need MyDll.dll!\n";
		exit(0);
	}

	LPFNDLLinit init = (LPFNDLLinit)GetProcAddress(hLib, "init");
	HANDLE WriteOffEvent = init(directory, outputFileName);

	if (WriteOffEvent == 0)
	{
		cout << "Error of input information!\n";
		exit(0);
	}

	LPTHREAD_START_ROUTINE  reader = (LPTHREAD_START_ROUTINE)GetProcAddress(hLib, "ThreadReader");
	LPTHREAD_START_ROUTINE  writer = (LPTHREAD_START_ROUTINE)GetProcAddress(hLib, "ThreadWriter");


	HANDLE threadReader = CreateThread(NULL, 0, reader, NULL, 0, 0);
	HANDLE threadWriter = CreateThread(NULL, 0, writer, NULL, 0, 0);

	WaitForSingleObject(threadReader, INFINITE);
	WaitForSingleObject(WriteOffEvent, INFINITE);

	TerminateThread(threadWriter, 0);

	cout << "Combining completed successfully!\n";
	FreeLibrary(hLib);
	exit(0);
}

