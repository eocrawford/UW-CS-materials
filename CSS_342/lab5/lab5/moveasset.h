#ifndef MOVEASSET_H
#define MOVEASSET_H

//---------------------------------------------------------------------------
// moveasset.h
// MoveAsset transaction implementation
// Author(s): Mary Angelo and Ethan Crawford
// Date: May 16, 2007
//---------------------------------------------------------------------------
// MoveAsset class: Implements a transaction to move an asset
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

class MoveAsset : public Transaction
{
public:
    // Default constructor
    MoveAsset();

    // Perform transaction on provided account
    virtual bool Perform(const BSTree* pTree);

    // For simplicity, uses cout
    virtual void Display() const;

    // Reads a single input line, populating internal variables
    virtual bool SetData(ifstream& input);

private:
    int m_accountFrom;
    int m_subscriptFrom;
	int m_accountTo;
	int m_subscriptTo;
    string m_detailFrom;
    string m_detailTo;
};

#endif // MoveAsset_H
