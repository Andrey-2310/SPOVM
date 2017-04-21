#pragma once
#pragma once
//
//  Product.hpp
//  C++
//
//  Created by Иван on 04.03.17.
//  Copyright © 2017 IvanCode. All rights reserved.
//

#ifndef Product_hpp
#define Product_hpp

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <fstream>
#include<string>

using namespace std;

class Product {
private:
	int amount;
	int price;
	char name[20];

public:
	Product() {
		amount = 0;
		price = 0;

		strcpy_s(name, "Nothing");
	}
	Product(int amount, int price, char* name) {
		this->amount = amount;
		this->price = price;

		strcpy_s(this->name, name);
	}

	void showProductNameAmountPrice() {
		cout << name << ", " << amount << " ? "<< price << " р." << endl;
	}

	void showName() {
		cout << name;
	}

	int getAmount() {
		return amount;
	}

	int getPrice() {
		return price;
	}

	void changeAmount(int newAmount) {
		amount = newAmount;
	}

	char* getName() {
		return name;
	}

};

#endif /* Product_hpp */