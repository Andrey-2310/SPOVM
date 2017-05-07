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
	// находит место и возвращает дескриптор

	void*  pLocal;
	UINT   uMaxFreeMem;

	pLocal = memoryAllocation(size);

	if (pLocal == NULL) {
		// попытка дефрагментации памяти и ее повторного выделения (возвращает размер наибольшего блока памяти)
		uMaxFreeMem = GlobalCompact(size);
		std::cout << "Trying to defragmentate memory" << std::endl;
		// снова пытаемся выделить память
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

	// получение дескриптора выделяемой памяти (GHND - выделяет "двигаемую" (логический адрес может перемещаться) в локальной куче и заполняет её нулями)
	hmemLocal = GlobalAlloc(GHND, size + sizeof(HLOCAL));

	if (hmemLocal != NULL) {
		// Если буфер получен, фиксируем его в памяти (нужно для получения доступа к памяти)
		pLocal = GlobalLock(hmemLocal);
		if (pLocal != NULL) {
			// Запись дескриптора в начало выделенной памяти
			pLocal = hmemLocal;
			// установка начала памяти на следующий после дескриптора байт
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
	// получение дескриптора памяти
	hmemLocal = (char*)ptr - sizeof(HLOCAL);
	// разлок hmemLocal
	GlobalUnlock(hmemLocal);
	// возвращает NULL при успешном освобождении памяти
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
	// получили идентификатор блока памяти
	HLOCAL hmemLoDiscard = (char*)ptr - sizeof(HLOCAL);
	// восстанавливаем блок памяти с идентификатором hmemLoDiscard, причём задаём ему новый размер
	hmemLoDiscard = GlobalReAlloc(hmemLoDiscard, newSize, NULL);

	// в случае ошибки возвращаем nullptr
	if (hmemLoDiscard == NULL) {
		std::cout << std::endl << "Error in locking block";
		return nullptr;
	}

	// получаем указатель на старый дескриптор памяти
	void* startMem = (char*)ptr - sizeof(HLOCAL);
	// "локаем" память и записываем в начало выделенной памяти новый дескриптор 
	startMem = GlobalLock(hmemLoDiscard);

	if (startMem == NULL) {
		std::cout << std::endl << "Error in locking block";
		std::cout << std::endl << GetLastError();

	}

	// 
	startMem = hmemLoDiscard;
	// возвращаем указатель на следующий после дескриптора байт
	return (char*)startMem + sizeof(HLOCAL);
}