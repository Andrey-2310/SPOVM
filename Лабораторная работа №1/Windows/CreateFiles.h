#pragma once
#pragma once
//
//  CreateFiles.hpp
//  C++
//
//  Created by ���� on 04.03.17.
//  Copyright � 2017 IvanCode. All rights reserved.
//

#ifndef CreateFiles_hpp
#define CreateFiles_hpp

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "Product.h"

using namespace std;

class MySpace {
public:
	void createFiles() {
		Product cola(10, 2, "���� ����");
		Product cashew(6, 4, "�����");
		Product water(12, 1, "����");
		Product snickers(8, 2, "�������");
		Product fanta(9, 2, "�����");

		vector<Product> products;
		products.push_back(cola);
		products.push_back(cashew);
		products.push_back(water);
		products.push_back(snickers);
		products.push_back(fanta);

		fstream productsF;
		productsF.open("CP.txt", ios::trunc | ios::in | ios::out | ios::binary);
		if (!productsF.is_open()) {
			cout << "���� �� ������ (choose product)!" << endl;
			return;
		}

		productsF.seekp(0, ios_base::beg);
		for (int i = 0; i < products.size(); i++) {
			productsF.write(reinterpret_cast<char*>(&products[i]), sizeof(Product));
		}

		productsF.close();

		fstream moneyBill;
		moneyBill.open("Bill.txt", ios::trunc | ios::in | ios::out | ios::binary);
		if (!moneyBill.is_open()) {
			cout << "���� �� ������ (bill)!" << endl;
			return;
		}

		int money = 20;

		moneyBill.seekp(0, ios_base::beg);
		moneyBill.write(reinterpret_cast<char*>(&money), sizeof(int));

		moneyBill.close();
	}

	void askManToPutMoney() {
		int amountToPut, money;

		fstream moneyBill;
		moneyBill.open("Bill.txt", ios::in | ios::out | ios::binary);
		if (!moneyBill.is_open()) {
			cout << "���� �� ������ (bill)!" << endl;
			return;
		}

		moneyBill.seekg(0, ios_base::beg);
		moneyBill.read(reinterpret_cast<char*>(&money), sizeof(int));

		cout << "� ��� ����� " << money << " ������." << endl;
		cout << "�������� ������� ����� (��������, 5 ������):" << endl;
		while (1) {
			cin >> amountToPut;
			if (amountToPut > money) {
				cout << "� ��� �� ������� ����� �� �����. ��� ���� - " << money << " ������. ��������� �����:" << endl;
			}
			else {
				money -= amountToPut;
				break;
			}
		}

		moneyBill.seekp(0, ios_base::beg);
		moneyBill.write(reinterpret_cast<char*>(&money), sizeof(int));

		moneyBill.seekp(0 + sizeof(int), ios_base::beg);
		moneyBill.write(reinterpret_cast<char*>(&amountToPut), sizeof(int));

		moneyBill.close();

	}

	void showAmountOfMoneyInVendingMachine() {
		int amountToPut, money;

		fstream moneyBill;
		moneyBill.open("Bill.txt", ios::in | ios::out | ios::binary);
		if (!moneyBill.is_open()) {
			cout << "���� �� ������ (bill)!" << endl;
			return;
		}

		moneyBill.seekg(0, ios_base::beg);
		moneyBill.read(reinterpret_cast<char*>(&money), sizeof(int));

		moneyBill.seekg(0 + sizeof(int), ios_base::beg);
		moneyBill.read(reinterpret_cast<char*>(&amountToPut), sizeof(int));

		cout << "�� �������� " << amountToPut << " ������." << endl;
	}

	void showProducts() {
		cout << endl << "������� � ��� � �������: " << endl;

		fstream productsF;
		productsF.open("CP.txt", ios::in | ios::out | ios::binary);
		if (!productsF.is_open()) {
			cout << "���� �� ������ (choose product)!" << endl;
			return;
		}

		Product p;

		productsF.seekg(0, ios_base::end);

		int s = (int)productsF.tellg() / sizeof(Product);


		for (int i = 0; i < s; i++) {
			productsF.seekg(sizeof(Product) * i, ios_base::beg);
			productsF.read(reinterpret_cast<char*>(&p), sizeof(Product));
			cout << i << ") ";
			p.showProductNameAmountPrice();
		}

		//productsF.close();
	}

	void askManToEnterProducts() {
		fstream order;
		order.open("Order.txt", ios::trunc | ios::in | ios::out | ios::binary);
		if (!order.is_open()) {
			cout << "���� �� ������ (order)!" << endl;
			return;
		}
		order.seekp(0, ios_base::beg);
		int am;

		fstream productsF;
		productsF.open("CP.txt", ios::in | ios::out | ios::binary);
		if (!productsF.is_open()) {
			cout << "���� �� ������ (choose product)!" << endl;
			return;
		}

		Product p;

		productsF.seekg(0, ios_base::end);

		int s = (int)productsF.tellg() / sizeof(Product);

		cout << endl << "�����������: " << endl;

		for (int i = 0; i < s; i++) {
			productsF.seekg(sizeof(Product) * i, ios_base::beg);
			productsF.read(reinterpret_cast<char*>(&p), sizeof(Product));
			cout << "������� ���������� ������ ";
			p.showName();
			cout << ", ������� �� ������ ������: ";
			cin >> am;
			if (am < 0) {
				cout << "����� ����. � � ���� �� ����. ����� ���������� ����� ��������� ����." << endl;
				am = 0;
			}
			order.seekp(i * sizeof(int), ios_base::beg);
			order.write(reinterpret_cast<char*>(&am), sizeof(int));
		}

		productsF.close();
		order.close();
	}

	void performOrder() {
		fstream order;
		order.open("Order.txt", ios::in | ios::out | ios::binary);
		if (!order.is_open()) {
			cout << "���� �� ������ (order)!" << endl;
			return;
		}

		order.seekg(0, ios_base::end);
		int amount = (int)order.tellg() / sizeof(int);
		int n;

		fstream productsF;
		productsF.open("CP.txt", ios::in | ios::out | ios::binary);
		if (!productsF.is_open()) {
			cout << "���� �� ������ (choose product)!" << endl;
			return;
		}

		Product p;

		int amountToPut, money;

		fstream moneyBill;
		moneyBill.open("Bill.txt", ios::in | ios::out | ios::binary);
		if (!moneyBill.is_open()) {
			cout << "���� �� ������ (bill)!" << endl;
			return;
		}

		moneyBill.seekg(0, ios_base::beg);
		moneyBill.read(reinterpret_cast<char*>(&money), sizeof(int));

		moneyBill.seekg(0 + sizeof(int), ios_base::beg);
		moneyBill.read(reinterpret_cast<char*>(&amountToPut), sizeof(int));

		fstream manBought;
		manBought.open("ManBought.txt", ios::trunc | ios::in | ios::out | ios::binary);
		if (!manBought.is_open()) {
			cout << "���� �� ������ (order)!" << endl;
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
				cout << "� ������� ������ " << p.getAmount() << " ";
				p.showName();
				cout << endl << "���� �� �������� �� ���� ���������� ����������, �� � ��� ����� ����������." << endl;
				n = p.getAmount();
			}

			if (n * p.getPrice() > amountToPut) {
				cout << "���, � ��� �� ������� " << (n * p.getPrice() - amountToPut) << " ������." << endl;
				n = amountToPut / p.getPrice();
				cout << "� ��� ����� " << n << " ";
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

		//������� ��������� � ���� � ��������
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

	void showPurchases() {
		fstream manBought;
		manBought.open("ManBought.txt", ios::in | ios::out | ios::binary);
		if (!manBought.is_open()) {
			cout << "���� �� ������ (order)!" << endl;
			return;
		}
		manBought.seekg(0, ios_base::end);
		int size = (int)manBought.tellg() / sizeof(Product);

		fstream moneyBill;
		moneyBill.open("Bill.txt", ios::in | ios::out | ios::binary);
		if (!moneyBill.is_open()) {
			cout << "���� �� ������ (bill)!" << endl;
			return;
		}
		int money;

		moneyBill.seekg(0, ios_base::beg);
		moneyBill.read(reinterpret_cast<char*>(&money), sizeof(int));

		cout << "�� ������: " << endl;
		for (int i = 0; i < size; i++) {
			manBought.seekg(i * sizeof(Product), ios_base::beg);
			Product p;
			manBought.read(reinterpret_cast<char*>(&p), sizeof(Product));

			cout << p.getAmount() << " ";
			p.showName();
			cout << endl;
		}

		cout << "� ��� �������� " << money << " �." << endl;

		manBought.close();
		moneyBill.close();
	}
};

#endif /* CreateFiles_hpp */