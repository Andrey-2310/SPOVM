#include <windows.h>
#include <windowsx.h>
#include <iostream>
using namespace std;
void* memoryAllocation(int size);
void* myMalloc(int size);
void  myFree(void* ptr);
void* myRealloc(void* ptr, int newSize);

int main() {
	char* tmp;
	tmp = (char*)myMalloc(20);
	cin >> tmp;
	cout << tmp << endl;
	//myFree(tmp);

	tmp = (char*)myRealloc(tmp, 30);
//	cin >> tmp;
	cout << tmp << endl;
	myFree(tmp);

	return 0;
}


void* myMalloc(int size) {
	// ������� ����� � ���������� ����������

	void*  pLocal;
	UINT   uMaxFreeMem;

	pLocal = memoryAllocation(size);

	if (pLocal == NULL) {
		// ������� �������������� ������ � �� ���������� ��������� (���������� ������ ����������� ����� ������)
		uMaxFreeMem = GlobalCompact(size);
		std::cout << "Trying to defragmentate memory" << std::endl;
		// ����� �������� �������� ������
		pLocal = memoryAllocation(size);
		if (pLocal == NULL) {
			std::cout << "Not enough memory" << std::endl;
			return nullptr;
		}
		else {
			return nullptr;
		}
	}
	return pLocal;
}

void* memoryAllocation(int size) {
	HLOCAL hmemLocal;
	void*  pLocal;

	// ��������� ����������� ���������� ������ (GHND - �������� "���������" (���������� ����� ����� ������������) � ��������� ���� � ��������� � ������)
	hmemLocal = GlobalAlloc(GHND, size + sizeof(HLOCAL));

	if (hmemLocal != NULL) {
		// ���� ����� �������, ��������� ��� � ������ (����� ��� ��������� ������� � ������)
		pLocal = GlobalLock(hmemLocal);
		if (pLocal != NULL) {
			// ������ ����������� � ������ ���������� ������
			pLocal = hmemLocal;
			// ��������� ������ ������ �� ��������� ����� ����������� ����
			pLocal = (char*)pLocal + sizeof(HLOCAL);
			return pLocal;
		}
		else {
			std::cout << std::endl << "Error in locking block";
		}
	}
	else {
		std::cout << std::endl << "Not enough memory";
	}
	return nullptr;
}

void  myFree(void* ptr) {
	HLOCAL hmemLocal;
	// ��������� ����������� ������
	hmemLocal = (char*)ptr - sizeof(HLOCAL);
	// ������ hmemLocal
	GlobalUnlock(hmemLocal);
	// ���������� NULL ��� �������� ������������ ������
	if (GlobalFree(hmemLocal) != NULL) {
		std::cout << std::endl << "Error in making memory free";
	}
	else {
		std::cout << std::endl << "Memory is free";
	}
	ptr = nullptr;
}

void* myRealloc(void* ptr, int newSize) {
/*	int size = 0;
	for (size = 0; ((char*)ptr)[size] != '\0'; size++);
	char *buffer = (char*)myMalloc(size*sizeof(char));
	strcpy(buffer, (char*)ptr);*/
	// �������� ������������� ����� ������
	HLOCAL hmemLoDiscard = (char*)ptr - sizeof(HLOCAL);
	// ��������������� ���� ������ � ��������������� hmemLoDiscard, ������ ����� ��� ����� ������
	hmemLoDiscard = GlobalReAlloc(hmemLoDiscard, newSize, NULL);

	// � ������ ������ ���������� nullptr
	if (hmemLoDiscard == NULL) {
		std::cout << std::endl << "Error in locking block";
		return nullptr;
	}

	// �������� ��������� �� ������ ���������� ������
	void* startMem = (char*)ptr - sizeof(HLOCAL);
	// "������" ������ � ���������� � ������ ���������� ������ ����� ���������� 
	startMem = GlobalLock(hmemLoDiscard);

	if (startMem == NULL) {
		std::cout << std::endl << "Error in locking block";
		std::cout << std::endl << GetLastError();

	}

	// 
	startMem = hmemLoDiscard;
	// ���������� ��������� �� ��������� ����� ����������� ����
	return (char*)startMem + sizeof(HLOCAL);
}