#ifndef COMEDY_H
#define COMEDY_H

//---------------------------------------------------------------------------
// comedy.h
// Comic movie implementation of Movie class
// Author(s): Mary Angelo and Ethan Crawford
// Date: Nov 14, 2007
//---------------------------------------------------------------------------
// Comedy movie class: Implements a Comedy Movie
//
// Assumptions:
//   -- Input data is a valid format
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "movie.h"

class Comedy : public Movie
{
public:

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
};

#endif // COMEDY_H
