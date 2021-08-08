#include <iostream>
#include "Transaction.h"

using namespace std;


Transaction::Transaction(string an, int tn, double ta, string m)
{
	account_number = an;
	transaction_number = tn;
	transaction_amount = ta;
	merchant = m;

}

string Transaction::getAccountNumber()
{
	return account_number;
}

int Transaction::getTransactionNumber()
{
	return transaction_number;
}

string Transaction::getMerchant()
{
	return merchant;
}

double Transaction::getTransactionAmount()
{
	return transaction_amount;
}

bool Transaction::getFraud()
{
	return fraud;
}

bool Transaction::getOOState()
{
	return OOState;
}

void Transaction::setFraudFlag()
{
	fraud = true;
}

void Transaction::setOOStateFlag()
{
	OOState = true;
}

Transaction::~Transaction()
{
}
