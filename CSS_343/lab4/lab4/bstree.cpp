#include "bstree.h"

//-----------------------------------------------------------------------------
// overloaded << operator
// Displays the tree, in-order
//-----------------------------------------------------------------------------
ostream& operator<<(ostream& output, const BSTree& b)
{
    output << b.m_collectionName << endl;
    b.displayInOrderRecursive(b.m_pRoot, output);
    output << ' ' << endl;

    return output;
}

//-----------------------------------------------------------------------------
// BSTree constructor
// Initialize default BStree state
//-----------------------------------------------------------------------------
BSTree::BSTree()
{
    init();
}

//-----------------------------------------------------------------------------
// == operator
// Compare two BSTrees; return true if they are the same
//-----------------------------------------------------------------------------
bool BSTree::operator==(const BSTree& b) const
{
    return compareTreesRecursive(m_pRoot, b.m_pRoot);
}

//-----------------------------------------------------------------------------
// != operator
// Compare two BSTrees; return false if they are the same
//-----------------------------------------------------------------------------
bool BSTree::operator!=(const BSTree& b) const
{
    return !(*this == b);
}

//-----------------------------------------------------------------------------
// Overloaded subscript operator
// Return an item with subscript index
//-----------------------------------------------------------------------------
const NodeData* BSTree::getData(int index)
{
    NodeData* pRet = NULL;

    // Regenerate the internal array if necessary
    if (m_fIsDirty)
    {
        bstreeToArray();
        m_fIsDirty = false;
    }

    // Return the item at the desired subscript
    if (index >= 0 && index < m_nodeCount)
    {
        pRet = m_prgNodeData[index];
    }

    return pRet;
}

//-----------------------------------------------------------------------------
// getSize
// Returns the size of the BSTree
//-----------------------------------------------------------------------------
int BSTree::getSize() const
{
    return m_nodeCount;
}

//-----------------------------------------------------------------------------
// BSTree destructor
// Clean up memory
//-----------------------------------------------------------------------------
BSTree::~BSTree()
{
    delete[] m_prgNodeData;
}

//-----------------------------------------------------------------------------
// insert
// Insert object into the tree, parameter holds pointer to NodeData to insert
//-----------------------------------------------------------------------------
bool BSTree::insert(const NodeData* pItem)
{
    return insertRecursive(m_pRoot, pItem);
}

//-----------------------------------------------------------------------------
// insertRecursive
// Insert object into the tree, parameter holds pointer to NodeData to insert
//-----------------------------------------------------------------------------
bool BSTree::insertRecursive(Node*& pNode, const NodeData* pItem)
{
    if (NULL == pNode)
    {
        pNode = new Node();

        if (NULL == pNode)
        {
            return false;
        }

        pNode->pData = const_cast<NodeData*>(pItem);
        m_nodeCount++;
        m_fIsDirty = true;
        return true;
    }
    // Discard duplicates
    else if (*(pNode->pData) == *(pItem))
    {
        return true;
    }
    else if (*(pNode->pData) < *(pItem))
    {
        return insertRecursive(pNode->pRight, pItem);
    }
    else
    {
        return insertRecursive(pNode->pLeft, pItem);
    }
}

//-----------------------------------------------------------------------------
// remove
// Remove an object from the tree, parameter holds pointer to target
// object to remove.
//-----------------------------------------------------------------------------
bool BSTree::remove(const NodeData& target)
{
    Node* pNode = NULL;
    Node* pParentNode = NULL;
    Node** ppSplice = NULL;

    // Find the target node and its parent
    retrieveRecursive(m_pRoot, target, &pNode, &pParentNode);

    if (NULL == pNode)
    {
        return false;
    }

    // Get a pointer to the actual child node. That way, we will be modifying
    // the actual pointer stored in the tree when it comes time to delete it,
    // instead of just a copied pointer to the same object.
    // If this step is skipped, the tree value will end up pointing to freed 
    // memory.
    if (NULL != pParentNode)
    {
        ppSplice = (pParentNode->pLeft == pNode) ? 
            &(pParentNode->pLeft) : &(pParentNode->pRight);
    }
    // special-case the root node
    else
    {
        ppSplice = &m_pRoot;
    }

    return removeRecursive(*ppSplice);
}

//-----------------------------------------------------------------------------
// removeRecursive
// Remove the specified node from the BSTree
//-----------------------------------------------------------------------------
bool BSTree::removeRecursive(Node*& pNode)
{
    Node* pTemp = pNode;

    // Handle 0 to 1 children
    if (NULL == pNode->pRight)
    {
        pNode = pNode->pLeft;
        delete pTemp->pData;
        delete pTemp;
        m_nodeCount--;
        m_fIsDirty = true;
    }
    else if (NULL == pNode->pLeft)
    {
        pNode = pNode->pRight;
        delete pTemp->pData;
        delete pTemp;
        m_nodeCount--;
        m_fIsDirty = true;
    }
    // Handle 2 children
    else
    {
        // Save the address of the left sub-tree.
        Node** ppPrecessor = &pNode->pLeft;

        // Find the inline precessor (right-most child of the left sub-tree)
        // (This can also be implemented with an inline successor, the left-most
        // child of the right sub-tree).
        while (NULL != (*ppPrecessor)->pRight)
        {
            ppPrecessor = &(*ppPrecessor)->pRight;
        }

        // Move the payload in preparation for deleting the precessor
        delete pNode->pData;
        pNode->pData = (*ppPrecessor)->pData;

        // Zero out the precessor data pointer so that the moved 
        // payload won't be deleted
        (*ppPrecessor)->pData = NULL;

        // Remove the precessor with a recursive call
        removeRecursive(*ppPrecessor);
    }

    return true;
}

//-----------------------------------------------------------------------------
// retrieve
// Retrieve NodeData, first parameter is NodeData to retrieve.
// Second parameter holds pointer to found NodeData, NULL if not found
//-----------------------------------------------------------------------------
NodeData* const BSTree::retrieve(const NodeData& target) const
{
    Node* pNode;
    Node* pParentNode;
    return retrieveRecursive(m_pRoot, target, &pNode, &pParentNode);
}

//-----------------------------------------------------------------------------
// RetrieveRecursive
// Retrieve NodeData, first parameter is current node
// Retrieve NodeData, second parameter is NodeData to retrieve.
// Third parameter holds pointer to found NodeData, NULL if not found
//-----------------------------------------------------------------------------
NodeData* const BSTree::retrieveRecursive(const Node* pNode, 
    const NodeData& target, Node** ppTargetNode, 
    Node** ppParentNode) const
{
    if (NULL == pNode)
    {
        return NULL;
    }

    if (*(pNode->pData) < target)
    {
        *ppParentNode = const_cast<BSTree::Node*>(pNode);
        return retrieveRecursive(pNode->pRight, target, ppTargetNode, ppParentNode);
    }
    else if (*(pNode->pData) > target)
    {
        *ppParentNode = const_cast<BSTree::Node*>(pNode);
        return retrieveRecursive(pNode->pLeft, target, ppTargetNode, ppParentNode);
    }
    else
    {
        *ppTargetNode = const_cast<BSTree::Node*>(pNode);
        return pNode->pData;
    }
}

//-----------------------------------------------------------------------------
// displayInOrderRecursive
// Displays the in-order contents of a BSTree
//-----------------------------------------------------------------------------
void BSTree::displayInOrderRecursive(const Node* pNode, ostream& output) const
{
    if (pNode == NULL)
    {
        return;
    }

    displayInOrderRecursive(pNode->pLeft, output);
    pNode->pData->display();
    output << endl;
    displayInOrderRecursive(pNode->pRight, output);
}


//-----------------------------------------------------------------------------
// makeEmpty
// Empties the current tree
//-----------------------------------------------------------------------------
void BSTree::makeEmpty()
{
    makeEmptyRecursive(m_pRoot);
    delete[] m_prgNodeData;
    m_nodeCount = 0;
    m_prgNodeData = NULL;
    m_fIsDirty = false;
}

//-----------------------------------------------------------------------------
// MakeEmptyRecursive
// Empties the current tree
//-----------------------------------------------------------------------------
void BSTree::makeEmptyRecursive(Node*& pNode)
{
    if (NULL == pNode)
    {
        return;
    }

    makeEmptyRecursive(pNode->pLeft);
    makeEmptyRecursive(pNode->pRight);

    delete pNode->pData;
    delete pNode;
    pNode = NULL;
}

//-----------------------------------------------------------------------------
// isEmpty
// Returns true if tree is empty
//-----------------------------------------------------------------------------
bool BSTree::isEmpty() const
{
    return (NULL == m_pRoot);
}

//-----------------------------------------------------------------------------
// init
// Initialize class members
//-----------------------------------------------------------------------------
void BSTree::init()
{
    m_pRoot = NULL;
    m_nodeCount = 0;
    m_prgNodeData = NULL;
    m_fIsDirty = false;
}

//-----------------------------------------------------------------------------
// compareTreesRecursive
// Recursively compares two trees
//-----------------------------------------------------------------------------
bool BSTree::compareTreesRecursive(const Node* pA, const Node* pB) const
{
    bool ret = true;

    // Check for null.
    if (NULL == pA || NULL == pB)
    {
        if (!(NULL == pA && NULL == pB))
        {
            ret = false;
        }
        else
        {
            ret = true;
        }
    }
    else if (*(pA->pData) != *(pB->pData))
    {
        ret = false;
    }

    if (ret)
    {
        if (NULL != pA)
        {
            ret &= compareTreesRecursive(pA->pLeft, pB->pLeft);
        }

        if (NULL != pB)
        {
            ret &= compareTreesRecursive(pA->pRight, pB->pRight);
        }
    }

    return ret;
}

//-----------------------------------------------------------------------------
// getDepth
// Retrieves the depth of specified NodeData object
//-----------------------------------------------------------------------------
int BSTree::getDepth(const NodeData& target) const
{
    return getDepthRecursive(m_pRoot, target, 1);
}

//-----------------------------------------------------------------------------
// getDepth
// Gets the depth of the specified NodeData
//-----------------------------------------------------------------------------
int BSTree::getDepthRecursive(const Node* pNode, const NodeData& target, 
    int currentDepth) const
{
    int depth = 0;

    if (NULL == pNode)
    {
        return 0;
    }

    if (*(pNode->pData) == target)
    {
        return currentDepth;
    }

    currentDepth++;

    // Try to find the item by going left
    depth = getDepthRecursive(pNode->pLeft, target, currentDepth);

    // If it wasn't found, go right
    if (depth <= 0)
    {
        depth = getDepthRecursive(pNode->pRight, target, currentDepth);
    }

    return depth;
}

//-----------------------------------------------------------------------------
// setName
// Sets the name of the collection
//-----------------------------------------------------------------------------
void BSTree::setName(const string& name)
{
    m_collectionName = name;
}

//-----------------------------------------------------------------------------
// getName
// Gets the name of the collection
//-----------------------------------------------------------------------------
const string& BSTree::getName()
{
    return m_collectionName;
}

//-----------------------------------------------------------------------------
// bstreeToArray
// Outputs a BSTree to an array, populating an array 
//-----------------------------------------------------------------------------
void BSTree::bstreeToArray()
{
    int arrayIndex = 0;

    // Delete and recreate the NodeData pointer array
    delete[] m_prgNodeData;
    m_prgNodeData = new NodeData*[m_nodeCount];
    memset(m_prgNodeData, 0, m_nodeCount);

    bstreeToArrayRecursive(m_pRoot, arrayIndex);
}

//-----------------------------------------------------------------------------
// bstreeToArrayRecursive
// Outputs a BSTree to an array, populating an array 
// The array output should be sorted from smallest to largest,
// using an in-order traversal to determine the array index.
//-----------------------------------------------------------------------------
void BSTree::bstreeToArrayRecursive(Node* pNode, int& arrayIndex)
{
    if (NULL == pNode)
    {
        return;
    }

    Node* pLeft = pNode->pLeft;
    Node* pRight = pNode->pRight;

    bstreeToArrayRecursive(pLeft, arrayIndex);
    m_prgNodeData[arrayIndex++] = pNode->pData;
    bstreeToArrayRecursive(pRight, arrayIndex);
}
