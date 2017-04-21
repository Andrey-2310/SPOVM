#include<iostream>
#include<windows.h>
#include<vector>
#include<time.h>
#include<conio.h>
using namespace std;
//vector<char> threadsNames;
vector<HANDLE> threads;
DWORD WINAPI SubThread(LPVOID);

struct infoForSubThread {
	int timeToOutput;
    int numberOfThread;
};
infoForSubThread *threadsinfo;
int Counter = 0;
int main()
{
	threadsinfo = (struct infoForSubThread*)malloc(sizeof(struct infoForSubThread)*Counter);
	HANDLE semaphore = CreateSemaphore(NULL, 1, 1, L"OutputName");
	setlocale(LC_ALL, "Russian");
	cout << "Введите интервал, с которым потоки будут автоматически создаваться" << endl;
	int timeToCreateNewThread;
	cin >> timeToCreateNewThread;

	cout << "Введите интервал, с которым будут выводиться имена потоков на экран" << endl;
	int interThreadInterval;
	cin >> interThreadInterval;
	cout << "Для создания нового потока нажмите +" << endl;
	cout << "Для удаления потока нажмите -" << endl;
	cout << "Для выхода из программы нажмите Esc" << endl;
	cin.ignore(cin.rdbuf()->in_avail());
	int curTime = clock();
	int finTime;
	while (true) {
		infoForSubThread info;
		finTime = clock();
		if (finTime - curTime > timeToCreateNewThread) {	//создание нового потока
			Counter++;
			threadsinfo=(struct infoForSubThread*)realloc(threadsinfo, Counter*sizeof(struct infoForSubThread));
			threadsinfo[Counter - 1].numberOfThread = Counter;
			threadsinfo[Counter - 1].timeToOutput = interThreadInterval;
			
			threads.push_back(CreateThread(NULL, 0, &SubThread, &(threadsinfo[Counter-1]), 0, NULL));
			curTime = clock();
		}
		if (_kbhit()) {
			char ch = _getch();
			//int num = threads.size() + 1;
			switch (ch) {
			case '+':
				cout << endl;
				Counter++;
				threadsinfo = (struct infoForSubThread*)realloc(threadsinfo, Counter*sizeof(infoForSubThread));
				threadsinfo[Counter - 1].numberOfThread = Counter;
				threadsinfo[Counter - 1].timeToOutput = interThreadInterval;
				threads.push_back(CreateThread(NULL, 0, &SubThread, &(threadsinfo[Counter - 1]), 0, NULL));
				break;
			case '-':
				if (Counter > 1) {
					cout << endl;
					TerminateThread(threads[--Counter], 0);//остановка работы потока
					threadsinfo = (struct infoForSubThread*)realloc(threadsinfo, Counter*sizeof(struct infoForSubThread));
					threads.pop_back();
				}
				break;
			case 27:
				cout << "Dosviduli" << endl;
				return 0;

				//	default: cin.ignore(cin.rdbuf()->in_avail());
			}
		}
	}
	return 0;
}

DWORD WINAPI SubThread(LPVOID t) {
	HANDLE semaphore = OpenSemaphore(SEMAPHORE_ALL_ACCESS, FALSE, L"OutputName");
	infoForSubThread temp = (*(infoForSubThread *)t);
	while (true) {
		WaitForSingleObject(semaphore, INFINITE);
		if (temp.numberOfThread == 1)
			Sleep(temp.timeToOutput);
		cout << temp.numberOfThread << " ";
		if (temp.numberOfThread == Counter) cout << endl;
		ReleaseSemaphore(semaphore, 1, NULL);
	}
}