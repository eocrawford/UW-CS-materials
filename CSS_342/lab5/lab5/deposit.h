#ifndef DEPOSIT_H
#define DEPOSIT_H

//---------------------------------------------------------------------------
// deposit.h
// Deposit transaction implementation
// Author(s): Mary Angelo and Ethan Crawford
// Date: May 16, 2007
//---------------------------------------------------------------------------
// Deposit class: Implements a deposit transaction
//   Includes the following methods:
//   -- Do transaction
//   -- Display transaction output
//   -- Set transaction data
//
// Assumptions:
//   -- Input data is a valid format
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "bstree.h"
#include "transaction.h"

class Deposit : public Transaction
{
public:
    // Default constructor
    Deposit();

    // Perform transaction on provided account
    virtual bool Perform(const BSTree* pTree);

    // For simplicity, uses cout
    virtual void Display() const;

    // Reads a single input line, populating internal variables
    virtual bool SetData(ifstream& input);

private:
    int m_account;
    int m_subscript;
};

#endif // DEPOSIT_H
