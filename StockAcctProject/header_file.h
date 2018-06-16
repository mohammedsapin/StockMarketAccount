//============================================================================
// Course Number and section: 14:332:254:XX
// Submitted by: MOHAMMMED SAPIN, 170006256
//============================================================================
#ifndef HEADER_FILE_H_
#define HEADER_FILE_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <cctype>
#include <iomanip>

using namespace std;

class Account {
private:
	float cashbalance;
public:
	Account() {
		cashbalance = 0;
	}
	Account(float amount);
	float getCashBalance();
	void setCashBalance(float amount);
};

class BankAccount: public Account {
public:
	BankAccount() :
			Account() {
	}
	;
	BankAccount(float amount) :
			Account(amount) {
	}
	;
	float deposit(float amount);
	bool withdraw(float amount);
};

class Stock {
private:
	string symbol;
	int quantity;
public:
	Stock() {
		symbol = "";
		quantity = 0;
	}
	Stock(string sym, int qty);
	int getQuantity();
	void setQuantity(int quantity);
	string getSymbol();
	void setSymbol(string symbol);
};

class StockAccount: public Account {
private:
	BankAccount bankAccount;
	vector<Stock> stocksList;
	ifstream priceFiles[4];
	int numFiles;
	float searchFile(ifstream &file, string symbol, string &name);
	void addStock(string symbol, int quantity);
	bool removeStock(string symbol);
	int findStock(string symbol);
	bool hasStock(string symbol);
	void initPriceFiles(string filenames[], int n);
	void setCashBalance(float amount);
public:
	StockAccount(float amount, string priceFileNames[], int n);
	void displayPortfolio();
	bool buyStock(string symbol, float buyPrice, int quantity);
	bool sellStock(string symbol, float sellPrice, int quantity);
	BankAccount getBankAccount();
	float getCurrentPrice(string symbol, string &name);
	void closeFiles();
	bool withdraw(float amount);
	void deposit(float amount);
};
#endif /* HEADER_FILE_H_ */
