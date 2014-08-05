#ifndef COMPUTETAX_H
#define COMPUTETAX_H

//---------------------------------------------------------------------------
// ComputeTax.h
// ComputeTax transaction implementation
// Author(s): Mary Angelo and Ethan Crawford
// Date: May 16, 2007
//---------------------------------------------------------------------------
// ComputeTax class: Implements a transaction that will compute and display
//   taxes on a fund.
//
//   Includes the following methods:
//   -- Do transaction
//   -- Display transaction output
//   -- Set transaction data
//
// Assumptions:
//   -- Input data is a valid format
//   -- Tax is 8%
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "bstree.h"
#include "transaction.h"

class ComputeTax : public Transaction
{
public:
    // Default constructor
    ComputeTax();

    // Perform transaction on provided account
    virtual bool Perform(const BSTree* pTree);

    // For simplicity, uses cout
    virtual void Display() const;

    // Reads a single input line, populating internal variables
    virtual bool SetData(ifstream& input);

private:
    int m_account;
    int m_subscript;
	double m_tax;
};

#endif // ComputeTax_H
