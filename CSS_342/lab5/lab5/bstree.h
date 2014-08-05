#ifndef BSTREE_H
#define BSTREE_H

//---------------------------------------------------------------------------
// bstree.h
// Binary search tree class
// Author(s): Mary Angelo and Ethan Crawford
// Date: May 16, 2007
//---------------------------------------------------------------------------
// Binary Search tree: stores a BST of accounts.
//   Includes the following features:
//   -- Build tree from input file
//   -- Insert user account into tree
//   -- Retrieve user account from tree
//   -- Empty tree
//
// Assumptions:
//   -- Input files are in the valid format for the type of object loading
//      them.
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "useraccount.h"

class UserAccount;

class BSTree
{
public:
    BSTree();
    ~BSTree();

    // insert object into the tree, parameter holds pointer to account to insert
    bool Insert(UserAccount* pItem);

    // Retrieve account, first parameter is account to retrieve.
    // Second parameter holds pointer to found account, NULL if not found
    bool Retrieve(const UserAccount& target, UserAccount*& pItem) const;

    // Displays the contents of a tree
    bool Display() const;

    // Fills a tree from a data file
    void BuildTree(ifstream& infile);

    // Empties the current tree
    void MakeEmpty();

    // Returns true if tree is empty
    bool IsEmpty() const;

private:
    struct Node
    {
        Node() :
            data(NULL),
            right(NULL),
            left(NULL)
        {
        }
        UserAccount* data; 
        Node* right;
        Node* left;
    };

    bool InsertRecursive(Node*& pNode, UserAccount* pItem);
    bool RetrieveRecursive(Node* pNode, const UserAccount& target, UserAccount*& pItem) const;
    bool DisplayRecursive(Node* pNode) const;
    void MakeEmptyRecursive(Node*& pNode);

    // root of the tree
    Node* m_pRoot;
};

#endif // BSTREE_H
