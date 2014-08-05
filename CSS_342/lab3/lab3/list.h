#ifndef _LIST_H
#define _LIST_H

//---------------------------------------------------------------------------
// list.h
// List template class
// Author(s): Ethan Crawford (spec from Dr. Carol Zander)
// Date: April 25, 2007
//---------------------------------------------------------------------------
// List template class: stores a linked list of objects.
//   Includes the following features:
//   -- 
//   -- 
//   -- 
//      
//   -- 
//
// Assumptions:
//   -- Input files are in the valid format for the type of object loading
//      them.
//   -- Types used with this template class must support the following
//      methods and operators:
//      
//---------------------------------------------------------------------------

#include <iostream>

using namespace std; 

template <typename T>
class List
{
    // Output the list as text (do not print any endl)
    friend ostream& operator<< <T> (ostream& output, const List& a);

public:
    // Default constructor - creates an empty list
    List();

    // Copy constructor
    List(const List& a);

    // Destructor
    ~List();

    // Build an ordered list from the provided input stream
    // (object T must understand how to read from a file.) 
    // This is implemented as an insertion sort, O(n^2) time). 
    void buildList(ifstream& infile);

    // insert a pointer to an object 'T' into its correct sorted position.
    // Does not make a copy of the object.
    // Returns true if the insertion was successful; false otherwise
    bool insert(const T* pItem);

    // Search for the provided object 'T'. If found, remove the first
    // instance from the list and return it on the second parameter.
    // Returns true if the removal was successful; false otherwise
    bool remove(const T& targetItem, T*& pRemovedItem);

    // Search for the provided object 'T'. If found, return the first
    // instance on the second parameter.
    bool retrieve(const T& targetItem, T*& pRetrievedItem);

    // Merges two Lists into one List, emptying the provided Lists and 
    // assigning the result to the current instance.
    // (Any existing list inside the current instance is overwritten.)
    // No new memory is allocated.
    void merge(List& one, List& two);

    // void intersect:
    // Finds the items common to both provided lists, assigning the 
    // result to the current instance. The provided lists are unchanged.
    // New memory is allocated.
    void intersect(const List& one, const List& two);

    // Returns true if the list is empty; false otherwise
    bool isEmpty() const;

    // Emptys the list, deallocating all memory for all Nodes and objects of type 'T'
    void makeEmpty();

    // the assignment operator to assign one List to another. Takes O(n) time.
    List& operator=(const List& b);

    // bool equality
    bool operator==(const List& b) const;

    // bool inequality
    bool operator!=(const List& b) const;

private:

    // Template class representing a single node in the list
    template <typename T>
    struct Node
    {
        Node() :
            pData(NULL),
            pNext(NULL)
        {
        }

        Node(const T* pD, Node* pN) :
            pData(pD),
            pNext(pN)
        {
        }

	    const T* pData;
	    Node<T>* pNext;
    };

    // Initialize data members in constructors
    void init();

    // Copy the src node into the dest node
    bool copyNode(Node<T>*& pDst, const Node<T>& src);

    // Move the src node from src to the beginning of dst
    void moveNode(Node<T>*& dst, Node<T>*& src);

    // Copy the src list into the dst list
    bool copyList(List& dst, const List& src);

    // pointer to the head element in the list
    Node<T>* m_pHead;

    // size of the list
    int m_size;
};

//---------------------------------  <<  ------------------------------------
// Overloaded left-shift operator <<:
// Output the list as text (do not print any endl)
template <typename T>
ostream& operator<<(ostream& output, const List<T>& a)
{
    List<T>::Node<T>* pCurrent = a.m_pHead;

    // Walk the list
    while (NULL != pCurrent)
    {
        // Print each item according to its individual implementation of the 
        // '<<' operator
        output << *(pCurrent->pData);
        pCurrent = pCurrent->pNext;
    }

    return output;
}

//------------------------------- List<T> -----------------------------------
// Default constructor.
template <typename T>
List<T>::List()
{
    init();
}

//------------------------------- List<T> -----------------------------------
// Copy constructor.
template <typename T>
List<T>::List(const List& a)
{
    init();

    copyList(*this, a);
}

//------------------------------- init --------------------------------------
// Helper function to initialize data members in constructors
// Set up default state
template <typename T>
void List<T>::init()
{
    m_pHead = NULL;
    m_size = 0;
}

//------------------------------ ~List<T> -----------------------------------
// Destructor
template <typename T>
List<T>::~List()
{
    makeEmpty();
}

//----------------------------- buildList -----------------------------------
// Build an ordered list from the provided input stream (object T must 
// understand how to read from a file.) This is implemented as an insertion 
// sort, O(n^2) time).
template <typename T>
void List<T>::buildList(ifstream& infile)
{
    while (true) 
    {
        T* pItem = new T();

        // Check for out-of-memory
        if (NULL == pItem)
        {
            break;
        }

        // Get the object from the input file
        pItem->setNodeData(infile);

        // Insert the item into the list
        if (!infile.good() || infile.eof() || !insert(pItem))
        {
            delete pItem;
            break;
        }
    }
}

//------------------------------- insert ------------------------------------
// Insert a pointer to an object of type T into its correct sorted position.
// Does not make a copy of the object.
// Returns true if the insertion was successful; false otherwise
template <typename T>
bool List<T>::insert(const T* pItem)
{
    Node<T>* pNode = new Node<T>();

    // Handle out-of-memory and invalid parameters
    if (NULL == pNode || NULL == pItem)
    {
        return false;
    }

    // Special case - populate the new node w/ provided data. 
    // Node constructor automatically sets ->next = NULL.
    if (NULL == m_pHead)
    {
        m_pHead = pNode;
        m_pHead->pData = pItem;
    }
    else
    {
        Node<T>* pCurrent;

        // Add the new node at the beginning of the list
        pNode->pData = pItem;
        pNode->pNext = m_pHead;
        m_pHead = pNode;

        pCurrent = m_pHead;

        // Move the current item into the correct place
        while (pCurrent->pNext != NULL && 
            *(pCurrent->pData) > *(pCurrent->pNext->pData))
        {
            swap(pCurrent->pData, pCurrent->pNext->pData);
            pCurrent = pCurrent->pNext;
        }
    }

    m_size++;
    return true;
}

//------------------------------- remove ------------------------------------
// Searches for the provided object 'T'. If found, removes the first instance 
// from the list and returns it on the second parameter.
// Returns true if the removal was successful; false otherwise
template <typename T>
bool List<T>::remove(const T& targetItem, T*& pRemovedItem)
{
    Node<T>* pPrevious = m_pHead;
    Node<T>* pCurrent = m_pHead;
    T* pCopiedItem = NULL;

    while (NULL != pCurrent)
    {
        if (*(pCurrent->pData) == targetItem)
        {
            // Allocate memory for the item to be removed
            pCopiedItem = new T();

            // Check for out-of-memory
            if (NULL == pCopiedItem)
            {
                return false;
            }

            // Copy the data into the new object
            *pCopiedItem = *(pCurrent->pData);

            // Handle the special-case of deleting the head
            if (pPrevious == pCurrent)
            {
                m_pHead = pCurrent->pNext;
            }
            else
            {
                // Re-route the list around the current node
                pPrevious->pNext = pCurrent->pNext;
            }

            // Delete the current node
            delete pCurrent->pData;
            delete pCurrent;

            // All is well; assign the out pointer
            pRemovedItem = pCopiedItem;
            m_size--;

            break;
        }

        pPrevious = pCurrent;
        pCurrent = pCurrent->pNext;
    }

    return (NULL != pCopiedItem);
}

//------------------------------ retrieve -----------------------------------
// Searches for the provided object 'T'. If found, returns the first instance
// on the second parameter.
// Returns true if the retrieval was successful; false otherwise
template <typename T>
bool List<T>::retrieve(const T& targetItem, T*& pRetrievedItem)
{
    Node<T>* pCurrent = m_pHead;
    T* pCopiedItem = NULL;

    while (NULL != pCurrent)
    {
        if (*(pCurrent->pData) == targetItem)
        {
            pCopiedItem = new T();

            if (NULL == pCopiedItem)
            {
                return false;
            }

            *pCopiedItem = *(pCurrent->pData);

            // All is well; assign the out pointer
            pRetrievedItem = pCopiedItem;

            break;
        }

        pCurrent = pCurrent->pNext;
    }

    return (NULL != pCopiedItem);
}

//-------------------------------- merge ------------------------------------
// Takes 2 sorted lists and merges them into one long sorted list.
// No new memory is allocated. At termination of function, the 
// two parameter lists are empty unless one is also the current object.  
// Duplicates are allowed in a merged list.
//(O(n) time, so insert must  not be used or time would be O(n2))
//
// E.g., after this merge, company1 and company2 are empty, company3 
// contains all employees of company1 and company2 
// company3.merge(company1, company2)
//
// after merge, company3 is empty, 
// company4 contains all employees of company3 and company4 
// company4.merge(company4, company3)
//
// list1.merge(list2, list3)
// list1 will contain only the items of list2 merged with list3. 
// The merge of list2 and list3 replaces list1. 
// list1 may or may not be empty originally, which must be dealt with.
template <typename T>
void List<T>::merge(List<T>& one, List<T>& two)
{
    Node<T>* pHead = NULL;
    int size = 0;

    // The merge is technically already complete if any of these conditions 
    // are satisfied:
    // one and two are the same object OR 
    // one or two contains zero items
    // If one of the lists is empty, the merged head pointer should be
    // set to the head pointer of the non-empty list.
    if (&one == &two || one.m_size <= 0 || two.m_size <= 0)
    {
        pHead = (one.m_size > 0) ? one.m_pHead : two.m_pHead;
        size = (one.m_size > 0) ? one.m_size : two.m_size;
    }
    else
    {
        // Set the current pointers
        Node<T>* pCurrentOne = one.m_pHead;
        Node<T>* pCurrentTwo = two.m_pHead;
        Node<T>* pTail = NULL;

        // Calculate the size
        size = one.m_size + two.m_size; 

        // Set the list head to the smallest item
        if (*(pCurrentOne->pData) < *(pCurrentTwo->pData))
        {
            pHead = pCurrentOne;
            pCurrentOne = pCurrentOne->pNext;
        }
        else
        {
            pHead = pCurrentTwo;
            pCurrentTwo = pCurrentTwo->pNext;
        }

        // Set the tail to the head
        pTail = pHead;

        // Merge the lists
        while (true)
        {
            // Tack on the longer list after the shorter list 
            // has run out of elements
            if (NULL == pCurrentTwo)
            {
                pTail->pNext = pCurrentOne;
                break;
            }
            else if (NULL == pCurrentOne)
            {
                pTail->pNext = pCurrentTwo;
                break;
            }

            // Add the smallest node to the merged list
            if (*(pCurrentOne->pData) < *(pCurrentTwo->pData))
            {
                moveNode(pTail->pNext, pCurrentOne);
            }
            else
            {
                moveNode(pTail->pNext, pCurrentTwo);
            }

            // Advance the tail to the current tail
            pTail = pTail->pNext;
        }
    }

    // The internal list should not be emptied after the merge if either of 
    // the provided lists is this instance.
    if (&one != this && &two != this)
    {
        makeEmpty();
    }

    // Pointers to each item in each list have been stored internally, so
    // there's no memory leak, but we need to zero out the component lists
    // to avoid the possibility of circular references later.
    one.m_size = 0;
    one.m_pHead = NULL;
    two.m_size = 0;
    two.m_pHead = NULL;

    m_size = size;
    m_pHead = pHead;
}

//-------------------------------- intersect --------------------------------
// takes 2 sorted lists and finds the items in common in both lists 
// (New memory is allocated at termination of the function, the two 
// parameter lists are unchanged unless one is also the current object.) 
// E.g.,
// after intersect, company1 and company2 are unchanged,
// company3 holds intersection of company1 and 2
// company3.intersect(company1, company2)
// 
// after intersect, company3 is unchanged,
// company4 holds the intersection of company3 and 4
// company4.intersect(company3, company4)
template <typename T>
void List<T>::intersect(const List<T>& one, const List<T>& two)
{
    Node<T>* pCurrentTwo = one.m_pHead;
    Node<T>* pCurrentOne = two.m_pHead;
    Node<T>* pHead = NULL;
    Node<T>* pTail = NULL;
    int size = 0;

    // Same list; it is already intersected with itself.
    if (&one == &two)
    {
        return;
    }

    // Iterate both lists; if one goes empty, there are no more
    // intersections (because both are sorted).
    while (pCurrentOne != NULL && pCurrentTwo != NULL)
    {
        if (*(pCurrentOne->pData) == *(pCurrentTwo->pData))
        {
            Node<T>* pNode;

            // Make a copy of one fo the identical nodes
            if (!copyNode(pNode, *pCurrentOne))
            {
                return;
            }

            // Start a new list or append to an existing one
            if (NULL == pHead)
            {
                pHead = pNode;
                pTail = pHead;
            }
            else
            {
                pTail->pNext = pNode;
                pTail = pTail->pNext;
            }

            size++;
            pCurrentOne = pCurrentOne->pNext;
            pCurrentTwo = pCurrentTwo->pNext;
        }
        // Increment the list containing the smaller item
        else if (*(pCurrentOne->pData) > *(pCurrentTwo->pData))
        {
            pCurrentTwo = pCurrentTwo->pNext;
        }
        else if (*(pCurrentOne->pData) < *(pCurrentTwo->pData))
        {
            pCurrentOne = pCurrentOne->pNext;
        }
    }

    // Assign the new list to the current instance
    makeEmpty();
    m_size = size;
    m_pHead = pHead;
}

//--------------------------------- isEmpty ---------------------------------
// Returns true if the list is empty false otherwise
template <typename T>
bool List<T>::isEmpty() const
{
    return (m_size <= 0);
}

//---------------------------------  =  -------------------------------------
// Overloaded assignment operator =:
// Assigns the contents of the List parameter to the current object.
// the assignment operator to assign one List to another. Takes O(n) time.
template <typename T>
List<T>& List<T>::operator=(const List<T>& a)
{
    if (&a != this)
    {
        makeEmpty();
        copyList(*this, a);
    }

    return *this;
}

//---------------------------------  ==  ------------------------------------
// Overloaded equality operator ==:
// Returns true if the IntSet parameter is the same as the current object;
// false otherwise.
template <typename T>
bool List<T>::operator==(const List<T>& a) const
{
    bool ret = true;
    Node<T>* pCurrentTwo = a.m_pHead;
    Node<T>* pCurrentOne = m_pHead;

    // Self-comparison; return true
    if (this == &a)
    {
        return true;
    }

    // Different list sizes; return false
    if (a.m_size != m_size)
    {
        return false;
    }

    // Walk the entire list and compare values.
    while (NULL != pCurrentOne && NULL != pCurrentTwo)
    {
        // Exit the loop early if a difference is found
        if (*(pCurrentOne->pData) != *(pCurrentTwo->pData))
        {
            ret = false;
            break;
        }

        pCurrentOne = pCurrentOne->pNext;
        pCurrentTwo = pCurrentTwo->pNext;
    }

    return ret;
}

//---------------------------------  !=  ------------------------------------
// Overloaded inequality operator !=:
// Returns true if the IntSet parameter is different than the current object;
// false otherwise.
template <typename T>
bool List<T>::operator!=(const List<T>& a) const
{
    return (!operator==(a));
}

//-------------------------------- copyList ---------------------------------
// Copies a list from src to dst. Performs a deep copy using the assignment
// operator.
template <typename T>
bool List<T>::copyList(List<T>& dst, const List<T>& src)
{
    Node<T>* pCurrent = src.m_pHead;
    Node<T>* pTail = NULL;

    // Add each item in the existing list to a new list
    while (NULL != pCurrent)
    {
        Node<T>* pNode;

        // Check for out-of-memory
        if (! copyNode(pNode, *pCurrent))
        {
            return false;
        }

        // If this is the first element, assign it to the head
        if (NULL == dst.m_pHead)
        {
            dst.m_pHead = pNode;
            pTail = dst.m_pHead;
        }
        // Otherwise, add the new node to the tail element and reset
        // the current tail element
        else
        {
            pTail->pNext = pNode;
            pTail = pTail->pNext;
        }

        pCurrent = pCurrent->pNext;
        dst.m_size++;
    }

    return true;
}

//-------------------------------- copyNode ---------------------------------
// Copy the src node and its contents, allocating heap memory to do so.
template <typename T>
bool List<T>::copyNode(Node<T>*& pDst, const Node<T>& src)
{
    // Allocate space for the new node and its contents
    Node<T>* pNode = new Node<T>();
    T* pData = new T();

    // Check for out-of-memory
    if (NULL == pNode || NULL == pData)
    {
        return false;
    }

    // Copy the data from the src node to the dst node
    *pData = *(src.pData);

    // Assign the data to the new node
    pNode->pData = pData;

    // Assign the newly-copied node to pDst
    pDst = pNode;

    return true;
}

//-------------------------------- moveNode ---------------------------------
// Move one node from src to dst
template <typename T>
void List<T>::moveNode(Node<T>*& pDst, Node<T>*& pSrc)
{
    // Save off the passed-in source node
    Node<T>* pTemp = pSrc;

    // Advance the source node to the source element
    pSrc = pTemp->pNext;

    // Add the temp node to the head of the destination list
    pTemp->pNext = pDst;

    // Repoint the destination list to the temp node that was just moved
    pDst = pTemp;
}

//-------------------------------- makeEmpty --------------------------------
// empty out the list, deallocate all memory for all Nodes and each Object
template <typename T>
void List<T>::makeEmpty()
{
    Node<T>* pCurrent = m_pHead;

    while (pCurrent != NULL)
    {
        // Save the current object pointer for deleting
        Node<T>* pTemp = pCurrent;

        // Advance the pointer
        pCurrent = pCurrent->pNext;

        // Delete the data within the node
        delete pTemp->pData;

        // Delete the current node
        delete pTemp;
    }

    m_pHead = NULL;
    m_size = 0;
}

#endif // _LIST_H
