#include "customer.h"

Customer::~Customer()
{
    // Empty the queue
    while (!m_transactionQueue.empty())
    {
        delete m_transactionQueue.front();
        m_transactionQueue.pop();
    }
}

//-----------------------------------------------------------------------------
// addTransaction(const Transaction* pTrans)
// Saves a transaction to a list
//-----------------------------------------------------------------------------
void Customer::addTransaction(Transaction* pTransaction)
{
    if (pTransaction != NULL)
    {
        m_transactionQueue.push(pTransaction);
    }
}

//-----------------------------------------------------------------------------
// display()
// Displays formatted info
//-----------------------------------------------------------------------------
void Customer::display()
{
    cout << "  *** Customer ID = " << m_id << "  " << m_lastName 
        << " " << m_firstName << endl;

    if (! m_transactionQueue.empty())
    {
        Transaction* pTransFront = m_transactionQueue.front();
        Transaction* pTransCurrent = pTransFront;

        // Print every element in the queue, rebuilding it in-place
        // to make the action non-destructive.
        do
        {
            pTransCurrent->display();
            m_transactionQueue.push(pTransCurrent);
            m_transactionQueue.pop();
            pTransCurrent = m_transactionQueue.front();
        }
        while (pTransCurrent != pTransFront);
    }
}

//-----------------------------------------------------------------------------
// create()
// Creates a new instance of a NodeData object
//-----------------------------------------------------------------------------
NodeData* Customer::create()
{
    return new Customer();
}

//-----------------------------------------------------------------------------
// setKey()
// Sets a key to be used in a hashtable
//-----------------------------------------------------------------------------
void Customer::setKey(const string& hashKey)
{
    stringstream ss;

    ss << hashKey;
    ss >> m_id;

    m_hashKey = hashKey;
}

//-----------------------------------------------------------------------------
// setData(ifstream& input)
// Reads a single input line, populating internal variables
//-----------------------------------------------------------------------------
bool Customer::setData(ifstream& input)
{
    stringstream ss;
    input >> m_id >> m_lastName >> m_firstName;

    // itoa
    ss << m_id;

    // Set hash key
    m_hashKey = ss.str();

    // Clean up
    input.clear();
    input.ignore(numeric_limits<streamsize>::max(), '\n');

    return input.good();
}

//-----------------------------------------------------------------------------
// operator<(const NodeData& b) const
// overloaded < operator
//-----------------------------------------------------------------------------
bool Customer::operator<(const NodeData& b) const
{
    return (compare(*this, b) < 0);
}

//-----------------------------------------------------------------------------
// operator>(const NodeData& b) const
// overloaded > operator
//-----------------------------------------------------------------------------
bool Customer::operator>(const NodeData& b) const
{
    return (compare(*this, b) > 0);
}

//-----------------------------------------------------------------------------
// operator==(const NodeData& b) const
// overloaded == operator
//-----------------------------------------------------------------------------
bool Customer::operator==(const NodeData& b) const
{
    return (compare(*this, b) == 0);
}

//-----------------------------------------------------------------------------
// operator!=(const NodeData& b) const
// overloaded != operator
//-----------------------------------------------------------------------------
bool Customer::operator!=(const NodeData& b) const
{
    return !(*this == b);
}

//-----------------------------------------------------------------------------
// compare(const NodeData& a, const NodeData& b) const
// compare method, used to implement comparison operators
//-----------------------------------------------------------------------------
int Customer::compare(const NodeData& a, const NodeData& b) const
{
    int ret = 0;
    const Customer* pA = dynamic_cast<const Customer*>(&a);
    const Customer* pB = dynamic_cast<const Customer*>(&b);

    if (pA->m_id > pB->m_id)
    {
        ret = 1;
    }
    else if (pA->m_id < pB->m_id)
    {
        ret = -1;
    }

    return ret;
}
