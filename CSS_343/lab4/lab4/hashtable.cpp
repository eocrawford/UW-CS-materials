#include "hashtable.h"

// Pre-calculated selection of prime numbers up to 100,000,
// used when resizing the hashtable. Note that this isn't
// required for chaining implementations, but all closed
// addressing implementations require it.
// (i + 1) > (i * 2)
const int rgPrimes[] =
{
    11,
    23,
    47,
    97,
    197,
    397,
    797,
    1597,
    3203,
    6421,
    12853,
    25717,
    51437,
    102877,
};

//-----------------------------------------------------------------------------
// HashTable()
//-----------------------------------------------------------------------------
HashTable::HashTable() :
    m_capacity(0),
    m_used(0),
    m_prgHashTable(NULL),
    m_loadFactor(0.75)
{
}

//-----------------------------------------------------------------------------
// ~HashTable()
//-----------------------------------------------------------------------------
HashTable::~HashTable()
{
    makeEmpty();
}

//-----------------------------------------------------------------------------
// makeEmpty()
// Empty hashtable
//-----------------------------------------------------------------------------
void HashTable::makeEmpty()
{
    // Delete each bucket
    for (int i = 0; i < m_capacity; i++)
    {
        delete m_prgHashTable[i];
    }

    // Delete the array of bucket pointers
    delete[] m_prgHashTable;
    m_prgHashTable = NULL;
    m_capacity = 0;
    m_used = 0;
}

//-----------------------------------------------------------------------------
// isEmpty() const
// Returns true if hashtable is empty
//-----------------------------------------------------------------------------
bool HashTable::isEmpty() const
{
    bool ret = true;

    if (m_prgHashTable)
    {
        for (int i = 0; i < m_capacity; i++)
        {
            if (m_prgHashTable[i] && !m_prgHashTable[i]->isEmpty())
            {
                ret = false;
                break;
            }
        }
    }

    return ret;
}

//-----------------------------------------------------------------------------
// retrieve(const NodeData& target) const
// Retrieves a value from the hashtable
//-----------------------------------------------------------------------------
NodeData* const HashTable::retrieve(const NodeData& target) const
{
    NodeData* pRet = NULL;
    int index = getHashIndex(&target, m_capacity);

    // Get the item
    if (m_prgHashTable[index] != NULL)
    {
        pRet = m_prgHashTable[index]->retrieve(target);
    }

    return pRet;
}

//-----------------------------------------------------------------------------
// insert(const NodeData* pItem)
// Inserts a value into the hashtable, using the provided key
//-----------------------------------------------------------------------------
void HashTable::insert(const NodeData* pItem)
{
    unsigned int index;

    // Check capacity and resize the array, if necessary
    if (((double)++m_used / m_capacity) >= m_loadFactor)
    {
        BSTree** prgTemp;
        int i = 0;
        int prevCapacity = m_capacity;

        // Pick the next biggest value in the primes array
        while (m_capacity >= rgPrimes[i])
        {
            if (i > ARRAYSIZE(rgPrimes)) return;
            i++;
        }

        // Allocate a new array
        prgTemp = new BSTree*[rgPrimes[i]];
        if (NULL == prgTemp) return;
        memset(prgTemp, 0, rgPrimes[i] * sizeof(BSTree*));

        m_capacity = rgPrimes[i];

        for (int i = 0; i < prevCapacity; i++)
        {
            // Re-hash every item in every bucket
            if (NULL != m_prgHashTable[i])
            {
                for (int j = m_prgHashTable[i]->getSize() - 1; j >= 0; j--)
                {
                    const NodeData* p = m_prgHashTable[i]->getData(j);

                    if (NULL != p)
                    {
                        int newIndex = getHashIndex(p, m_capacity);

                        if (NULL == prgTemp[newIndex])
                        {
                            prgTemp[newIndex] = new BSTree();
                        }

                        prgTemp[newIndex]->insert(p);
                    }
                }

                delete m_prgHashTable[i];
            }
        }

        // Delete the old array
        delete[] m_prgHashTable;

        // Re-initialize class members
        m_prgHashTable = prgTemp;
    }

    // Get hash index
    index = getHashIndex(pItem, m_capacity);

    // Allocate a new bucket, if necessary
    if (NULL == m_prgHashTable[index])
    {
        m_prgHashTable[index] = new BSTree();
    }

    // Insert the item into the bucket
    m_prgHashTable[index]->insert(pItem);
}

//-----------------------------------------------------------------------------
// remove(const NodeData& target)
// Removes a value from the hashtable
//-----------------------------------------------------------------------------
void HashTable::remove(const NodeData& target)
{
    int index = getHashIndex(&target, m_capacity);

    if (m_prgHashTable[index] != NULL && !m_prgHashTable[index]->isEmpty())
    {
        m_prgHashTable[index]->remove(target);
    }
}

//-----------------------------------------------------------------------------
// getHashIndex(const NodeData* pItem)
// Gets the hash index of the selected item, using Jenkins one-at-a-time
// hashing algorithm
//-----------------------------------------------------------------------------
unsigned int HashTable::getHashIndex(const NodeData* pItem, int capacity) const
{
    string key;
    size_t cKey;
    unsigned int hash = 0;

    // Get a hash key
    const_cast<NodeData*>(pItem)->getKey(key);
    cKey = key.length();

    for (unsigned int i = 0; i < cKey; i++)
    {
        hash += key.c_str()[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return (hash % capacity);
}
