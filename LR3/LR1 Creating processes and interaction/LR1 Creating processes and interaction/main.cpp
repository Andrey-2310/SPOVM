//
//  main.cpp
//  C++
//
//  Created by ���� on 01.07.16.
//  Copyright � 2016 IvanCode. All rights reserved.
//

/*#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include "Product.h"
#include "CreateFiles.h"

using namespace std;

MySpace s;

int main2(void) {
	//������ ����� ����������, ���� ��� ��� �� �������
	cout << "�� ������ ������� ����� � �������� � ���������� (y/n)?" << endl;
	char c;
	cin >> c;
	if (c == 'y') {
		s.createFiles();
	}

	//� ����� ��� ��� ������ � ����������
	pid_t process;
	process = fork();
	if (process == 0) {
		//�� � �������� ��������
		s.askManToPutMoney();

		return EXIT_SUCCESS;
	}
	else if (process < 0) {
		fprintf(stderr, "Fork failed.\n");
		return EXIT_FAILURE;
	}
	//������������ �������
	waitpid(process, NULL, NULL);
	s.showAmountOfMoneyInVendingMachine();
	s.showProducts();

	process = fork();

	if (process == 0) {
		//�� ����� � �������� ��������, ������ ��� � ����� ��������
		s.askManToEnterProducts();

		return EXIT_SUCCESS;
	}
	else if (process < 0) {
		fprintf(stderr, "Fork failed.\n");
		return EXIT_FAILURE;
	}
	waitpid(process, NULL, NULL);
	s.performOrder();

	process = fork();

	if (process == 0) {
		//�� ����� � �������� ��������, �� ����� �� ������� ���������� � ���������, ������� ���� ������� �������, � ����� ���������� ������
		s.showPurchases();

		return EXIT_SUCCESS;
	}
	else if (process < 0) {
		fprintf(stderr, "Fork failed.\n");
		return EXIT_FAILURE;
	}


	return 0;
}*/