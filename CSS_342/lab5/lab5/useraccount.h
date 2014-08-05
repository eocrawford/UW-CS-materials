#ifndef USERACCOUNT_H
#define USERACCOUNT_H

//---------------------------------------------------------------------------
// useraccount.h
// User Account class
// Author(s): Mary Angelo and Ethan Crawford
// Date: May 16, 2007
//---------------------------------------------------------------------------
// User Account: Represents a single LOSE financial firm user account
//   Includes the following features:
//   -- Initializes member functions from an input file
//   -- Stores FIRMACCOUNT friendly names and types
//   -- Adds the transaction to a history queue
//   -- Displays transaction history that corresponds to this account
//   -- Displays initial and final balances with overloaded << operator
//
// Assumptions:
//   -- Input data is in a valid format
//---------------------------------------------------------------------------

#include "stdinc.h"
#include <queue>
#include "transaction.h"

// Enum representing different types of firm accounts
enum EAccountType
{
    EAccountNone,
    EAccountTypeMoneyMarket,
    EAccountTypeBond,
    EAccountTypeMutualFund
};

// Struct representing a firm account
struct FIRMACCOUNT
{
    string friendlyName;
    EAccountType type;
    int amount;

    FIRMACCOUNT() :
        type(EAccountNone),
        amount(0)
    {
    }
};

class UserAccount
{
    // Output the account starting and ending balances
    friend ostream& operator<<(ostream& output, const UserAccount& account);

public:
    // Default constructor
    UserAccount();

    // Destructor
    ~UserAccount();

    // Set internal state based on a single line in an input file
    void SetData(ifstream& infile);

    // Print the transaction queue
    void DisplayTransactionHistory();

    // Add a transaction to the transaction queue
    void AddTransaction(Transaction* pTransaction);

    // Set account number
    void SetAccount(int account);

    // Get account number
    int GetAccount() const;

    // Get the FIRMACCOUNT struct array
    void GetFirmAccounts(FIRMACCOUNT*& accounts, int& arraySize);

private:
    // Set default values for firm accounts
    void InitializeFirmAccounts();

    // Display a formatted list of account balances
void DisplayBalances(ostream& output, const char* text, 
    const FIRMACCOUNT accounts[], int arraySize) const;

    int m_account;
    string m_firstName;
    string m_lastName;
    FIRMACCOUNT m_initialBalances[10];
    FIRMACCOUNT m_currentBalances[10];
    queue<Transaction*> m_transactionQueue;
};

#endif // USERACCOUNT_H
