#ifndef NODEDATA_H
#define NODEDATA_H

//---------------------------------------------------------------------------
// movie.h
// Abstract base class representing the data contained within a node
// Author(s): Mary Angelo and Ethan Crawford
// Date: Nov 14, 2007
//---------------------------------------------------------------------------
// NodeData class: Provides a template and a set of common methods 
// for implementing an object that can be stored in either a hashtable or
// a tree.
//---------------------------------------------------------------------------

#include "stdinc.h"

class NodeData
{
public:
    virtual ~NodeData()
    {
    }

    // Returns a key suitable for a hashtable
    virtual inline void getKey(string& hashKey)
    {
        hashKey = m_hashKey;
    }

    // Sets a key to be used in a hashtable
    virtual void setKey(const string& hashKey) = 0;

    // Displays formatted info
    virtual void display() = 0;

    // Creates a new instance of a NodeData object
    virtual NodeData* create() = 0;

    // Reads a single input line, populating internal variables
    virtual bool setData(ifstream& input) = 0;

    // overloaded < operator
    virtual bool operator<(const NodeData& b) const = 0;

    // overloaded > operator
    virtual bool operator>(const NodeData& b) const = 0;

    // overloaded == operator
    virtual bool operator==(const NodeData& b) const = 0;

    // overloaded != operator
    virtual bool operator!=(const NodeData& b) const = 0;

protected:
    string m_hashKey;
};

#endif // NODEDATA_H
