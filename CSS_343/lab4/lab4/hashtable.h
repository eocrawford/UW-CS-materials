#ifndef HASHTABLE_H
#define HASHTABLE_H

//---------------------------------------------------------------------------
// hashtable.h
// HashTable class
// Author(s): Mary Angelo and Ethan Crawford
// Date: Nov 14, 2007
//---------------------------------------------------------------------------
// HashTable: stores an array of hash keys and NodeData objects
//   Includes the following features:
//   -- Insert item, given a key
//   -- Retrive item, given a key
//   -- Delete item, given a key
//   -- Empty hashtable
//   -- Rebuild hashtable 
//
// Assumptions:
//   -- Input files are in the valid format for the type of object loading
//      them.
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "nodedata.h"
#include "bstree.h"

class HashTable
{
public:
    // Constructor
    HashTable();

    // Destructor
    ~HashTable();

    // Empties hashtable
    void makeEmpty();

    // Returns true if hashtable is empty
    bool isEmpty() const;

    // Retrieve NodeData, first parameter is NodeData to retrieve.
    // Return value holds pointer to found NodeData, NULL if not found
    NodeData* const retrieve(const NodeData& target) const;

    // Inserts a value into the hashtable
    void insert(const NodeData* pItem);

    // Removes a value from the hashtable
    void remove(const NodeData& target);

private:
    // Gets the hash index of the selected item, using Jenkins one-at-a-time
    // hashing algorithm
    unsigned int getHashIndex(const NodeData* pItem, int capacity) const;

    int m_used;
    int m_capacity;
    const double m_loadFactor;
    BSTree** m_prgHashTable;
};

#endif // HASHTABLE_H
