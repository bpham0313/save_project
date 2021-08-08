#pragma once
#ifndef Transaction_h
#define Transaction_h
#include <iostream>


using namespace std;

class Transaction
{
private:
	string account_number;
	int transaction_number;
	double transaction_amount;
	string merchant;
	bool fraud = false;
	bool OOState = false;
public:
	Transaction();
	Transaction(string an, int tn, double ta, string m);
	string getAccountNumber();
	int getTransactionNumber();
	string getMerchant();
	double getTransactionAmount();
	bool getFraud();
	bool getOOState();
	void setFraudFlag();
	void setOOStateFlag();
	~Transaction();


};

#endif 