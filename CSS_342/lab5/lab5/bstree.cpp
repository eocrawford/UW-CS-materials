#include "bstree.h"

//-----------------------------------------------------------------------------
// BSTree constructor
// Initialize default BStree state
//-----------------------------------------------------------------------------
BSTree::BSTree() :
    m_pRoot(NULL)
{
}

//-----------------------------------------------------------------------------
// BSTree destructor
// Clean up memory
//-----------------------------------------------------------------------------
BSTree::~BSTree()
{
    MakeEmpty();
}

//-----------------------------------------------------------------------------
// insert
// Insert object into the tree, parameter holds pointer to account to insert
//-----------------------------------------------------------------------------
bool BSTree::Insert(UserAccount* pItem)
{
    return InsertRecursive(m_pRoot, pItem);
}

//-----------------------------------------------------------------------------
// InsertRecursive
// Insert object into the tree, parameter holds pointer to account to insert
//-----------------------------------------------------------------------------
bool BSTree::InsertRecursive(Node*& pNode, UserAccount* pItem)
{
    if (NULL == pNode)
    {
        pNode = new Node();

        if (NULL == pNode)
        {
            return false;
        }

        pNode->data = pItem;
        return true;
    }
    else if (pNode->data->GetAccount() < pItem->GetAccount())
    {
        return InsertRecursive(pNode->right, pItem);
    }
    else
    {
        return InsertRecursive(pNode->left, pItem);
    }
}

//-----------------------------------------------------------------------------
// retrieve
// Retrieve account, first parameter is account to retrieve.
// Second parameter holds pointer to found account, NULL if not found
//-----------------------------------------------------------------------------
bool BSTree::Retrieve(const UserAccount& target, UserAccount*& pItem) const
{
    return RetrieveRecursive(m_pRoot, target, pItem);
}

//-----------------------------------------------------------------------------
// RetrieveRecursive
// Retrieve account, first parameter is current node
// Retrieve account, second parameter is account to retrieve.
// Third parameter holds pointer to found account, NULL if not found
//-----------------------------------------------------------------------------
bool BSTree::RetrieveRecursive(Node* pNode, const UserAccount& target, UserAccount*& pItem) const
{
    if (NULL == pNode)
    {
        pItem = NULL;
        return false;
    }

    if (pNode->data->GetAccount() < target.GetAccount())
    {
        return RetrieveRecursive(pNode->right, target, pItem);
    }
    else if (pNode->data->GetAccount() > target.GetAccount())
    {
        return RetrieveRecursive(pNode->left, target, pItem);
    }
    else
    {
        pItem = pNode->data;
        return true;
    }
}

//-----------------------------------------------------------------------------
// display
// Displays the contents of a tree
//-----------------------------------------------------------------------------
bool BSTree::Display() const
{
    cout << endl << "REPORT" << endl << endl;
    return DisplayRecursive(m_pRoot);
}

//-----------------------------------------------------------------------------
// DisplayRecursive
// Displays the contents of a tree
//-----------------------------------------------------------------------------
bool BSTree::DisplayRecursive(Node* pNode) const
{
    if (pNode == NULL)
    {
        return false;
    }

    DisplayRecursive(pNode->left);
    cout << *(pNode->data);
    DisplayRecursive(pNode->right);

    return true;
}


//-----------------------------------------------------------------------------
// buildTree
// Fills a tree from a data file
//-----------------------------------------------------------------------------
void BSTree::BuildTree(ifstream& infile)
{
    while (true)
    {
        UserAccount* pItem = new UserAccount();

        if (NULL == pItem)
        {
            break;
        }

        pItem->SetData(infile);

        // If the infile is not in a good state or 
        // is at eof, exit the loop
        if (!infile.good() || infile.eof() || !Insert(pItem))
        {
            delete pItem;
            break;
        }
    }
}

//-----------------------------------------------------------------------------
// makeEmpty
// Empties the current tree
//-----------------------------------------------------------------------------
void BSTree::MakeEmpty()
{
    MakeEmptyRecursive(m_pRoot);
}

//-----------------------------------------------------------------------------
// MakeEmptyRecursive
// Empties the current tree
//-----------------------------------------------------------------------------
void BSTree::MakeEmptyRecursive(Node*& pNode)
{
    if (NULL == pNode)
    {
        return;
    }

    MakeEmptyRecursive(pNode->left);
    MakeEmptyRecursive(pNode->right);

    delete pNode->data;
    delete pNode;
    pNode = NULL;
}

//-----------------------------------------------------------------------------
// isEmpty
// Returns true if tree is empty
//-----------------------------------------------------------------------------
bool BSTree::IsEmpty() const
{
    return (m_pRoot == NULL);
}
