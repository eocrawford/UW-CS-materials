#ifndef MOVIESTORE_H
#define MOVIESTORE_H

//---------------------------------------------------------------------------
// moviestore.h
// Movie Store manager class
// Author(s): Mary Angelo and Ethan Crawford
// Date: Nov 14, 2007
//---------------------------------------------------------------------------
// Movie Store class: Implements the functionality of a movie store
//   Includes the following methods:
//   -- Initialize accounts from a data file
//   -- Process a list of transactions from a data file
//   -- Display a list of movies contained within the store
//   -- Display a list of customers and their borrowing history
//
// Assumptions:
//   -- Input data is a valid format
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "bstree.h"
#include "hashtable.h"
#include "classic.h"
#include "comedy.h"
#include "customer.h"
#include "drama.h"
#include "movie.h"
#include "moviemanager.h"
#include "transaction.h"

class MovieStore
{
public:
    // Initialize default movie store state
    MovieStore();

    // Clean up memory
    ~MovieStore();

    // Initialize customer list
    void initAccounts(ifstream& infile);

    // Initialize movie list
    void initMovies(ifstream& infile);

    // Process a list of transactions from an input file
    void processTransactions(ifstream& infile);

private:
    // Display list of movies contained in the store
    void displayMovies() const;

    // Display customer history
    void displayHistory(ifstream& infile) const;

    // Borrows or returns a movie
    // Returns true if transaction was successful, false otherwise
    bool toggleCheckoutState(bool fBorrow, Movie* pMovie, 
        int custId, char format);

    BSTree** m_prgMovies;
    HashTable** m_prgMoviesHash;
    BSTree m_customers;
    HashTable m_customersHash;
    MovieManager m_movieManager;
};

#endif // MOVIESTORE_H
