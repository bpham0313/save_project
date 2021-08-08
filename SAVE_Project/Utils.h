#pragma once
#ifndef Utils_h
#define Utils_h
#include <stdlib.h>
#include <iostream>
#include <mysql/jdbc.h>
#include "Transaction.h"
#include <vector>
#include <numeric>
#include <unordered_map>
#include <string>

string getNameFromAccNum(sql::Connection*& con, string account_number);


vector<string> getAllTransFromAccNum(sql::Connection*& con, string account_number);

string getHomeStateFromAccNum(sql::Connection*& con, string account_number);

string getStateFromTrans(sql::Connection*& con, Transaction* trans);

Transaction* getTrans(sql::Connection*& con, string account_number, string transaction_number);

vector<string> getAllAccNums(sql::Connection*& con);

double standardDev(vector<Transaction*> trans);

void  toUpperCase(string& str);

string get_password();

string get_host();

string get_user();

string getStateName(string SC);

#endif