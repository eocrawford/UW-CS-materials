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

#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <limits>
#include "nodedata.h"

using namespace std;

class BSTree
{
    // Accessor: overloaded << operator
    // Displays the tree, in-order
    friend ostream& operator<<(ostream& output, const BSTree& b);

public:
    // Constructor
    BSTree();

    // Copy constructor
    BSTree(const BSTree& b);

    // Destructor
    ~BSTree();

    // Assign one BSTree to another
    BSTree& operator=(const BSTree& b);

    // Compare two BSTrees; return true if they are the same
    bool operator==(const BSTree& b) const;

    // Compare two BSTrees; return false if they are the same
    bool operator!=(const BSTree& b) const;

    // insert object into the tree, parameter holds pointer to NodeData 
    // to insert
    bool insert(const NodeData* pItem);

    // Remove an object from the tree, parameter holds pointer to target
    // object to remove.
    bool remove(const NodeData& target);

    // Retrieve NodeData, first parameter is NodeData to retrieve.
    // Second parameter holds pointer to found NodeData, NULL if not found
    NodeData* const retrieve(const NodeData& target) const;

    // Outputs a BSTree to an array, populating an array 
    // and emptying the BSTree.
	void bstreeToArray(NodeData* rgNodeData[]);

    // Outputs an array to a BSTree, populating a BSTree 
    // and emptying the array.
    void arrayToBSTree(NodeData* rgNodeData[]);

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

    // Copy the contents of src into dst
    void copy(BSTree& dst, const BSTree& src);

    // Copy the contents of the src tree into the dst tree
    void copyRecursive(Node*& pDstNode, const Node* pSrcNode);

    void copyNode(Node*& pDst, const Node* pSrc) const;

    // Compare trees A and B
    bool compareTreesRecursive(const Node* pA, const Node* pB) const;

    // Output a BSTree to an array, populating an array 
    // and emptying the BSTree.
    void bstreeToArrayRecursive(Node* pNode, NodeData* rgNodeData[], 
    int& arrayIndex);

    // Output an array to a BSTree, populating a BSTree 
    // and emptying the array.
    void arrayToBSTreeRecursive(Node*& pNode, NodeData* rgNodeData[], 
        int low, int high);

    // root of the tree
    Node* m_pRoot;
};

#endif // BSTREE_H
