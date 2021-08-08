#pragma once
#ifndef Account_h
#define Account_h

#include <iostream>
#include <string>
#include "Account.cpp"

using namespace std;

class Account
{
private:
    string accountNumber;
    string name;
    string primaryState;

public:
    string getAccountNumber();
    string getName();
    string getPrimaryState();

    Account();
    Account(string accountNumber,string name, string primaryState);
};


#endif /* Account_hpp */ 
