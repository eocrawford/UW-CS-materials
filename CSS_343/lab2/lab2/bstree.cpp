#include "bstree.h"

//-----------------------------------------------------------------------------
// overloaded << operator
// Displays the tree, in-order
//-----------------------------------------------------------------------------
ostream& operator<<(ostream& output, const BSTree& b)
{
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
// BSTree copy constructor
// Initialize default BStree state with the contents of the provided BSTREE
//-----------------------------------------------------------------------------
BSTree::BSTree(const BSTree& b)
{
    init();

    copy(*this, b);
}

//-----------------------------------------------------------------------------
// = operator
// Assign one BSTree to another
//-----------------------------------------------------------------------------
BSTree& BSTree::operator=(const BSTree& b)
{
    if (this != &b)
    {
        makeEmpty();
        copy(*this, b);
    }

    return *this;
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
// BSTree destructor
// Clean up memory
//-----------------------------------------------------------------------------
BSTree::~BSTree()
{
    makeEmpty();
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
    }
    else if (NULL == pNode->pLeft)
    {
        pNode = pNode->pRight;
        delete pTemp->pData;
        delete pTemp;
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
    output << *(pNode->pData) << ' ';
    displayInOrderRecursive(pNode->pRight, output);
}


//-----------------------------------------------------------------------------
// makeEmpty
// Empties the current tree
//-----------------------------------------------------------------------------
void BSTree::makeEmpty()
{
    makeEmptyRecursive(m_pRoot);
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
}

//-----------------------------------------------------------------------------
// copy
// Copy the contents of src into dst
//-----------------------------------------------------------------------------
void BSTree::copy(BSTree& dst, const BSTree& src)
{
    copyRecursive(dst.m_pRoot, src.m_pRoot);
}

//-----------------------------------------------------------------------------
// copyRecursive
// Copy the contents of src into dst, recursively
//-----------------------------------------------------------------------------
void BSTree::copyRecursive(Node*& pDstNode, const Node* pSrcNode)
{
    if (NULL == pSrcNode)
    {
        return;
    }

    // base case - copy parent node
    if (NULL == pDstNode)
    {
        copyNode(pDstNode, pSrcNode);
    }

    // copyright
    if (NULL != pSrcNode->pRight)
    {
        copyNode(pDstNode->pRight, pSrcNode->pRight);
        copyRecursive(pDstNode->pRight, pSrcNode->pRight);
    }

    // copyleft!
    if (NULL != pSrcNode->pLeft)
    {
        copyNode(pDstNode->pLeft, pSrcNode->pLeft);
        copyRecursive(pDstNode->pLeft, pSrcNode->pLeft);
    }
}

//-----------------------------------------------------------------------------
// copyNode
// Copy the contents of node src into dst
//-----------------------------------------------------------------------------
void BSTree::copyNode(Node*& pDst, const Node* pSrc) const
{
    Node* pNode = new Node();
    NodeData* pNodeData = new NodeData(*(pSrc->pData));

    if (NULL == pNode || NULL == pNodeData)
    {
        return;
    }

    pDst = pNode;
    pDst->pData = pNodeData;
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
// bstreeToArray
// Outputs a BSTree to an array, populating an array 
// and emptying the BSTree.
//-----------------------------------------------------------------------------
void BSTree::bstreeToArray(NodeData* rgNodeData[])
{
    int arrayIndex = 0;
    bstreeToArrayRecursive(m_pRoot, rgNodeData, arrayIndex);
    m_pRoot = NULL;
}

//-----------------------------------------------------------------------------
// bstreeToArrayRecursive
// Outputs a BSTree to an array, populating an array 
// and emptying the BSTree.
// The array output should be sorted from smallest to largest,
// using an in-order traversal to determine the array index.
//-----------------------------------------------------------------------------
void BSTree::bstreeToArrayRecursive(Node* pNode, NodeData* rgNodeData[], 
    int& arrayIndex)
{
    if (NULL == pNode)
    {
        return;
    }

    Node* pLeft = pNode->pLeft;
    Node* pRight = pNode->pRight;

    bstreeToArrayRecursive(pLeft, rgNodeData, arrayIndex);
    rgNodeData[arrayIndex++] = pNode->pData;
    delete pNode;
    bstreeToArrayRecursive(pRight, rgNodeData, arrayIndex);
}

//-----------------------------------------------------------------------------
// arrayToBSTree
// Outputs an array to a BSTree, populating a BSTree 
// and emptying the array.
//-----------------------------------------------------------------------------
void BSTree::arrayToBSTree(NodeData* rgNodeData[])
{
    int maxIndex = 99;

    // Only copy non-NULL pointers; assume the data is contiguous
    while (!rgNodeData[maxIndex])
    {
        maxIndex--;
    }

    // Empty ourselves, this is equivalent to an assignment
    makeEmpty();

    // Reassign the root pointer to the root of the balanced node
    arrayToBSTreeRecursive(m_pRoot, rgNodeData, 0, maxIndex);
}

//-----------------------------------------------------------------------------
// arrayToBSTreeRecursive
// Outputs an array to a BSTree, creating a balanced BSTree and emptying 
// the array.
//-----------------------------------------------------------------------------
void BSTree::arrayToBSTreeRecursive(Node*& pNode,
    NodeData* rgNodeData[], int low, int high)
{
    // watch for index overlap
    if (high < low)
    {
        return;
    }

    pNode = new Node();

    // Find the middle
    int headIndex = (high + low) / 2;

    // insert the middle
    pNode->pData = rgNodeData[headIndex];
    rgNodeData[headIndex] = NULL;

    // Get the Node's children
    arrayToBSTreeRecursive(pNode->pLeft, rgNodeData, low, headIndex - 1);
    arrayToBSTreeRecursive(pNode->pRight, rgNodeData, headIndex + 1, high);
}
