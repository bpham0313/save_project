#include <stdlib.h>
#include <iostream>
#include <mysql/jdbc.h>
#include <iomanip>
#include "Transaction.h"
#include <vector>
#include <cmath>
#include <numeric>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
#include <string>
#include <windows.h>
#include "Utils.h"
#include <conio.h>

using namespace std;


string getNameFromAccNum(sql::Connection*& con, string account_number) {
    sql::PreparedStatement* ps;
    sql::ResultSet* rs;
    string sql = "SELECT  first_name, last_name\
        FROM accounts  \
        WHERE account_number = ?;";
    ps = con->prepareStatement(sql);
    ps->setString(1, account_number);
    rs = ps->executeQuery();
    if (rs->next()) {
        return rs->getString(1) + " " + rs->getString(2);
    }
    else {
        return "Can't find name for account:" + account_number;
    }
}



vector<string> getAllTransFromAccNum(sql::Connection*& con, string account_number) {
    sql::PreparedStatement* ps;
    sql::ResultSet* rs;
    string sql = "SELECT  transaction_number\
        FROM transactions  \
        WHERE account_number = ?;";
    ps = con->prepareStatement(sql);
    ps->setString(1, account_number);
    rs = ps->executeQuery();
    vector<string> trans;
    while (rs->next()) {
        trans.push_back(rs->getString(1));
    }
    return trans;
}

string getHomeStateFromAccNum(sql::Connection*& con, string account_number) {
    sql::PreparedStatement* ps;
    sql::ResultSet* rs;
    string sql = "SELECT  state\
        FROM accounts  \
        WHERE account_number = ?;";
    ps = con->prepareStatement(sql);
    ps->setString(1, account_number);
    rs = ps->executeQuery();
    if (rs->next()) {
        string state = rs->getString(1);
        boost::trim(state);
        return state;
    }
    else {
        return "Can't find home state for account:" + account_number;
    }
}

string getStateFromTrans(sql::Connection*& con, Transaction* trans) {
    sql::PreparedStatement* ps;
    sql::ResultSet* rs;

    string sql = "SELECT merchant_description FROM transactions WHERE account_number = ? AND transaction_number = ? ;";
    ps = con->prepareStatement(sql);

    ps->setString(1, trans->getAccountNumber());
    ps->setInt(2, trans->getTransactionNumber());

    rs = ps->executeQuery();
    if (rs->next()) {

        string desc = rs->getString(1);
        string state = desc.substr(desc.find_last_of(" "));
        boost::trim(state);
        if (state.length() > 2) {

            return state.substr(0, 2);
        }

        return state;
    }

}

Transaction* getTrans(sql::Connection*& con, string account_number, string transaction_number) {
    sql::PreparedStatement* ps;
    sql::ResultSet* rs;
    string sql = "SELECT  account_number,transaction_number,transaction_amount,merchant\
        FROM transactions  \
        WHERE account_number = ? AND transaction_number = ? ;";
    ps = con->prepareStatement(sql);
    ps->setString(1, account_number);
    ps->setString(2, transaction_number);
    rs = ps->executeQuery();
    if (rs->next()) {

        return new Transaction(rs->getString(1), rs->getInt(2), rs->getDouble(3), rs->getString(4));
    }
    else {
        cout << "No transaction " << transaction_number << " for account " << account_number << endl;
        return nullptr;
    }
}

vector<string> getAllAccNums(sql::Connection*& con) {
    vector<string> accountNums;
    sql::ResultSet* rs;
    sql::Statement* stmt;
    stmt = con->createStatement();
    rs = stmt->executeQuery("SELECT account_number FROM accounts;");
    while (rs->next())
    {
        accountNums.push_back(rs->getString(1));
    }
    return accountNums;
}

void  toUpperCase(string& str)
{
    transform(str.begin(), str.end(), str.begin(), ::toupper);
}


double standardDev(vector<Transaction*> trans) {
    vector<double> v;
    for (auto i = trans.begin(); i != trans.end(); ++i) {
        v.push_back((*i)->getTransactionAmount());
    }

    double sum = std::accumulate(v.begin(), v.end(), 0.0);
    double mean = sum / v.size();

    double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
    double stdev = std::sqrt(sq_sum / v.size() - mean * mean);
    return stdev;
}

string get_password() {
    cout << "Password " ;

    string ipt = "";
    char ipt_ch;
    while (true) {
        ipt_ch = _getch();
        if (ipt_ch < 32) {
            cout << endl;
            return ipt;
        }
        ipt.push_back(ipt_ch);
    }
    cout << endl;
}

string get_user() {
    cout << "User: ";
    string user;
    cin >> user;
    cout << endl;
    return user;
}

string get_host() {
    cout << "Hostname: ";
    string host;
    cin >> host;
    
    return host;
}

string getStateName(string SC) {

    toUpperCase(SC);
    const string abbreviations[] = { "AK","AL","AR","AS","AZ","CA","CO","CT","DC","DE","FL","GA","GU","HI","IA","ID","IL","IN","KS","KY","LA","MA","MD",
        "ME","MI","MN","MO","MS","MT","NC","ND","NE","NH","NJ","NM","NV","NY","OH","OK","OR","PA","PR","RI","SC","SD","TN","TX","UT","VA","VI",
        "VT","WA","WI","WV","WY" };
    const string states[] = { "Alaska","Alabama","Arkansas","American Samoa","Arizona","California","Colorado","Connecticut","District of Columbia",
                  "Delaware","Florida","Georgia","Guam","Hawaii","Iowa","Idaho","Illinois","Indiana","Kansas","Kentucky","Louisiana","Massachusetts",
                  "Maryland","Maine","Michigan","Minnesota","Missouri","Mississippi","Montana","North Carolina","North Dakota","Nebraska","New Hampshire",
                  "New Jersey","New Mexico","Nevada","New York","Ohio","Oklahoma","Oregon","Pennsylvania","Puerto Rico","Rhode Island","South Carolina",
                  "South Dakota","Tennessee","Texas","Utah","Virginia","Virgin Islands","Vermont","Washington","Wisconsin","West Virginia","Wyoming" };
    unordered_map<string, string> stateMap;
    for (int i = 0; i < sizeof(abbreviations) / sizeof(abbreviations[0]); i++) {
        stateMap[abbreviations[i]] = states[i];
    }

    return stateMap[SC];
   
}