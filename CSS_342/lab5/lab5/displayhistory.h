#ifndef DISPLAYHISTORY_H
#define DISPLAYHISTORY_H

//---------------------------------------------------------------------------
// displayhistory.h
// DisplayHistory transaction implementation
// Author(s): Mary Angelo and Ethan Crawford
// Date: May 16, 2007
//---------------------------------------------------------------------------
// DisplayHistory class: Implements a transaction that displays account history
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

class DisplayHistory : public Transaction
{
public:
    // Default constructor
    DisplayHistory();

    // Perform transaction on provided account
    virtual bool Perform(const BSTree* pTree);

    // For simplicity, uses cout
    virtual void Display() const;

    // Reads a single input line, populating internal variables
    virtual bool SetData(ifstream& input);

private:
    int m_account;
};

#endif // DISPLAYHISTORY_H
