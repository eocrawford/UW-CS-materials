#ifndef BSTREE_H
#define BSTREE_H

//---------------------------------------------------------------------------
// bstree.h
// Binary search tree class
// Author(s): Ethan Crawford
// Date: Oct 15, 2007
//---------------------------------------------------------------------------
// Binary Search tree: stores a BST of NodeData objects
//   Includes the following features:
//   -- Build tree from input file
//   -- Insert nodedata into tree
//   -- Retrieve nodedata from tree
//   -- Empty tree
//
// Assumptions:
//   -- Input files are in the valid format for the type of object loading
//      them.
//---------------------------------------------------------------------------

#include "stdinc.h"
#include "nodedata.h"

class BSTree
{
    // Accessor: overloaded << operator
    // Displays the tree, in-order
    friend ostream& operator<<(ostream& output, const BSTree& b);

public:
    // Constructor
    BSTree();

    // Destructor
    ~BSTree();

    // Compare two BSTrees; return true if they are the same
    bool operator==(const BSTree& b) const;

    // Compare two BSTrees; return false if they are the same
    bool operator!=(const BSTree& b) const;

    // Return an item with subscript index
    const NodeData* getData(int index);

    // Returns the size of the BSTree
    int getSize() const;

    // Gets the name of the collection
    const string& getName();

    // Sets the name of the collection
    void setName(const string& name);

    // insert object into the tree, parameter holds pointer to NodeData 
    // to insert
    bool insert(const NodeData* pItem);

    // Remove an object from the tree, parameter holds pointer to target
    // object to remove.
    bool remove(const NodeData& target);

    // Retrieve NodeData, first parameter is NodeData to retrieve.
    // Second parameter holds pointer to found NodeData, NULL if not found
    NodeData* const retrieve(const NodeData& target) const;

    // Empties the current tree
    void makeEmpty();

    // Returns true if tree is empty
    bool isEmpty() const;

    // Gets the depth of the specified NodeData
    int getDepth(const NodeData& target) const;

private:
    struct Node
    {
        Node() :
            pData(NULL),
            pRight(NULL),
            pLeft(NULL)
        {
        }

        NodeData* pData; 
        Node* pRight;
        Node* pLeft;
    };

private:
    // Initialize member variables
    void init();

    // Insert the specified node into the BSTree
    bool insertRecursive(Node*& pNode, const NodeData* pItem);

    // Remove the specified node from the BSTree
    bool removeRecursive(Node*& pNode);

    // retrieve the specified node
    NodeData* const retrieveRecursive(const Node* pNode, 
    const NodeData& target, Node** ppTargetNode, 
    Node** ppParentNode) const;

    // display the provided tree
    void displayInOrderRecursive(const Node* pNode, ostream& output) const;

    // Empty the provided tree
    void makeEmptyRecursive(Node*& pNode);

    // Get the depth of the target node
    int getDepthRecursive(const Node* pNode, const NodeData& target, 
        int currentDepth) const;

    // Compare trees A and B
    bool compareTreesRecursive(const Node* pA, const Node* pB) const;

    // Outputs a BSTree to an array, populating an array 
    // and emptying the BSTree.
	void bstreeToArray();

    // Output a BSTree to an array, populating an array 
    // and emptying the BSTree.
    void bstreeToArrayRecursive(Node* pNode, int& arrayIndex);

    // root of the tree
    Node* m_pRoot;
    NodeData** m_prgNodeData;
    int m_nodeCount;
    bool m_fIsDirty;
    string m_collectionName;
};

#endif // BSTREE_H
