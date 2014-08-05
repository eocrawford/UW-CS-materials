#ifndef TRANSACTION_H
#define TRANSACTION_H

//---------------------------------------------------------------------------
// transaction.h
// Abstract (pure virtual) transaction class
// Author(s): Mary Angelo and Ethan Crawford
// Date: May 16, 2007
//---------------------------------------------------------------------------
// Transaction class: Provides a template for implementing a transaction.
//   Includes the following abstract (pure virtual) methods:
//   -- Do transaction
//   -- Display transaction output
//   -- Set transaction data
//
// Assumptions:
//   -- Input data is a valid format
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "bstree.h"

class BSTree;

class Transaction 
{
public:
    // Performs transaction on provided account
    virtual bool Perform(const BSTree* pTree) = 0;

    // Displays a formatted transaction record
    virtual void Display() const = 0;

    // Reads a single input line, populating internal variables
    virtual bool SetData(ifstream& input) = 0;

protected:
    string m_type;
    string m_detail;
    int m_amount;
};

#endif // TRANSACTION_H
