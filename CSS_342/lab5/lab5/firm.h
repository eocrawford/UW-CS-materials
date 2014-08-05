#ifndef FIRM_H
#define FIRM_H

//---------------------------------------------------------------------------
// firm.h
// Firm manager class
// Author(s): Mary Angelo and Ethan Crawford
// Date: May 16, 2007
//---------------------------------------------------------------------------
// Firm class: Implements the functionality of an investment firm
//   Includes the following methods:
//   -- Initialize accounts from a data file
//   -- Process a list of transactions from a data file
//
// Assumptions:
//   -- Input data is a valid format
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "bstree.h"
#include "transaction.h"
#include "deposit.h"
#include "withdraw.h"
#include "computetax.h"
#include "displayhistory.h"
#include "moveasset.h"

class Firm
{
public:
    // Initialize default firm state
    Firm();

    // Print the day's firm report and clean up memory
    ~Firm();

    // Initialize firm accounts from an input file
    void InitializeAccounts(ifstream& infile) const;

    // Process a list of transactions from an input file
    void ProcessTransactions(ifstream& infile) const;

    // Display all the queued transations
    void DisplayTransactions() const;

private:

    // Process a single transaction from an input file
    void ProcessTransaction(ifstream& infile) const;

    // Get the Transaction object that corresponds to the provided transaction
    // code. Returns true if the transaction can be found, false otherwise.
    bool GetTransaction(char transactionCode, Transaction*& pTransaction) const;

    BSTree* m_pAccountRoot;
};

#endif // FIRM_H
