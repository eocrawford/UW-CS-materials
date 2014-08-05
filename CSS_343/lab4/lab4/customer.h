#ifndef CUSTOMER_H
#define CUSTOMER_H

//---------------------------------------------------------------------------
// customer.h
// Customer class
// Author(s): Mary Angelo and Ethan Crawford
// Date: Nov 14, 2007
//---------------------------------------------------------------------------
// Customer: Represents a single MovieStore Customer
//   Includes the following features:
//   -- Inherits from NodeData
//
// Assumptions:
//   -- Input data is in a valid format
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "transaction.h"
#include "nodedata.h"

class Transaction;

class Customer : public NodeData
{
public:
    // Default constructor
    Customer() : 
      m_id(0)
    {
    }

    ~Customer();

    // Saves a transaction to a list
    void addTransaction(Transaction* pTransaction);

    // Displays formatted info
    virtual void display();

    // Creates a new instance of a NodeData object
    virtual NodeData* create();

    // Sets a key to be used in a hashtable
    virtual void setKey(const string& hashKey);

    // Reads a single input line, populating internal variables
    virtual bool setData(ifstream& input);

    // overloaded < operator
    virtual bool operator<(const NodeData& b) const;

    // overloaded > operator
    virtual bool operator>(const NodeData& b) const;

    // overloaded == operator
    virtual bool operator==(const NodeData& b) const;

    // overloaded != operator
    virtual bool operator!=(const NodeData& b) const;

protected:
    // compare method, used to implement comparison operators
    virtual int compare(const NodeData& a, const NodeData& b) const;

private:
    string m_firstName;
    string m_lastName;
    int m_id;
    queue<Transaction*> m_transactionQueue;
};

#endif // CUSTOMER_H
