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
#include "Utils.h"

using namespace std;

vector<Transaction*> getFraudTransactionsFromAccount(sql::Connection* &con, string account_number);
void flagFraudTransactions(vector<Transaction*> &trans);
vector<Transaction*> getOOStateTrans(sql::Connection*& con, string account_number);
template<typename T> void printElement(T t, const int& width);

int main(void)
{
    sql::Driver* driver;
    sql::Connection* con;
    
    driver = get_driver_instance();
    string host = get_host();//"localhost:3306"
    string user = get_user();//"root"
    string pw = get_password();
    con = driver->connect(host, user, pw);

    con->setSchema("save_project");


    cout << "======================================== Rule 1 : Identify Abnormally High Transactions==========================" << endl;
    
    const int printWidth = 25;
    const int printWidthMerchant = 35; 
    printElement("Name", printWidth);
    printElement("Account Number", printWidth);
    printElement("Transaciton Number", printWidth);
    printElement("Merchant", printWidthMerchant);
    printElement("Transaction Amount", printWidth);
    cout << endl;
    vector<string> accNumsFromDB = getAllAccNums(con);
    
    for (string accNum : accNumsFromDB) {
        vector<Transaction*> transactions = getFraudTransactionsFromAccount(con, accNum);

        cout.precision(2);
        for (auto i = transactions.begin(); i != transactions.end(); ++i) {
            if ((*i)->getFraud() == true) {
                printElement(getNameFromAccNum(con, (*i)->getAccountNumber()), printWidth);
                printElement((*i)->getAccountNumber(), printWidth);
                printElement((*i)->getTransactionNumber(), printWidth);
                printElement((*i)->getMerchant(), printWidthMerchant);
                printElement((*i)->getTransactionAmount(), printWidth);
                
                cout << endl;
            }
        }
    }
    
    cout << "\n\n\n\n\n";

    cout << "======================================== Rule 2 : Identify Out Of State Transactions==========================" << endl;
    const int printWidthLocation = 40;
    printElement("Name", printWidth);
    printElement("Account Number", printWidth);
    printElement("Transaciton Number", printWidth);
    printElement("Expected Transaction Location", printWidthLocation);
    printElement("Actual Transaction Location", printWidthLocation);
    cout << endl;
    
    for (string accNum : accNumsFromDB) {

        vector<Transaction*> transactions = getOOStateTrans(con, accNum);
        
        for (auto i = transactions.begin(); i != transactions.end(); ++i) {

            if ((*i)->getOOState() == true) {

                printElement(getNameFromAccNum(con, (*i)->getAccountNumber()), printWidth);
                printElement((*i)->getAccountNumber(), printWidth);
                printElement((*i)->getTransactionNumber(), printWidth);
                printElement(getStateName(getHomeStateFromAccNum(con,accNum)), printWidthLocation);
                printElement(getStateName(getStateFromTrans(con,(*i))), printWidthLocation);

                cout << endl;
            }
        }
    }
    
    
    
    delete con;
}

vector<Transaction*> getFraudTransactionsFromAccount(sql::Connection* &con, string account_number) {
    sql::PreparedStatement* ps;
    sql::ResultSet* rs;
    /*
    Send a query to the database to get all transactions from an account number and group the transactions based on the merchants
    */
    string sql = "SELECT tr.account_number,tr.transaction_number,tr.transaction_amount,tr.merchant,counter.count \
        FROM transactions tr \
        LEFT JOIN( \
            SELECT a.account_number, t.transaction_number, t.transaction_amount, t.merchant as m, COUNT(t.merchant) as count \
            FROM accounts a  \
            LEFT JOIN transactions t  \
            ON a.account_number = t.account_number  \
            WHERE a.account_number = ?  \
            GROUP BY SUBSTR(t.merchant, 1, 6)  \
            ORDER BY COUNT(t.merchant) DESC) as counter   \
        ON SUBSTR(tr.merchant, 1, 6) = SUBSTR(counter.m, 1, 6)  \
        WHERE tr.account_number = ?  \
        ORDER BY  counter.count DESC, tr.merchant_number; ";
    ps = con->prepareStatement(sql);
    ps->setString(1, account_number);
    ps->setString(2, account_number);
    rs = ps->executeQuery();
    vector<Transaction*> transFromAcc;

    while (rs->next()) {
        /*
        Only consider if there is a fraud transaction if the account number has made more than 4 (or 4) transactions at the same merchant
        */
        if (rs->getInt(5) < 4) {
            break;
        }
        transFromAcc.push_back(new Transaction(rs->getString(1), rs->getInt(2), rs->getDouble(3), rs->getString(4)));
    }

    unordered_map<string, vector<Transaction*>> mapMerchantToTrans;
    for (auto i = transFromAcc.begin(); i != transFromAcc.end(); ++i) {
        string merchant = ((*i)->getMerchant()).substr(0, 6);
        toUpperCase(merchant);
        mapMerchantToTrans[merchant].push_back(*i);
    }
 
    

    for (const auto &iter : mapMerchantToTrans) {
        vector<Transaction*> flagFraud;
        for (Transaction* t : iter.second) {
            flagFraud.push_back(t);
        }
        flagFraudTransactions(flagFraud);
    }
    

    delete ps;
    delete rs;

    return transFromAcc;
}

void flagFraudTransactions(vector<Transaction*> &trans) {
    /*
    Detecting fraud transactions by using standard deviation. If removing the transaction from the group makes the total standard deviation
    goes down more than 5 times => mark transaction as fraud.
    
    */
    double initStd = standardDev(trans);
    for (auto i = trans.begin(); i != trans.end(); ++i) {
        vector<Transaction*> v;
        for (auto k = trans.begin(); k != trans.end(); ++k) {
            if (*i == *k) {
                continue;
            }
            else {
                v.push_back((*k));
            }
        }

        double std = standardDev(v);

        if (initStd > (5 * std) && std !=0) {
            (*i)->setFraudFlag();
        }
                
    }

}


vector<Transaction*> getOOStateTrans(sql::Connection*& con, string account_number) {
    string homeState = getHomeStateFromAccNum(con, account_number);
    
    vector<string> transFromAcc = getAllTransFromAccNum(con, account_number);
    
    vector<Transaction*> OOStateTrans;
    for (string transNum : transFromAcc) {
        Transaction* transaction = getTrans(con, account_number, transNum);
        
        if (homeState != getStateFromTrans(con, transaction)) {
            
            transaction->setOOStateFlag();
        }
        OOStateTrans.push_back(transaction);
    }
    return OOStateTrans;
}

template<typename T> void printElement(T t, const int& width) {
    if constexpr (is_same_v<T, double>) {
        cout << fixed << "$" << t;
        cout << left << setw(width) << setfill(' ') << " ";
    }
    else {
        cout << left << setw(width) << setfill(' ') << t;
    }

}










