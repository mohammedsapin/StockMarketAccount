//============================================================================
// Course Number and section: 14:332:254:XX
// Submitted by: MOHAMMMED SAPIN, 170006256
//============================================================================
#include "header_file.h"

Account::Account(float amount) {
	cashbalance = amount;
}

float Account::getCashBalance() {
	return cashbalance;
}

void Account::setCashBalance(float amount) {
	cashbalance = amount;
}

float BankAccount::deposit(float amount) {
	setCashBalance(getCashBalance() + amount);
	return getCashBalance();
}

bool BankAccount::withdraw(float amount) {
	if (amount > getCashBalance())
		return false;
	else {
		setCashBalance(getCashBalance() - amount);
		return true;
	}
}

Stock::Stock(string sym, int qty) {
	symbol = sym;
	quantity = qty;
}

int Stock::getQuantity() {
	return quantity;
}

void Stock::setQuantity(int qty) {
	quantity = qty;
}

string Stock::getSymbol() {
	return symbol;
}

void Stock::setSymbol(string sym) {
	symbol = sym;
}

StockAccount::StockAccount(float initialAmount, string priceFileNames[], int n) :
		Account(initialAmount) {
	bankAccount = BankAccount(initialAmount);
	stocksList = vector<Stock>();
	initPriceFiles(priceFileNames, n);
}

void StockAccount::setCashBalance(float amount) {
	Account::setCashBalance(amount);
	bankAccount.setCashBalance(amount);
}

void StockAccount::displayPortfolio() {
	float price, total, grandTotal = 0;
	string name;
	int qty;
	cout << "\n\tCash balance = $" << fixed << setprecision(2)
			<< getCashBalance();
	cout << "\n\tSymbol\t\tCompany\t\t\t\t\tNumber\t\tPrice\t\tTotal" << endl
			<< endl;
	for (int i = 0; i < stocksList.size(); i++) {
		price = getCurrentPrice(stocksList[i].getSymbol(), name);
		qty = stocksList[i].getQuantity();
		total = price * qty;
		cout << "\t" << stocksList[i].getSymbol() << "\t\t";
		cout.width(30);
		cout << left << name << "\t\t" << qty << "\t\t$" << price << "\t\t$"
				<< total << endl;
		grandTotal += total;
	}
	grandTotal += getCashBalance();
	cout << "\n\n\tTotal portfolio value: $" << grandTotal << endl;
}

bool StockAccount::buyStock(string symbol, float buyPrice, int quantity) {
	string name;
	float price = getCurrentPrice(symbol, name);
	float total = buyPrice * quantity;

	if (name.empty()) {
		cout << "\n\tCannot complete transaction. No such symbol - " << symbol
				<< endl << endl;
		return false;
	} else if (buyPrice < price) {
		cout << "\n\tCannot complete transaction. Buy price $" << buyPrice
				<< " is less than current price $" << price << endl << endl;
		return false;
	} else if (total > getCashBalance()) {
		cout << "\n\tCannot complete transaction. Insufficient cash balance."
				<< endl << endl;
		return false;
	} else {
		addStock(symbol, quantity);
		setCashBalance(getCashBalance() - total);
		return true;
	}
}

bool StockAccount::sellStock(string symbol, float sellPrice, int quantity) {
	int idx = findStock(symbol);
	string name;
	float price;
	if (idx == -1) {
		cout << "\n\tCannot complete transaction. No shares of " << symbol
				<< " in portfolio." << endl << endl;
		return false;
	}
	if (stocksList[idx].getQuantity() < quantity) {
		cout << "\n\tCannot complete transaction. Not enough shares of "
				<< symbol << " in portfolio." << endl << endl;
		return false;
	} else {
		price = getCurrentPrice(symbol, name);
		if (sellPrice > price) {
			cout << "\n\tCannot complete transaction. Sell price $" << sellPrice
					<< " is greater than current price $" << price << endl
					<< endl;
			return false;
		} else {
			float total = sellPrice * quantity;
			stocksList[idx].setQuantity(
					stocksList[idx].getQuantity() - quantity);
			setCashBalance(getCashBalance() + total);
			if (stocksList[idx].getQuantity() == 0) {
				removeStock(symbol);
			}
			return true;
		}
	}
}

void StockAccount::addStock(std::string symbol, int quantity) {
	int idx = findStock(symbol);
	if (idx == -1) {
		cout << " " << endl;
		Stock s = Stock(symbol, quantity);
		stocksList.push_back(s);
	} else {
		stocksList[idx].setQuantity(stocksList[idx].getQuantity() + quantity);
	}
}

bool StockAccount::hasStock(string symbol) {
	for (int i = 0; i < stocksList.size(); i++) {
		if (stocksList[i].getSymbol() == symbol) {
			return true;
		}
	}
	return false;
}

int StockAccount::findStock(std::string symbol) {
	for (int i = 0; i < stocksList.size(); i++) {
		if (stocksList[i].getSymbol() == symbol) {
			return i;
		}
	}
	return -1;
}

bool StockAccount::removeStock(std::string symbol) {
	for (vector<Stock>::iterator it = stocksList.begin();
			it != stocksList.end(); ++it) {
		if ((*it).getSymbol() == symbol) {
			stocksList.erase(it);
			return true;
		}
	}
	return false;
}

BankAccount StockAccount::getBankAccount() {
	return bankAccount;
}

float StockAccount::getCurrentPrice(string symbol, string &name) {
	int randomFile = rand() % numFiles;
	return searchFile(priceFiles[randomFile], symbol, name);
}

float StockAccount::searchFile(ifstream &file, string symbol, string &name) {
	string sym;
	float price;
	string company;
	string line, token;
	int idx1, idx2;
	file.clear();
	file.seekg(0, ios::beg);
	while (file >> sym) {
		if (sym != symbol)
			continue;
		company = "";
		while (file >> token) {
			if (token.find_first_not_of("0123456789.") != string::npos) //check if its not a number
				company += " " + token;
			else {
				name = company;
				price = atof(token.c_str());
				return price;
			}
		}
	}
	name = "";
	return 0;
}

void StockAccount::initPriceFiles(string filenames[], int n) {
	numFiles = n;
	srand(time(NULL));
	for (int i = 0; i < n; i++) {
		priceFiles[i].open(filenames[i].c_str());
		if (!priceFiles[i].is_open()) {
			cout << "\n\tError opening file " << filenames[i] << endl << endl;
			exit(1);
		}
	}
}

void StockAccount::closeFiles() {
	for (int i = 0; i < numFiles; i++) {
		priceFiles[i].close();
	}
}

void StockAccount::deposit(float amount) {
	setCashBalance(getCashBalance() + amount);
}

bool StockAccount::withdraw(float amount) {
	if (bankAccount.withdraw(amount)) {
		Account::setCashBalance(bankAccount.getCashBalance());
		return true;
	} else {
		return false;
	}
}
