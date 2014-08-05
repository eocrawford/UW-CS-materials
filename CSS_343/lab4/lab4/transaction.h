#ifndef TRANSACTION_H
#define TRANSACTION_H

//---------------------------------------------------------------------------
// transaction.h
// Abstract (pure virtual) transaction class
// Author(s): Mary Angelo and Ethan Crawford
// Date: Nov 14, 2007
//---------------------------------------------------------------------------
// Transaction class: a place to store information about a transaction
//   Includes the following features:
//   -- display the transaction
//   -- Store pointers to the corresponding customer and movie
//
// Assumptions:
//   -- None
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "customer.h"
#include "movie.h"

class Customer;
class Movie;

// Transaction type
enum TransactionType
{
    TransactionTypeBorrow,
    TransactionTypeReturn,
};

class Transaction
{
public:
    // Constructor
    Transaction(Movie* pMovie, char type, char format);

    // Displays a formatted transaction record
    virtual void display();

protected:
    Movie* m_pMovie;
    char m_type;
    char m_format;
};

#endif // TRANSACTION_H
