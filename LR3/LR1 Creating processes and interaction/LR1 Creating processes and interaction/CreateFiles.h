#pragma once



#define CreateFiles_hpp

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include<list>
#include <fstream>

#include "Product.h"

using namespace std;

class MySpace {
public:
	MySpace() {};


	void showAmountOfMoneyInVendingMachine() {
		int amountToPut, money;

		fstream moneyBill;
		moneyBill.open("Bill.txt", ios::in | ios::out | ios::binary);
		if (!moneyBill.is_open()) {
			cout << "Файл не открыт (bill)!" << endl;
			return;
		}

		moneyBill.seekg(0, ios::beg);
		moneyBill.read(reinterpret_cast<char*>(&money), sizeof(int));

		moneyBill.seekg(0 + sizeof(int), ios::beg);
		moneyBill.read(reinterpret_cast<char*>(&amountToPut), sizeof(int));

		cout << endl << "Вы положили " << amountToPut << " рублей." << endl;
		moneyBill.close();
	}

	void showProducts() {
		cout << endl << "Сегодня у нас в продаже: " << endl;

		fstream productsF;
		productsF.open("CP.txt", ios::in | ios::binary);
		if (!productsF.is_open()) {
			cout << "Файл не открыт (choose product)!" << endl;
			return;
		}

		Product p;

		productsF.seekg(0, ios::end);

		int s = (int)productsF.tellg() / sizeof(Product);


		for (int i = 0; i < s; i++) {
			productsF.seekg(sizeof(Product) * i, ios::beg);
			productsF.read(reinterpret_cast<char*>(&p), sizeof(Product));
			cout << i << ") ";
			p.showProductNameAmountPrice();
		}

		productsF.clear();

		productsF.close();
		return;
	}




	void askManToEnterProducts() {
		fstream order;
		order.open("Order.txt", ios::trunc | ios::in | ios::out | ios::binary);
		if (!order.is_open()) {
			cout << "Файл не открыт (order)!" << endl;
			return;
		}
		order.seekp(0, ios::beg);
		int am;

		fstream productsF;
		productsF.open("CP.txt", ios::in | ios::out | ios::binary);
		if (!productsF.is_open()) {
			cout << "Файл не открыт (choose product)!" << endl;
			return;
		}

		Product p;

		productsF.seekg(0, ios::end);

		int s = (int)productsF.tellg() / sizeof(Product);

		cout << endl << "Заказывайте: " << endl;

		for (int i = 0; i < s; i++) {
			productsF.seekg(sizeof(Product) * i, ios::beg);
			productsF.read(reinterpret_cast<char*>(&p), sizeof(Product));
			cout << "Введите количество товара ";
			p.showName();
			cout << ", которое вы хотите купить: ";
			cin >> am;
			if (am < 0) {
				cout << "Умный типо. И я тоже не глуп. Пусть количество будет равняться нулю." << endl;
				am = 0;
			}
			order.seekp(i * sizeof(int), ios::beg);
			order.write(reinterpret_cast<char*>(&am), sizeof(int));
		}

		productsF.close();
		order.close();
	}

	void performOrder() {
		fstream order;
		order.open("Order.txt", ios::in | ios::out | ios::binary);
		if (!order.is_open()) {
			cout << "Файл не открыт (order)!" << endl;
			return;
		}

		order.seekg(0, ios_base::end);
		int amount = (int)order.tellg() / sizeof(int);
		int n;

		fstream productsF;
		productsF.open("CP.txt", ios::in | ios::out | ios::binary);
		if (!productsF.is_open()) {
			cout << "Файл не открыт (choose product)!" << endl;
			return;
		}

		Product p;

		int amountToPut, money;

		fstream moneyBill;
		moneyBill.open("Bill.txt", ios::in | ios::out | ios::binary);
		if (!moneyBill.is_open()) {
			cout << "Файл не открыт (bill)!" << endl;
			return;
		}

		moneyBill.seekg(0, ios_base::beg);
		moneyBill.read(reinterpret_cast<char*>(&money), sizeof(int));

		moneyBill.seekg(0 + sizeof(int), ios_base::beg);
		moneyBill.read(reinterpret_cast<char*>(&amountToPut), sizeof(int));

		fstream manBought;
		manBought.open("ManBought.txt", ios::trunc | ios::in | ios::out | ios::binary);
		if (!manBought.is_open()) {
			cout << "Файл не открыт (order)!" << endl;
			return;
		}
		manBought.seekp(0, ios_base::beg);

		int mBi = 0;

		for (int i = 0; i < amount; i++) {
			productsF.seekg(sizeof(Product) * i, ios_base::beg);
			productsF.read(reinterpret_cast<char*>(&p), sizeof(Product));

			order.seekg(i * sizeof(int), ios_base::beg);
			order.read(reinterpret_cast<char*>(&n), sizeof(int));

			if (n == 0) {
				continue;
			}

			if (n > p.getAmount()) {
				cout << "В наличии только " << p.getAmount() << " ";
				p.showName();
				cout << endl << "Если Вы положили на счёт достаточно наличных, то я Вам выдам оставшееся." << endl;
				n = p.getAmount();
			}

			if (n * p.getPrice() > amountToPut) {
				cout << "Увы, у Вас не хватает " << (n * p.getPrice() - amountToPut) << " рублей." << endl;
				n = amountToPut / p.getPrice();
				cout << "Я Вам выдам " << n << " ";
				p.showName();
				cout << "." << endl;
			}

			amountToPut -= n * p.getPrice();
			p.changeAmount(p.getAmount() - n);

			productsF.seekp(sizeof(Product) * i, ios_base::beg);
			productsF.write(reinterpret_cast<char*>(&p), sizeof(Product));

			Product inBF(n, p.getPrice(), p.getName());
			manBought.seekp(sizeof(Product) * mBi++, ios_base::beg);
			manBought.write(reinterpret_cast<char*>(&inBF), sizeof(Product));
		}

		//Занесли изменения в файл с деньгами
		if (amountToPut > 0) {
			money += amountToPut;
			moneyBill.seekp(0, ios_base::beg);
			moneyBill.write(reinterpret_cast<char*>(&money), sizeof(int));

			amountToPut = 0;
			moneyBill.seekp(sizeof(int), ios_base::beg);
			moneyBill.write(reinterpret_cast<char*>(&amountToPut), sizeof(int));

			moneyBill.close();
		}

		productsF.close();
		order.close();
		manBought.close();
	}

	

	char OutputAnswer()
	{
		char ch;
		fstream answerFile;
		answerFile.open("answerFile.txt",  ios::in | ios::out | ios::binary);
		if (!answerFile.is_open()) {
			cout << "Файл не открыт (answerFile)!" << endl;
			return NULL;
		}
		answerFile.seekg(0, ios_base::beg);
		answerFile.read(reinterpret_cast<char*>(&ch), sizeof(char));
		return ch;
	}

	vector<char> ReadUsersID()
	{
		vector<char> usersID;
		char ch;
		fstream readIDs;

	
		readIDs.open("usersID.txt", ios::in | ios::binary);
		if (!readIDs.is_open()) {
			cout << "Файл не открыт (usersID.txt)!" << endl;
			return usersID;
		}
		readIDs.seekg(0, ios::end);
		int amountOfUsers = readIDs.tellg()/sizeof(char);
		readIDs.seekg(0, ios::beg);
		do {
			readIDs.read(reinterpret_cast<char*>(&ch), sizeof(char));
			usersID.push_back(ch);
		} while (--amountOfUsers);
		readIDs.clear();
		readIDs.close();
		ClearUserID();
		return usersID;
	}

	void ClearUserID()
	{
		fstream delIDs;
		delIDs.open("usersID.txt", ios::trunc | ios::binary | ios::out);
		delIDs.close();
		//delIDs.open("helpUsersID.txt", ios::trunc | ios::binary | ios::out);
		//delIDs.close();
	}

	void ClearHelpUserID()
	{
		fstream delIDs;
		delIDs.open("helpUsersID.txt", ios::trunc | ios::binary | ios::out);
		delIDs.close();
	}

	void CreateUserNames() {
		char ch = 'a';
		fstream writeID;
		writeID.open("usersNames.txt", ios::trunc | ios::out | ios::binary);
		if (!writeID.is_open()) {
			cout << "Файл не открыт (usersNames)!" << endl;
			return;
		}
		for (int i = 0; i < 26; i++) {
			writeID.write(reinterpret_cast<char*>(&ch), sizeof(char));
			ch++;
		}
		writeID.close();
	}
};

